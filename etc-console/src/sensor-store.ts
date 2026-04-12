/**
 * SensorStore — IndexedDB-backed time-series storage with in-memory ring buffer.
 *
 * All sensor data is appended here.  The most recent MEM_LIMIT points stay in
 * memory for fast chart rendering.  Older data is flushed to IndexedDB in
 * chunks and can be retrieved for zoom-out / CSV export.
 *
 * Column layout (same indices everywhere):
 *   0:ts  1:a1  2:a2  3:i  4:t1  5:t2  6:tgt  7:a1r 8:a2r 9:ir 10:t1r 11:t2r 12:br 13:b
 */
import type { SensorData } from "./types";

// ── Column definition ────────────────────────────────
export const COL_KEYS: (keyof SensorData)[] = [
  "ts",
  "a1",
  "a2",
  "i",
  "t1",
  "t2",
  "tgt",
  "a1r",
  "a2r",
  "ir",
  "t1r",
  "t2r",
  "br",
  "b",
];
export const NUM_COLS = COL_KEYS.length;

/** Indices for the 6 chart series used by SensorChart (matches SERIES_KEYS order) */
export const CHART_COL = {
  ts: 0,
  a1: 1,
  a2: 2,
  i: 3,
  t1: 4,
  t2: 5,
  tgt: 6,
} as const;

// ── Tunables ─────────────────────────────────────────
/** Points kept in memory (≈5 min at 50 Hz) */
const MEM_LIMIT = 15_000;
/** Points per IndexedDB chunk */
const CHUNK_SIZE = 5_000;

const DB_NAME = "etc-sensor";
const DB_VERSION = 1;
const STORE_NAME = "chunks";

// ── Types ────────────────────────────────────────────
export interface Chunk {
  /** auto-increment id */
  id?: number;
  /** session identifier (connection timestamp) */
  session: number;
  /** startTs (seconds) */
  startTs: number;
  /** endTs (seconds) */
  endTs: number;
  /** column-major Float64Arrays packed into a single ArrayBuffer */
  data: ArrayBuffer;
  /** number of rows in this chunk */
  rows: number;
}

// ── Helpers ──────────────────────────────────────────
function packColumns(cols: number[][]): ArrayBuffer {
  const rows = cols[0].length;
  const buf = new ArrayBuffer(NUM_COLS * rows * 8);
  const view = new Float64Array(buf);
  for (let c = 0; c < NUM_COLS; c++) {
    const col = cols[c];
    const off = c * rows;
    for (let r = 0; r < rows; r++) view[off + r] = col[r];
  }
  return buf;
}

function unpackColumns(buf: ArrayBuffer, rows: number): number[][] {
  const view = new Float64Array(buf);
  const cols: number[][] = [];
  for (let c = 0; c < NUM_COLS; c++) {
    const off = c * rows;
    cols.push(Array.from(view.subarray(off, off + rows)));
  }
  return cols;
}

// ── SensorStore class ────────────────────────────────
export class SensorStore {
  /** Column-major in-memory buffer — most recent data */
  mem: number[][] = Array.from({ length: NUM_COLS }, () => []);
  /** Pending (not yet flushed) staging buffer */
  private staging: number[][] = Array.from({ length: NUM_COLS }, () => []);
  private db: IDBDatabase | null = null;
  private session = 0;
  private flushPending = false;
  private _totalRows = 0;
  /** Latest raw SensorData — updated on every push (50Hz) */
  latest: SensorData | null = null;

  get totalRows(): number {
    return this._totalRows;
  }

  /** Number of rows currently in memory */
  get memRows(): number {
    return this.mem[0].length;
  }

  // ── Lifecycle ────────────────────────────────
  async open(): Promise<void> {
    return new Promise((resolve, reject) => {
      const req = indexedDB.open(DB_NAME, DB_VERSION);
      req.onupgradeneeded = () => {
        const db = req.result;
        if (!db.objectStoreNames.contains(STORE_NAME)) {
          const store = db.createObjectStore(STORE_NAME, {
            keyPath: "id",
            autoIncrement: true,
          });
          store.createIndex("session", "session", { unique: false });
          store.createIndex("endTs", "endTs", { unique: false });
        }
      };
      req.onsuccess = () => {
        this.db = req.result;
        resolve();
      };
      req.onerror = () => reject(req.error);
    });
  }

  /** Start a new recording session (call on connect) */
  startSession(): void {
    this.session = Date.now();
    // Clear memory buffers
    this.mem = Array.from({ length: NUM_COLS }, () => []);
    this.staging = Array.from({ length: NUM_COLS }, () => []);
    this._totalRows = 0;
    this.flushPending = false;
  }

  // ── Append ───────────────────────────────────
  push(data: SensorData): void {
    this.latest = data;
    const ts = data.ts / 1000;
    const vals: number[] = [
      ts,
      data.a1,
      data.a2,
      data.i,
      data.t1,
      data.t2,
      data.tgt,
      data.a1r,
      data.a2r,
      data.ir,
      data.t1r,
      data.t2r,
      data.br,
      data.b,
    ];
    for (let c = 0; c < NUM_COLS; c++) {
      this.mem[c].push(vals[c]);
      this.staging[c].push(vals[c]);
    }
    this._totalRows++;

    // Flush staging to IndexedDB when it reaches CHUNK_SIZE
    if (this.staging[0].length >= CHUNK_SIZE) {
      this.flushStaging();
    }

    // Trim memory to MEM_LIMIT
    if (this.mem[0].length > MEM_LIMIT + CHUNK_SIZE) {
      const excess = this.mem[0].length - MEM_LIMIT;
      for (let c = 0; c < NUM_COLS; c++) {
        this.mem[c] = this.mem[c].slice(excess);
      }
    }
  }

  // ── Flush to IndexedDB ───────────────────────
  private flushStaging(): void {
    if (this.flushPending || !this.db || this.staging[0].length === 0) return;
    this.flushPending = true;

    const rows = this.staging[0].length;
    const chunk: Chunk = {
      session: this.session,
      startTs: this.staging[0][0],
      endTs: this.staging[0][rows - 1],
      data: packColumns(this.staging),
      rows,
    };

    // Reset staging immediately so new data goes to fresh arrays
    this.staging = Array.from({ length: NUM_COLS }, () => []);

    const tx = this.db.transaction(STORE_NAME, "readwrite");
    tx.objectStore(STORE_NAME).add(chunk);
    tx.oncomplete = () => {
      this.flushPending = false;
    };
    tx.onerror = () => {
      this.flushPending = false;
    };
  }

  /** Force-flush any remaining staging data (call on disconnect) */
  async flush(): Promise<void> {
    if (!this.db || this.staging[0].length === 0) return;
    this.flushPending = false; // allow immediate flush
    return new Promise((resolve) => {
      const rows = this.staging[0].length;
      const chunk: Chunk = {
        session: this.session,
        startTs: this.staging[0][0],
        endTs: this.staging[0][rows - 1],
        data: packColumns(this.staging),
        rows,
      };
      this.staging = Array.from({ length: NUM_COLS }, () => []);

      const tx = this.db!.transaction(STORE_NAME, "readwrite");
      tx.objectStore(STORE_NAME).add(chunk);
      tx.oncomplete = () => resolve();
      tx.onerror = () => resolve();
    });
  }

  // ── Query ────────────────────────────────────
  /**
   * Load all data for the current session from IndexedDB + memory,
   * returning column-major arrays covering [minTs, maxTs].
   * Used for CSV export and full-range chart views.
   */
  async loadRange(minTs: number, maxTs: number): Promise<number[][]> {
    const cols: number[][] = Array.from({ length: NUM_COLS }, () => []);

    // 1) IndexedDB chunks overlapping range
    if (this.db) {
      const chunks = await this.queryChunks(minTs, maxTs);
      for (const chunk of chunks) {
        const unpacked = unpackColumns(chunk.data, chunk.rows);
        const tsCol = unpacked[0];
        for (let r = 0; r < chunk.rows; r++) {
          if (tsCol[r] >= minTs && tsCol[r] <= maxTs) {
            for (let c = 0; c < NUM_COLS; c++) {
              cols[c].push(unpacked[c][r]);
            }
          }
        }
      }
    }

    // 2) Append in-memory data in range (avoid duplicates — mem may overlap last chunk)
    const memTs = this.mem[0];
    const lastDbTs =
      cols[0].length > 0 ? cols[0][cols[0].length - 1] : -Infinity;
    for (let r = 0; r < memTs.length; r++) {
      const t = memTs[r];
      if (t > lastDbTs && t >= minTs && t <= maxTs) {
        for (let c = 0; c < NUM_COLS; c++) {
          cols[c].push(this.mem[c][r]);
        }
      }
    }

    return cols;
  }

  /** Load ALL data for the current session */
  async loadAll(): Promise<number[][]> {
    return this.loadRange(-Infinity, Infinity);
  }

  private queryChunks(minTs: number, maxTs: number): Promise<Chunk[]> {
    return new Promise((resolve, reject) => {
      if (!this.db) {
        resolve([]);
        return;
      }
      const tx = this.db.transaction(STORE_NAME, "readonly");
      const store = tx.objectStore(STORE_NAME);
      const index = store.index("session");
      const req = index.getAll(IDBKeyRange.only(this.session));
      req.onsuccess = () => {
        const all = req.result as Chunk[];
        // Filter to chunks that overlap [minTs, maxTs]
        resolve(all.filter((c) => c.endTs >= minTs && c.startTs <= maxTs));
      };
      req.onerror = () => reject(req.error);
    });
  }

  // ── Export ───────────────────────────────────
  async exportCsv(): Promise<Blob> {
    const cols = await this.loadAll();
    const rows = cols[0].length;
    const header = COL_KEYS.join(",") + "\n";
    const lines: string[] = [header];
    for (let r = 0; r < rows; r++) {
      const vals: string[] = [];
      for (let c = 0; c < NUM_COLS; c++) {
        vals.push(String(cols[c][r]));
      }
      lines.push(vals.join(",") + "\n");
    }
    return new Blob(lines, { type: "text/csv" });
  }

  /** Delete all chunks for sessions older than current */
  async cleanOldSessions(): Promise<void> {
    if (!this.db) return;
    return new Promise((resolve) => {
      const tx = this.db!.transaction(STORE_NAME, "readwrite");
      const store = tx.objectStore(STORE_NAME);
      const index = store.index("session");
      const range = IDBKeyRange.upperBound(this.session, true);
      const req = index.openCursor(range);
      req.onsuccess = () => {
        const cursor = req.result;
        if (cursor) {
          cursor.delete();
          cursor.continue();
        }
      };
      tx.oncomplete = () => resolve();
      tx.onerror = () => resolve();
    });
  }
}

/** Singleton store instance */
export const sensorStore = new SensorStore();

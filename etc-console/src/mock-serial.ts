import type { Transport } from "./transport";
import type { DeviceConfig } from "./types";

const SENSOR_INTERVAL = 20; // 50Hz

let connected = false;
let timer: ReturnType<typeof setInterval> | null = null;
let onLineReceived: ((line: string) => void) | null = null;
let onDisconnect: (() => void) | null = null;

// Simulated device state
let t0 = 0;
let manualMode = false;
const flags: Record<string, boolean> = {
  apps: true,
  tps: true,
  apps1: true,
  apps2: true,
  tps1: true,
  tps2: true,
  target: true,
  bps: true,
  bpsTps: true,
};
let useIttr = false;
const sensorValues = {
  apps1Min: 200,
  apps1Max: 3800,
  apps2Min: 200,
  apps2Max: 3800,
  ittrMin: 100,
  ittrMax: 4000,
  tps1Min: 300,
  tps1Max: 3600,
  tps2Min: 300,
  tps2Max: 3600,
  idling: 5.0,
};

function emit(line: string) {
  onLineReceived?.(line);
}

function sensorTick() {
  const elapsed = (Date.now() - t0) / 1000;
  // Sine waves at different frequencies to simulate realistic-looking data
  const base = Math.sin(elapsed * 0.5) * 40 + 50; // 10~90% range
  const noise = () => (Math.random() - 0.5) * 2;

  const a1 = Math.max(0, Math.min(100, base + noise()));
  const a2 = Math.max(0, Math.min(100, base + noise() + 0.5));
  const tgt = manualMode ? 30 : base;
  const t1 = Math.max(
    0,
    Math.min(100, tgt + noise() * 3 + Math.sin(elapsed * 2) * 2),
  );
  const t2 = Math.max(
    0,
    Math.min(100, tgt + noise() * 3 + Math.sin(elapsed * 2) * 2 + 0.3),
  );
  const ittr = base * 0.8 + noise();
  const bpsVal = 14.7 + Math.sin(elapsed * 0.3) * 2 + noise() * 0.5;

  const msg = {
    t: "s",
    ts: Date.now() - t0,
    a1r: Math.round(a1 * 36 + 200),
    a2r: Math.round(a2 * 36 + 200),
    a1: +a1.toFixed(2),
    a2: +a2.toFixed(2),
    ir: Math.round(ittr * 39 + 100),
    i: +ittr.toFixed(2),
    t1r: Math.round(t1 * 33 + 300),
    t2r: Math.round(t2 * 33 + 300),
    t1: +t1.toFixed(2),
    t2: +t2.toFixed(2),
    br: Math.round(bpsVal * 100),
    b: +bpsVal.toFixed(2),
    tgt: +tgt.toFixed(2),
    m: "Normal",
    manual: manualMode,
    tgt_ittr: false,
    v: true,
    err: [] as number[],
  };

  emit(JSON.stringify(msg));
}

function handleCommand(text: string) {
  let parsed: { c: string; id: number; d?: Record<string, unknown> };
  try {
    parsed = JSON.parse(text);
  } catch {
    return;
  }

  const { c: cmd, id, d: data } = parsed;

  // Simulate processing delay
  setTimeout(() => {
    switch (cmd) {
      case "get_config": {
        const cfg: DeviceConfig = {
          sensorValues,
          plausibilityFlags: { ...flags },
          useIttr,
          pid: { kP: 3.0, kI: 0.4, kD: 0.0 },
        };
        emit(JSON.stringify({ t: "r", id, ok: true, data: cfg }));
        break;
      }
      case "set_plausibility_check_flags": {
        for (const key of Object.keys(flags)) {
          if (key in (data ?? {})) {
            flags[key] = (data as Record<string, boolean>)[key];
          }
        }
        emit(JSON.stringify({ t: "r", id, ok: true }));
        break;
      }
      case "set_ittr":
        useIttr = (data?.val as boolean) ?? false;
        emit(JSON.stringify({ t: "r", id, ok: true }));
        break;
      case "set_manual":
        manualMode = !manualMode;
        emit(JSON.stringify({ t: "r", id, ok: true }));
        break;
      case "save":
        emit(JSON.stringify({ t: "r", id, ok: true }));
        emit(
          JSON.stringify({
            t: "d",
            ts: Date.now() - t0,
            msg: "Config saved",
          }),
        );
        break;
      case "revert":
        emit(JSON.stringify({ t: "r", id, ok: true }));
        emit(
          JSON.stringify({
            t: "d",
            ts: Date.now() - t0,
            msg: "Config reverted",
          }),
        );
        break;
      case "set_config":
        emit(JSON.stringify({ t: "r", id, ok: true }));
        emit(
          JSON.stringify({
            t: "d",
            ts: Date.now() - t0,
            msg: "Config imported",
          }),
        );
        break;
      case "reboot":
        emit(JSON.stringify({ t: "r", id, ok: true }));
        emit(
          JSON.stringify({ t: "d", ts: Date.now() - t0, msg: "Mock reboot" }),
        );
        break;
      default:
        // Generic OK for calibration setters etc.
        emit(JSON.stringify({ t: "r", id, ok: true }));
        break;
    }
  }, 5);
}

export const mockSerial: Transport = {
  async connect() {
    connected = true;
    t0 = Date.now();
    timer = setInterval(sensorTick, SENSOR_INTERVAL);
  },
  async disconnect() {
    connected = false;
    if (timer) {
      clearInterval(timer);
      timer = null;
    }
    onDisconnect?.();
  },
  async send(text: string) {
    handleCommand(text);
  },
  isConnected() {
    return connected;
  },
  setOnLineReceived(fn: (line: string) => void) {
    onLineReceived = fn;
  },
  setOnDisconnect(fn: () => void) {
    onDisconnect = fn;
  },
};

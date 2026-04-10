import type { Transport } from "./transport";

const BAUD_RATE = 115200;

let port: SerialPort | null = null;
let reader: ReadableStreamDefaultReader<string> | null = null;
let writer: WritableStreamDefaultWriter<Uint8Array> | null = null;
let abortController: AbortController | null = null;
let buffer = "";

let onLineReceived: ((line: string) => void) | null = null;
let onDisconnect: (() => void) | null = null;

async function connect(): Promise<void> {
  port = await navigator.serial.requestPort();
  await port.open({ baudRate: BAUD_RATE });

  writer = port.writable!.getWriter();

  abortController = new AbortController();
  const decoder = new TextDecoderStream();
  (port.readable as unknown as ReadableStream)
    .pipeTo(decoder.writable, { signal: abortController.signal })
    .catch(() => {});
  reader = decoder.readable.getReader();

  readLines();

  port.addEventListener("disconnect", () => {
    cleanup();
    onDisconnect?.();
  });
}

async function readLines(): Promise<void> {
  try {
    while (true) {
      const { value, done } = await reader!.read();
      if (done) break;
      buffer += value;
      const lines = buffer.split("\n");
      buffer = lines.pop() ?? "";
      for (const line of lines) {
        const trimmed = line.trim();
        if (trimmed) onLineReceived?.(trimmed);
      }
    }
  } catch {
    // Port closed or error
  }
}

async function send(text: string): Promise<void> {
  if (!writer) return;
  const encoder = new TextEncoder();
  await writer.write(encoder.encode(text + "\n"));
}

async function disconnect(): Promise<void> {
  await cleanup();
}

async function cleanup(): Promise<void> {
  const r = reader;
  const w = writer;
  const p = port;
  const ac = abortController;
  reader = null;
  writer = null;
  port = null;
  abortController = null;
  buffer = "";

  try {
    ac?.abort();
  } catch {}
  try {
    await r?.cancel();
  } catch {}
  try {
    r?.releaseLock();
  } catch {}
  try {
    await w?.close();
  } catch {}
  try {
    w?.releaseLock();
  } catch {}
  try {
    await p?.close();
  } catch {}
}

function isConnected(): boolean {
  return port !== null;
}

export const serial: Transport = {
  connect,
  disconnect,
  send,
  isConnected,
  setOnLineReceived(fn: (line: string) => void) {
    onLineReceived = fn;
  },
  setOnDisconnect(fn: () => void) {
    onDisconnect = fn;
  },
};

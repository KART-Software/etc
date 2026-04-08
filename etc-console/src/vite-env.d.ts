/// <reference types="vite/client" />

// Web Serial API types (not yet in lib.dom.d.ts)
interface SerialPort extends EventTarget {
  readonly readable: ReadableStream<Uint8Array>;
  readonly writable: WritableStream<Uint8Array>;
  open(options: { baudRate: number }): Promise<void>;
  close(): Promise<void>;
  addEventListener(type: "disconnect", listener: () => void): void;
}

interface Serial extends EventTarget {
  requestPort(): Promise<SerialPort>;
}

interface Navigator {
  readonly serial: Serial;
}

import type {
  SensorData,
  DebugMessage,
  ResponseMessage,
  Message,
} from "./types";
import type { Transport } from "./transport";

const RESPONSE_TIMEOUT = 3000;

let commandId = 0;
let transport: Transport | null = null;
const pending = new Map<
  number,
  {
    resolve: (msg: ResponseMessage) => void;
    reject: (err: Error) => void;
    timer: ReturnType<typeof setTimeout>;
  }
>();

let onSensorData: ((data: SensorData) => void) | null = null;
let onDebugLog: ((msg: string, ts: number) => void) | null = null;

function handleLine(line: string): void {
  let msg: Message;
  try {
    msg = JSON.parse(line);
  } catch {
    return;
  }

  switch (msg.t) {
    case "s":
      onSensorData?.(msg);
      break;
    case "d":
      onDebugLog?.(msg.msg, msg.ts);
      break;
    case "r":
      resolvePending(msg);
      break;
  }
}

function resolvePending(msg: ResponseMessage): void {
  const entry = pending.get(msg.id);
  if (!entry) return;
  clearTimeout(entry.timer);
  pending.delete(msg.id);
  entry.resolve(msg);
}

function sendCommand(
  cmd: string,
  params: Record<string, unknown> = {},
): Promise<ResponseMessage> {
  const id = ++commandId;
  const message = {
    c: cmd,
    id,
    d: Object.keys(params).length > 0 ? params : undefined,
  };

  return new Promise<ResponseMessage>((resolve, reject) => {
    const timer = setTimeout(() => {
      pending.delete(id);
      reject(new Error(`Command '${cmd}' timed out`));
    }, RESPONSE_TIMEOUT);

    pending.set(id, { resolve, reject, timer });
    transport?.send(JSON.stringify(message));
  });
}

export const protocol = {
  handleLine,
  sendCommand,
  setOnSensorData(fn: (data: SensorData) => void) {
    onSensorData = fn;
  },
  setOnDebugLog(fn: (msg: string, ts: number) => void) {
    onDebugLog = fn;
  },
  setTransport(t: Transport) {
    transport = t;
  },
};

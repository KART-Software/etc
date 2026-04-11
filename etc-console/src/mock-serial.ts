import type { Transport } from "./transport";

const SENSOR_INTERVAL = 20; // 50Hz

let connected = false;
let timer: ReturnType<typeof setInterval> | null = null;
let onLineReceived: ((line: string) => void) | null = null;
let onDisconnect: (() => void) | null = null;

// Simulated device state
let t0 = 0;
let manualMode = false;
let manualTarget = 30;
let configChanged = false;
const MOCK_MODES = ["Calib", "Normal", "Restrict"] as const;
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
const pidGains = { kP: 3.0, kI: 0.4, kD: 0.0 };
const targetCurve = { a4: 0, a3: 0, a2: 0.0087, a1: 0.13, a0: 0 };

function getFullConfig() {
  return {
    sensorValues: { ...sensorValues },
    plausibilityFlags: { ...flags },
    useIttr,
    pid: { ...pidGains },
    targetCurve: { ...targetCurve },
    configChanged,
  };
}

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
  const tgt = manualMode ? manualTarget : base;
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
    m: MOCK_MODES[Math.floor(elapsed / 3) % MOCK_MODES.length],
    manual: manualMode,
    tgt_ittr: useIttr,
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
        emit(JSON.stringify({ t: "r", id, ok: true, data: getFullConfig() }));
        break;
      }
      case "save": {
        configChanged = false;
        emit(JSON.stringify({ t: "r", id, ok: true, data: getFullConfig() }));
        break;
      }
      case "revert": {
        configChanged = false;
        emit(JSON.stringify({ t: "r", id, ok: true, data: getFullConfig() }));
        break;
      }
      case "set_plausibility_check_flags": {
        for (const key of Object.keys(flags)) {
          if (key in (data ?? {})) {
            flags[key] = (data as Record<string, boolean>)[key];
          }
        }
        configChanged = true;
        emit(JSON.stringify({ t: "r", id, ok: true, data: { ...flags } }));
        break;
      }
      case "set_ittr":
        useIttr = (data?.val as boolean) ?? false;
        configChanged = true;
        emit(JSON.stringify({ t: "r", id, ok: true }));
        break;
      case "set_pid": {
        if (data?.kP != null) pidGains.kP = data.kP as number;
        if (data?.kI != null) pidGains.kI = data.kI as number;
        if (data?.kD != null) pidGains.kD = data.kD as number;
        configChanged = true;
        emit(JSON.stringify({ t: "r", id, ok: true, data: { ...pidGains } }));
        break;
      }
      case "set_target_curve": {
        if (data?.a4 != null) targetCurve.a4 = data.a4 as number;
        if (data?.a3 != null) targetCurve.a3 = data.a3 as number;
        if (data?.a2 != null) targetCurve.a2 = data.a2 as number;
        if (data?.a1 != null) targetCurve.a1 = data.a1 as number;
        if (data?.a0 != null) targetCurve.a0 = data.a0 as number;
        configChanged = true;
        emit(
          JSON.stringify({ t: "r", id, ok: true, data: { ...targetCurve } }),
        );
        break;
      }
      case "set_manual":
        manualMode = !manualMode;
        if (manualMode) manualTarget = 30;
        emit(JSON.stringify({ t: "r", id, ok: manualMode || true }));
        break;
      case "manual_adjust": {
        const amount = (data?.amount as number) ?? 0;
        manualTarget = Math.max(-10, Math.min(110, manualTarget + amount));
        emit(JSON.stringify({ t: "r", id, ok: true }));
        break;
      }
      case "set_apps_min": {
        sensorValues.apps1Min = 200 + Math.round(Math.random() * 50);
        sensorValues.apps2Min = 200 + Math.round(Math.random() * 50);
        sensorValues.ittrMin = 100 + Math.round(Math.random() * 50);
        configChanged = true;
        emit(
          JSON.stringify({
            t: "r",
            id,
            ok: true,
            data: {
              apps1Min: sensorValues.apps1Min,
              apps2Min: sensorValues.apps2Min,
              ittrMin: sensorValues.ittrMin,
            },
          }),
        );
        break;
      }
      case "set_apps_max": {
        sensorValues.apps1Max = 3700 + Math.round(Math.random() * 200);
        sensorValues.apps2Max = 3700 + Math.round(Math.random() * 200);
        sensorValues.ittrMax = 3900 + Math.round(Math.random() * 200);
        configChanged = true;
        emit(
          JSON.stringify({
            t: "r",
            id,
            ok: true,
            data: {
              apps1Max: sensorValues.apps1Max,
              apps2Max: sensorValues.apps2Max,
              ittrMax: sensorValues.ittrMax,
            },
          }),
        );
        break;
      }
      case "set_tps_min": {
        sensorValues.tps1Min = 300 + Math.round(Math.random() * 50);
        sensorValues.tps2Min = 300 + Math.round(Math.random() * 50);
        configChanged = true;
        emit(
          JSON.stringify({
            t: "r",
            id,
            ok: true,
            data: {
              tps1Min: sensorValues.tps1Min,
              tps2Min: sensorValues.tps2Min,
            },
          }),
        );
        break;
      }
      case "set_tps_max": {
        sensorValues.tps1Max = 3500 + Math.round(Math.random() * 200);
        sensorValues.tps2Max = 3500 + Math.round(Math.random() * 200);
        configChanged = true;
        emit(
          JSON.stringify({
            t: "r",
            id,
            ok: true,
            data: {
              tps1Max: sensorValues.tps1Max,
              tps2Max: sensorValues.tps2Max,
            },
          }),
        );
        break;
      }
      case "set_idling": {
        sensorValues.idling = +(4 + Math.random() * 3).toFixed(1);
        configChanged = true;
        emit(
          JSON.stringify({
            t: "r",
            id,
            ok: true,
            data: { idling: sensorValues.idling },
          }),
        );
        break;
      }
      case "set_config": {
        const jsonStr = data?.config as string | undefined;
        const ok = !!jsonStr;
        if (ok) configChanged = true;
        emit(JSON.stringify({ t: "r", id, ok }));
        break;
      }
      case "reboot":
        emit(JSON.stringify({ t: "r", id, ok: true }));
        break;
      case "motor_off":
        emit(JSON.stringify({ t: "r", id, ok: true }));
        break;
      default:
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

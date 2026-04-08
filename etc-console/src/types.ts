export interface SensorData {
  t: "s";
  ts: number;
  a1r: number;
  a2r: number;
  a1: number;
  a2: number;
  ir: number;
  i: number;
  t1r: number;
  t2r: number;
  t1: number;
  t2: number;
  br: number;
  b: number;
  tgt: number;
  m: string;
  v: boolean;
  err: number[];
}

export interface DebugMessage {
  t: "d";
  ts: number;
  msg: string;
}

export interface ResponseMessage {
  t: "r";
  id: number;
  ok: boolean;
  data?: Record<string, unknown>;
}

export type Message = SensorData | DebugMessage | ResponseMessage;

export interface DeviceConfig {
  sensorValues: {
    apps1Min: number;
    apps1Max: number;
    apps2Min: number;
    apps2Max: number;
    ittrMin: number;
    ittrMax: number;
    tps1Min: number;
    tps1Max: number;
    tps2Min: number;
    tps2Max: number;
    idling: number;
  };
  plausibilityFlags: Record<string, boolean>;
  useIttr: boolean;
  calibrating: boolean;
}

import { useState, useEffect } from "preact/hooks";
import type { SensorData, DeviceConfig } from "../types";
import { protocol } from "../protocol";
import { sensorStore } from "../sensor-store";

interface RawBarProps {
  label: string;
  value: number;
  scaleMax: number;
  markerMin?: number;
  markerMax?: number;
  color: string;
}

function RawBar({ label, value, scaleMax, markerMin, markerMax, color }: RawBarProps) {
  const pct = (Math.max(0, Math.min(scaleMax, value)) / scaleMax) * 100;
  const minPct = markerMin != null ? (markerMin / scaleMax) * 100 : null;
  const maxPct = markerMax != null ? (markerMax / scaleMax) * 100 : null;

  return (
    <div class="bar-gauge">
      <div class="bar-label">
        <span>{label}</span>
        <span class="bar-value">{value}</span>
      </div>
      <div class="bar-track raw-bar-track">
        <div class="bar-fill" style={{ width: `${pct}%`, background: color }} />
        {minPct != null && (
          <div class="bar-marker bar-marker-min" style={{ left: `${minPct}%` }} title={`min: ${markerMin}`} />
        )}
        {maxPct != null && (
          <div class="bar-marker bar-marker-max" style={{ left: `${maxPct}%` }} title={`max: ${markerMax}`} />
        )}
      </div>
    </div>
  );
}

interface Props {
  config: DeviceConfig | null;
  addLog: (msg: string) => void;
  onConfigUpdate: (partial: Partial<DeviceConfig["sensorValues"]>) => void;
  onDirty: () => void;
}

const SCALE_MAX = 65535;

export function RawBarGauges({ config, addLog, onConfigUpdate, onDirty }: Props) {
  const [data, setData] = useState<SensorData | null>(null);

  useEffect(() => {
    const iv = setInterval(() => {
      const d = sensorStore.latest;
      if (d) setData(d);
    }, 100); // 10Hz
    return () => clearInterval(iv);
  }, []);

  const sv = config?.sensorValues;

  async function calibrate(name: string) {
    try {
      const resp = await protocol.sendCommand(name);
      if (resp.ok && resp.data) {
        onConfigUpdate(resp.data as Partial<DeviceConfig["sensorValues"]>);
        onDirty();
      }
    } catch (err) {
      addLog("Command error: " + (err as Error).message);
    }
  }

  if (!data) {
    return (
      <section>
        <h2>Raw Sensors</h2>
        <p style={{ color: "var(--text-dim)", fontSize: "0.85rem" }}>No data</p>
      </section>
    );
  }

  return (
    <section>
      <h2>Raw Sensors</h2>
      <div class="raw-sensors-layout">
        <RawBar label="APPS1" value={data.a1r} scaleMax={SCALE_MAX} markerMin={sv?.apps1Min} markerMax={sv?.apps1Max} color="var(--accent)" />
        <RawBar label="APPS2" value={data.a2r} scaleMax={SCALE_MAX} markerMin={sv?.apps2Min} markerMax={sv?.apps2Max} color="var(--accent)" />
        <RawBar label="ITTR" value={data.ir} scaleMax={SCALE_MAX} markerMin={sv?.ittrMin} markerMax={sv?.ittrMax} color="var(--warn)" />
        <RawBar label="TPS1" value={data.t1r} scaleMax={SCALE_MAX} markerMin={sv?.tps1Min} markerMax={sv?.tps1Max} color="var(--ok)" />
        <RawBar label="TPS2" value={data.t2r} scaleMax={SCALE_MAX} markerMin={sv?.tps2Min} markerMax={sv?.tps2Max} color="var(--ok)" />
        <RawBar label="BPS" value={data.br} scaleMax={SCALE_MAX} color="var(--err)" />
        <div />
        <div class="raw-cal-group raw-cal-apps">
          <span class="raw-cal-title">APPS</span>
          <div class="raw-cal-row">
            <button class="raw-cal-btn" onClick={() => calibrate("set_apps_min")}>Min</button>
            <button class="raw-cal-btn" onClick={() => calibrate("set_apps_max")}>Max</button>
          </div>
        </div>
        <div class="raw-cal-group raw-cal-tps">
          <span class="raw-cal-title">TPS</span>
          <div class="raw-cal-row">
            <button class="raw-cal-btn" onClick={() => calibrate("set_tps_min")}>Min</button>
            <button class="raw-cal-btn" onClick={() => calibrate("set_tps_max")}>Max</button>
          </div>
          <button class="raw-cal-btn" style={{ marginTop: "8px" }} onClick={() => calibrate("set_idling")}>Idling</button>
        </div>
      </div>
    </section>
  );
}

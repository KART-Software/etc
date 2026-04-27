import { useState, useEffect, useRef } from "preact/hooks";
import type { SensorData } from "../types";
import { protocol } from "../protocol";
import { sensorStore } from "../sensor-store";

interface BarProps {
  label: string;
  value: number;
  min: number;
  max: number;
  color: string;
}

function Bar({ label, value, min, max, color }: BarProps) {
  const v = value ?? 0;
  const clamped = Math.max(min, Math.min(max, v));
  const pct = ((clamped - min) / (max - min)) * 100;
  return (
    <div class="bar-gauge">
      <div class="bar-label">
        <span>{label}</span>
        <span class="bar-value">{v.toFixed(1)}</span>
      </div>
      <div class="bar-track">
        <div class="bar-fill" style={{ width: `${pct}%`, background: color }} />
      </div>
    </div>
  );
}

interface Props {
  addLog: (msg: string) => void;
  onDirty: () => void;
}

export function BarGauges({ addLog, onDirty }: Props) {
  const [data, setData] = useState<SensorData | null>(null);

  useEffect(() => {
    const iv = setInterval(() => {
      const d = sensorStore.latest;
      if (d) setData(d);
    }, 100); // 10Hz
    return () => clearInterval(iv);
  }, []);
  if (!data) {
    return (
      <section>
        <h2>Bar Gauges</h2>
        <p style={{ color: "var(--text-dim)", fontSize: "0.85rem" }}>No data</p>
      </section>
    );
  }

  function toggleManual() {
    protocol.sendCommand("set_manual").catch((err: Error) => addLog("Command error: " + err.message));
  }

  function toggleIttr() {
    protocol.sendCommand("set_ittr", { val: !data!.tgt_ittr })
      .then(() => onDirty())
      .catch((err: Error) => addLog("Command error: " + err.message));
  }

  function adjust(amount: number) {
    protocol.sendCommand("manual_adjust", { amount }).catch((err: Error) => addLog("Command error: " + err.message));
  }

  return (
    <section>
      <h2>Bar Gauges</h2>
      <div class="bar-gauges-grid">
        <Bar label="APPS1" value={data.a1} min={0} max={100} color="var(--accent)" />
        <Bar label="APPS2" value={data.a2} min={0} max={100} color="var(--accent)" />
        <Bar label="ITTR" value={data.i} min={0} max={100} color="var(--warn)" />
        <div class="bar-gauge">
          <div class="bar-label">
            <span>Target</span>
            <label class={`target-source-toggle toggle-label${data.manual ? " disabled" : ""}`}>
              <span class="toggle-side-label">APPS</span>
              <div class={`toggle-switch ${data.tgt_ittr ? "on" : ""}`} onClick={data.manual ? undefined : toggleIttr}>
                <div class="toggle-knob" />
              </div>
              <span class="toggle-side-label">ITTR</span>
            </label>
            <span class="bar-value">{(data.tgt ?? 0).toFixed(1)}</span>
          </div>
          <div class="bar-track">
            <div class="bar-fill" style={{ width: `${((Math.max(0, Math.min(100, data.tgt ?? 0)) / 100) * 100)}%`, background: "#a78bfa" }} />
          </div>
        </div>
        <div class="target-info-row">
          <label class="target-info-item toggle-label">
            <span class="target-info-label">Manual</span>
            <div class={`toggle-switch ${data.manual ? "on" : ""}`} onClick={toggleManual}>
              <div class="toggle-knob" />
            </div>
          </label>
          <span class="target-info-item manual-adjust-buttons">
            {[-10, -1, -0.1, 0.1, 1, 10].map((v) => (
              <button class="adjust-btn" disabled={!data.manual} onClick={() => adjust(v)}>
                {v > 0 ? `+${v}` : v}
              </button>
            ))}
          </span>
        </div>
        <Bar label="TPS1" value={data.t1} min={0} max={100} color="var(--ok)" />
        <Bar label="TPS2" value={data.t2} min={0} max={100} color="var(--ok)" />
        <Bar label="BPS" value={data.b} min={0} max={1000} color="var(--err)" />
      </div>
    </section>
  );
}

import type { SensorData } from "../types";

interface BarProps {
  label: string;
  value: number;
  min: number;
  max: number;
  color: string;
}

function Bar({ label, value, min, max, color }: BarProps) {
  const clamped = Math.max(min, Math.min(max, value));
  const pct = ((clamped - min) / (max - min)) * 100;
  return (
    <div class="bar-gauge">
      <div class="bar-label">
        <span>{label}</span>
        <span class="bar-value">{value.toFixed(1)}</span>
      </div>
      <div class="bar-track">
        <div class="bar-fill" style={{ width: `${pct}%`, background: color }} />
      </div>
    </div>
  );
}

interface Props {
  data: SensorData | null;
}

export function BarGauges({ data }: Props) {
  if (!data) {
    return (
      <section>
        <h2>Bar Gauges</h2>
        <p style={{ color: "var(--text-dim)", fontSize: "0.85rem" }}>No data</p>
      </section>
    );
  }

  return (
    <section>
      <h2>Bar Gauges</h2>
      <div class="bar-gauges-grid">
        <Bar label="APPS1" value={data.a1} min={0} max={100} color="var(--accent)" />
        <Bar label="APPS2" value={data.a2} min={0} max={100} color="var(--accent)" />
        <Bar label="TPS1" value={data.t1} min={0} max={100} color="var(--ok)" />
        <Bar label="TPS2" value={data.t2} min={0} max={100} color="var(--ok)" />
        <Bar label="ITTR" value={data.i} min={0} max={100} color="var(--warn)" />
        <Bar label="BPS" value={data.b} min={0} max={1000} color="var(--err)" />
        <Bar label="Target" value={data.tgt} min={0} max={100} color="#a78bfa" />
      </div>
    </section>
  );
}

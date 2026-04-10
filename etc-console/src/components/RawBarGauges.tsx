import type { SensorData, DeviceConfig } from "../types";

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
  data: SensorData | null;
  config: DeviceConfig | null;
}

const SCALE_MAX = 65535;

export function RawBarGauges({ data, config }: Props) {
  const sv = config?.sensorValues;

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
      <div class="bar-gauges-grid">
        <RawBar label="APPS1" value={data.a1r} scaleMax={SCALE_MAX} markerMin={sv?.apps1Min} markerMax={sv?.apps1Max} color="var(--accent)" />
        <RawBar label="APPS2" value={data.a2r} scaleMax={SCALE_MAX} markerMin={sv?.apps2Min} markerMax={sv?.apps2Max} color="var(--accent)" />
        <RawBar label="TPS1" value={data.t1r} scaleMax={SCALE_MAX} markerMin={sv?.tps1Min} markerMax={sv?.tps1Max} color="var(--ok)" />
        <RawBar label="TPS2" value={data.t2r} scaleMax={SCALE_MAX} markerMin={sv?.tps2Min} markerMax={sv?.tps2Max} color="var(--ok)" />
        <RawBar label="ITTR" value={data.ir} scaleMax={SCALE_MAX} markerMin={sv?.ittrMin} markerMax={sv?.ittrMax} color="var(--warn)" />
        <RawBar label="BPS" value={data.br} scaleMax={SCALE_MAX} color="var(--err)" />
      </div>
    </section>
  );
}

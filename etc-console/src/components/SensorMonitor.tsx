import { useState, useEffect } from "preact/hooks";
import type { SensorData } from "../types";
import { sensorStore } from "../sensor-store";

interface Props {}

function fmt(v: number | undefined | null): string {
  return v != null ? v.toFixed(1) : "-";
}

function SensorRow({ label, raw, val, unit }: { label: string; raw?: number; val?: number; unit: string }) {
  return (
    <div class="sensor-row">
      <span class="label">{label}</span>
      {raw != null && <span class="raw">{raw}</span>}
      <span class="val">{fmt(val)}</span>
      <span class="unit">{unit}</span>
    </div>
  );
}

export function SensorMonitor(_props: Props) {
  const [data, setData] = useState<SensorData | null>(null);

  useEffect(() => {
    const iv = setInterval(() => {
      const d = sensorStore.latest;
      if (d) setData(d);
    }, 100); // 10Hz
    return () => clearInterval(iv);
  }, []);
  return (
    <section>
      <h2>Sensor Monitor</h2>
      <div class="sensor-grid">
        <div class="sensor-grid-row">
          <div class="sensor-group">
            <h3>APPS</h3>
            <SensorRow label="APPS1" raw={data?.a1r} val={data?.a1} unit="%" />
            <SensorRow label="APPS2" raw={data?.a2r} val={data?.a2} unit="%" />
          </div>
          <div class="sensor-group">
            <h3>ITTR</h3>
            <SensorRow label="ITTR" raw={data?.ir} val={data?.i} unit="%" />
          </div>
          <div class="sensor-group">
            <h3>Target</h3>
            <div class="sensor-row">
              <span class="label">Target</span>
              <span class="val">{fmt(data?.tgt)}</span>
              <span class="unit">%</span>
            </div>
          </div>
        </div>
        <div class="sensor-grid-row">
          <div class="sensor-group">
            <h3>TPS</h3>
            <SensorRow label="TPS1" raw={data?.t1r} val={data?.t1} unit="%" />
            <SensorRow label="TPS2" raw={data?.t2r} val={data?.t2} unit="%" />
          </div>
          <div class="sensor-group">
            <h3>BPS</h3>
            <SensorRow label="BPS" raw={data?.br} val={data?.b} unit="psi" />
          </div>
        </div>
      </div>
    </section>
  );
}

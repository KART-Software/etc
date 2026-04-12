import { useState, useEffect } from "preact/hooks";
import { protocol } from "../protocol";

interface TargetCurve {
  a4: number;
  a3: number;
  a2: number;
  a1: number;
}

interface Props {
  config: { targetCurve?: TargetCurve } | null;
  addLog: (msg: string) => void;
  onDirty: () => void;
  onCurveUpdate: (curve: TargetCurve) => void;
  onPreview?: (curve: TargetCurve) => void;
}

export function TargetCurveTuner({ config, addLog, onDirty, onCurveUpdate, onPreview }: Props) {
  const [a4, setA4] = useState("");
  const [a3, setA3] = useState("");
  const [a2, setA2] = useState("");
  const [a1, setA1] = useState("");
  const [open, setOpen] = useState(false);

  // Display scales: UI value = internal value * scale
  const S4 = 1e6, S3 = 1e4, S2 = 100;

  function emitPreview(v4: string, v3: string, v2: string, v1: string) {
    const vals = [v4, v3, v2, v1].map(parseFloat);
    if (vals.some(Number.isNaN)) return;
    onPreview?.({ a4: vals[0] / S4, a3: vals[1] / S3, a2: vals[2] / S2, a1: vals[3] });
  }

  useEffect(() => {
    if (config?.targetCurve) {
      const c = config.targetCurve;
      setA4(String((c.a4 ?? 0) * S4));
      setA3(String((c.a3 ?? 0) * S3));
      setA2(String((c.a2 ?? 0) * S2));
      setA1(String(c.a1 ?? 0));
    }
  }, [config?.targetCurve?.a4, config?.targetCurve?.a3, config?.targetCurve?.a2, config?.targetCurve?.a1]);

  async function send() {
    const vals = [a4, a3, a2, a1].map(parseFloat);
    if (vals.some(Number.isNaN)) {
      addLog("Curve: invalid number");
      return;
    }
    // Convert display values back to internal values
    const internal = { a4: vals[0] / S4, a3: vals[1] / S3, a2: vals[2] / S2, a1: vals[3] };
    try {
      const resp = await protocol.sendCommand("set_target_curve", internal);
      if (resp.ok && resp.data) {
        const updated: TargetCurve = {
          a4: (resp.data.a4 as number) ?? internal.a4,
          a3: (resp.data.a3 as number) ?? internal.a3,
          a2: (resp.data.a2 as number) ?? internal.a2,
          a1: (resp.data.a1 as number) ?? internal.a1,
        };
        onCurveUpdate(updated);
        addLog(`Curve set: a4=${updated.a4} a3=${updated.a3} a2=${updated.a2} a1=${updated.a1}`);
        onDirty();
      } else {
        addLog("Curve set failed");
      }
    } catch (err) {
      addLog("Curve error: " + (err as Error).message);
    }
  }

  return (
    <section>
      <div class="pid-header" onClick={() => setOpen(!open)}>
        <h2>Target Curve</h2>
        {config?.targetCurve && (
          <span class="pid-current">
            <b>{config.targetCurve.a4}</b> x⁴ + <b>{config.targetCurve.a3}</b> x³ + <b>{config.targetCurve.a2}</b> x² + <b>{config.targetCurve.a1}</b> x
          </span>
        )}
        <span class={`pid-chevron ${open ? "open" : ""}`}>▶</span>
      </div>
      {open && (
        <div class="pid-row">
          <label class="pid-field">
            <span>a4 (×10⁻⁶)</span>
            <input type="number" step="0.1" value={a4} onInput={(e) => { const v = (e.target as HTMLInputElement).value; setA4(v); emitPreview(v, a3, a2, a1); }} />
          </label>
          <label class="pid-field">
            <span>a3 (×10⁻⁴)</span>
            <input type="number" step="0.1" value={a3} onInput={(e) => { const v = (e.target as HTMLInputElement).value; setA3(v); emitPreview(a4, v, a2, a1); }} />
          </label>
          <label class="pid-field">
            <span>a2 (×10⁻²)</span>
            <input type="number" step="0.1" value={a2} onInput={(e) => { const v = (e.target as HTMLInputElement).value; setA2(v); emitPreview(a4, a3, v, a1); }} />
          </label>
          <label class="pid-field">
            <span>a1</span>
            <input type="number" step="0.01" value={a1} onInput={(e) => { const v = (e.target as HTMLInputElement).value; setA1(v); emitPreview(a4, a3, a2, v); }} />
          </label>
          <button class="pid-apply" onClick={send}>Apply</button>
        </div>
      )}
    </section>
  );
}

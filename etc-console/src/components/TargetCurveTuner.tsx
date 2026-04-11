import { useState, useEffect } from "preact/hooks";
import { protocol } from "../protocol";

interface TargetCurve {
  a4: number;
  a3: number;
  a2: number;
  a1: number;
  a0: number;
}

interface Props {
  config: { targetCurve?: TargetCurve } | null;
  addLog: (msg: string) => void;
  onDirty: () => void;
  onCurveUpdate: (curve: TargetCurve) => void;
}

export function TargetCurveTuner({ config, addLog, onDirty, onCurveUpdate }: Props) {
  const [a4, setA4] = useState("");
  const [a3, setA3] = useState("");
  const [a2, setA2] = useState("");
  const [a1, setA1] = useState("");
  const [a0, setA0] = useState("");
  const [open, setOpen] = useState(false);

  useEffect(() => {
    if (config?.targetCurve) {
      const c = config.targetCurve;
      setA4(String(c.a4 ?? 0));
      setA3(String(c.a3 ?? 0));
      setA2(String(c.a2 ?? 0));
      setA1(String(c.a1 ?? 0));
      setA0(String(c.a0 ?? 0));
    }
  }, [config?.targetCurve?.a4, config?.targetCurve?.a3, config?.targetCurve?.a2, config?.targetCurve?.a1, config?.targetCurve?.a0]);

  async function send() {
    const vals = [a4, a3, a2, a1, a0].map(parseFloat);
    if (vals.some(Number.isNaN)) {
      addLog("Curve: invalid number");
      return;
    }
    try {
      const resp = await protocol.sendCommand("set_target_curve", {
        a4: vals[0], a3: vals[1], a2: vals[2], a1: vals[3], a0: vals[4],
      });
      if (resp.ok && resp.data) {
        const updated: TargetCurve = {
          a4: (resp.data.a4 as number) ?? vals[0],
          a3: (resp.data.a3 as number) ?? vals[1],
          a2: (resp.data.a2 as number) ?? vals[2],
          a1: (resp.data.a1 as number) ?? vals[3],
          a0: (resp.data.a0 as number) ?? vals[4],
        };
        onCurveUpdate(updated);
        addLog(`Curve set: a4=${updated.a4} a3=${updated.a3} a2=${updated.a2} a1=${updated.a1} a0=${updated.a0}`);
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
            {config.targetCurve.a4}x⁴ + {config.targetCurve.a3}x³ + {config.targetCurve.a2}x² + {config.targetCurve.a1}x + {config.targetCurve.a0}
          </span>
        )}
        <span class={`pid-chevron ${open ? "open" : ""}`}>▶</span>
      </div>
      {open && (
        <div class="pid-row">
          <label class="pid-field">
            <span>a4</span>
            <input type="number" step="0.0000001" value={a4} onInput={(e) => setA4((e.target as HTMLInputElement).value)} />
          </label>
          <label class="pid-field">
            <span>a3</span>
            <input type="number" step="0.000001" value={a3} onInput={(e) => setA3((e.target as HTMLInputElement).value)} />
          </label>
          <label class="pid-field">
            <span>a2</span>
            <input type="number" step="0.0001" value={a2} onInput={(e) => setA2((e.target as HTMLInputElement).value)} />
          </label>
          <label class="pid-field">
            <span>a1</span>
            <input type="number" step="0.01" value={a1} onInput={(e) => setA1((e.target as HTMLInputElement).value)} />
          </label>
          <label class="pid-field">
            <span>a0</span>
            <input type="number" step="0.1" value={a0} onInput={(e) => setA0((e.target as HTMLInputElement).value)} />
          </label>
          <button class="pid-apply" onClick={send}>Apply</button>
        </div>
      )}
    </section>
  );
}

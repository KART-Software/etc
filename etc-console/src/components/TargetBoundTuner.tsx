import { useState, useEffect } from "preact/hooks";
import { protocol } from "../protocol";

interface TargetBound {
  idling: number;
  normalMax: number;
  restrictedMax: number;
}

interface Props {
  config: { sensorValues?: TargetBound } | null;
  addLog: (msg: string) => void;
  onDirty: () => void;
  onBoundUpdate: (bound: TargetBound) => void;
}

export function TargetBoundTuner({ config, addLog, onDirty, onBoundUpdate }: Props) {
  const [idling, setIdling] = useState("");
  const [normalMax, setNormalMax] = useState("");
  const [restrictedMax, setRestrictedMax] = useState("");
  const [open, setOpen] = useState(false);

  useEffect(() => {
    if (config?.sensorValues) {
      setIdling(String(config.sensorValues.idling ?? 0));
      setNormalMax(String(config.sensorValues.normalMax ?? 0));
      setRestrictedMax(String(config.sensorValues.restrictedMax ?? 0));
    }
  }, [config?.sensorValues?.idling, config?.sensorValues?.normalMax, config?.sensorValues?.restrictedMax]);

  async function send() {
    const i = parseFloat(idling);
    const n = parseFloat(normalMax);
    const r = parseFloat(restrictedMax);
    if ([i, n, r].some(Number.isNaN)) {
      addLog("TargetBound: invalid number");
      return;
    }
    try {
      const resp = await protocol.sendCommand("set_target_bound", { idling: i, normalMax: n, restrictedMax: r });
      if (resp.ok && resp.data) {
        const updated: TargetBound = {
          idling: (resp.data.idling as number) ?? i,
          normalMax: (resp.data.normalMax as number) ?? n,
          restrictedMax: (resp.data.restrictedMax as number) ?? r,
        };
        onBoundUpdate(updated);
        addLog(`TargetBound set: idling=${updated.idling} normalMax=${updated.normalMax} restrictedMax=${updated.restrictedMax}`);
        onDirty();
      } else {
        addLog("TargetBound set failed");
      }
    } catch (err) {
      addLog("TargetBound error: " + (err as Error).message);
    }
  }

  return (
    <section>
      <div class="pid-header" onClick={() => setOpen(!open)}>
        <h2>Target Bounds</h2>
        {config?.sensorValues && (
          <span class="pid-current">
            Idling: <b>{config.sensorValues.idling}</b>　Normal Max: <b>{config.sensorValues.normalMax}</b>　Restricted Max: <b>{config.sensorValues.restrictedMax}</b>
          </span>
        )}
        <span class={`pid-chevron ${open ? "open" : ""}`}>▶</span>
      </div>
      {open && (
        <div class="pid-row">
          <label class="pid-field">
            <span>Idling</span>
            <input type="number" step="1" value={idling} onInput={(e) => setIdling((e.target as HTMLInputElement).value)} />
          </label>
          <label class="pid-field">
            <span>Normal Max</span>
            <input type="number" step="1" value={normalMax} onInput={(e) => setNormalMax((e.target as HTMLInputElement).value)} />
          </label>
          <label class="pid-field">
            <span>Restricted Max</span>
            <input type="number" step="1" value={restrictedMax} onInput={(e) => setRestrictedMax((e.target as HTMLInputElement).value)} />
          </label>
          <button class="pid-apply" onClick={send}>Apply</button>
        </div>
      )}
    </section>
  );
}

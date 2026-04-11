import { useState, useEffect } from "preact/hooks";
import { protocol } from "../protocol";

interface PidGains {
  kP: number;
  kI: number;
  kD: number;
}

interface Props {
  config: { pid?: PidGains } | null;
  addLog: (msg: string) => void;
  onDirty: () => void;
  onPidUpdate: (pid: PidGains) => void;
}

export function PidTuner({ config, addLog, onDirty, onPidUpdate }: Props) {
  const [kP, setKP] = useState("");
  const [kI, setKI] = useState("");
  const [kD, setKD] = useState("");
  const [open, setOpen] = useState(false);

  useEffect(() => {
    if (config?.pid) {
      setKP(String(config.pid.kP ?? 0));
      setKI(String(config.pid.kI ?? 0));
      setKD(String(config.pid.kD ?? 0));
    }
  }, [config?.pid?.kP, config?.pid?.kI, config?.pid?.kD]);

  async function send() {
    const p = parseFloat(kP);
    const i = parseFloat(kI);
    const d = parseFloat(kD);
    if ([p, i, d].some(Number.isNaN)) {
      addLog("PID: invalid number");
      return;
    }
    try {
      const resp = await protocol.sendCommand("set_pid", { kP: p, kI: i, kD: d });
      if (resp.ok && resp.data) {
        const updated: PidGains = {
          kP: (resp.data.kP as number) ?? p,
          kI: (resp.data.kI as number) ?? i,
          kD: (resp.data.kD as number) ?? d,
        };
        onPidUpdate(updated);
        addLog(`PID set: kP=${updated.kP} kI=${updated.kI} kD=${updated.kD}`);
        onDirty();
      } else {
        addLog("PID set failed");
      }
    } catch (err) {
      addLog("PID error: " + (err as Error).message);
    }
  }

  return (
    <section>
      <div class="pid-header" onClick={() => setOpen(!open)}>
        <h2>PID Gains</h2>
        {config?.pid && (
          <span class="pid-current">P: {config.pid.kP}　I: {config.pid.kI}　D: {config.pid.kD}</span>
        )}
        <span class={`pid-chevron ${open ? "open" : ""}`}>▶</span>
      </div>
      {open && (
        <div class="pid-row">
          <label class="pid-field">
            <span>kP</span>
            <input type="number" step="0.1" value={kP} onInput={(e) => setKP((e.target as HTMLInputElement).value)} />
          </label>
          <label class="pid-field">
            <span>kI</span>
            <input type="number" step="0.1" value={kI} onInput={(e) => setKI((e.target as HTMLInputElement).value)} />
          </label>
          <label class="pid-field">
            <span>kD</span>
            <input type="number" step="0.01" value={kD} onInput={(e) => setKD((e.target as HTMLInputElement).value)} />
          </label>
          <button class="pid-apply" onClick={send}>Apply</button>
        </div>
      )}
    </section>
  );
}

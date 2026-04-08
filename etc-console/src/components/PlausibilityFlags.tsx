import { protocol } from "../protocol";

interface Props {
  flags: Record<string, boolean>;
  useIttr: boolean;
  addLog: (msg: string) => void;
}

const FLAG_KEYS = ["apps", "tps", "apps1", "apps2", "tps1", "tps2", "target", "bps", "bpsTps"] as const;
const FLAG_LABELS: Record<string, string> = {
  apps: "APPS", tps: "TPS", apps1: "APPS1", apps2: "APPS2",
  tps1: "TPS1", tps2: "TPS2", target: "Target", bps: "BPS", bpsTps: "BPS-TPS",
};

export function PlausibilityFlags({ flags, useIttr, addLog }: Props) {
  function onFlagChange(key: string, checked: boolean) {
    protocol.sendCommand("set_flag", { key, val: checked })
      .catch((err: Error) => addLog("Flag error: " + err.message));
  }

  function onIttrChange(checked: boolean) {
    protocol.sendCommand("set_ittr", { val: checked })
      .catch((err: Error) => addLog("ITTR error: " + err.message));
  }

  return (
    <section>
      <h2>Plausibility Check Flags</h2>
      <div class="flag-grid">
        {FLAG_KEYS.map((key) => (
          <label class="flag-item" key={key}>
            <input
              type="checkbox"
              checked={flags[key] ?? false}
              onChange={(e) => onFlagChange(key, (e.target as HTMLInputElement).checked)}
            />
            <span>{FLAG_LABELS[key]}</span>
          </label>
        ))}
      </div>
      <div class="controls-row" style={{ marginTop: "8px" }}>
        <label class="flag-item">
          <input
            type="checkbox"
            checked={useIttr}
            onChange={(e) => onIttrChange((e.target as HTMLInputElement).checked)}
          />
          <span>Use ITTR</span>
        </label>
      </div>
    </section>
  );
}

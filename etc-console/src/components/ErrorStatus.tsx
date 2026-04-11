import { protocol } from "../protocol";

const ERROR_LABELS: Record<number, string> = {
  0: "TPS Implausible",
  1: "APPS Implausible",
  2: "TPS1 Circuit",
  3: "TPS2 Circuit",
  4: "APPS1 Circuit",
  5: "APPS2 Circuit",
  6: "APPS-TPS Target",
  7: "BPS Circuit",
  8: "BPS-TPS Implausible",
};

const ERROR_TO_FLAG: Record<number, string> = {
  0: "tps",
  1: "apps",
  2: "tps1",
  3: "tps2",
  4: "apps1",
  5: "apps2",
  6: "target",
  7: "bps",
  8: "bpsTps",
};

interface Props {
  errors: number[];
  flags: Record<string, boolean>;
  addLog: (msg: string) => void;
  onFlagsUpdate: (flags: Record<string, boolean>) => void;
  onDirty: () => void;
}

export function ErrorStatus({ errors, flags, addLog, onFlagsUpdate, onDirty }: Props) {
  const errSet = new Set(errors);

  function onFlagChange(errorId: number, checked: boolean) {
    const flagKey = ERROR_TO_FLAG[errorId];
    if (!flagKey) return;
    const updated = { ...flags, [flagKey]: checked };
    protocol.sendCommand("set_plausibility_check_flags", updated)
      .then((resp) => {
        if (resp.ok && resp.data) {
          onFlagsUpdate(resp.data as Record<string, boolean>);
        }
      })
      .catch((err: Error) => addLog("Flag error: " + err.message));
    onDirty();
  }

  return (
    <section>
      <h2>Error Status</h2>
      <div class="error-grid">
        {Object.entries(ERROR_LABELS).map(([id, label]) => {
          const numId = Number(id);
          const flagKey = ERROR_TO_FLAG[numId];
          const enabled = flagKey ? (flags[flagKey] ?? false) : true;
          const hasError = errSet.has(numId);
          return (
            <div class={`error-item${enabled ? "" : " disabled"}`} key={id}>
              {flagKey && (
                <input
                  type="checkbox"
                  checked={enabled}
                  onChange={(e) => onFlagChange(numId, (e.target as HTMLInputElement).checked)}
                />
              )}
              <span class={`led${hasError ? " error" : ""}`} />
              {label}
            </div>
          );
        })}
      </div>
    </section>
  );
}

import { protocol } from "../protocol";
import type { DeviceConfig } from "../types";

interface Props {
  addLog: (msg: string) => void;
  onConfigUpdate: (partial: Partial<DeviceConfig["sensorValues"]>) => void;
}

function cmd(name: string, addLog: (msg: string) => void, params: Record<string, unknown> = {}) {
  protocol.sendCommand(name, params).catch((err: Error) => addLog("Command error: " + err.message));
}

export function Calibration({ addLog, onConfigUpdate }: Props) {
  async function calibrate(name: string) {
    try {
      const resp = await protocol.sendCommand(name);
      if (resp.ok && resp.data) {
        onConfigUpdate(resp.data as Partial<DeviceConfig["sensorValues"]>);
      }
    } catch (err) {
      addLog("Command error: " + (err as Error).message);
    }
  }

  return (
    <section>
      <h2>Calibration</h2>
      <div class="controls-row">
        <button onClick={() => cmd("save", addLog)}>Save</button>
        <button onClick={() => cmd("revert", addLog)}>Revert</button>
      </div>
      <div class="controls-row">
        <button onClick={() => calibrate("set_apps_min")}>Set APPS Min</button>
        <button onClick={() => calibrate("set_apps_max")}>Set APPS Max</button>
        <button onClick={() => calibrate("set_tps_min")}>Set TPS Min</button>
        <button onClick={() => calibrate("set_tps_max")}>Set TPS Max</button>
        <button onClick={() => calibrate("set_idling")}>Set Idling</button>
      </div>
    </section>
  );
}

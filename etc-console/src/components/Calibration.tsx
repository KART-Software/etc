import { protocol } from "../protocol";

interface Props {
  addLog: (msg: string) => void;
}

function cmd(name: string, addLog: (msg: string) => void, params: Record<string, unknown> = {}) {
  protocol.sendCommand(name, params).catch((err: Error) => addLog("Command error: " + err.message));
}

export function Calibration({ addLog }: Props) {
  return (
    <section>
      <h2>Calibration</h2>
      <div class="controls-row">
        <button class="danger" onClick={() => cmd("motor_off", addLog)}>Motor OFF</button>
        <button onClick={() => cmd("save", addLog)}>Save</button>
        <button onClick={() => cmd("revert", addLog)}>Revert</button>
      </div>
      <div class="controls-row">
        <button onClick={() => cmd("set_apps_min", addLog)}>Set APPS Min</button>
        <button onClick={() => cmd("set_apps_max", addLog)}>Set APPS Max</button>
        <button onClick={() => cmd("set_tps_min", addLog)}>Set TPS Min</button>
        <button onClick={() => cmd("set_tps_max", addLog)}>Set TPS Max</button>
        <button onClick={() => cmd("set_idling", addLog)}>Set Idling</button>
      </div>
      <div class="controls-row">
        <button onClick={() => cmd("set_manual", addLog)}>Toggle Manual</button>
        <button onClick={() => cmd("manual_adjust", addLog, { amount: -0.1 })}>Manual -</button>
        <button onClick={() => cmd("manual_adjust", addLog, { amount: 0.1 })}>Manual +</button>
        <button class="danger" onClick={() => cmd("reboot", addLog)}>Reboot</button>
      </div>
    </section>
  );
}

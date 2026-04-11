import { useRef } from "preact/hooks";
import { protocol } from "../protocol";
import type { DeviceConfig } from "../types";

interface Props {
  config: DeviceConfig | null;
  onConfigLoaded: (config: DeviceConfig) => void;
  addLog: (msg: string) => void;
}

export function ConfigPanel({ config, onConfigLoaded, addLog }: Props) {
  const fileRef = useRef<HTMLInputElement>(null);

  async function getConfig() {
    try {
      const resp = await protocol.sendCommand("get_config");
      if (resp.ok && resp.data) {
        const cfg = resp.data as unknown as DeviceConfig;
        onConfigLoaded(cfg);
        addLog("Config loaded from device");
      }
    } catch (err) {
      addLog("Failed to get config: " + (err as Error).message);
    }
  }

  function exportConfig() {
    if (!config) {
      addLog('No config loaded. Click "Get Config" first.');
      return;
    }
    const blob = new Blob([JSON.stringify(config, null, 2)], { type: "application/json" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = "etc-config.json";
    a.click();
    URL.revokeObjectURL(url);
    addLog("Config exported");
  }

  async function importConfig(e: Event) {
    const file = (e.target as HTMLInputElement).files?.[0];
    if (!file) return;
    try {
      const text = await file.text();
      const data = JSON.parse(text) as DeviceConfig;
      if (!data.sensorValues) {
        addLog("Invalid config file");
        return;
      }
      const resp = await protocol.sendCommand("set_config", { config: JSON.stringify(data) });
      if (resp.ok) {
        addLog("Config imported and applied");
        onConfigLoaded(data);
      } else {
        addLog("Device rejected config");
      }
    } catch (err) {
      addLog("Import failed: " + (err as Error).message);
    }
    if (fileRef.current) fileRef.current.value = "";
  }

  return (
    <section>
      <h2>Configuration</h2>
      <div class="controls-row">
        <button onClick={getConfig}>Get Config</button>
        <button onClick={exportConfig}>Export JSON</button>
        <button onClick={() => fileRef.current?.click()}>Import JSON</button>
        <input ref={fileRef} type="file" accept=".json" hidden onChange={importConfig} />
      </div>
    </section>
  );
}

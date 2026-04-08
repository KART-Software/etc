import { useState, useCallback, useRef, useEffect } from "preact/hooks";
import { serial } from "./serial";
import { mockSerial } from "./mock-serial";
import { protocol } from "./protocol";
import type { Transport } from "./transport";
import type { SensorData, DeviceConfig } from "./types";
import { SensorMonitor } from "./components/SensorMonitor";
import { ErrorStatus } from "./components/ErrorStatus";
import { SensorChart } from "./components/SensorChart";
import { Calibration } from "./components/Calibration";
import { PlausibilityFlags } from "./components/PlausibilityFlags";
import { ConfigPanel } from "./components/ConfigPanel";
import { DebugLog, type LogEntry } from "./components/DebugLog";

const MAX_LOG_ENTRIES = 100;
const isMock = new URLSearchParams(window.location.search).has("mock");

export function App() {
  const transportRef = useRef<Transport>(isMock ? mockSerial : serial);
  const [connected, setConnected] = useState(false);
  const [sensorData, setSensorData] = useState<SensorData | null>(null);
  const [config, setConfig] = useState<DeviceConfig | null>(null);
  const [logs, setLogs] = useState<LogEntry[]>([]);
  const logRef = useRef(logs);
  logRef.current = logs;

  const addLog = useCallback((msg: string, ts?: number) => {
    const tsStr = ts != null ? (ts / 1000).toFixed(1) + "s" : new Date().toLocaleTimeString();
    setLogs((prev) => {
      const next = [...prev, { ts: tsStr, msg }];
      return next.length > MAX_LOG_ENTRIES ? next.slice(-MAX_LOG_ENTRIES) : next;
    });
  }, []);

  // Wire protocol callbacks once
  useEffect(() => {
    const t = transportRef.current;
    protocol.setTransport(t);
    protocol.setOnSensorData((data) => setSensorData(data));
    protocol.setOnDebugLog((msg, ts) => addLog(msg, ts));
    t.setOnLineReceived((line) => protocol.handleLine(line));
    t.setOnDisconnect(() => {
      setConnected(false);
      addLog("Disconnected");
    });
    if (isMock) addLog("Mock mode enabled");
  }, [addLog]);

  const webSerialAvailable = isMock || "serial" in navigator;

  async function handleConnect() {
    try {
      await transportRef.current.connect();
      setConnected(true);
      addLog("Connected");
      setTimeout(async () => {
        try {
          const resp = await protocol.sendCommand("get_config");
          if (resp.ok && resp.data) {
            setConfig(resp.data as unknown as DeviceConfig);
            addLog("Config loaded from device");
          }
        } catch {}
      }, 500);
    } catch (err) {
      addLog("Connection failed: " + (err as Error).message);
    }
  }

  async function handleDisconnect() {
    await transportRef.current.disconnect();
    setConnected(false);
    addLog("Disconnected");
  }

  return (
    <>
      <header>
        <h1>ETC Console</h1>
        <div class="connection-controls">
          <button onClick={handleConnect} disabled={connected || !webSerialAvailable}>Connect</button>
          <button onClick={handleDisconnect} disabled={!connected}>Disconnect</button>
          <span class={`status-badge ${connected ? "connected" : "disconnected"}`}>
            {connected ? "Connected" : "Disconnected"}
          </span>
        </div>
      </header>

      <main>
        <SensorMonitor data={sensorData} />
        <ErrorStatus errors={sensorData?.err ?? []} />
        <SensorChart data={sensorData} />
        <Calibration addLog={addLog} />
        <PlausibilityFlags
          flags={config?.plausibilityFlags ?? {}}
          useIttr={config?.useIttr ?? false}
          addLog={addLog}
        />
        <ConfigPanel config={config} onConfigLoaded={setConfig} addLog={addLog} />
        <DebugLog entries={logs} />
        {!webSerialAvailable && (
          <section>
            <p style={{ color: "var(--err)" }}>Web Serial API is not available. Use Chrome or Edge.</p>
          </section>
        )}
      </main>
    </>
  );
}

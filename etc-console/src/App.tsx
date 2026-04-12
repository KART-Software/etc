import { useState, useCallback, useRef, useEffect } from "preact/hooks";
import { serial } from "./serial";
import { mockSerial } from "./mock-serial";
import { protocol } from "./protocol";
import type { Transport } from "./transport";
import type { DeviceConfig } from "./types";
import { SensorMonitor } from "./components/SensorMonitor";
import { ErrorStatus } from "./components/ErrorStatus";
import { SensorChart } from "./components/SensorChart";
import { CorrelationCharts } from "./components/CorrelationCharts";
import { ConfigPanel } from "./components/ConfigPanel";
import { DebugLog, type LogEntry } from "./components/DebugLog";
import { BarGauges } from "./components/BarGauges";
import { RawBarGauges } from "./components/RawBarGauges";
import { PidTuner } from "./components/PidTuner";
import { TargetBoundTuner } from "./components/TargetBoundTuner";
import { TargetCurveTuner } from "./components/TargetCurveTuner";
import { CurvePreview } from "./components/CurvePreview";
import { ModeKnob } from "./components/ModeKnob";
import { sensorStore } from "./sensor-store";

const MAX_LOG_ENTRIES = 100;
const isMock = new URLSearchParams(window.location.search).has("mock");

export function App() {
  const transportRef = useRef<Transport>(isMock ? mockSerial : serial);
  const [connected, setConnected] = useState(false);
  const [timeRange, setTimeRange] = useState<[number, number] | null>(null);
  const timeRangeRef = useRef(timeRange);
  const [hoverTime, setHoverTime] = useState<number | null>(null);
  const [config, setConfig] = useState<DeviceConfig | null>(null);
  const [dirty, setDirty] = useState(false);
  const [previewCurve, setPreviewCurve] = useState<{ a4: number; a3: number; a2: number; a1: number } | null>(null);
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
    sensorStore.open().then(() => sensorStore.cleanOldSessions());
    const t = transportRef.current;
    protocol.setTransport(t);

    protocol.setOnSensorData((data) => {
      sensorStore.push(data);
    });
    protocol.setOnDebugLog((msg, ts) => addLog(msg, ts));
    if (import.meta.env.DEV) {
      protocol.setOnSerialLog((dir, line) => {
        console.log(`[Serial ${dir.toUpperCase()}]`, line);
      });
    }
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
      sensorStore.startSession();
      setConnected(true);
      addLog("Connected");
      setTimeout(async () => {
        try {
          const resp = await protocol.sendCommand("get_config");
          if (resp.ok && resp.data) {
            setConfig(resp.data as unknown as DeviceConfig);
            setDirty(!!(resp.data as Record<string, unknown>).configChanged);
            addLog("Config loaded from device");
          }
        } catch {}
      }, 500);
    } catch (err) {
      addLog("Connection failed: " + (err as Error).message);
    }
  }

  async function handleDisconnect() {
    await sensorStore.flush();
    await transportRef.current.disconnect();
    setConnected(false);
    addLog("Disconnected");
  }

  function handleSave() {
    protocol.sendCommand("save")
      .then((resp) => {
        if (resp.ok && resp.data) {
          setConfig(resp.data as unknown as DeviceConfig);
          setDirty(!!(resp.data as Record<string, unknown>).configChanged);
        }
        addLog("Config saved");
      })
      .catch((err: Error) => addLog("Save error: " + err.message));
  }

  function handleRevert() {
    protocol.sendCommand("revert")
      .then((resp) => {
        if (resp.ok && resp.data) {
          setConfig(resp.data as unknown as DeviceConfig);
          setDirty(!!(resp.data as Record<string, unknown>).configChanged);
        }
        addLog("Config reverted");
      })
      .catch((err: Error) => addLog("Revert error: " + err.message));
  }

  return (
    <div id="app-root">
      <header>
        <h1>ETC Console</h1>
        <div class="header-actions">
          <button class="danger" disabled={!connected} onClick={() => protocol.sendCommand("motor_off").catch((err: Error) => addLog("Command error: " + err.message))}>Motor OFF</button>
          <button class="danger" disabled={!connected} onClick={() => protocol.sendCommand("reboot").catch((err: Error) => addLog("Command error: " + err.message))}>Reboot</button>
          <button disabled={sensorStore.totalRows === 0} onClick={async () => {
            try {
              const blob = await sensorStore.exportCsv();
              const url = URL.createObjectURL(blob);
              const a = document.createElement("a");
              a.href = url;
              a.download = `sensor-${new Date().toISOString().slice(0, 19).replace(/:/g, "")}.csv`;
              a.click();
              URL.revokeObjectURL(url);
              addLog(`CSV exported (${sensorStore.totalRows} rows)`);
            } catch (err) { addLog("CSV export error: " + (err as Error).message); }
          }}>Export CSV</button>
        </div>
        <div class="connection-controls">
          <button onClick={handleConnect} disabled={connected || !webSerialAvailable}>Connect</button>
          <button onClick={handleDisconnect} disabled={!connected}>Disconnect</button>
          <span class={`status-badge ${connected ? "connected" : "disconnected"}`}>
            {connected ? "Connected" : "Disconnected"}
          </span>
        </div>
      </header>
      {dirty && (
        <div class="action-bar">
          <span class="action-bar-label">Unsaved changes</span>
          <button onClick={handleSave}>Save</button>
          <button onClick={handleRevert}>Revert</button>
        </div>
      )}

      <main>
        <div class="area-sensors"><SensorMonitor /></div>
        <div class="area-mode">
          <ModeKnob />
        </div>
        <div class="area-errors"><ErrorStatus flags={config?.plausibilityFlags ?? {}} addLog={addLog} onFlagsUpdate={(pf) => {
          setConfig((prev) => prev ? { ...prev, plausibilityFlags: pf } : prev);
        }} onDirty={() => setDirty(true)} /></div>
        <div class="area-gauges">
          <div class="gauges-col"><BarGauges addLog={addLog} onDirty={() => setDirty(true)} /></div>
          <div class="gauges-col"><RawBarGauges config={config} addLog={addLog} onConfigUpdate={(partial) => {
            setConfig((prev) => prev ? { ...prev, sensorValues: { ...prev.sensorValues, ...partial } } : prev);
          }} onDirty={() => setDirty(true)} />
            <PidTuner config={config} addLog={addLog} onDirty={() => setDirty(true)} onPidUpdate={(pid) => {
              setConfig((prev) => prev ? { ...prev, pid } : prev);
            }} />
            <TargetBoundTuner config={config} addLog={addLog} onDirty={() => setDirty(true)} onBoundUpdate={(bound) => {
              setConfig((prev) => prev ? { ...prev, sensorValues: { ...prev.sensorValues, ...bound } } : prev);
            }} />
          </div>
        </div>
        <div class="area-chart"><SensorChart onTimeRange={(min, max) => {
          const prev = timeRangeRef.current;
          if (!prev || Math.abs(prev[0] - min) > 0.05 || Math.abs(prev[1] - max) > 0.05) {
            const next: [number, number] = [min, max];
            timeRangeRef.current = next;
            setTimeRange(next);
          }
        }} hoverTime={hoverTime} /></div>
        <div class="area-corr"><CorrelationCharts timeRange={timeRange} onHoverTime={setHoverTime}
          targetCurve={previewCurve ?? config?.targetCurve} idling={config?.sensorValues.idling} normalMax={config?.sensorValues.normalMax} restrictedMax={config?.sensorValues.restrictedMax}
          curvePreview={<CurvePreview targetCurve={previewCurve ?? config?.targetCurve} />} footer={
          <TargetCurveTuner config={config} addLog={addLog} onDirty={() => setDirty(true)} onPreview={setPreviewCurve} onCurveUpdate={(targetCurve) => {
            setPreviewCurve(null);
            setConfig((prev) => prev ? { ...prev, targetCurve } : prev);
          }} />
        } /></div>
        <div class="area-config">
          <ConfigPanel config={config} onConfigLoaded={setConfig} addLog={addLog} />
        </div>
        <div class="area-log"><DebugLog entries={logs} /></div>
        {!webSerialAvailable && (
          <section>
            <p style={{ color: "var(--err)" }}>Web Serial API is not available. Use Chrome or Edge.</p>
          </section>
        )}
      </main>
    </div>
  );
}

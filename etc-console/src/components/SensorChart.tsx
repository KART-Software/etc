import { useRef, useEffect, useState } from "preact/hooks";
import uPlot from "uplot";
import "uplot/dist/uPlot.min.css";
import type { SensorData } from "../types";

const SERIES_KEYS = ["a1", "a2", "i", "t1", "t2", "tgt"] as const;
const SERIES_COLORS = ["#0ea5e9", "#38bdf8", "#f59e0b", "#22c55e", "#4ade80", "#a78bfa"];
const SERIES_LABELS = ["APPS1", "APPS2", "ITTR", "TPS1", "TPS2", "Target"];

// Unlimited buffer — no dropping
const buf: number[][] = [[], [], [], [], [], [], []];

function pushData(data: SensorData) {
  buf[0].push(data.ts / 1000);
  SERIES_KEYS.forEach((k, i) => buf[i + 1].push(data[k]));
}

type TimeWindow = "all" | 10 | 5 | 2 | "custom" | "frozen";
const PRESETS: { label: string; value: TimeWindow }[] = [
  { label: "All", value: "all" },
  { label: "10s", value: 10 },
  { label: "5s", value: 5 },
  { label: "2s", value: 2 },
  { label: "Custom", value: "custom" },
];

interface Props {
  data: SensorData | null;
  onTimeRange?: (min: number, max: number) => void;
  hoverTime?: number | null;
}

export function SensorChart({ data, onTimeRange, hoverTime }: Props) {
  const wrapRef = useRef<HTMLDivElement>(null);
  const plotRef = useRef<uPlot | null>(null);
  const onTimeRangeRef = useRef(onTimeRange);
  onTimeRangeRef.current = onTimeRange;
  const [window_, setWindow] = useState<TimeWindow>(10);
  const [customSec, setCustomSec] = useState(30);
  const frozenRange = useRef<{ min: number; max: number } | null>(null);

  const windowSec = window_ === "all" || window_ === "frozen" ? null : window_ === "custom" ? customSec : window_;

  // Move cursor to hoverTime position (shows crosshair + legend values)
  useEffect(() => {
    const plot = plotRef.current;
    if (!plot) return;
    if (hoverTime != null) {
      const left = plot.valToPos(hoverTime, "x");
      plot.setCursor({ left, top: plot.bbox.height / (2 * uPlot.pxRatio) });
    } else {
      // Move cursor off-screen to hide it
      plot.setCursor({ left: -10, top: -10 });
    }
  }, [hoverTime]);

  // Push new data and set scale
  useEffect(() => {
    if (!data) return;
    pushData(data);
    const plot = plotRef.current;
    if (!plot) return;

    plot.setData(buf as uPlot.AlignedData);

    let rangeMin: number;
    let rangeMax: number;

    if (window_ === "frozen" && frozenRange.current) {
      plot.setScale("x", frozenRange.current);
      rangeMin = frozenRange.current.min;
      rangeMax = frozenRange.current.max;
    } else if (windowSec != null && buf[0].length > 0) {
      const latest = buf[0][buf[0].length - 1];
      plot.setScale("x", { min: latest - windowSec, max: latest });
      rangeMin = latest - windowSec;
      rangeMax = latest;
    } else if (buf[0].length > 1) {
      plot.setScale("x", { min: buf[0][0], max: buf[0][buf[0].length - 1] });
      rangeMin = buf[0][0];
      rangeMax = buf[0][buf[0].length - 1];
    } else {
      return;
    }

    onTimeRange?.(rangeMin, rangeMax);
  }, [data, windowSec, window_]);

  // Create uPlot instance
  useEffect(() => {
    if (!wrapRef.current) return;

    const opts: uPlot.Options = {
      width: wrapRef.current.offsetWidth,
      height: 300,
      cursor: {
        drag: { x: true, y: false },
        y: false,
      },
      hooks: {
        setSelect: [
          (u: uPlot) => {
            const left = u.posToVal(u.select.left, "x");
            const right = u.posToVal(u.select.left + u.select.width, "x");
            if (right - left > 0.05) {
              frozenRange.current = { min: left, max: right };
              setWindow("frozen");
              onTimeRangeRef.current?.(left, right);
            }
            // Clear the visual selection box
            u.setSelect({ left: 0, width: 0, top: 0, height: 0 }, false);
          },
        ],
      },
      series: [
        {
          label: "Time",
          value: (_u: uPlot, ts: number) => {
            if (ts == null) return "—";
            const d = new Date(ts * 1000);
            const hh = String(d.getHours()).padStart(2, "0");
            const mm = String(d.getMinutes()).padStart(2, "0");
            const ss = String(d.getSeconds()).padStart(2, "0");
            const ms = String(d.getMilliseconds()).padStart(3, "0");
            return `${hh}:${mm}:${ss}.${ms}`;
          },
        },
        ...SERIES_KEYS.map((_, i) => ({
          label: SERIES_LABELS[i],
          stroke: SERIES_COLORS[i],
          width: 1.5,
        })),
      ],
      scales: {
        y: { range: [0, 100] },
      },
      axes: [
        { stroke: "#8888aa", grid: { stroke: "#1e3a5f" } },
        { stroke: "#8888aa", grid: { stroke: "#1e3a5f" } },
      ],
    };

    plotRef.current = new uPlot(opts, buf as uPlot.AlignedData, wrapRef.current);

    const onResize = () => {
      if (wrapRef.current && plotRef.current) {
        plotRef.current.setSize({ width: wrapRef.current.offsetWidth, height: 300 });
      }
    };
    window.addEventListener("resize", onResize);

    return () => {
      window.removeEventListener("resize", onResize);
      plotRef.current?.destroy();
      plotRef.current = null;
    };
  }, []);

  // Zoom: adjust time window
  function zoomIn() {
    if (window_ === "frozen" && frozenRange.current) {
      const range = frozenRange.current.max - frozenRange.current.min;
      const center = (frozenRange.current.min + frozenRange.current.max) / 2;
      const next = Math.max(0.1, range / 2);
      frozenRange.current = { min: center - next / 2, max: center + next / 2 };
      setWindow("frozen");
      return;
    }
    if (window_ === "all") { setWindow(10); return; }
    const cur = window_ === "custom" ? customSec : window_ as number;
    const next = Math.max(1, Math.round(cur / 2));
    setCustomSec(next);
    setWindow("custom");
  }
  function zoomOut() {
    if (window_ === "frozen" && frozenRange.current) {
      const range = frozenRange.current.max - frozenRange.current.min;
      const center = (frozenRange.current.min + frozenRange.current.max) / 2;
      const next = range * 2;
      frozenRange.current = { min: center - next / 2, max: center + next / 2 };
      setWindow("frozen");
      return;
    }
    if (window_ === "all") return;
    const cur = window_ === "custom" ? customSec : window_ as number;
    const next = Math.round(cur * 2);
    setCustomSec(next);
    setWindow("custom");
  }
  function goLive() {
    if (frozenRange.current) {
      const sec = Math.round(frozenRange.current.max - frozenRange.current.min);
      setCustomSec(Math.max(1, sec));
      setWindow("custom");
    } else {
      setWindow(10);
    }
    frozenRange.current = null;
  }

  return (
    <section>
      <div class="chart-header">
        <h2>Time Series</h2>
        <div class="chart-controls">
          {PRESETS.map((p) => (
            <button
              key={p.label}
              class={`chart-btn${(window_ === p.value) ? " active" : ""}`}
              onClick={() => setWindow(p.value)}
            >
              {p.label}
            </button>
          ))}
          {window_ === "custom" && (
            <input
              type="number"
              class="chart-input"
              value={customSec}
              min={1}
              max={600}
              onInput={(e) => setCustomSec(Math.max(1, +(e.target as HTMLInputElement).value))}
            />
          )}
          <span class="chart-sep" />
          <button class="chart-btn" onClick={zoomIn} title="Zoom in (shorter window)">+</button>
          <button class="chart-btn" onClick={zoomOut} title="Zoom out (longer window)">−</button>
          {window_ === "frozen" && (
            <>
              <span class="chart-sep" />
              <button class="chart-btn active" onClick={goLive} title="Resume live scrolling">Live ▶</button>
            </>
          )}
        </div>
      </div>
      <div class="chart-wrap" ref={wrapRef} />
    </section>
  );
}

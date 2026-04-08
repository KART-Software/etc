import { useRef, useEffect } from "preact/hooks";
import uPlot from "uplot";
import "uplot/dist/uPlot.min.css";
import type { SensorData } from "../types";

const MAX_POINTS = 500; // ~10s at 50Hz

const SERIES_KEYS = ["a1", "a2", "t1", "t2", "tgt"] as const;
const SERIES_COLORS = ["#0ea5e9", "#38bdf8", "#22c55e", "#4ade80", "#f59e0b"];
const SERIES_LABELS = ["APPS1", "APPS2", "TPS1", "TPS2", "Target"];

// Ring buffer for chart data: [timestamps, a1, a2, t1, t2, tgt]
const buf: number[][] = [[], [], [], [], [], []];

function pushData(data: SensorData) {
  buf[0].push(data.ts / 1000); // uPlot expects seconds
  SERIES_KEYS.forEach((k, i) => buf[i + 1].push(data[k]));
  if (buf[0].length > MAX_POINTS) {
    for (const arr of buf) arr.shift();
  }
}

interface Props {
  data: SensorData | null;
}

export function SensorChart({ data }: Props) {
  const wrapRef = useRef<HTMLDivElement>(null);
  const plotRef = useRef<uPlot | null>(null);

  // Push new data and update chart
  useEffect(() => {
    if (!data) return;
    pushData(data);
    if (plotRef.current) {
      plotRef.current.setData(buf as uPlot.AlignedData);
    }
  }, [data]);

  // Create uPlot instance
  useEffect(() => {
    if (!wrapRef.current) return;

    const opts: uPlot.Options = {
      width: wrapRef.current.offsetWidth,
      height: 300,
      series: [
        { label: "Time" },
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

  return (
    <section>
      <h2>Time Series</h2>
      <div class="chart-wrap" ref={wrapRef} />
    </section>
  );
}

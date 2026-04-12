import { useRef, useEffect } from "preact/hooks";
import type { ComponentChildren } from "preact";
import uPlot from "uplot";
import "uplot/dist/uPlot.min.css";
import type { SensorData } from "../types";
import { sensorStore, CHART_COL } from "../sensor-store";

const CHART_H = 300;

/** Column index in sensorStore.mem for each SensorData key used by scatter charts */
const COL_MAP: Record<string, number> = {
  a1: CHART_COL.a1,
  tgt: CHART_COL.tgt,
  t1: CHART_COL.t1,
};

interface XYConfig {
  title: string;
  xKey: keyof SensorData;
  yKey: keyof SensorData;
  xLabel: string;
  yLabel: string;
  color: string;
}

const CHARTS: XYConfig[] = [
  { title: "APPS1 → Target", xKey: "a1", yKey: "tgt", xLabel: "APPS1 (%)", yLabel: "Target (%)", color: "#f59e0b" },
  { title: "Target → TPS1", xKey: "tgt", yKey: "t1", xLabel: "Target (%)", yLabel: "TPS1 (%)", color: "#22c55e" },
];

// mode:2 data format: data[0]=null, data[seriesIdx]=[xArr, yArr]
type ScatterData = [null, ...([number[], number[]])[]]

/** Binary search: find first index where arr[i] >= val */
function lowerBound(arr: number[], val: number): number {
  let lo = 0, hi = arr.length;
  while (lo < hi) { const mid = (lo + hi) >> 1; if (arr[mid] < val) lo = mid + 1; else hi = mid; }
  return lo;
}
/** Binary search: find first index where arr[i] > val */
function upperBound(arr: number[], val: number): number {
  let lo = 0, hi = arr.length;
  while (lo < hi) { const mid = (lo + hi) >> 1; if (arr[mid] <= val) lo = mid + 1; else hi = mid; }
  return lo;
}

/** Build scatter data from sensorStore.mem, filtered by timeRange */
let filteredTs: number[][] = CHARTS.map(() => []);

function filteredBuf(chartIdx: number, timeRange: [number, number] | null): ScatterData {
  const mem = sensorStore.mem;
  const tsCol = mem[CHART_COL.ts];
  const cfg = CHARTS[chartIdx];
  const xCol = mem[COL_MAP[cfg.xKey as string]];
  const yCol = mem[COL_MAP[cfg.yKey as string]];

  if (!timeRange) {
    filteredTs[chartIdx] = tsCol;
    return [null, [xCol, yCol]];
  }
  // Binary search on sorted timestamps — O(log n) instead of O(n)
  const start = lowerBound(tsCol, timeRange[0]);
  const end = upperBound(tsCol, timeRange[1]);
  filteredTs[chartIdx] = tsCol.slice(start, end);
  return [null, [xCol.slice(start, end), yCol.slice(start, end)]];
}

// Exactly follows uPlot scatter.html demo drawPoints pattern + connecting lines
const drawPoints: uPlot.Series.PathBuilder = (u, seriesIdx, _idx0, _idx1) => {
  const size = 5 * uPlot.pxRatio;

  uPlot.orient(u, seriesIdx, (
    series, _dataX, _dataY, scaleX, scaleY,
    valToPosX, valToPosY, xOff, yOff, xDim, yDim,
    _moveTo, _lineTo, _rect, arc,
  ) => {
    const d = u.data[seriesIdx] as unknown as [number[], number[]];
    if (!d || !d[0] || d[0].length === 0) return;

    const strokeColor = (series.stroke as () => string)();
    const deg360 = 2 * Math.PI;

    // 1) Draw connecting lines
    const linePath = new Path2D();
    let started = false;
    for (let i = 0; i < d[0].length; i++) {
      const cx = valToPosX(d[0][i], scaleX, xDim, xOff);
      const cy = valToPosY(d[1][i], scaleY, yDim, yOff);
      if (!started) {
        linePath.moveTo(cx, cy);
        started = true;
      } else {
        linePath.lineTo(cx, cy);
      }
    }
    u.ctx.save();
    u.ctx.strokeStyle = strokeColor;
    u.ctx.globalAlpha = 0.9;
    u.ctx.lineWidth = 1.5 * uPlot.pxRatio;
    u.ctx.stroke(linePath);
    u.ctx.restore();
  });

  return null;
};

interface TargetCurve {
  a4: number;
  a3: number;
  a2: number;
  a1: number;
}

interface CurveOverlayParams {
  curve: TargetCurve;
  mode: string;
  idling: number;
  normalMax: number;
  restrictedMax: number;
}

function curveMinMax(p: CurveOverlayParams): [number, number] {
  switch (p.mode) {
    case "Normal":   return [p.idling, p.normalMax];
    case "Restrict": return [p.idling, p.restrictedMax];
    case "Calib":    return [0, 100];
    default:         return [0, 100];
  }
}

function createOpts(cfg: XYConfig, size: number, overlayRef?: { current: CurveOverlayParams | null }): uPlot.Options {
  const hooks: uPlot.Options["hooks"] = {
    drawClear: [
      (u: uPlot) => {
        u.series.forEach((s, i) => {
          if (i > 0) (s as any)._paths = null;
        });
      },
    ],
  };

  if (overlayRef) {
    hooks.drawAxes = [
      (u: uPlot) => {
        const p = overlayRef.current;
        if (!p) return;
        const { curve } = p;
        const [minV, maxV] = curveMinMax(p);
        const ctx = u.ctx;

        ctx.save();
        ctx.beginPath();
        const N = 101;
        for (let i = 0; i < N; i++) {
          const x = i;
          const poly = ((((curve.a4 * x + curve.a3) * x + curve.a2) * x + curve.a1) * x);
          const y = minV + poly * (maxV - minV) / 100;
          const px = u.valToPos(x, "x", true);
          const py = u.valToPos(y, "y", true);
          if (i === 0) ctx.moveTo(px, py);
          else ctx.lineTo(px, py);
        }
        ctx.strokeStyle = "#a78bfa";
        ctx.lineWidth = 2 * devicePixelRatio;
        ctx.globalAlpha = 0.6;
        ctx.stroke();
        ctx.restore();
      },
    ];
  }

  return {
    mode: 2,
    width: size,
    height: size,
    legend: { show: false },
    hooks,
    scales: {
      x: { time: false, range: [0, 100] as [number, number] },
      y: { range: [0, 100] as [number, number] },
    },
    axes: [
      {
        label: cfg.xLabel,
        stroke: "#8888aa",
        grid: { stroke: "#1e3a5f" },
        ticks: { stroke: "#1e3a5f" },
      },
      {
        label: cfg.yLabel,
        stroke: "#8888aa",
        grid: { stroke: "#1e3a5f" },
        ticks: { stroke: "#1e3a5f" },
      },
    ],
    series: [
      {},
      {
        stroke: cfg.color,
        fill: cfg.color + "1a",
        paths: drawPoints,
      },
    ],
  };
}

interface Props {
  timeRange: [number, number] | null;
  onHoverTime?: (ts: number | null) => void;
  curvePreview?: ComponentChildren;
  footer?: ComponentChildren;
  targetCurve?: TargetCurve;
  idling?: number;
  normalMax?: number;
  restrictedMax?: number;
}

export function CorrelationCharts({ timeRange, onHoverTime, curvePreview, footer, targetCurve, idling, normalMax, restrictedMax }: Props) {
  const wrapRefs = [useRef<HTMLDivElement>(null), useRef<HTMLDivElement>(null)];
  const plotRefs = useRef<(uPlot | null)[]>([null, null]);
  const onHoverTimeRef = useRef(onHoverTime);
  onHoverTimeRef.current = onHoverTime;
  const timeRangeRef = useRef(timeRange);
  timeRangeRef.current = timeRange;

  const overlayRef = useRef<CurveOverlayParams | null>(null);
  const mode = sensorStore.latest?.m;
  if (targetCurve && mode) {
    overlayRef.current = {
      curve: targetCurve,
      mode,
      idling: idling ?? 0,
      normalMax: normalMax ?? 100,
      restrictedMax: restrictedMax ?? 60,
    };
  }

  // Self-managed polling interval — updates scatter plots from sensorStore
  useEffect(() => {
    const iv = setInterval(() => {
      CHARTS.forEach((_cfg, i) => {
        const plot = plotRefs.current[i];
        if (plot && sensorStore.memRows > 0) {
          plot.setData(filteredBuf(i, timeRangeRef.current) as unknown as uPlot.AlignedData);
        }
      });
    }, 100); // 10Hz
    return () => clearInterval(iv);
  }, []);

  useEffect(() => {
    // Defer creation so layout is settled
    const raf = requestAnimationFrame(() => {
      CHARTS.forEach((cfg, i) => {
        const el = wrapRefs[i].current;
        if (!el) return;
        plotRefs.current[i] = new uPlot(
          createOpts(cfg, el.clientWidth, i === 0 ? overlayRef : undefined),
          filteredBuf(i, null) as unknown as uPlot.AlignedData,
          el,
        );
      });
    });

    const ro = new ResizeObserver(() => {
      CHARTS.forEach((_cfg, i) => {
        const el = wrapRefs[i].current;
        const plot = plotRefs.current[i];
        if (el && plot) {
          const w = el.clientWidth;
          if (w > 0) plot.setSize({ width: w, height: w });
        }
      });
    });
    wrapRefs.forEach((ref) => { if (ref.current) ro.observe(ref.current); });

    // Hover detection: find nearest point, report its timestamp
    const HOVER_RADIUS = 15; // px threshold
    const handleMouseMove = (chartIdx: number, e: MouseEvent) => {
      const plot = plotRefs.current[chartIdx];
      if (!plot) return;
      const rect = plot.over.getBoundingClientRect();
      const mx = e.clientX - rect.left;
      const my = e.clientY - rect.top;
      const xVal = plot.posToVal(mx, "x");
      const yVal = plot.posToVal(my, "y");

      const d = plot.data[1] as unknown as [number[], number[]];
      if (!d || !d[0] || d[0].length === 0) { onHoverTimeRef.current?.(null); return; }

      let bestDist = Infinity;
      let bestIdx = -1;
      for (let i = 0; i < d[0].length; i++) {
        const px = plot.valToPos(d[0][i], "x") - mx;
        const py = plot.valToPos(d[1][i], "y") - my;
        const dist = Math.sqrt(px * px + py * py);
        if (dist < bestDist) {
          bestDist = dist;
          bestIdx = i;
        }
      }
      if (bestIdx >= 0 && bestDist <= HOVER_RADIUS) {
        const ts = filteredTs[chartIdx][bestIdx];
        onHoverTimeRef.current?.(ts ?? null);
      } else {
        onHoverTimeRef.current?.(null);
      }
    };
    const handleMouseLeave = () => { onHoverTimeRef.current?.(null); };

    // Attach listeners after plots are created
    const attachRaf = requestAnimationFrame(() => {
      CHARTS.forEach((_cfg, i) => {
        const plot = plotRefs.current[i];
        if (!plot) return;
        plot.over.addEventListener("mousemove", (e) => handleMouseMove(i, e));
        plot.over.addEventListener("mouseleave", handleMouseLeave);
      });
    });

    return () => {
      cancelAnimationFrame(raf);
      cancelAnimationFrame(attachRaf);
      ro.disconnect();
      plotRefs.current.forEach((p) => p?.destroy());
      plotRefs.current = [null, null];
    };
  }, []);

  return (
    <section>
      <h2>Correlation</h2>
      <div class="correlation-grid">
        {curvePreview}
        {CHARTS.map((cfg, i) => (
          <div class="correlation-item" key={cfg.title}>
            <h3>{cfg.title}</h3>
            <div ref={wrapRefs[i]} />
          </div>
        ))}
      </div>
      {footer}
    </section>
  );
}

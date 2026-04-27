import { useRef, useEffect } from "preact/hooks";
import uPlot from "uplot";
import "uplot/dist/uPlot.min.css";

interface TargetCurve {
  a4: number;
  a3: number;
  a2: number;
  a1: number;
}

interface Props {
  targetCurve: TargetCurve | undefined;
}

const N = 101; // 0,1,2,...,100

function buildData(c: TargetCurve): uPlot.AlignedData {
  const xs = new Float64Array(N);
  const ys = new Float64Array(N);
  for (let i = 0; i < N; i++) {
    const x = i;
    xs[i] = x;
    ys[i] = ((((c.a4 * x + c.a3) * x + c.a2) * x + c.a1) * x);
  }
  return [Array.from(xs), Array.from(ys)];
}

const DEFAULT_CURVE: TargetCurve = { a4: 0, a3: 0, a2: 0.0087, a1: 0.13 };

function createOpts(w: number, h: number): uPlot.Options {
  return {
    width: w,
    height: h,
    legend: { show: false },
    cursor: { show: false },
    scales: {
      x: { time: false, range: [0, 100] as [number, number] },
      y: { range: [0, 100] as [number, number] },
    },
    axes: [
      {
        label: "Input (%)",
        stroke: "#8888aa",
        grid: { stroke: "#1e3a5f" },
        ticks: { stroke: "#1e3a5f" },
      },
      {
        label: "Output (%)",
        stroke: "#8888aa",
        grid: { stroke: "#1e3a5f" },
        ticks: { stroke: "#1e3a5f" },
      },
    ],
    series: [
      {},
      {
        stroke: "#a78bfa",
        width: 2,
      },
    ],
  };
}

export function CurvePreview({ targetCurve }: Props) {
  const wrapRef = useRef<HTMLDivElement>(null);
  const plotRef = useRef<uPlot | null>(null);

  // Create plot on mount
  useEffect(() => {
    const raf = requestAnimationFrame(() => {
      const el = wrapRef.current;
      if (!el) return;
      const w = el.clientWidth;
      const curve = targetCurve ?? DEFAULT_CURVE;
      plotRef.current = new uPlot(createOpts(w, w), buildData(curve), el);
    });

    const ro = new ResizeObserver(() => {
      const el = wrapRef.current;
      const plot = plotRef.current;
      if (el && plot) {
        const w = el.clientWidth;
        if (w > 0) plot.setSize({ width: w, height: w });
      }
    });
    if (wrapRef.current) ro.observe(wrapRef.current);

    return () => {
      cancelAnimationFrame(raf);
      ro.disconnect();
      plotRef.current?.destroy();
      plotRef.current = null;
    };
  }, []);

  // Update data when curve changes
  useEffect(() => {
    const plot = plotRef.current;
    if (!plot) return;
    const curve = targetCurve ?? DEFAULT_CURVE;
    plot.setData(buildData(curve));
  }, [targetCurve?.a4, targetCurve?.a3, targetCurve?.a2, targetCurve?.a1]);

  return (
    <div class="correlation-item">
      <h3>Target Curve</h3>
      <div ref={wrapRef} />
    </div>
  );
}

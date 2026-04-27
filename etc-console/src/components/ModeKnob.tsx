import { useState, useEffect } from "preact/hooks";
import { sensorStore } from "../sensor-store";

const CX = 200;
const CY = 120;
const R = 50;

const MODES = [
  { key: "Calib", deg: 240, name: "Calib", label: "0 - 100", color: "#fbbf24" },
  { key: "Normal", deg: 300, name: "Normal", label: "Idle - Max", color: "#34d399" },
  { key: "Restrict", deg: 360, name: "Restricted", label: "Idle - Restrict Max", color: "#f87171" },
];

function xy(r: number, deg: number): [number, number] {
  const rad = (deg * Math.PI) / 180;
  return [CX + r * Math.sin(rad), CY - r * Math.cos(rad)];
}

interface Props {}

export function ModeKnob(_props: Props) {
  const [mode, setMode] = useState<string | undefined>(undefined);

  useEffect(() => {
    const iv = setInterval(() => {
      const d = sensorStore.latest;
      if (d) setMode(d.m);
    }, 100); // 10Hz
    return () => clearInterval(iv);
  }, []);

  const cur = MODES.find((m) => m.key === mode);

  return (
    <section>
      <h2>Mode</h2>
      <svg viewBox="0 0 340 195" width="330">
        {/* Knob face */}
        <circle cx={CX} cy={CY} r={R} fill="var(--bg)" stroke="var(--border)" stroke-width="2" />

        {/* Ticks and labels */}
        {MODES.map((m) => {
          const active = mode === m.key;
          const [tx, ty] = xy(R + 10, m.deg);
          const [lx, ly] = xy(R + 32, m.deg);
          const anchor = m.deg === 360 ? "middle" : "end";
          return (
            <g key={m.key}>
              <circle cx={tx} cy={ty} r={active ? 5 : 3} fill={active ? m.color : "#555"} />
              <text
                x={lx}
                y={ly + 9}
                text-anchor={anchor}
                dominant-baseline="middle"
                fill={active ? m.color : "#777"}
                font-size={active ? "20" : "16"}
                font-weight={active ? "bold" : "normal"}
              >
                {m.label}
              </text>
              <text
                x={lx}
                y={ly - 9}
                text-anchor={anchor}
                dominant-baseline="middle"
                fill={active ? m.color : "#555"}
                font-size="14"
                font-weight={active ? "bold" : "normal"}
              >
                {m.name}
              </text>
            </g>
          );
        })}

        {/* Pointer */}
        {cur &&
          (() => {
            const [px, py] = xy(R - 14, cur.deg);
            return (
              <line
                x1={CX}
                y1={CY}
                x2={px}
                y2={py}
                stroke={cur.color}
                stroke-width="4"
                stroke-linecap="round"
              />
            );
          })()}

        {/* Center dot */}
        <circle cx={CX} cy={CY} r="5" fill={cur?.color ?? "#555"} />
      </svg>
    </section>
  );
}

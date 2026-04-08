interface LogEntry {
  ts: string;
  msg: string;
}

interface Props {
  entries: LogEntry[];
}

export function DebugLog({ entries }: Props) {
  return (
    <section>
      <h2>Debug Log</h2>
      <div class="log-output">
        {entries.map((e, i) => (
          <div class="log-entry" key={i}>
            <span class="log-ts">{e.ts}</span>
            {e.msg}
          </div>
        ))}
      </div>
    </section>
  );
}

export type { LogEntry };

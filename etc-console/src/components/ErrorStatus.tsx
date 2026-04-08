const ERROR_LABELS: Record<number, string> = {
  0: "TPS Implausible",
  1: "APPS Implausible",
  2: "TPS1 Circuit",
  3: "TPS2 Circuit",
  4: "APPS1 Circuit",
  5: "APPS2 Circuit",
  6: "APPS-TPS Target",
  7: "BPS Circuit",
  8: "BPS-TPS Implausible",
};

interface Props {
  errors: number[];
}

export function ErrorStatus({ errors }: Props) {
  const errSet = new Set(errors);
  return (
    <section>
      <h2>Error Status</h2>
      <div class="error-grid">
        {Object.entries(ERROR_LABELS).map(([id, label]) => (
          <div class="error-item" key={id}>
            <span class={`led${errSet.has(Number(id)) ? " error" : ""}`} />
            {label}
          </div>
        ))}
      </div>
    </section>
  );
}

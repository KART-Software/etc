import { defineConfig } from "vite";
import preact from "@preact/preset-vite";

export default defineConfig({
  plugins: [preact({ devtoolsInProd: false, reactAliasesEnabled: false })],
  base: process.env.VITE_BASE || "/",
});

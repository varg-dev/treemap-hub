import { Renderer } from "../viewer/source/treemap-2d.ts";
import { encode } from "https://deno.land/x/pngs@0.1.1/mod.ts";
import { exists } from "https://deno.land/std@0.106.0/fs/mod.ts";

type Preset = Parameters<typeof renderer.setRenderingPreset>[0];

// =================
// --- CLI logic ---
// =================

const [layoutPath, ...options] = Deno.args;

if (!layoutPath || !(await exists(layoutPath))) {
    console.error("No layout file given or given file does not exist.");
    Deno.exit(1);
}

const optionsMap: Record<string, string> = Object.fromEntries(
    options.map(option => option.split("="))
);

const size = parseInt(optionsMap["-s"]) || 1024;
const preset: Preset = (optionsMap["-p"] ?? "depth") as Preset;

// =======================
// --- Rendering logic ---
// =======================

const renderer = new Renderer({ width: size, height: size });

await renderer.initialize();
// await renderer.setColorScheme([0, 0, 0, 255], [255, 255, 255, 255]);
await renderer.setRenderingPreset(preset);

const treemapLayout = JSON.parse(await Deno.readTextFile(layoutPath));

await renderer.loadTreemap(treemapLayout);

const treemap = await renderer.snapshot();
const image = encode(treemap, size, size);

await Deno.writeFile("./treemap.png", image);

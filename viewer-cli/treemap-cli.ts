import { Renderer } from "../viewer/source/treemap-2d.ts";
import { encode } from "https://deno.land/x/pngs@0.1.1/mod.ts";

const size = 4096;

const renderer = new Renderer({ width: size, height: size });

await renderer.initialize();

const treemapLayout = JSON.parse(await Deno.readTextFile("../datasets/firefox-xl-layout.json"));

await renderer.loadTreemap(treemapLayout);

const treemap = await renderer.snapshot();
const image = encode(treemap, size, size);

// console.log(treemap);

await Deno.writeFile("./treemap.png", image);

import { Renderer } from "./dist/js/treemap-2d.js";

const canvas = document.querySelector("#canvas");

const renderer = new Renderer(canvas);

(async () => {
    await renderer.initialize();
    await renderer.loadTreemap();

    renderer.start();
})();

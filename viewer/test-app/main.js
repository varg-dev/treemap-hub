import { Renderer } from './dist/js/treemap-2d.js';

/** @type {HTMLCanvasElement} */
const canvas = document.querySelector('#canvas');
/** @type {HTMLInputElement} */
const layoutInput = document.querySelector('#layout-input');

const renderer = new Renderer(canvas);

(async () => {
    await renderer.initialize();

    layoutInput.addEventListener('change', async () => {
        const layoutFile = layoutInput.files.item(0);

        if (layoutFile) {
            const layoutText = await layoutFile.text();
            const treemapLayout = JSON.parse(layoutText);

            await renderer.loadTreemap(treemapLayout);
            renderer.start();
        }
    });
})();

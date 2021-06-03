import { Renderer } from './dist/js/treemap-2d.js';

/** @type {HTMLCanvasElement} */
const canvas = document.querySelector('#canvas');
/** @type {HTMLInputElement} */
const layoutInput = document.querySelector('#layout-input');
/** @type {HTMLInputElement} */
const startColorInput = document.querySelector('#start-color');
/** @type {HTMLInputElement} */
const endColorInput = document.querySelector('#end-color');

const renderer = new Renderer(canvas);
let startColor = '#000000';
let endColor = '#ffffff';

startColorInput.value = startColor;
endColorInput.value = endColor;

(async () => {
    await renderer.initialize();

    renderer.setColorScheme(startColor, endColor);

    layoutInput.addEventListener('change', async () => {
        const layoutFile = layoutInput.files.item(0);

        if (layoutFile) {
            renderer.stop();

            const layoutText = await layoutFile.text();
            const treemapLayout = JSON.parse(layoutText);

            await renderer.loadTreemap(treemapLayout);
            renderer.start();
        }
    });

    startColorInput.addEventListener('input', event => {
        startColor = event.target.value;
        renderer.setColorScheme(startColor, endColor, false);
    });

    endColorInput.addEventListener('input', event => {
        endColor = event.target.value;
        renderer.setColorScheme(startColor, endColor);
    });
})();

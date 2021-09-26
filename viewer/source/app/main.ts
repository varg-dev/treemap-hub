/* This file is excluded from compilation and is used for the demo app (index.html) only */
import './style.css';
import { Renderer } from '../treemap-2d';

const canvas = document.querySelector<HTMLCanvasElement>('#canvas');
const layoutInput = document.querySelector<HTMLInputElement>('#layout-input');
const saveImageBtn = document.querySelector<HTMLButtonElement>('#save-image-btn');
const presetSelect = document.querySelector<HTMLSelectElement>('#preset-select');
// Depth parameters
const rootColorInput = document.querySelector<HTMLInputElement>('#root-color');
const leafColorInput = document.querySelector<HTMLInputElement>('#leaf-color');
// Outlines parameters
const bgColorInput = document.querySelector<HTMLInputElement>('#bg-color');
const outlineColorInput = document.querySelector<HTMLInputElement>('#outline-color');

const renderer = new Renderer(canvas);
let preset: Parameters<typeof renderer.setRenderingPreset>[0] = 'depth';
let rootColor = '#000000';
let leafColor = '#ffffff';
let bgColor = '#ffffff';
let outlineColor = '#000000';

presetSelect.value = preset;
rootColorInput.value = rootColor;
leafColorInput.value = leafColor;
bgColorInput.value = bgColor;
outlineColorInput.value = outlineColor;

updateParametersView();

(async () => {
    await renderer.initialize();
    await renderer.setColorScheme(rootColor, leafColor);

    layoutInput.addEventListener('change', async () => {
        const layoutFile = layoutInput.files.item(0);

        if (layoutFile) {
            renderer.stop();

            const layoutText = await layoutFile.text();
            const treemapLayout = JSON.parse(layoutText);

            console.time('load treemap');
            await renderer.loadTreemap(treemapLayout);
            console.timeEnd('load treemap');
            renderer.start();
        }
    });

    saveImageBtn.addEventListener('click', async () => {
        const image = await renderer.snapshot();
        const handle = await showSaveFilePicker({
            suggestedName: 'treemap.png',
        } as SaveFilePickerOptions);
        const stream = await handle.createWritable();

        await stream.write(image);
        await stream.close();
    });

    presetSelect.addEventListener('change', (event: ChangeEvent<typeof preset>) => {
        preset = event.target.value;

        if (preset === 'depth') {
            renderer.setColorScheme(rootColor, leafColor);
        } else {
            renderer.setBackgroundColor(bgColor);
            renderer.setOutlineColor(outlineColor);
        }

        renderer.setRenderingPreset(preset);
        updateParametersView();
    });

    rootColorInput.addEventListener('input', (event: ChangeEvent<string>) => {
        rootColor = event.target.value;
        renderer.setColorScheme(rootColor, leafColor);
    });

    leafColorInput.addEventListener('input', (event: ChangeEvent<string>) => {
        leafColor = event.target.value;
        renderer.setColorScheme(rootColor, leafColor);
    });

    bgColorInput.addEventListener('input', (event: ChangeEvent<string>) => {
        bgColor = event.target.value;
        renderer.setBackgroundColor(bgColor);
    });

    outlineColorInput.addEventListener('input', (event: ChangeEvent<string>) => {
        outlineColor = event.target.value;
        renderer.setOutlineColor(outlineColor);
    });
})();

function updateParametersView() {
    // Hide all parameters
    document.querySelectorAll('.param').forEach(param => param.classList.add('hidden'));
    // Show parameters of the VCA
    document.querySelectorAll(`.${preset}`).forEach(param => param.classList.remove('hidden'));
}

interface ChangeEvent<T> extends Event {
    target: EventTarget & { value?: T };
}

interface SaveFilePickerOptions extends globalThis.SaveFilePickerOptions {
    suggestedName: string;
}

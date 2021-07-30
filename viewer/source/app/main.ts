/* This file is excluded from compilation and is used for the demo app (index.html) only */
import './style.css';
import { Renderer } from '../treemap-2d';

const canvas = document.querySelector<HTMLCanvasElement>('#canvas');
const layoutInput = document.querySelector<HTMLInputElement>('#layout-input');
const saveImageBtn = document.querySelector<HTMLButtonElement>('#save-image-btn');
const presetSelect = document.querySelector<HTMLSelectElement>('#preset-select');
const startColorInput = document.querySelector<HTMLInputElement>('#start-color');
const endColorInput = document.querySelector<HTMLInputElement>('#end-color');

const renderer = new Renderer(canvas);
let preset: Parameters<typeof renderer.setRenderingPreset>[0] = 'depth';
let startColor = '#000000';
let endColor = '#ffffff';

presetSelect.value = preset;
startColorInput.value = startColor;
endColorInput.value = endColor;

(async () => {
    await renderer.initialize();
    await renderer.setColorScheme(startColor, endColor);

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
        renderer.setRenderingPreset(preset);
    });

    startColorInput.addEventListener('input', (event: ChangeEvent<string>) => {
        startColor = event.target.value;
        renderer.setColorScheme(startColor, endColor);
    });

    endColorInput.addEventListener('input', (event: ChangeEvent<string>) => {
        endColor = event.target.value;
        renderer.setColorScheme(startColor, endColor);
    });
})();

interface ChangeEvent<T> extends Event {
    target: EventTarget & { value?: T };
}

interface SaveFilePickerOptions extends globalThis.SaveFilePickerOptions {
    suggestedName: string;
}

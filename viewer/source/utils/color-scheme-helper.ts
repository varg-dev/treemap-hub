import type { RgbColor } from '../types/utils';

const canvas = document.createElement('canvas');

canvas.width = 100;
canvas.height = 1;

const context = canvas.getContext('2d');

export const generateSchemeBitmap = (colorA: string, colorB: string): Promise<ImageBitmap> => {
    const gradient = context.createLinearGradient(0, 0, canvas.width, 0);

    gradient.addColorStop(0, colorA);
    gradient.addColorStop(1, colorB);

    context.fillStyle = gradient;
    context.fillRect(0, 0, canvas.width, 1);

    return createImageBitmap(canvas);
};

export const convertColorStringToRgb = (color: string): RgbColor => {
    const red = parseInt(color.substr(1, 2), 16);
    const green = parseInt(color.substr(3, 2), 16);
    const blue = parseInt(color.substr(5, 2), 16);

    return [red, green, blue].map(channel => channel / 255) as RgbColor;
};

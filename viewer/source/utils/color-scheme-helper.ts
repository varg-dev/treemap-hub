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

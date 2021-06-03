export const generateSchemeBitmap = (colorA: string, colorB: string): ImageBitmap => {
    const canvas = new OffscreenCanvas(100, 1);
    const context = canvas.getContext('2d');

    const gradient = context.createLinearGradient(0, 0, canvas.width, 0);

    gradient.addColorStop(0, colorA);
    gradient.addColorStop(1, colorB);

    context.fillStyle = gradient;
    context.fillRect(0, 0, canvas.width, 1);

    return canvas.transferToImageBitmap();
};

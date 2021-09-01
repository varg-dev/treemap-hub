export type Color = [number, number, number, number];

const GRADIENT_STEPS = 100;

export const generateSchemeBitmap = async (
    colorA: Color,
    colorB: Color
): Promise<Uint8ClampedArray> => {
    const gradient = new Uint8ClampedArray(GRADIENT_STEPS * 4);
    const diff = colorA.map((channel, idx) => colorB[idx] - channel);

    for (let i = 0; i < GRADIENT_STEPS; i++) {
        const color = diff.map(
            (channel, idx) => colorA[idx] + (i / (GRADIENT_STEPS - 1)) * channel
        );

        gradient.set(color, i * 4);
    }

    return gradient;
};

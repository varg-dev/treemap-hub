export const loadShader = async (name: string): Promise<Uint32Array> => {
    const shaderUrl = (await import(`../assets/spirv/${name}.spv`)).default;
    const shader = await (await fetch(shaderUrl)).arrayBuffer();

    return new Uint32Array(shader);
};

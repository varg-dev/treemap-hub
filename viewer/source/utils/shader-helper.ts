export const loadShader = async (name: string): Promise<Uint32Array> => {
    const shader: Uint32Array = (await import(`../assets/shaders/${name}.glsl`)).default;

    return shader;
};

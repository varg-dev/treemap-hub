const USE_WGSL_SHADERS = true;

const loadGlslShader = async (name: string): Promise<Uint32Array> => {
    const shader: Uint32Array = (await import(`../assets/shaders/glsl/${name}.glsl`)).default;

    return shader;
};

const loadWgslShader = async (name: string): Promise<string> => {
    const shader: string = await Deno.readTextFile(
        new URL(`../assets/shaders/wgsl/${name}.wgsl`, import.meta.url)
    );

    return shader;
};

export const loadShader = (name: string): Promise<string> => {
    return USE_WGSL_SHADERS ? loadWgslShader(name) : (loadGlslShader(name) as any);
};

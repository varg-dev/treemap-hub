const SHADER_URL = '/dist/assets/shaders';

export const loadShader = async (name: string): Promise<Uint32Array> => {
    const shader = await (await fetch(`${SHADER_URL}/${name}`)).arrayBuffer();

    return new Uint32Array(shader);
};

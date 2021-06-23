import loadGlslang, { Glslang, ShaderStage } from '@webgpu/glslang';

const createShaderTypes = <ShaderTypes extends Record<string, ShaderStage>>(types: ShaderTypes) =>
    types;

const shaderTypes = createShaderTypes({
    vert: 'vertex',
    frag: 'fragment',
    comp: 'compute',
});

const glslang = loadGlslang() as unknown as Glslang;

export const compileShader = async (glsl: string, type: ShaderType): Promise<Uint32Array> => {
    const spirv = glslang.compileGLSL(glsl, shaderTypes[type], false);

    return spirv;
};

export type ShaderType = keyof typeof shaderTypes;

import { ShaderType, compileShader } from './compile';
import type { Plugin } from 'vite';

export default function shaders(): Plugin {
    return {
        name: 'vite:shaders',
        async transform(code, id) {
            if (!/\/assets\/shaders\/[a-z-]+\.(vert|frag|comp)\.glsl$/.test(id)) {
                return null;
            }

            const type = id.split('.').slice(-2, -1)[0] as ShaderType;
            const spirv = await compileShader(code, type);

            return {
                code: `export default new Uint32Array([${spirv.toString()}]);`,
                map: {
                    mappings: '',
                },
            };
        },
    };
}

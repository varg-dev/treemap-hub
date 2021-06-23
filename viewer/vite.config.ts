import { defineConfig } from 'vite';
import path from 'path';

// https://vitejs.dev/config/
export default defineConfig(({ mode }) => ({
    assetsInclude: new RegExp('\\.spv$'),
    build: {
        lib: {
            entry: path.resolve(__dirname, 'source/treemap-2d.ts'),
            name: 'Treemap2D',
            formats: ['es', 'cjs'],
        },
        minify: mode === 'minified' ? 'terser' : false,
        rollupOptions: {
            // External dependencies that shouldn't be bundles go here
            external: [],
            output: {
                // Global variables to use in the UMD build for externalized dependencies
                globals: {},
            },
        },
        sourcemap: true,
    },
    plugins: [],
}));

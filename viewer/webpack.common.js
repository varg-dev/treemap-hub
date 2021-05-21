/* eslint-env node */
/* eslint-disable @typescript-eslint/no-var-requires */
const CopyPlugin = require('copy-webpack-plugin');
const HookPlugin = require('webpack-hook-plugin');

module.exports = function (env) {
    return {
        context: __dirname + '/source',
        cache: false,
        entry: {
            main: ['./treemap-2d.ts'],
        },

        target: ['web', 'es2020'],

        output: {
            path: __dirname + '/dist/js',
            module: true,
            library: {
                type: 'module',
            },
        },

        resolve: {
            modules: ['node_modules', 'source'],
            extensions: ['.ts', '.js'],
        },

        module: {
            rules: [
                {
                    test: /\.tsx?$/,
                    include: /(source)/,
                    exclude: /(node_modules|tests)/,
                    use: { loader: 'ts-loader' },
                },
            ],
        },
        plugins: [
            new CopyPlugin({
                patterns: [{ from: 'assets/spirv', to: '../assets/shaders' }],
            }),
            new HookPlugin({
                onBuildStart: ['node ./scripts/compile-shaders.js'],
                // TODO: Remove when running the test-app with a webpack-dev-server
                onBuildEnd: ['npx rimraf ./test-app/dist && cp -r dist ./test-app'],
                safe: true,
            }),
        ],
        experiments: {
            outputModule: true,
        },
    };
};

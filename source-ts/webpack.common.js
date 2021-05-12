'use strict';

module.exports = function (env) {
    return {
        context: __dirname + '/source',
        cache: false,
        entry: {
            main: ['./treemap-layout.ts']
        },

        output: {
            path: __dirname + '/dist/js',
            library: 'TreemapLayout',
        },

        resolve: {
            modules: ['node_modules', 'source'],
            extensions: ['.ts', '.js']
        },

        module: {
            rules: [
                {
                    test: /\.tsx?$/,
                    include: /(source)/,
                    exclude: /(node_modules|tests)/,
                    use: { loader: 'ts-loader' }
                }]
        },
        plugins: [],
    };
}

/* eslint-disable no-undef */
/* eslint-disable @typescript-eslint/no-var-requires */

const { merge } = require('webpack-merge');
const common = require('./webpack.common.js');

module.exports = function (env) {
    return merge(
        common(env),
        {
            mode: 'development',
            optimization: {
                minimize: false
            },
            devtool: 'source-map',
            output: {
                filename: 'treemap-layout.js'
            }
        })
};

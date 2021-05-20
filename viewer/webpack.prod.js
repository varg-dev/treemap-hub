/* eslint-env node */
/* eslint-disable @typescript-eslint/no-var-requires */

const TerserPlugin = require("terser-webpack-plugin");

const { merge } = require("webpack-merge");
const common = require("./webpack.common.js");

module.exports = function (env) {
    return merge(common(env), {
        mode: "production",
        optimization: {
            minimize: true,
            minimizer: [new TerserPlugin()],
        },
        devtool: "nosources-source-map",
        output: {
            filename: "treemap-2d.min.js",
        },
    });
};

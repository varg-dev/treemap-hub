/* eslint-disable no-undef */
/* eslint-disable @typescript-eslint/no-var-requires */

const karma = require('./karma.common.js');

karma.watch = false;
karma.singleRun = true;

module.exports = function (config) {
    config.set(karma);
};

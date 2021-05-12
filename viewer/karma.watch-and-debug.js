/* eslint-disable no-undef */
/* eslint-disable @typescript-eslint/no-var-requires */

const karma = require('./karma.common.js');

karma.port = 9876;
karma.watch = true;
karma.singleRun = false;

module.exports = function (config) {
    config.set(karma);
};

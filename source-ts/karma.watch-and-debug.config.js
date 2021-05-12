
import { karma } from './karma.base.config.js';

karma.port = 9876;
karma.watch = true;
karma.singleRun = false;

// eslint-disable-next-line no-undef
module.exports = function (config) {
    config.set(karma);
};


import { karma } from './karma.base.config.js';

karma.watch = false;
karma.singleRun = true;

// eslint-disable-next-line no-undef
module.exports = function (config) {
    config.set(karma);
};

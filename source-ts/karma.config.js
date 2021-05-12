
var karma = require('./karma.base.config.js');

karma.watch = false;
karma.singleRun = true;

module.exports = function (config) { config.set(karma); };

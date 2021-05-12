
var karma = require('./karma.base.config.js');

karma.port = 9876;
karma.watch = true;
karma.singleRun = false;

module.exports = function (config) { config.set(karma); };

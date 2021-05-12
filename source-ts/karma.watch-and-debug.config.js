
var karma = require('./karma.base.config.js');

// http://www.fretless.com/debugging-karma-tests-with-phantomjs/

// open http://localhost:9876/debug.html and use, e.g., chrome dev tools for debugging
// in order to set a break point, write 'debugger;' in the test or code ... :D

karma.port = 9876;
karma.watch = true;
karma.singleRun = false;

module.exports = function (config) { config.set(karma); };

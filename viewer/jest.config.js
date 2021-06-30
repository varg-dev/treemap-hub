/* eslint-env node */

module.exports = {
    coverageDirectory: 'dist/coverage',
    preset: 'ts-jest',
    setupFiles: ['jest-canvas-mock'],
    testEnvironment: 'jsdom',
    testPathIgnorePatterns: ['/node_modules/', '/dist/', '/scripts/'],
};

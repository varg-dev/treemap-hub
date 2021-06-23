/* eslint-env node */
/* eslint-disable @typescript-eslint/no-var-requires */
const glslang = require('@webgpu/glslang')();
const fs = require('fs');
const path = require('path');

const SHADER_PATH = path.join(__dirname, '../source/assets/shaders');
const TARGET_PATH = path.join(__dirname, '../source/assets/spirv');
const SHADER_TYPE_MAP = {
    vert: 'vertex',
    frag: 'fragment',
    comp: 'compute',
};

const getAllFiles = (dirPath, arrayOfFiles) => {
    const files = fs.readdirSync(dirPath);

    arrayOfFiles = arrayOfFiles || [];

    files.forEach(file => {
        if (fs.statSync(dirPath + '/' + file).isDirectory()) {
            arrayOfFiles = getAllFiles(dirPath + '/' + file, arrayOfFiles);
        } else {
            arrayOfFiles.push(path.join(dirPath, '/', file));
        }
    });

    return arrayOfFiles;
};

const main = () => {
    // Ensure the output directory exists
    try {
        fs.statSync(TARGET_PATH);
    } catch {
        fs.mkdirSync(TARGET_PATH);
    }

    /** @type {string[]} */
    const shaderFiles = getAllFiles(SHADER_PATH);

    for (const shaderPath of shaderFiles) {
        const glsl = fs.readFileSync(shaderPath, { encoding: 'utf-8' });
        const type = SHADER_TYPE_MAP[shaderPath.split('.').pop()];

        const spirv = glslang.compileGLSL(glsl, type, false);
        const result = Buffer.from(new Uint8Array(spirv.buffer));
        const resultPath = path.join(TARGET_PATH, path.basename(shaderPath) + '.spv');

        fs.writeFileSync(resultPath, result);
    }
};

main();

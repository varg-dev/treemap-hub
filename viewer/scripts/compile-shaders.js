/* eslint-env node */
/* eslint-disable @typescript-eslint/no-var-requires */
const { execFileSync } = require("child_process");
const fs = require("fs");
const path = require("path");

const SHADER_PATH = path.join(__dirname, "../source/assets/shaders");
const TARGET_PATH = path.join(__dirname, "../source/assets/spirv");

const getAllFiles = (dirPath, arrayOfFiles) => {
    const files = fs.readdirSync(dirPath);

    arrayOfFiles = arrayOfFiles || [];

    files.forEach(file => {
        if (fs.statSync(dirPath + "/" + file).isDirectory()) {
            arrayOfFiles = getAllFiles(dirPath + "/" + file, arrayOfFiles);
        } else {
            arrayOfFiles.push(path.join(dirPath, "/", file));
        }
    });

    return arrayOfFiles;
};

// Ensure the output directory exists
try {
    fs.statSync(TARGET_PATH);
} catch {
    fs.mkdirSync(TARGET_PATH);
}

/** @type {string[]} */
const shaderFiles = getAllFiles(SHADER_PATH);

for (const shaderPath of shaderFiles) {
    execFileSync("glslc", [
        shaderPath,
        "-o",
        path.join(TARGET_PATH, path.basename(shaderPath) + ".spv"),
    ]);
}

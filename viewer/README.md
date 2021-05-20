# Treemap 2D Rendering Library (TypeScript)

## Prerequisites

-   [glslc](https://github.com/google/shaderc) - Compiles GLSL shaders to SPIR-V. The latest builds can be downloaded [here](https://github.com/google/shaderc/blob/main/downloads.md).

## Browser Support

In order to use the WebGPU-based Treemap viewer you have to use one of the following browsers:

-   [Chrome Canary](http://chrome.com/canary)
-   [Edge Canary](https://www.microsoftedgeinsider.com/en-us/download)
-   [Firefox Nightly](https://nightly.mozilla.org/)

For Chromium browsers (Chrome, Edge) you have to enable the `chrome://flags/#enable-unsafe-webgpu` or `edge://flags/#enable-unsafe-webgpu` flag respectively.

For Firefox you have to enable the settings `dom.webgpu.enabled` and `gfx.webrender.all` on the `about:config` page.

The implementation status and browser support of WebGPU can also be found on [GitHub](https://github.com/gpuweb/gpuweb/wiki/Implementation-Status).

import { TypedArray, createPresets } from './types/utils';
import type { TreemapLayout } from './types/treemap';
import { TreemapNode } from './treemap-node';
import { Vertex } from './vertex';
import { chooseArray } from './utils/array-helper';
import { generateSchemeBitmap } from './utils/color-scheme-helper';
import { loadShader } from './utils/shader-helper';

/**
 * WebGPU-based 2D Treemap Renderer.
 */
export class Renderer {
    private static PRESETS = createPresets({
        depth: ['default.vert', 'depth.frag'],
        outlines: ['outlines.vert', 'outlines.frag'],
    });

    private canvas: HTMLCanvasElement;
    private animationId: number;

    private adapter: GPUAdapter;
    private device: GPUDevice;
    private queue: GPUQueue;
    private context: GPUPresentationContext;
    private presentationFormat: GPUTextureFormat;
    private offscreenTexture: GPUTexture;
    private offscreenBuffer: GPUBuffer;

    private vertexBuffer: GPUBuffer;
    private instanceBuffer: GPUBuffer;
    private indexBuffer: GPUBuffer;
    private instanceCount: number;
    private indexCount: number;

    private renderParams: {
        resolution: number; // This assumes the same resolution for width & height
        maxDepth: number;
    };
    private renderParamsBuffer: GPUBuffer;

    private layoutData: TreemapLayout | undefined;
    private renderingPreset: keyof typeof Renderer.PRESETS;
    private colorScheme: ImageBitmap;
    private colorSchemeTexture: GPUTexture;
    private bindGroups: GPUBindGroup[];
    private renderPipeline: GPURenderPipeline;

    constructor(canvas: HTMLCanvasElement) {
        this.canvas = canvas;

        this.bindGroups = [];
        this.renderParams = {
            resolution: this.canvas.width,
            maxDepth: 0,
        };

        // Set default rendering preset
        this.renderingPreset = 'depth';

        // Create default color scheme
        this.setColorScheme('black', 'white');
    }

    public async initialize(): Promise<void> {
        await this.setupAPI();
    }

    public async loadTreemap(layoutData: TreemapLayout): Promise<void> {
        this.preprocessData(layoutData);

        if (!this.vertexBuffer || !this.indexBuffer) {
            const vertexData = new Float32Array(TreemapNode.vertexData());
            const indexData = new Uint32Array(TreemapNode.indexData());

            this.vertexBuffer = this.createBuffer(vertexData, GPUBufferUsage.VERTEX);
            this.indexBuffer = this.createBuffer(indexData, GPUBufferUsage.INDEX);
            this.indexCount = indexData.length;
        }

        const nodeCount = this.layoutData.length;
        const nodeSize = TreemapNode.bufferSize() / Float32Array.BYTES_PER_ELEMENT;
        const instanceData = new Float32Array(nodeSize * nodeCount);

        this.renderParams.maxDepth = 0;

        for (let i = 0; i < nodeCount; i++) {
            const bufferData = TreemapNode.instanceData(this.layoutData[i]);
            const nodeDepth = bufferData[5];

            if (nodeDepth > this.renderParams.maxDepth) {
                this.renderParams.maxDepth = nodeDepth;
            }

            instanceData.set(bufferData, i * nodeSize);
        }

        this.instanceBuffer = this.createBuffer(instanceData, GPUBufferUsage.VERTEX);
        this.instanceCount = nodeCount;

        if (!this.renderPipeline) {
            await this.setupRenderPipeline();

            if (this.renderingPreset === 'depth') {
                this.createColorScheme();
            }
        }

        this.setRenderParams();
    }

    public start(): void {
        this.animationId = requestAnimationFrame(() => this.render());
    }

    public stop(): void {
        cancelAnimationFrame(this.animationId);
        this.animationId = null;
    }

    public isRunning(): boolean {
        return Boolean(this.animationId);
    }

    public async snapshot(): Promise<Blob> {
        if (this.isRunning()) {
            this.stop();
            this.render(true); // Renders only one frame
            this.start();
        } else {
            this.render(true);
        }

        await this.offscreenBuffer.mapAsync(GPUMapMode.READ);

        const buffer = this.offscreenBuffer.getMappedRange();
        const array = new Uint8ClampedArray(buffer.slice(0));
        const data = new ImageData(array, this.canvas.width);

        this.offscreenBuffer.unmap();

        const exportCanvas = document.createElement('canvas');
        exportCanvas.width = this.canvas.width;
        exportCanvas.height = this.canvas.height;

        const ctx = exportCanvas.getContext('2d');

        ctx.putImageData(data, 0, 0);

        return new Promise(resolve => exportCanvas.toBlob(resolve));
    }

    public async setRenderingPreset(preset: keyof typeof Renderer.PRESETS): Promise<void> {
        if (this.isRunning()) {
            this.stop();
        }

        this.renderingPreset = preset;

        // TODO: When rendering with proper depth/stencil testing, it might not be necessary
        // to recreate all buffers & bindgroups, but just the rendering pipeline as the buffer data
        // does not change
        if (this.layoutData) {
            this.renderPipeline = null;
            this.bindGroups = [];
            this.renderParamsBuffer.destroy();
            this.renderParamsBuffer = null;
            await this.loadTreemap(this.layoutData);

            if (!this.isRunning()) {
                this.start();
            }
        }
    }

    public async setColorScheme(startColor: string, endColor: string): Promise<void> {
        this.colorScheme = await generateSchemeBitmap(startColor, endColor);
    }

    private async setupAPI(): Promise<void> {
        try {
            this.adapter = await navigator.gpu.requestAdapter();
            this.device = await this.adapter.requestDevice();
            this.queue = this.device.queue;

            this.createPresentationContext();
        } catch (error) {
            console.error(
                'Error while setting up the API. Please make sure that your browser does support WebGPU.',
                error
            );
        }
    }

    private async setupRenderPipeline() {
        const [vertexShaderName, fragmentShaderName] = Renderer.PRESETS[this.renderingPreset];
        const [vertexShader, fragmentShader] = await Promise.all([
            loadShader(vertexShaderName),
            loadShader(fragmentShaderName),
        ]);

        const vertexShaderModule = this.device.createShaderModule({ code: vertexShader });
        const fragmentShaderModule = this.device.createShaderModule({ code: fragmentShader });

        this.renderPipeline = this.device.createRenderPipeline({
            primitive: {
                topology: 'triangle-list',
                frontFace: 'ccw',
                cullMode: 'none',
            },
            vertex: {
                module: vertexShaderModule,
                entryPoint: 'main',
                buffers: [Vertex.bufferLayout(), TreemapNode.bufferLayout()],
            },
            fragment: {
                module: fragmentShaderModule,
                entryPoint: 'main',
                targets: [
                    {
                        format: this.presentationFormat,
                    },
                ],
            },
        });
    }

    private createPresentationContext() {
        this.context = this.canvas.getContext('webgpu');
        this.presentationFormat = this.context.getPreferredFormat(this.adapter);

        this.context.configure({
            device: this.device,
            format: this.presentationFormat,
            usage: GPUTextureUsage.RENDER_ATTACHMENT,
        });

        // We recreate the offscreen target on resize (= context recreation)
        this.offscreenTexture = this.device.createTexture({
            format: this.presentationFormat,
            size: {
                width: this.canvas.width,
                height: this.canvas.height,
            },
            usage: GPUTextureUsage.COPY_SRC | GPUTextureUsage.RENDER_ATTACHMENT,
        });

        this.offscreenBuffer = this.device.createBuffer({
            size: this.canvas.width * this.canvas.height * 4,
            usage: GPUBufferUsage.COPY_DST | GPUBufferUsage.MAP_READ,
        });
    }

    private createBuffer(data: TypedArray, usage: GPUBufferUsageFlags) {
        const buffer = this.device.createBuffer({
            size: data.byteLength,
            usage,
            mappedAtCreation: true,
        });
        const BufferArray = chooseArray(data);
        const writeArray = new BufferArray(buffer.getMappedRange());

        writeArray.set(data);
        buffer.unmap();

        return buffer;
    }

    private writeBuffer(buffer: GPUBuffer, data: TypedArray) {
        this.queue.writeBuffer(buffer, 0, data);
    }

    private addBindGroup(...entries: GPUBindingResource[]) {
        const bindGroup = this.device.createBindGroup({
            layout: this.renderPipeline.getBindGroupLayout(this.bindGroups.length),
            entries: entries.map((entry, index) => ({ binding: index, resource: entry })),
        });

        this.bindGroups.push(bindGroup);
    }

    private preprocessData(layoutData: TreemapLayout) {
        // TODO: This is hacky because it assumes a node order by depth from root to deepest leaves
        // This should be handled with proper depth/stencil testing
        if (
            (this.renderingPreset === 'outlines' && layoutData[0][0] === 0) ||
            (this.renderingPreset === 'depth' && layoutData[0][0] !== 0)
        ) {
            this.layoutData = layoutData.reverse();
        } else {
            this.layoutData = layoutData;
        }
    }

    private createColorScheme() {
        this.colorSchemeTexture = this.device.createTexture({
            format: 'rgba8unorm',
            size: {
                width: this.colorScheme.width,
                height: this.colorScheme.height,
            },
            usage:
                GPUTextureUsage.COPY_DST |
                GPUTextureUsage.SAMPLED |
                GPUTextureUsage.RENDER_ATTACHMENT,
        });

        const sampler = this.device.createSampler();

        this.addBindGroup(sampler, this.colorSchemeTexture.createView());
    }

    private setRenderParams() {
        const data = new Float32Array([this.renderParams.resolution, this.renderParams.maxDepth]);

        if (this.renderParamsBuffer) {
            this.writeBuffer(this.renderParamsBuffer, data);
        } else {
            this.renderParamsBuffer = this.createBuffer(
                data,
                GPUBufferUsage.COPY_DST | GPUBufferUsage.UNIFORM
            );

            this.addBindGroup({ buffer: this.renderParamsBuffer });
        }
    }

    private render(offscreen = false): void {
        this.queue.copyExternalImageToTexture(
            { source: this.colorScheme },
            { texture: this.colorSchemeTexture },
            { width: this.colorScheme.width, height: this.colorScheme.height }
        );

        const frameView = offscreen
            ? this.offscreenTexture.createView()
            : this.context.getCurrentTexture().createView();

        const encoder = this.device.createCommandEncoder();
        const renderPass = encoder.beginRenderPass({
            colorAttachments: [
                {
                    view: frameView,
                    loadValue: {
                        r: 1.0,
                        g: 1.0,
                        b: 1.0,
                        a: 1.0,
                    },
                    storeOp: 'store',
                },
            ],
        });

        renderPass.setPipeline(this.renderPipeline);

        this.bindGroups.forEach((bindGroup, index) => {
            renderPass.setBindGroup(index, bindGroup);
        });

        renderPass.setVertexBuffer(0, this.vertexBuffer);
        renderPass.setVertexBuffer(1, this.instanceBuffer);
        renderPass.setIndexBuffer(this.indexBuffer, 'uint32');
        renderPass.drawIndexed(this.indexCount, this.instanceCount);
        renderPass.endPass();

        if (offscreen) {
            encoder.copyTextureToBuffer(
                {
                    texture: this.offscreenTexture,
                },
                {
                    buffer: this.offscreenBuffer,
                    bytesPerRow: this.canvas.width * 4,
                },
                {
                    width: this.canvas.width,
                    height: this.canvas.height,
                }
            );
        }

        this.queue.submit([encoder.finish()]);

        if (!offscreen) {
            this.animationId = requestAnimationFrame(() => this.render());
        }
    }
}

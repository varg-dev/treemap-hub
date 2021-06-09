import type { TreemapLayout } from './types/treemap';
import { TreemapNode } from './treemap-node';
import type { TypedArray } from './types/utils';
import { Vertex } from './vertex';
import { chooseArray } from './utils/array-helper';
import { generateSchemeBitmap } from './utils/color-scheme-helper';
import { loadShader } from './utils/shader-helper';

/**
 * WebGPU-based 2D Treemap Renderer.
 */
export class Renderer {
    static SWAP_CHAIN_FORMAT: GPUTextureFormat = 'bgra8unorm';

    private canvas: HTMLCanvasElement;
    private animationId: number;

    private device: GPUDevice;
    private queue: GPUQueue;
    private swapChain: GPUSwapChain;

    private vertexBuffer: GPUBuffer;
    private instanceBuffer: GPUBuffer;
    private indexBuffer: GPUBuffer;
    private instanceCount: number;
    private indexCount: number;

    private colorScheme: ImageBitmap;
    private colorSchemeTexture: GPUTexture;
    private bindGroups: GPUBindGroup[];
    private renderPipeline: GPURenderPipeline;

    constructor(canvas: HTMLCanvasElement) {
        this.canvas = canvas;

        this.bindGroups = [];

        // Create default color scheme
        this.setColorScheme('black', 'white');
    }

    public async initialize(): Promise<void> {
        await this.setupAPI();
    }

    public async loadTreemap(layoutData: TreemapLayout): Promise<void> {
        const nodeCount = layoutData.length;
        const nodeSize = TreemapNode.bufferSize() / Float32Array.BYTES_PER_ELEMENT;

        const vertexData = new Float32Array([0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0]);
        const instanceData = new Float32Array(nodeSize * nodeCount);
        const indexData = new Uint32Array([0, 1, 2, 2, 1, 3]);

        for (let i = 0; i < nodeCount; i++) {
            instanceData.set(TreemapNode.bufferData(layoutData[i]), i * nodeSize);
        }

        this.vertexBuffer = this.createBuffer(vertexData, GPUBufferUsage.VERTEX);
        this.instanceBuffer = this.createBuffer(instanceData, GPUBufferUsage.VERTEX);
        this.indexBuffer = this.createBuffer(indexData, GPUBufferUsage.INDEX);
        this.instanceCount = nodeCount;
        this.indexCount = indexData.length;

        if (!this.renderPipeline) {
            await this.setupRenderPipeline();
            this.createColorScheme();
        }
    }

    public start(): void {
        this.animationId = requestAnimationFrame(() => this.render());
    }

    public stop(): void {
        cancelAnimationFrame(this.animationId);
    }

    public setColorScheme(startColor: string, endColor: string): void {
        this.colorScheme = generateSchemeBitmap(startColor, endColor);
    }

    private async setupAPI(): Promise<void> {
        try {
            const adapter = await navigator.gpu.requestAdapter();

            this.device = await adapter.requestDevice();
            this.queue = this.device.queue;

            this.createSwapChain();
        } catch (error) {
            console.error(
                'Error while setting up the API. Please make sure that your browser does support WebGPU.',
                error
            );
        }
    }

    private async setupRenderPipeline() {
        const vertexShaderModule = this.device.createShaderModule({
            code: await loadShader('shader.vert.spv'),
        });
        const fragmentShaderModule = this.device.createShaderModule({
            code: await loadShader('shader.frag.spv'),
        });

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
                        format: Renderer.SWAP_CHAIN_FORMAT,
                    },
                ],
            },
        });
    }

    private createSwapChain() {
        const context = this.canvas.getContext('gpupresent');

        this.swapChain = context.configureSwapChain({
            device: this.device,
            format: Renderer.SWAP_CHAIN_FORMAT,
            usage: GPUTextureUsage.RENDER_ATTACHMENT,
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

    private addBindGroup(...entries: GPUBindingResource[]) {
        const bindGroup = this.device.createBindGroup({
            layout: this.renderPipeline.getBindGroupLayout(this.bindGroups.length),
            entries: entries.map((entry, index) => ({ binding: index, resource: entry })),
        });

        this.bindGroups.push(bindGroup);
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

    private render(): void {
        this.queue.copyExternalImageToTexture(
            { source: this.colorScheme },
            { texture: this.colorSchemeTexture },
            { width: this.colorScheme.width, height: this.colorScheme.height }
        );

        const frameView = this.swapChain.getCurrentTexture().createView();

        const encoder = this.device.createCommandEncoder();
        const renderPass = encoder.beginRenderPass({
            colorAttachments: [
                {
                    view: frameView,
                    loadValue: {
                        r: 0.0,
                        g: 0.0,
                        b: 0.0,
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

        this.queue.submit([encoder.finish()]);

        this.animationId = requestAnimationFrame(() => this.render());
    }
}

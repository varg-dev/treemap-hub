import type { TypedArray, TypedArrayConstructor } from './types/utils';
import type { TreemapLayout } from './types/treemap';
import { Vertex } from './vertex';

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
    private indexBuffer: GPUBuffer;
    private indexCount: number;

    private renderPipeline: GPURenderPipeline;

    constructor(canvas: HTMLCanvasElement) {
        this.canvas = canvas;
    }

    public async initialize(): Promise<void> {
        await this.setupAPI();
    }

    public async loadTreemap(_layoutData: TreemapLayout): Promise<void> {
        // TODO: use actual layout data
        const indices = [0, 1, 2, 1, 3, 2];

        this.vertexBuffer = this.createBuffer(
            new Float32Array([
                ...new Vertex([-1.0, -1.0, 0.0], [1.0, 0.0, 0.0]).data,
                ...new Vertex([-1.0, +1.0, 0.0], [0.0, 1.0, 0.0]).data,
                ...new Vertex([+1.0, -1.0, 0.0], [0.0, 0.0, 1.0]).data,
                ...new Vertex([+1.0, +1.0, 0.0], [0.0, 0.0, 0.0]).data,
            ]),
            GPUBufferUsage.VERTEX
        );

        this.indexBuffer = this.createBuffer(new Uint16Array(indices), GPUBufferUsage.INDEX);
        this.indexCount = indices.length;

        await this.setupRenderPipeline();
    }

    public start(): void {
        this.animationId = requestAnimationFrame(() => this.render());
    }

    public stop(): void {
        cancelAnimationFrame(this.animationId);
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
            code: await this.loadShader('/dist/assets/shaders/shader.vert.spv'),
        });
        const fragmentShaderModule = this.device.createShaderModule({
            code: await this.loadShader('/dist/assets/shaders/shader.frag.spv'),
        });

        const renderPipelineLayout = this.device.createPipelineLayout({
            bindGroupLayouts: [],
        });

        this.renderPipeline = this.device.createRenderPipeline({
            layout: renderPipelineLayout,
            primitive: {
                topology: 'triangle-list',
                frontFace: 'ccw',
                cullMode: 'none',
            },
            vertex: {
                module: vertexShaderModule,
                entryPoint: 'main',
                buffers: [Vertex.bufferLayout()],
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
        const writeArray = new (this.chooseArray(data))(buffer.getMappedRange());

        writeArray.set(data);
        buffer.unmap();

        return buffer;
    }

    private chooseArray(data: TypedArray): TypedArrayConstructor {
        if (data instanceof Float32Array) {
            return Float32Array;
        } else if (data instanceof Float64Array) {
            return Float64Array;
        } else if (data instanceof Int8Array) {
            return Int8Array;
        } else if (data instanceof Int16Array) {
            return Int16Array;
        } else if (data instanceof Int32Array) {
            return Int32Array;
        } else if (data instanceof Uint8Array) {
            return Uint8Array;
        } else if (data instanceof Uint16Array) {
            return Uint16Array;
        } else if (data instanceof Uint32Array) {
            return Uint32Array;
        }
    }

    private async loadShader(url: string) {
        const shader = await (await fetch(url)).arrayBuffer();

        return new Uint32Array(shader);
    }

    private render(): void {
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
        renderPass.setVertexBuffer(0, this.vertexBuffer);
        renderPass.setIndexBuffer(this.indexBuffer, 'uint16');
        renderPass.drawIndexed(this.indexCount);
        renderPass.endPass();

        this.queue.submit([encoder.finish()]);

        this.animationId = requestAnimationFrame(() => this.render());
    }
}

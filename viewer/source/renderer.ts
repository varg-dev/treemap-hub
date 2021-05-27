import type { TreemapLayout } from './types/treemap';
import { TreemapNode } from './treemap-node';
import type { TypedArray } from './types/utils';
import { Vertex } from './vertex';
import { chooseArray } from './utils/array-helper';
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
    private indexBuffer: GPUBuffer;
    private indexCount: number;

    private renderPipeline: GPURenderPipeline;

    constructor(canvas: HTMLCanvasElement) {
        this.canvas = canvas;
    }

    public async initialize(): Promise<void> {
        await this.setupAPI();
    }

    public async loadTreemap(layoutData: TreemapLayout): Promise<void> {
        const nodeCount = layoutData.length;
        const treemapNodes: TreemapNode[] = [];
        const vertexData = new Float32Array(TreemapNode.VERTEX_BUFFER_SIZE * nodeCount);
        const indexData = new Uint32Array(TreemapNode.INDEX_BUFFER_SIZE * nodeCount);

        for (let i = 0; i < nodeCount; i++) {
            const node = new TreemapNode(layoutData[i]);

            treemapNodes.push(node);

            vertexData.set(node.vertices(), i * TreemapNode.VERTEX_BUFFER_SIZE);
            indexData.set(node.indices(i * 4), i * TreemapNode.INDEX_BUFFER_SIZE);
        }

        this.vertexBuffer = this.createBuffer(vertexData, GPUBufferUsage.VERTEX);
        this.indexBuffer = this.createBuffer(indexData, GPUBufferUsage.INDEX);
        this.indexCount = indexData.length;

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
            code: await loadShader('shader.vert.spv'),
        });
        const fragmentShaderModule = this.device.createShaderModule({
            code: await loadShader('shader.frag.spv'),
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
        const BufferArray = chooseArray(data);
        const writeArray = new BufferArray(buffer.getMappedRange());

        writeArray.set(data);
        buffer.unmap();

        return buffer;
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
        renderPass.setIndexBuffer(this.indexBuffer, 'uint32');
        renderPass.drawIndexed(this.indexCount);
        renderPass.endPass();

        this.queue.submit([encoder.finish()]);

        this.animationId = requestAnimationFrame(() => this.render());
    }
}

import { calculateBufferSize, createVertexBufferLayout } from './utils/buffer-helper';
import type { TreemapNodeData } from './types/treemap';

export class TreemapNode {
    private static VERTEX_BUFFER_ATTRIBUTES: GPUVertexFormat[] = [
        'float32x2', // vec2 position
        'float32x2', // vec2 size
        'float32x3', // vec3 info (ID, depth, isLeaf)
    ];

    static bufferData(data: TreemapNodeData): number[] {
        const [id, depth, isLeaf, x, y, width, height] = data;

        return [x, y, width, height, id, depth, isLeaf];
    }

    static bufferLayout(): GPUVertexBufferLayout {
        return createVertexBufferLayout('instance', 1, TreemapNode.VERTEX_BUFFER_ATTRIBUTES);
    }

    static bufferSize(): number {
        return calculateBufferSize(TreemapNode.VERTEX_BUFFER_ATTRIBUTES);
    }
}

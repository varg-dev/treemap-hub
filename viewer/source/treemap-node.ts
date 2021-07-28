import { calculateBufferSize, createVertexBufferLayout } from './utils/buffer-helper';
import type { TreemapNodeData } from './types/treemap';

export class TreemapNode {
    private static VERTEX_BUFFER_ATTRIBUTES: GPUVertexFormat[] = [
        'float32x2', // vec2 position
        'float32x2', // vec2 size
        'float32x3', // vec3 info (ID, depth, isLeaf)
    ];

    static vertexData(): number[] {
        // prettier-ignore
        return [
            // top border (w = 0)
            0, 0, 0, 0,
            1, 0, 0, 0,
            0, 1, 0, 0,
            1, 1, 0, 0,
            // right border (w = 1)
            0, 0, 0, 1,
            1, 0, 0, 1,
            0, 1, 0, 1,
            1, 1, 0, 1,
            // bottom border (w = 2)
            0, 0, 0, 2,
            1, 0, 0, 2,
            0, 1, 0, 2,
            1, 1, 0, 2,
            // left border (w = 3)
            0, 0, 0, 3,
            1, 0, 0, 3,
            0, 1, 0, 3,
            1, 1, 0, 3,
        ];
    }

    static indexData(): number[] {
        return [
            // top border
            0, 1, 2, 2, 1, 3,
            // right border
            4, 5, 6, 6, 5, 7,
            // bottom border
            8, 9, 10, 10, 9, 11,
            // left border
            12, 13, 14, 14, 13, 15,
        ];
    }

    static instanceData(data: TreemapNodeData): number[] {
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

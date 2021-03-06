import { createVertexBufferLayout } from './utils/buffer-helper';

export class Vertex {
    static bufferLayout(): GPUVertexBufferLayout {
        return createVertexBufferLayout('vertex', 0, ['float32x4']);
    }
}

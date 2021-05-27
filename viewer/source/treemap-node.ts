import { Color, Vertex } from './vertex';
import type { TreemapNodeData } from './types/treemap';

export class TreemapNode {
    public static VERTEX_BUFFER_SIZE = 4 * 6;
    public static INDEX_BUFFER_SIZE = 6;

    #vertices: Vertex[];

    constructor(data: TreemapNodeData) {
        const [x, y, width, height] = data;
        const color: Color = [0, 0, 0].map(() => Math.random()) as Color;

        this.#vertices = [];

        this.#vertices.push(new Vertex([x, y, 0.0], color));
        this.#vertices.push(new Vertex([x, y + height, 0.0], color));
        this.#vertices.push(new Vertex([x + width, y, 0.0], color));
        this.#vertices.push(new Vertex([x + width, y + height, 0.0], color));
    }

    public vertices(): number[] {
        return this.#vertices.map(vertex => vertex.data).reduce((agg, val) => [...agg, ...val]);
    }

    public indices(offset: number): number[] {
        return [0, 1, 2, 1, 3, 2].map(index => index + offset);
    }
}

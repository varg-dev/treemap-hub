type Position = [number, number, number];
export type Color = [number, number, number];

export class Vertex {
    #id: number;
    #depth: number;
    #isLeaf: boolean;
    #position: Position;
    #color: Color;

    constructor(id: number, depth: number, isLeaf: boolean, position: Position, color: Color) {
        this.id = id;
        this.depth = depth;
        this.isLeaf = isLeaf;
        this.position = position;
        this.color = color;
    }

    static bufferLayout(): GPUVertexBufferLayout {
        return {
            // Vertex size = 4 byte per float * 9 float values per vertex (3 info, 3 position, 3 color)
            arrayStride: 4 * 9,
            stepMode: 'vertex',
            attributes: [
                {
                    format: 'float32x3',
                    offset: 0,
                    shaderLocation: 0,
                },
                {
                    format: 'float32x3',
                    offset: 4 * 3, // 4 byte per float * 3 float values (info) to skip
                    shaderLocation: 1,
                },
                {
                    format: 'float32x3',
                    offset: 4 * 6, // 4 byte per float * 6 float values (info & position) to skip
                    shaderLocation: 2,
                },
            ],
        };
    }

    get data(): number[] {
        return [this.id, this.depth, Number(this.isLeaf), ...this.position, ...this.color];
    }

    get id(): number {
        return this.#id;
    }

    set id(id: number) {
        if (id < 0) {
            throw new Error('Invalid id specified');
        }

        this.#id = id;
    }

    get depth(): number {
        return this.#depth;
    }

    set depth(depth: number) {
        if (depth < 0) {
            throw new Error('Invalid depth specified');
        }

        this.#depth = depth;
    }

    get isLeaf(): boolean {
        return this.#isLeaf;
    }

    set isLeaf(isLeaf: boolean) {
        this.#isLeaf = isLeaf;
    }

    get position(): Position {
        return this.#position;
    }

    set position(position: Position) {
        this.#position = position;
    }

    get color(): Color {
        return this.#color;
    }

    set color(color: Color) {
        if (color.some(value => value < 0.0 || value > 1.0)) {
            throw new Error('Invalid color specified');
        }

        this.#color = color;
    }
}

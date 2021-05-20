type Position = [number, number, number];
type Color = [number, number, number];

export class Vertex {
    #position: Position;
    #color: Color;

    constructor(position: Position, color: Color) {
        this.position = position;
        this.color = color;
    }

    static bufferLayout(): GPUVertexBufferLayout {
        return {
            // Vertex size = 4 byte per float * 6 float values per vertex (3 position, 3 color)
            arrayStride: 4 * 6,
            stepMode: "vertex",
            attributes: [
                {
                    format: "float32x3",
                    offset: 0,
                    shaderLocation: 0,
                },
                {
                    format: "float32x3",
                    offset: 4 * 3, // 4 byte per float * 3 float values (position) to skip
                    shaderLocation: 1,
                },
            ],
        };
    }

    get data(): number[] {
        return [...this.position, ...this.color];
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
            throw new Error("Invalid color specified");
        }

        this.#color = color;
    }
}

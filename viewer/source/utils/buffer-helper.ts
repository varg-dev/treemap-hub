const VERTEX_FORMAT_SIZES: Record<GPUVertexFormat, number> = {
    float16x2: 4, // = 2 bytes * 2
    float16x4: 8, // = 2 bytes * 4
    float32: 4, // = 4 bytes * 1
    float32x2: 8, // = 4 bytes * 2
    float32x3: 12, // = 4 bytes * 3
    float32x4: 16, // = 4 bytes * 4
    sint16x2: 4, // = 2 bytes * 2
    sint16x4: 8, // = 2 bytes * 4
    sint32: 4, // = 4 bytes * 1
    sint32x2: 8, // = 4 bytes * 2
    sint32x3: 12, // = 4 bytes * 3
    sint32x4: 16, // = 4 bytes * 4
    sint8x2: 2, // = 1 byte * 2
    sint8x4: 4, // = 1 byte * 4
    snorm16x2: 4, // = 2 bytes * 2
    snorm16x4: 8, // = 2 bytes * 4
    snorm8x2: 2, // = 1 byte * 2
    snorm8x4: 4, // = 1 byte * 4
    uint16x2: 4, // = 2 bytes * 2
    uint16x4: 8, // = 2 bytes * 4
    uint32: 4, // = 4 bytes * 1
    uint32x2: 8, // = 4 bytes * 2
    uint32x3: 12, // = 4 bytes * 3
    uint32x4: 16, // = 4 bytes * 4
    uint8x2: 2, // = 1 byte * 2
    uint8x4: 4, // = 1 byte * 4
    unorm16x2: 4, // = 2 bytes * 2
    unorm16x4: 8, // = 2 bytes * 4
    unorm8x2: 2, // = 1 byte * 2
    unorm8x4: 4, // = 1 byte * 4
};

export const createVertexBufferLayout = (
    stepMode: GPUVertexStepMode,
    locationOffset: number,
    formats: GPUVertexFormat[]
): GPUVertexBufferLayout => {
    let offset = 0;
    const attributes: GPUVertexAttribute[] = formats.map((format, index) => {
        const attribute = {
            format,
            offset,
            shaderLocation: locationOffset + index,
        };

        offset += VERTEX_FORMAT_SIZES[format];

        return attribute;
    });

    return {
        stepMode,
        arrayStride: offset,
        attributes,
    };
};

export const calculateBufferSize = (formats: GPUVertexFormat[]): number => {
    return formats.reduce((sum, format) => sum + VERTEX_FORMAT_SIZES[format], 0);
};

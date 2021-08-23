#version 450

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inOffset;
layout(location = 2) in vec2 inSize;
layout(location = 3) in vec3 inInfo;

layout(set = 0, binding = 0) uniform Params {
    float resolution;
    float maxDepth;
} params;

void main() {
    float lineWidth = (4.0 / sqrt(inInfo.y + 0.1)) / params.resolution;
    vec2 nodePosition = inPosition.xy;

    switch (int(inPosition.w)) {
        case 0:
            // Scale & move top border
            nodePosition *= vec2(inSize.x, lineWidth);
            nodePosition += inOffset + vec2(0.0, inSize.y - lineWidth);
            break;

        case 1:
            // Scale & move right border
            nodePosition *= vec2(lineWidth, inSize.y);
            nodePosition += inOffset + vec2(inSize.x - lineWidth, 0.0);
            break;

        case 2:
            // Scale & move bottom border
            nodePosition *= vec2(inSize.x, lineWidth);
            nodePosition += inOffset;
            break;

        case 3:
            // Scale & move left border
            nodePosition *= vec2(lineWidth, inSize.y);
            nodePosition += inOffset;
            break;
    }

    // Map input xy-coords of range [0.0, 1.0] to range [-1.0, 1.0]
    vec3 position = vec3(nodePosition * 2.0 - 1.0, inPosition.z);

    gl_Position = vec4(position, 1.0);
}

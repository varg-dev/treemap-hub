#version 450

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inOffset;
layout(location = 2) in vec2 inSize;
layout(location = 3) in vec3 inInfo;

layout(set = 1, binding = 0) uniform Params {
    float resolution;
    float maxDepth;
} params;

layout(location = 0) out vec3 outInfo;

void main() {
    float lineWidth = 1.0 / params.resolution;

    if (inInfo.y < 3) {
        lineWidth *= 8.0 - inInfo.y;
    } else if (inInfo.y < 5) {
        lineWidth *= 5.0 - inInfo.y;
    }{
        lineWidth *= 1;
    }

    vec2 nodePosition = inPosition.xy;

    if (inPosition.w == 0.0) {
        // Scale & move top border
        nodePosition *= vec2(inSize.x, lineWidth);
        nodePosition += inOffset + vec2(0.0, inSize.y - lineWidth);
    } else if (inPosition.w == 1.0) {
        // Scale & move right border
        nodePosition *= vec2(lineWidth, inSize.y);
        nodePosition += inOffset + vec2(inSize.x - lineWidth, 0.0);
    } else if (inPosition.w == 2.0) {
        // Scale & move bottom border
        nodePosition *= vec2(inSize.x, lineWidth);
        nodePosition += inOffset;
    } else if (inPosition.w == 3.0) {
        // Scale & move left border
        nodePosition *= vec2(lineWidth, inSize.y);
        nodePosition += inOffset;
    }

    // Map input xy-coords of range [0.0, 1.0] to range [-1.0, 1.0]
    vec3 position = vec3(nodePosition * 2.0 - 1.0, inPosition.z);

    gl_Position = vec4(position, 1.0);
    outInfo = inInfo;
}

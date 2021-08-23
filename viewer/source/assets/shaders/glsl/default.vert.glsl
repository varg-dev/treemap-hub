#version 450

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inOffset;
layout(location = 2) in vec2 inSize;
layout(location = 3) in vec3 inInfo;

layout(location = 0) out vec3 outInfo;

void main() {
    // Apply offset & scale for each vertex per instance
    vec2 nodePosition = inPosition.xy * inSize + inOffset;

    // Map input xy-coords of range [0.0, 1.0] to range [-1.0, 1.0]
    vec3 position = vec3(nodePosition * 2.0 - 1.0, inPosition.z);

    gl_Position = vec4(position, 1.0);
    outInfo = inInfo;
}

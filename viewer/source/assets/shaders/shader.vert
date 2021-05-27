#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 outColor;

void main() {
    // Map input xy-coords of range [0.0, 1.0] to range [-1.0, 1.0]
    vec3 position = vec3(inPosition.xy * 2.0 - 1.0, inPosition.z);

    gl_Position = vec4(position, 1.0);
    outColor = inColor;
}

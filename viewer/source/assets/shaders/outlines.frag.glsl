#version 450

layout(location = 0) in struct NodeInfo {
    float id;
    float depth;
    float isLeaf;
} info;

layout(set = 0, binding = 0) uniform sampler sColorScheme;
layout(set = 0, binding = 1) uniform texture2D tColorScheme;
layout(set = 1, binding = 0) uniform Params {
    float resolution;
    float maxDepth;
} params;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = vec4(vec3(0.0), 1.0);
}

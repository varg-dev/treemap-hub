#version 450

layout(location = 0) in vec3 inInfo;

layout(set = 0, binding = 0) uniform sampler sColorScheme;
layout(set = 0, binding = 1) uniform texture2D tColorScheme;
layout(set = 1, binding = 0) uniform Params {
    float maxDepth;
} params;

layout(location = 0) out vec4 fragColor;

void main() {
    vec2 coords = vec2(inInfo.y / params.maxDepth, 0.0);
    vec3 color = texture(sampler2D(tColorScheme, sColorScheme), coords).rgb;

    fragColor = vec4(color, 1.0);
}

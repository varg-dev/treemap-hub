[[block]]
struct Params {
    resolution: f32;
    maxDepth: f32;
};

[[group(0), binding(0)]] var sColorScheme: sampler;
[[group(0), binding(1)]] var tColorScheme: texture_2d<f32>;
[[group(1), binding(0)]] var<uniform> params: Params;

[[stage(fragment)]]
fn main([[location(0)]] nodeInfo: vec3<f32>) -> [[location(0)]] vec4<f32> {
    var coords: vec2<f32> = vec2<f32>(nodeInfo.y / params.maxDepth, 0.0);
    var color: vec3<f32> = textureSample(tColorScheme, sColorScheme, coords).rgb;

    return vec4<f32>(color, 1.0);
}

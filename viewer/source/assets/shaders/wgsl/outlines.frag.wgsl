[[group(0), binding(0)]] var sColorScheme: sampler;
[[group(0), binding(1)]] var tColorScheme: texture_2d<f32>;

[[stage(fragment)]]
fn main() -> [[location(0)]] vec4<f32> {
    var color: vec3<f32> = textureSample(tColorScheme, sColorScheme, vec2<f32>(0.0, 0.0)).rgb;

    return vec4<f32>(color, 1.0);
}

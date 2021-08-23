struct VertexOutput {
    [[builtin(position)]] position: vec4<f32>;
    [[location(0)]] outInfo: vec3<f32>;
};

[[stage(vertex)]]
fn main(
    [[location(0)]] inPosition: vec4<f32>,
    [[location(1)]] inOffset: vec2<f32>,
    [[location(2)]] inSize: vec2<f32>,
    [[location(3)]] inInfo: vec3<f32>
) -> VertexOutput {
    // Apply offset & scale for each vertex per instance
    var nodePosition: vec2<f32> = inPosition.xy * inSize + inOffset;

    // Map input xy-coords of range [0.0, 1.0] to range [-1.0, 1.0]
    var position: vec4<f32> = vec4<f32>(nodePosition * 2.0 - 1.0, inPosition.z, 1.0);

    return VertexOutput(position, inInfo);
}

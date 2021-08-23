[[block]]
struct Params {
    resolution: f32;
    maxDepth: f32;
};

struct VertexOutput {
    [[builtin(position)]] position: vec4<f32>;
};

[[group(0), binding(0)]] var<uniform> params: Params;

[[stage(vertex)]]
fn main(
    [[location(0)]] inPosition: vec4<f32>,
    [[location(1)]] inOffset: vec2<f32>,
    [[location(2)]] inSize: vec2<f32>,
    [[location(3)]] inInfo: vec3<f32>
) -> VertexOutput {
    var lineWidth: f32 = (4.0 / sqrt(inInfo.y + 0.1)) / params.resolution;
    var nodePosition: vec2<f32> = inPosition.xy;

    switch(i32(inPosition.w)) {
        case 0: {
            // Scale & move top border
            nodePosition = nodePosition * vec2<f32>(inSize.x, lineWidth);
            nodePosition = nodePosition + inOffset + vec2<f32>(0.0, inSize.y - lineWidth);
            break;
        }

        case 1: {
            // Scale & move right border
            nodePosition = nodePosition * vec2<f32>(lineWidth, inSize.y);
            nodePosition = nodePosition + inOffset + vec2<f32>(inSize.x - lineWidth, 0.0);
            break;
        }

        case 2: {
            // Scale & move bottom border
            nodePosition = nodePosition * vec2<f32>(inSize.x, lineWidth);
            nodePosition = nodePosition + inOffset;
            break;
        }

        case 3: {
            // Scale & move left border
            nodePosition = nodePosition * vec2<f32>(lineWidth, inSize.y);
            nodePosition = nodePosition + inOffset;
            break;
        }

        default: {
            break;
        }
    }

    // Map input xy-coords of range [0.0, 1.0] to range [-1.0, 1.0]
    var position: vec4<f32> = vec4<f32>(nodePosition * 2.0 - 1.0, inPosition.z, 1.0);

    return VertexOutput(position);
}

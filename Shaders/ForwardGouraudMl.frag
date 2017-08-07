#include <simd/simd.h> // This is needed for matrix_float4x4.

using namespace metal;

struct VertexI
{
    float4 position;
    float4 normal;
    float4 tangent;
    float4 bitangent;
    float2 texcoord;
    float2 pad;
};

struct VertexO
{
    float4 position [[position]];
    float4 color;
};

struct Uniforms
{
    matrix_float4x4 mvp;
};

fragment float4 fragment_main(VertexO inVertex [[stage_in]])
{
    return inVertex.color;
}

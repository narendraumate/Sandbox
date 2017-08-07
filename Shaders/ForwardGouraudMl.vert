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

vertex VertexO vertex_main(device VertexI *vertices [[buffer(0)]], constant Uniforms & uniforms [[buffer(1)]], uint vid [[vertex_id]])
{
    VertexO v;
    v.position = uniforms.mvp * vertices[vid].position;
    v.color = vertices[vid].normal;
    return v;
}

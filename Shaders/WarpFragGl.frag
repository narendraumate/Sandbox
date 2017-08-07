//
//
//
//

#version 410 core

layout (location = 0) out vec4 fNewColor;
layout (location = 1) out float fNewDepth;

uniform sampler2D oldColorTexture;
uniform sampler2D oldDepthTexture;

//uniform mat4 straightWarpMat;
//uniform mat4 invertedWarpMat;
//Mat4 straightWarpMat = newProj * newView * oldView.inverse() * oldProj.inverse();
//Mat4 invertedWarpMat = straightWarpMat.inverse();

//Mat4 invertedWarpMat = (newProj * newView * oldView.inverse() * oldProj.inverse()).inverse();
//Mat4 invertedWarpMat = (newViewProj * oldViewProj.inverse()).inverse();

uniform mat4 oldViewProjectionStraight;
uniform mat4 oldViewProjectionInverted;
uniform mat4 newViewProjectionStraight;
uniform mat4 newViewProjectionInverted;

in vec2 fTex;

void main()
{
#if 0
    fNewColor = texture(oldColorTexture, fTex).rgba;
    gl_FragDepth = fNewDepth = texture(oldDepthTexture, fTex).r;
#else
    vec2 oldUv = fTex;
    vec2 oldCoord = vec2((oldUv.x * 2.0f) - 1.0f, (oldUv.y * 2.0f) - 1.0f);
    float oldDepth = (texture(oldDepthTexture, oldUv).r * 2.0f) - 1.0f;
    vec4 oldVertex = vec4(oldCoord.x, oldCoord.y, oldDepth, 1.0f);
    vec4 newVertex = oldViewProjectionStraight * newViewProjectionInverted * oldVertex;
    if (newVertex.w > 0.0f)
    {
        newVertex = newVertex / newVertex.w;
        float newDepth = (newVertex.z + 1.0f) / 2.0f;
        vec2 newCoord = vec2(newVertex.x, newVertex.y);
        vec2 newUv = vec2((newCoord.x + 1.0f) / 2.0f, (newCoord.y + 1.0f) / 2.0f);
        if (newUv.x > 0.0f && newUv.x < 1.0f && newUv.y > 0.0f && newUv.y < 1.0f)
        {
            fNewColor = texture(oldColorTexture, newUv).rgba;
            gl_FragDepth = fNewDepth = newDepth;
        }
        else
        {
            fNewColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
            gl_FragDepth = fNewDepth = 1.0f;
        }
    }
    else
    {
        fNewColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        gl_FragDepth = fNewDepth = 1.0f;
    }
#endif
}

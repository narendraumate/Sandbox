//
//
//
//

#version 410 core

layout (location = 0) out vec4 fColor;
layout (location = 1) out float fDepth;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;

in vec2 fTex;

void main()
{
    fColor = texture(colorTexture, fTex).rgba;
    gl_FragDepth = fDepth = texture(depthTexture, fTex).r;
}

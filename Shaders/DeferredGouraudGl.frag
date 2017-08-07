//
// http://http.developer.nvidia.com/CgTutorial/cg_tutorial_chapter05.html
// C2E2f_passthrough
//

#version 410 core

in vec4 vColor;
in float vDepth;

layout(location = 0) out vec4 fColor;
layout(location = 1) out float fDepth;

void main()
{
	fColor = vColor;
    gl_FragDepth = fDepth = vDepth;
}

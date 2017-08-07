//
// http://http.developer.nvidia.com/CgTutorial/cg_tutorial_chapter05.html
// C5E2v_fragmentLighting
//

#version 410 core

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in vec3 vNormal;
layout( location = 2 ) in vec3 vTangent;
layout( location = 3 ) in vec3 vBitangent;
layout( location = 4 ) in vec2 vTexcoord;
layout( location = 5 ) in vec2 vPad;

out vec4 fPosition;
out vec4 fNormal;
out vec4 fTangent;
out vec4 fBitangent;
out vec2 fTexcoord;
out vec2 fPad;

uniform mat3 worldViewNorm;
uniform mat4 worldViewProjection;

void main()
{
	gl_Position = worldViewProjection * vec4(vPosition.rgb, 1.0);
	fPosition = gl_Position;
	fNormal = vec4(normalize(worldViewNorm * vNormal.rgb), 0.0);
	fTangent = vec4(normalize(worldViewNorm * vTangent.rgb), 0.0);
	fBitangent = vec4(normalize(worldViewNorm * vBitangent.rgb), 0.0);
	fTexcoord = vTexcoord;
    fPad = vPad;
}

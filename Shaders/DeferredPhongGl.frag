//
// http://http.developer.nvidia.com/CgTutorial/cg_tutorial_chapter05.html
// C5E3f_basicLight
//

#version 410 core

#define MAX_LIGHT_COUNT 256

in vec4 fPosition;
in vec3 fNormal;
in vec3 fTangent;
in vec3 fBitangent;
in vec2 fTexcoord;
in vec2 fPad;

layout(location = 0) out vec4 fColor;
layout(location = 1) out float fDepth;

uniform int lightCount;
uniform vec3 lightCoefficients[MAX_LIGHT_COUNT];
uniform vec3 lightColor[MAX_LIGHT_COUNT];
uniform vec3 lightPosition[MAX_LIGHT_COUNT];

uniform vec3 eyePosition;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
//uniform vec3 transmittance;
uniform vec3 emission;
uniform float shininess;
//uniform float ior;
uniform sampler2D ambientTextureSampler;
uniform sampler2D diffuseTextureSampler;
uniform sampler2D specularTextureSampler;
uniform sampler2D specularHighlightTextureSampler;
uniform sampler2D bumpTextureSampler;
uniform sampler2D displacementTextureSampler;
uniform sampler2D alphaTextureSampler;

void main()
{
	vec3 P = fPosition.xyz;

	// Disable Bump Map
    // vec3 N = normalize(fNormal.rgb);
	// Enable Bump Map
	mat3 TBN = mat3(normalize(fTangent.rgb), normalize(fBitangent.rgb), normalize(fNormal.rgb));
	vec3 N = normalize(TBN * (texture(bumpTextureSampler, vec2(fTexcoord.x, fTexcoord.y)).rgb * 2.0f - 1.0f));

    vec3 emission_ = vec3(0, 0, 0);
    vec3 ambient_ = vec3(0, 0, 0);
    vec3 diffuse_ = vec3(0, 0, 0);
    vec3 specular_ = vec3(0, 0, 0);

    // emission
    emission_ = emission;

    // ambient
    ambient_ = ambient * texture(ambientTextureSampler, fTexcoord).rgb;

    for (int lightIndex = 0; lightIndex < MAX_LIGHT_COUNT; ++lightIndex)
    {
        // termination requires this hack
        if (lightIndex >= lightCount)
        {
            continue;
        }

        // attenuation, diffuse light and specular light
        float attenuation = 0.0f;
        float diffuseLight = 0.0f;
        float specularLight = 0.0f;
        {
            vec3 L = normalize(lightPosition[lightIndex] - P);

            float d = length(L);
            attenuation = 1 / (lightCoefficients[lightIndex].x + lightCoefficients[lightIndex].y * d + lightCoefficients[lightIndex].z * d * d);

            diffuseLight = max(dot(N, L), 0);
            if (diffuseLight <= 0.0f)
            {
                specularLight = 0.0f;
            }
            else
            {
                vec3 V = normalize(eyePosition - P);
                vec3 H = normalize(L + V);
                specularLight = pow(max(dot(N, H), 0), shininess);
            }
        }

        // diffuse
        diffuse_ = diffuse_ + (attenuation * diffuse * texture(diffuseTextureSampler, fTexcoord).rgb * lightColor[lightIndex].rgb * diffuseLight);

        // specular
        specular_ = specular_ + (attenuation * specular * texture(specularTextureSampler, fTexcoord).rgb * lightColor[lightIndex].rgb * specularLight);
    }
	fColor = vec4(emission_ + ambient_ + diffuse_ + specular_, 1.0f);
    gl_FragDepth = fDepth = 1.0f / fPosition.w;
}

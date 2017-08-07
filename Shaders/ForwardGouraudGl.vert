//
// http://http.developer.nvidia.com/CgTutorial/cg_tutorial_chapter05.html
// C5E1v_basicLight
//

#version 410 core

#define MAX_LIGHT_COUNT 256

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in vec3 vNormal;
layout( location = 2 ) in vec3 vTangent;
layout( location = 3 ) in vec3 vBitangent;
layout( location = 4 ) in vec2 vTexcoord;
layout( location = 5 ) in vec2 vPad;

out vec4 vColor;
out float vDepth;

uniform mat3 worldNormal;
uniform mat4 worldViewProjection;

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

uniform int rangeZeroToOne;

float C5E6_attenuation(vec3 L, vec3 coefficients)
{
    float d = length(L);
    return 1 / (coefficients.x + coefficients.y * d + coefficients.z * d * d);
}

void main()
{
	gl_Position = worldViewProjection * vec4(vPosition.rgb, 1.0);
	vec3 P = gl_Position.xyz;

	// Disable Bump Map
	// vec3 N = normalize(worldNormal * vNormal.rgb);
	// Enable Bump Map
	mat3 TBN = mat3(normalize(worldNormal * vTangent.rgb), normalize(worldNormal * vBitangent.rgb), normalize(worldNormal * vNormal.rgb));
	vec3 N = normalize(TBN * (texture(bumpTextureSampler, vTexcoord).rgb * 2.0 - 1.0));

    vec3 emission_ = vec3(0, 0, 0);
    vec3 ambient_ = vec3(0, 0, 0);
    vec3 diffuse_ = vec3(0, 0, 0);
    vec3 specular_ = vec3(0, 0, 0);

    // emission
    emission_ = emission;

    // ambient
    ambient_ = ambient * texture(ambientTextureSampler, vTexcoord).rgb;

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
            if (diffuseLight <= 0.0)
            {
                specularLight = 0.0;
            }
            else
            {
                vec3 V = normalize(eyePosition - P);
                vec3 H = normalize(L + V);
                specularLight = pow(max(dot(N, H), 0), shininess);
            }
        }

        // diffuse
        diffuse_ = diffuse_ + (diffuse * texture(diffuseTextureSampler, vTexcoord).rgb * lightColor[lightIndex].rgb * diffuseLight);

        // specular
        specular_ = specular_ + (specular * texture(specularTextureSampler, vTexcoord).rgb * lightColor[lightIndex].rgb * specularLight);
    }
	vColor = vec4(emission_ + ambient_ + diffuse_ + specular_, 1);
	vDepth = (rangeZeroToOne * (1.0f - (1.0f / gl_Position.w)) + (1 - rangeZeroToOne) * (1.0f / gl_Position.w));
}

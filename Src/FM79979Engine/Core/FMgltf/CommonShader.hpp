#include <string>

static std::string g_strSpotAndPointLightFunction = R"(

// Light data structure matching std140 layout
struct Light
{
    vec4    position;       // Aligned to 16 bytes
    vec4    color;          // Aligned to 16 bytes
    vec4    direction;      // Aligned to 16 bytes
    vec4    LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle;          // Aligned to 16 bytes
    ivec4   xTypeyEnable;           // Padding to align to 16 bytes
};

layout(std140) uniform uLightBlock
{
    Light lights[MAX_LIGHT];   // Array of light data (maximum 8 lights)
    ivec4 numLights;     // Number of lights and Padding to align to 16 bytes
};

float GetSpotLightAttenuation(int i, vec3 fragPos, out vec3 vLightDirection)
{
    // For shading, use the cone axis as the light direction
    vLightDirection = normalize(-lights[i].direction.xyz);
    float distance = length(lights[i].position.xyz - fragPos);
    float range = lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.y;
    if (range > 0.0)
    {
        float kLinear = 0.09 / range;
        float kQuadratic = 0.032 / (range * range);
        float attenuation = 1.0 / (1.0 + kLinear * distance + kQuadratic * distance * distance);
        float rangeFactor = smoothstep(range * 0.8, range, distance);
        attenuation *= (1.0 - rangeFactor);

        // For cone calculation, use vector from light to fragment
        vec3 lightToFrag = normalize(fragPos - lights[i].position.xyz);
        float theta = dot(lightToFrag, vLightDirection);
        float innerCos = lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.z;
        float outerCos = lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.w;
        float epsilon = innerCos - outerCos;
        float spotIntensity = 0.0;
        if (abs(epsilon) > 1e-5)
        {
            spotIntensity = clamp((theta - outerCos) / epsilon, 0.0, 1.0);
            spotIntensity = smoothstep(0.0, 1.0, spotIntensity);
        }
        attenuation *= spotIntensity;
        return attenuation;
    }
    return 0.0;
}

float GetPointLightAttenuation(int i, vec3 fragPos, out vec3 vLightDirection)
{
    vLightDirection = normalize(lights[i].position.xyz - fragPos);
    float distance = length(lights[i].position.xyz - fragPos);
    float range = lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.y;
    if (range > 0.0)
    {
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
        attenuation *= max(0.0, 1.0 - distance / range);
        return attenuation;
    }
    return 0.0;
}

)";
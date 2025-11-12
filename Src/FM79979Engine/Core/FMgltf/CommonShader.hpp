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

float GetPointLightAttenuation(int i, vec3 e_oVertexPos, out vec3 vLightDirection)
{
    vLightDirection = normalize(lights[i].position.xyz - e_oVertexPos);
    float distance = length(lights[i].position.xyz - e_oVertexPos);
    float range = lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.y;
    if (range > 0.0)
    {
        float kLinear = 0.09 / range;
        float kQuadratic = 0.032 / (range * range);
        float attenuation = 1.0 / (1.0 + kLinear * distance + kQuadratic * distance * distance);
        float rangeFactor = smoothstep(range * 0.8, range, distance);
        attenuation *= (1.0 - rangeFactor);
        return attenuation;
    }
    return 0.0;
}

float GetSpotLightAttenuation(int i, vec3 e_oVertexPos, out vec3 vLightDirection)
{
    // For shading, use the direction from light to fragment and reuse point light attenuation
    float attenuation = GetPointLightAttenuation(i, e_oVertexPos, vLightDirection);
    if(attenuation == 0.0)
    {
        return attenuation;
    }

    // For cone calculation, use cone axis
    vec3 coneAxis = normalize(-lights[i].direction.xyz);
    float theta = dot(vLightDirection, coneAxis);
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
)";


static std::string g_strShadowMapShaderFunction =
R"(
    uniform bool uEnableShadow;
    uniform sampler2D uShadowMap;
    // Spot light shadow map
    //uniform sampler2D uSpotShadowMap[MAX_LIGHT];
    //uniform mat4 uSpotLightViewProj[MAX_LIGHT];
    // Point light shadow map
    uniform samplerCube uPointShadowMap[MAX_LIGHT];
    uniform vec3 uPointLightPos[MAX_LIGHT];
    uniform float uPointLightFarPlane[MAX_LIGHT];

    float GetShadowVisibility(float shadow, vec3 projCoords)
    {
        if(projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0 || projCoords.z < 0.0 || projCoords.z > 1.0)
            return 1.0;
        return shadow;
    }

    float ShadowCalculationDirectionLight(vec4 lightSpacePos, vec3 normal, vec3 lightDir, float bias)
    {
        vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
        projCoords = projCoords * 0.5 + 0.5;
        float currentDepth = projCoords.z;
        float shadow = 0.0;
        vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;
        shadow = 1.0 - shadow; // 1.0 = lit, 0.0 = fully shadowed
        return GetShadowVisibility(shadow, projCoords);
    }

    float ShadowCalculationDirectionLight(vec4 lightSpacePos, vec3 normal, vec3 lightDir)
    {
        float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.005);
        return ShadowCalculationDirectionLight(lightSpacePos, normal, lightDir, bias);
    }

    float ShadowCalculationSpot(vec4 lightSpacePos, float bias)
    {
        vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
        projCoords = projCoords * 0.5 + 0.5;
        float closestDepth = texture(uShadowMap, projCoords.xy).r;
        float currentDepth = projCoords.z;
        float shadow = currentDepth - bias > closestDepth ? 0.01 : 1.0;
        return GetShadowVisibility(shadow, projCoords);
    }

    float ShadowCalculationSpot(vec4 lightSpacePos)
    {
        float bias = 0.01;
        return ShadowCalculationSpot(lightSpacePos, bias);
    }

    float ShadowCalculationPoint(int lightIdx, vec3 worldPos)
    {
        vec3 lightToFrag = worldPos - uPointLightPos[lightIdx];
        float currentDepth = length(lightToFrag);
        float closestDepth = texture(uPointShadowMap[lightIdx], lightToFrag).r * uPointLightFarPlane[lightIdx];
        float bias = 0.05;
        float shadow = currentDepth - bias > closestDepth ? 0.5 : 1.0;
        if(currentDepth >= uPointLightFarPlane[lightIdx])
            shadow = 1.0;
        // For point lights, projCoords is not used, so pass vec3(0) to always skip bounds check
        return GetShadowVisibility(shadow, vec3(0.5,0.5,0.5));
    }
)";
#include "../AllCoreInclude.h"
#include "LazyShaderGenerator.h"
#include "glTFAnimationMesh.h"
#include "glTFLight.h"
#include "glTFMaterial.h"
#ifdef DEBUG
std::string g_strVSForDebug;
std::string g_strFSForDebug;
#endif
std::string GenerateVertexShaderWithFVF(int64 e_i64FVFFlags, int e_iNumMorphTarget)
{
    if (e_i64FVFFlags & FVF_INSTANCING_FLAG && e_iNumMorphTarget > 0)
    {
        FMLOG("fuck....instance and morph exists at asame time....I am lazy to do this now fuck");
        assert(0 && "fuck....instance and morph exists at asame time....I am lazy to do this now fuck");
        return "not support format";
    }
    std::string l_strDefine;
    if (e_i64FVFFlags & FVF_POS_FLAG)
        l_strDefine += "#define USE_POSITION\n";
    if (e_i64FVFFlags & FVF_DIFFUSE_FLAG)
        l_strDefine += "#define USE_DIFFUSE\n";
    if (e_i64FVFFlags & FVF_NORMAL_FLAG)
        l_strDefine += "#define USE_NORMAL\n";
    if (e_i64FVFFlags & FVF_TANGENT_FLAG)
        l_strDefine += "#define USE_TANGENT\n";
    if (e_i64FVFFlags & FVF_BINORMAL_FLAG)
        l_strDefine += "#define USE_BINORMAL\n";
    if (e_i64FVFFlags & FVF_SKINNING_WEIGHT_FLAG)
        l_strDefine += "#define USE_WEIGHTS\n";
    if (e_i64FVFFlags & FVF_SKINNING_BONE_INDEX_FLAG)
        l_strDefine += "#define USE_JOINTS\n";
    if (e_i64FVFFlags & FVF_TEX0_FLAG)
        l_strDefine += "#define USE_TEXCOORD_0\n";
    if (e_i64FVFFlags & FVF_TEX1_FLAG)
        l_strDefine += "#define USE_TEXCOORD_1\n";
    if (e_i64FVFFlags & FVF_INSTANCING_FLAG)
        l_strDefine += "#define USE_INSTANCING\n";
    if (e_i64FVFFlags & FVF_ANIMATION_TEXTURE_FLAG)
        l_strDefine += "#define FVF_ANIMATION_TEXTURE_FLAG\n";

    std::string shaderCode;
#if defined(WIN32)
    shaderCode += "#version 330 core\n";
#else
    shaderCode += R"(#version 300 es
precision mediump float;
precision highp int;
precision highp sampler2D;
)";
#endif
    shaderCode += l_strDefine;
    shaderCode += R"(
#ifdef USE_POSITION
layout(location = 0) in vec3 aPosition;
#endif
#ifdef USE_NORMAL
layout(location = 1) in vec3 aNormal;
#endif
#ifdef USE_DIFFUSE
layout(location = 2) in vec3 aVertexColor;
#endif
#ifdef USE_TANGENT
layout(location = 3) in vec3 aTangent;
#endif
#ifdef USE_BINORMAL
layout(location = 4) in vec3 aBinormal;
#endif
#ifdef USE_WEIGHTS
layout(location = 5) in vec4 aWeights;
#endif
#ifdef USE_JOINTS
#ifdef GL_ES
layout(location = 6) in uvec4 aJoints;
#else
layout(location = 6) in ivec4 aJoints;
#endif
#endif
#ifdef USE_TEXCOORD_0
layout(location = 7) in vec2 aTexCoord0;
#endif
#ifdef USE_TEXCOORD_1
layout(location = 8) in vec2 aTexCoord1;
#endif
#ifdef USE_INSTANCING
//layout is not allow to assign mat4 so split it to vec4
// Per-instance matrix: locations 10,11,12,13
layout(location = 9) in vec4 aInstanceMatrix0;
layout(location = 10) in vec4 aInstanceMatrix1;
layout(location = 11) in vec4 aInstanceMatrix2;
layout(location = 12) in vec4 aInstanceMatrix3;
#endif
)";
#ifdef DEBUG
    shaderCode += R"(flat out int toFSInstanceID;)";
#endif

    if (e_iNumMorphTarget > 0)
    {
        for (int i = 0; i < e_iNumMorphTarget; i++)
        {
            shaderCode += "\nlayout(location = " + std::to_string(i + FVF_MORPHING_TARGET_POS1) + ") in vec3 aMorphTarget" + std::to_string(i) + ";";
        }
        shaderCode += "\nuniform float uMorphWeights[" + std::to_string(e_iNumMorphTarget) + "];";
    }
    shaderCode += R"(
uniform mat4 uMat4Model;
uniform mat4 uMat4View;
uniform mat4 uMat4Projection;

#ifdef USE_WEIGHTS
const int MAX_BONES = 100;
uniform mat4 uBoneTransforms[MAX_BONES];
#endif

out vec3 oVertexPos;
#ifdef USE_NORMAL
out vec3 toFSVec3Normal;
#endif
#ifdef USE_TEXCOORD_0
out vec2 toFSVec2TexCoord0;
#endif
#ifdef USE_TEXCOORD_1
out vec2 toFSVec2TexCoord1;
#endif
#ifdef USE_TANGENT
out vec3 toFSVec3Tangent;
#endif
#ifdef USE_BINORMAL
out vec3 toFSVec3Binormal;
#endif

//for animation texture

const int MAX_INSTANCES = )";
    shaderCode += ValueToString((int)MAX_INSTANCES);
    shaderCode += R"(;
uniform sampler2D uAnimTexture;
uniform ivec2 uCurrentAndNextFrameIndex[MAX_INSTANCES];
uniform float uAnimationLerpTime[MAX_INSTANCES];
uniform int uNumAnimationModel;
uniform int uNumBones;
uniform int uTextureSize;

mat4 LerpMat4(mat4 a, mat4 b, float t)
{
    mat4 result;
    for (int i = 0; i < 4; ++i)
    {
        result[i] = mix(a[i], b[i], t); // mix each row (vec4)
    }
    return result;
}

mat4 GetAnimationPoseUseTightTexture(int joint, int instance)
{
    int numBones = uNumBones;
    int textureSize = uTextureSize;
    int numRowsPerMatrix = 4;
    int frame0 = uCurrentAndNextFrameIndex[instance].x;
    int frame1 = uCurrentAndNextFrameIndex[instance].y;
    float t = uAnimationLerpTime[instance];

    // Compute base texel index for each frame
    int linearIndex0 = (frame0 * numBones + joint) * numRowsPerMatrix;
    int linearIndex1 = (frame1 * numBones + joint) * numRowsPerMatrix;


    // Fetch 4 rows for each frame
    mat4 mat0, mat1;
    for (int row = 0; row < 4; ++row)
    {
        int idx0 = linearIndex0 + row;
        int x0 = idx0 % textureSize;
        int y0 = idx0 / textureSize;
        mat0[row] = texelFetch(uAnimTexture, ivec2(x0, y0), 0);

        int idx1 = linearIndex1 + row;
        int x1 = idx1 % textureSize;
        int y1 = idx1 / textureSize;
        mat1[row] = texelFetch(uAnimTexture, ivec2(x1, y1), 0);
    }
    
    // Linear interpolate each matrix element
    mat4 pose = LerpMat4(mat0, mat1, t);
    return pose;
}

void main()
{
    vec3 position = aPosition;
)";
    if (e_iNumMorphTarget > 0)
    {
        for (int i = 0; i < e_iNumMorphTarget; i++)
        {
            shaderCode += "    position += uMorphWeights[" + std::to_string(i) + "] * aMorphTarget" + std::to_string(i) + ";\n";
        }
    }

    shaderCode += R"(
    vec4 worldPos = vec4(position, 1.0);

#ifdef USE_WEIGHTS
#ifdef USE_JOINTS
    mat4 skinMatrix;
    if(uNumAnimationModel != 0)
    {
        int instanceIndex = gl_InstanceID % uNumAnimationModel;
	    mat4 pose0 = GetAnimationPoseUseTightTexture(int(aJoints.x), instanceIndex);
	    mat4 pose1 = GetAnimationPoseUseTightTexture(int(aJoints.y), instanceIndex);
	    mat4 pose2 = GetAnimationPoseUseTightTexture(int(aJoints.z), instanceIndex);
	    mat4 pose3 = GetAnimationPoseUseTightTexture(int(aJoints.w), instanceIndex);
        skinMatrix = aWeights.x * pose0 +
                          aWeights.y * pose1 +
                          aWeights.z * pose2 +
                          aWeights.w * pose3;
    }
    else
    {
        skinMatrix = aWeights.x * uBoneTransforms[aJoints.x] +
                          aWeights.y * uBoneTransforms[aJoints.y] +
                          aWeights.z * uBoneTransforms[aJoints.z] +
                          aWeights.w * uBoneTransforms[aJoints.w];
    }   
    worldPos = skinMatrix * worldPos;
#endif
#endif
#ifdef USE_INSTANCING
    mat4 instanceMatrix = mat4(aInstanceMatrix0, aInstanceMatrix1, aInstanceMatrix2, aInstanceMatrix3);
    mat3 instanceNormalMatrix = mat3(instanceMatrix);
    worldPos = instanceMatrix * worldPos;)";
#ifdef DEBUG
    shaderCode += R"(
    toFSInstanceID = gl_InstanceID;)";
#endif
    shaderCode += R"(
#endif    
    worldPos = uMat4Model * worldPos;
    oVertexPos = worldPos.xyz;
    gl_Position = uMat4Projection * uMat4View * worldPos;

#ifdef USE_NORMAL
#ifdef USE_INSTANCING
    toFSVec3Normal = mat3(transpose(inverse(uMat4Model))) * (instanceNormalMatrix * aNormal);
#else
    toFSVec3Normal = mat3(transpose(inverse(uMat4Model))) * aNormal;
#endif
#endif

#ifdef USE_TANGENT
#ifdef USE_INSTANCING
    toFSVec3Tangent = mat3(transpose(inverse(uMat4Model))) * (instanceNormalMatrix * aTangent);
#else
    toFSVec3Tangent = mat3(transpose(inverse(uMat4Model))) * aTangent;
#endif
#endif

#ifdef USE_BINORMAL
#ifdef USE_INSTANCING
    toFSVec3Binormal = mat3(transpose(inverse(uMat4Model))) * (instanceNormalMatrix * aBinormal);
#else
    toFSVec3Binormal = mat3(transpose(inverse(uMat4Model))) * aBinormal;
#endif
#endif

#ifdef USE_TEXCOORD_0
    toFSVec2TexCoord0 = aTexCoord0;
#endif
#ifdef USE_TEXCOORD_1
    toFSVec2TexCoord1 = aTexCoord1;
#endif
}
)";

#if defined(DEBUG)
    //FMLOG(shaderCode.c_str());
    g_strVSForDebug = shaderCode;
#endif
    return shaderCode;
}


//from grok but didnt woks
//-----------------------------------

std::string GenerateFragmentShaderWithFVF(int64 e_i64FVFFlags, sTectureAndTexCoordinateIndex* e_pTectureAndTexCoordinateIndex)
{
    std::string l_strDefine;
    // Define macros based on FFV flags
    if (e_i64FVFFlags & FVF_TEX0_FLAG)
        l_strDefine += "#define USE_TEXCOORD_0\n";
    if (e_i64FVFFlags & FVF_TEX1_FLAG)
        l_strDefine += "#define USE_TEXCOORD_1\n";
    if (e_i64FVFFlags & FVF_NORMAL_FLAG)
        l_strDefine += "#define USE_NORMAL\n";
    if (e_i64FVFFlags & FVF_TANGENT_FLAG)
        l_strDefine += "#define USE_TANGENT\n";
    if (e_i64FVFFlags & FVF_BINORMAL_FLAG)
        l_strDefine += "#define USE_BINORMAL\n";
    if (e_i64FVFFlags & FVF_BASE_COLOR_TEXTURE_FLAG)
        l_strDefine += "#define FVF_BASE_COLOR_TEXTURE_FLAG\n";
    if (e_i64FVFFlags & FVF_NORMAL_MAP_TEXTURE_FLAG)
        l_strDefine += "#define USE_NORMAL_MAP\n";
    if (e_i64FVFFlags & FVF_FVF_OCCLUSION_TEXTURE_FLAG)
        l_strDefine += "#define USE_OCCLUSION\n";
    if (e_i64FVFFlags & FVF_METALLIC_ROUGHNESS_TEXTURE_FLAG)
        l_strDefine += "#define USE_METALLIC_ROUGHNESS\n";
    if (e_i64FVFFlags & FVF_EMISSIVE_TEXTURE_FLAG)
        l_strDefine += "#define USE_EMISSIVE\n";
    // Only add USE_SPECULAR for FS, and use sTectureAndTexCoordinateIndex for condition
    if (e_pTectureAndTexCoordinateIndex && e_pTectureAndTexCoordinateIndex->m_bUseSpecular)
        l_strDefine += "#define USE_SPECULAR\n";
    // KHR_materials_specular
    if (e_pTectureAndTexCoordinateIndex && e_pTectureAndTexCoordinateIndex->m_bUsePBR)
        l_strDefine += "#define USE_PBR\n";
    if (e_pTectureAndTexCoordinateIndex && e_pTectureAndTexCoordinateIndex->m_strAlphaMode == "MASK")
        l_strDefine += "#define ALPHA_MASK\n";
    l_strDefine += "#define MAX_LIGHT " + ValueToString(MAX_LIGHT);

    // --- Add defines for specular textures if present ---
    bool hasSpecularTexture = false, hasSpecularColorTexture = false;
    if (e_pTectureAndTexCoordinateIndex)
    {
        hasSpecularTexture = e_pTectureAndTexCoordinateIndex->m_iSpecularTextureCoordinateIndex >= 0;
        hasSpecularColorTexture = e_pTectureAndTexCoordinateIndex->m_iSpecularColorTextureCoordinateIndex >= 0;
    }
    if (hasSpecularTexture)
        l_strDefine += "\n#define HAS_SPECULAR_TEXTURE";
    if (hasSpecularColorTexture)
        l_strDefine += "\n#define HAS_SPECULAR_COLOR_TEXTURE";

    std::string shaderCode;
#if defined(WIN32)
    shaderCode += "#version 330 core\n";
#else
    shaderCode += R"(#version 300 es
precision mediump float;
precision highp int;
precision highp sampler2D;
)";
#endif
    shaderCode += l_strDefine;
#ifdef DEBUG
    shaderCode += R"(
flat in int toFSInstanceID; // Flat qualifier for instance ID
)";
#endif
    shaderCode += R"(
in vec3 oVertexPos;
#ifdef USE_NORMAL
in vec3 toFSVec3Normal;
#endif
#ifdef USE_TEXCOORD_0
in vec2 toFSVec2TexCoord0;
#endif
#ifdef USE_TEXCOORD_1
in vec2 toFSVec2TexCoord1;
#endif
#ifdef USE_TANGENT
in vec3 toFSVec3Tangent;
#endif
#ifdef USE_BINORMAL
in vec3 toFSVec3Binormal;
#endif

out vec4 FragColor;

uniform vec3 uVec3ViewPosition;

// Material uniforms
uniform sampler2D uTextureDiffuse;
uniform vec4 uBaseColorFactor;
#ifdef USE_NORMAL_MAP
uniform sampler2D uTextureNormal;
#endif
#ifdef USE_OCCLUSION
uniform sampler2D uTextureOcclusion;
uniform float uOcclusionStrength;
#endif
#ifdef USE_METALLIC_ROUGHNESS
uniform sampler2D uTextureMetallicRoughness;
#endif
uniform float uMetallicFactor;
uniform float uRoughnessFactor;
#ifdef USE_EMISSIVE
uniform sampler2D uTextureEmissive;
uniform vec3 uEmissiveFactor;
#endif
#ifdef USE_SPECULAR
uniform sampler2D uTextureSpecular;
uniform float uSpecularFactor;
uniform sampler2D uTextureSpecularColor;
uniform vec3 uSpecularColorFactor;
#endif
uniform float uAlphaCutoff;
uniform samplerCube uTextureEnvironment; // New: Environment cubemap

// Transmission uniforms
uniform float uTransmissionFactor;
uniform sampler2D uTextureTransmission;
uniform float uIOR;
uniform float uThicknessFactor;
uniform float uAttenuationDistance;
uniform vec3 uAttenuationColor;

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

// PBR functions
const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 CalculatePBRLighting(vec3 N, vec3 V, vec3 L, vec3 lightColor, float lightIntensity, vec3 albedo, float roughness, float metallic, float occlusion, vec3 specularColor)
{
    vec3 H = normalize(V + L);
    vec3 F0 = mix(vec3(0.04), albedo, metallic) * specularColor;
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kd = vec3(1.0) - F;
    kd *= 1.0 - metallic;

    vec3 diffuse = kd * albedo / PI;
    vec3 radiance = lightColor * lightIntensity;
    return (diffuse + specular) * radiance * NdotL * occlusion;
}

vec3 GetNormalFromMap()
{
#if defined(USE_NORMAL_MAP) && defined(USE_TANGENT) && defined(USE_NORMAL) && defined(USE_TEXCOORD_0)
    vec3 tangent = normalize(toFSVec3Tangent);
    vec3 normal = normalize(toFSVec3Normal);
#ifdef USE_BINORMAL
    vec3 bitangent = normalize(toFSVec3Binormal);
#else
    vec3 bitangent = normalize(cross(normal, tangent));
#endif
    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 normalMap = texture(uTextureNormal, toFSVec2TexCoord0).rgb * 2.0 - 1.0;
    return normalize(TBN * normalMap);
#else
#ifdef USE_NORMAL
    return normalize(toFSVec3Normal);
#else
    return vec3(0.0, 0.0, 1.0); // Default normal if none provided
#endif
#endif
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 GetEnvironmentReflection(vec3 N, vec3 V, float roughness, vec3 F0)
{
    vec3 R = reflect(-V, N);
    float lod = roughness * 6.0; // Adjust lod level based on roughness
    //vec3 envColor = textureLod(uTextureEnvironment, R, lod).rgb;
    vec3 envColor = vec3(roughness,roughness,roughness);
    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    return envColor * F;
}


void main()
{
    vec3 vModelNormal = GetNormalFromMap();
    vec3 vViewPostToVertexPos = normalize(uVec3ViewPosition - oVertexPos);
    vec3 color = vec3(0.0);
    // Handle albedo and alpha from diffuse texture or base color factor
    float alpha = uBaseColorFactor.a;
#if defined(USE_TEXCOORD_0) && defined(FVF_BASE_COLOR_TEXTURE_FLAG)
    vec4 diffuseSample = texture(uTextureDiffuse, toFSVec2TexCoord0);
    vec3 albedo = pow(diffuseSample.rgb, vec3(2.2)) * uBaseColorFactor.rgb;
    alpha *= diffuseSample.a; // Incorporate texture alpha for transparency
#else
    vec3 albedo = uBaseColorFactor.rgb;
#endif
#ifdef ALPHA_MASK
if (alpha < uAlphaCutoff)
    discard;
#endif
#ifdef USE_OCCLUSION
    float occlusion = texture(uTextureOcclusion, toFSVec2TexCoord0).r * uOcclusionStrength;
#else
    float occlusion = 1.0;
#endif

#ifdef USE_METALLIC_ROUGHNESS
    float metallic = texture(uTextureMetallicRoughness, toFSVec2TexCoord0).b * uMetallicFactor;
    float roughness = texture(uTextureMetallicRoughness, toFSVec2TexCoord0).g * uRoughnessFactor;
#else
    float metallic = uMetallicFactor;
    float roughness = uRoughnessFactor;
#endif

#ifdef USE_EMISSIVE
    vec3 emissive = pow(texture(uTextureEmissive, toFSVec2TexCoord0).rgb, vec3(2.0)) * uEmissiveFactor;
#else
    vec3 emissive = vec3(0.0);
#endif

#ifdef USE_SPECULAR
    float specularFactor = uSpecularFactor;
    #if defined(USE_TEXCOORD_0)
    #ifdef HAS_SPECULAR_TEXTURE
    // Use only the alpha channel for specularFactor
    specularFactor *= texture(uTextureSpecular, toFSVec2TexCoord0).a;
    #endif
    #endif
    vec3 specularColor = uSpecularColorFactor;
    #if defined(USE_TEXCOORD_0)
    #ifdef HAS_SPECULAR_COLOR_TEXTURE
    // Use only RGB for F0
    specularColor *= texture(uTextureSpecularColor, toFSVec2TexCoord0).rgb;
    #endif
    #endif
#else
    float specularFactor = 1.0;
    vec3 specularColor = vec3(1.0);
#endif

    for (int i = 0; i < numLights.x; i++)
    {
        if(lights[i].xTypeyEnable.y == 0) // If light is not enabled
        {
            continue;
        }
        int l_iType = lights[i].xTypeyEnable.x;
        vec3 vLightDirection;
        float attenuation = 1.0;
        if (l_iType == 0) // Directional light
        {
            vLightDirection = -normalize(lights[i].direction.xyz);
        }
        else 
        if (l_iType == 3) // Ambient light
        {
            color += lights[i].color.xyz * albedo * occlusion * 0.2;
            continue;
        }
        else
        if (l_iType == 2) // Spot light
        {
            vLightDirection = normalize(lights[i].position.xyz - oVertexPos);
            float distance = length(lights[i].position.xyz - oVertexPos);
            float range = lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.y;
            if (range > 0.0)
            {
                // Distance-based attenuation with smooth cutoff
                float kLinear = 0.09 / range; // Adjustable coefficient
                float kQuadratic = 0.032 / (range * range); // Adjustable coefficient
                attenuation = 1.0 / (1.0 + kLinear * distance + kQuadratic * distance * distance);
                float rangeFactor = smoothstep(range * 0.8, range, distance); // Smooth fade from 80% to 100% of range
                attenuation *= (1.0 - rangeFactor);

                // Spotlight cone calculation
                vec3 lightDir = normalize(lights[i].direction.xyz); // Ensure normalized direction
                float theta = dot(vLightDirection, lightDir);
                float innerCos = lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.z;
                float outerCos = lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.w;
                float epsilon = innerCos - outerCos;
                float spotIntensity = 0.0;
                if (abs(epsilon) > 1e-5)
                {
                    spotIntensity = clamp((theta - outerCos) / epsilon, 0.0, 1.0);
                    spotIntensity = smoothstep(0.0, 1.0, spotIntensity); // Smoother cone edge
                }
                attenuation *= spotIntensity;
            }
            else
            {
                attenuation = 0.0; // No contribution if range is zero
            }
        }
        else // Point
        {
            vLightDirection = normalize(lights[i].position.xyz - oVertexPos);
            float distance = length(lights[i].position.xyz - oVertexPos);
            float range = lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.y;
            if (range > 0.0)
            {
                attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
                attenuation *= max(0.0, 1.0 - distance / range); // Fade out at range boundary
            }
            else
            {
                attenuation = 0.0;
            }
        }
#ifdef USE_PBR
        // Correct: apply specularFactor only to the specular term
        color += CalculatePBRLighting(vModelNormal, vViewPostToVertexPos, vLightDirection, lights[i].color.xyz, lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.x * attenuation, albedo, roughness, metallic, occlusion, specularColor) * specularFactor;
#else
        float diff = max(dot(vModelNormal, vLightDirection), 0.0)*lights[i].LightData_xIntensityyRangezInnerConeAngelwOutterConeAngle.x;
        color += (diff*albedo);
#endif
    }
#ifdef USE_PBR
 // Environment Reflection
    vec3 F0 = mix(vec3(0.04), albedo, metallic) * specularColor;
    vec3 reflection = GetEnvironmentReflection(vModelNormal, vViewPostToVertexPos, roughness, F0) * (1.0 - occlusion); // Reduce reflection in occluded areas
    color += reflection * 0.5; // Blend with lighting
#endif

    color += emissive;

    // Transmission logic (simplified, real implementation should use screen-space ray tracing for refraction)
    float transmission = uTransmissionFactor;
    #ifdef USE_TEXCOORD_0
    vec3 transmissionColor = vec3(1.0);
    transmissionColor *= transmission;
    transmissionColor *= texture(uTextureTransmission, toFSVec2TexCoord0).r;
    #endif
    // Attenuation (volume absorption)
    float thickness = uThicknessFactor;
    float attenuationDist = uAttenuationDistance;
    vec3 attenuationColor = uAttenuationColor;
    if (thickness > 0.0 && attenuationDist > 0.0)
    {
        float att = exp(-thickness / attenuationDist);
#ifdef USE_TEXCOORD_0
        transmissionColor *= mix(vec3(1.0), attenuationColor, att);
#endif
    }
    // IOR (index of refraction) can be used for Fresnel blending
    float ior = uIOR;
    float eta = 1.0 / ior;
    float fresnel = pow(1.0 - max(dot(vModelNormal, vViewPostToVertexPos), 0.0), 5.0);
    // Blend transmission with surface color
#ifdef USE_TEXCOORD_0
    color = mix(color, transmissionColor, transmission * (1.0 - fresnel));
#endif

    color = vec3(1.0) - exp(-color * 0.3);           // Tone mapping
    FragColor = vec4(pow(color, vec3(1.0 / 2.2)), alpha);
    //for normal debug
    //FragColor = vec4(normalize(toFSVec3Normal) * 0.5 + 0.5, 1.0);


)";
#ifdef DEBUG
    shaderCode += R"(
    if(toFSInstanceID %10 == 1)
    {
        FragColor.rgb *= vec3(1.0, 1.0, 0.0); // Debug color for every 100th instance   
    })";
#endif
    shaderCode += R"(
}
)";
    // --- Begin fix: select correct texcoord for each texture type ---
        // Helper lambda to select the correct texcoord string
    auto GetTexCoordStr = [](int idx)
        {
            if (idx == 1)
                return std::string("toFSVec2TexCoord1");
            return std::string("toFSVec2TexCoord0");
        };

    // Default indices if pointer is null
    int baseColorIdx = 0, normalIdx = 0, occIdx = 0, emissiveIdx = 0, mrIdx = 0, specularIdx = 0, specularColorIdx = 0;
    if (e_pTectureAndTexCoordinateIndex)
    {
        baseColorIdx = e_pTectureAndTexCoordinateIndex->m_iBaseColorTextureCoordinateIndex;
        normalIdx = e_pTectureAndTexCoordinateIndex->m_iNormalTextureCoordinateIndex;
        occIdx = e_pTectureAndTexCoordinateIndex->m_iOocclusionTextureCoordinateIndex;
        emissiveIdx = e_pTectureAndTexCoordinateIndex->m_iEmissiveTextureCoordinateIndex;
        mrIdx = e_pTectureAndTexCoordinateIndex->m_iMetallicRoughnessTextureCoordinateIndex;
        specularIdx = e_pTectureAndTexCoordinateIndex->m_iSpecularTextureCoordinateIndex;
        specularColorIdx = e_pTectureAndTexCoordinateIndex->m_iSpecularColorTextureCoordinateIndex;
    }

    // Replace texcoord usages in the shader code
    // For each texture type, replace toFSVec2TexCoord0 with the correct one if needed
    // Only replace the first occurrence for each texture type

    // Base Color
    size_t pos = shaderCode.find("texture(uTextureDiffuse, toFSVec2TexCoord0)");
    if (pos != std::string::npos)
        shaderCode.replace(pos, strlen("texture(uTextureDiffuse, toFSVec2TexCoord0)"),
                           "texture(uTextureDiffuse, " + GetTexCoordStr(baseColorIdx) + ")");

    // Normal Map
    pos = shaderCode.find("texture(uTextureNormal, toFSVec2TexCoord0)");
    if (pos != std::string::npos)
        shaderCode.replace(pos, strlen("texture(uTextureNormal, toFSVec2TexCoord0)"),
                           "texture(uTextureNormal, " + GetTexCoordStr(normalIdx) + ")");

    // Occlusion
    pos = shaderCode.find("texture(uTextureOcclusion, toFSVec2TexCoord0)");
    if (pos != std::string::npos)
        shaderCode.replace(pos, strlen("texture(uTextureOcclusion, toFSVec2TexCoord0)"),
                           "texture(uTextureOcclusion, " + GetTexCoordStr(occIdx) + ")");

    // MetallicRoughness
    pos = shaderCode.find("texture(uTextureMetallicRoughness, toFSVec2TexCoord0)");
    while (pos != std::string::npos)
    {
        shaderCode.replace(pos, strlen("texture(uTextureMetallicRoughness, toFSVec2TexCoord0)"),
                           "texture(uTextureMetallicRoughness, " + GetTexCoordStr(mrIdx) + ")");
        pos = shaderCode.find("texture(uTextureMetallicRoughness, toFSVec2TexCoord0)", pos + 1);
    }

    // Emissive
    pos = shaderCode.find("texture(uTextureEmissive, toFSVec2TexCoord0)");
    if (pos != std::string::npos)
        shaderCode.replace(pos, strlen("texture(uTextureEmissive, toFSVec2TexCoord0)"),
                           "texture(uTextureEmissive, " + GetTexCoordStr(emissiveIdx) + ")");

    // Specular
    pos = shaderCode.find("texture(uTextureSpecular, toFSVec2TexCoord0)");
    if (pos != std::string::npos)
        shaderCode.replace(pos, strlen("texture(uTextureSpecular, toFSVec2TexCoord0)"),
                           "texture(uTextureSpecular, " + GetTexCoordStr(specularIdx) + ")");

    // SpecularColor
    pos = shaderCode.find("texture(uTextureSpecularColor, toFSVec2TexCoord0)");
    if (pos != std::string::npos)
        shaderCode.replace(pos, strlen("texture(uTextureSpecularColor, toFSVec2TexCoord0)"),
                           "texture(uTextureSpecularColor, " + GetTexCoordStr(specularColorIdx) + ")");

    // Clearcoat
    pos = shaderCode.find("texture(uTextureClearcoat, toFSVec2TexCoord0)");
    if (pos != std::string::npos)
        shaderCode.replace(pos, strlen("texture(uTextureClearcoat, toFSVec2TexCoord0)"),
                           "texture(uTextureClearcoat, " + GetTexCoordStr(baseColorIdx) + ")");

    // Clearcoat Roughness
    pos = shaderCode.find("texture(uTextureClearcoatRoughness, toFSVec2TexCoord0)");
    if (pos != std::string::npos)
        shaderCode.replace(pos, strlen("texture(uTextureClearcoatRoughness, toFSVec2TexCoord0)"),
                           "texture(uTextureClearcoatRoughness, " + GetTexCoordStr(mrIdx) + ")");

    // --- End fix ---
#if defined(DEBUG)// || defined(WASM)
    FMLOG(shaderCode.c_str());
    g_strFSForDebug = shaderCode;
#endif
    return shaderCode;
}


// --- Extension logic in main() ---
#ifdef USE_CLEARCOAT
    float clearcoat = uClearcoatFactor;
    float clearcoatRoughness = uClearcoatRoughnessFactor;
    #ifdef USE_TEXCOORD_0
    clearcoat *= texture(uTextureClearcoat, toFSVec2TexCoord0).r;
    clearcoatRoughness *= texture(uTextureClearcoatRoughness, toFSVec2TexCoord0).g;
    #endif
    // Clearcoat normal perturbation
    vec3 clearcoatNormal = vModelNormal;
    #ifdef USE_TEXCOORD_0
    clearcoatNormal = normalize(texture(uTextureClearcoatNormal, toFSVec2TexCoord0).rgb * 2.0 - 1.0);
    #endif
#endif
#ifdef USE_EMISSIVE_STRENGTH
    emissive *= uEmissiveStrength;
#endif
#ifdef USE_IRIDESCENCE
    float iridescence = uIridescenceFactor;
    float iridescenceIOR = uIridescenceIOR;
    float iridescenceThickness = mix(uIridescenceThicknessMin, uIridescenceThicknessMax, 0.5);
    #ifdef USE_TEXCOORD_0
    iridescence *= texture(uTextureIridescence, toFSVec2TexCoord0).r;
    iridescenceThickness *= texture(uTextureIridescenceThickness, toFSVec2TexCoord0).g;
    #endif
    // Iridescence effect can be added here
#endif
#ifdef USE_TRANSMISSION
    transmission *= uTransmissionFactor;
    #ifdef USE_TEXCOORD_0
    transmission *= texture(uTextureTransmission, toFSVec2TexCoord0).r;
    #endif
#endif
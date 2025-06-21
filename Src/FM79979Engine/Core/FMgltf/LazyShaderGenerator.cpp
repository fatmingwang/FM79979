#include "../AllCoreInclude.h"
#include "LazyShaderGenerator.h"
#include "glTFAnimationMesh.h"
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
        l_strDefine += "#define USE_TEXCOORD\n";
    if (e_i64FVFFlags & FVF_INSTANCING_FLAG)
        l_strDefine += "#define USE_INSTANCING\n";
    if (e_i64FVFFlags & FVF_ANIMATION_TEXTURE_FLAG)
        l_strDefine += "#define FVF_ANIMATION_TEXTURE_FLAG\n";

    std::string shaderCode = "//this is Vertex shader\n";
#if defined(WIN32)
    shaderCode += "#version 330 core\n";
#else
    shaderCode += R"(#version 300 es"
precision mediump float;
precision highp int;
precision highp sampler2D;)";
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
layout(location = 6) in ivec4 aJoints;
#endif
#ifdef USE_TEXCOORD
layout(location = 7) in vec2 aTexCoord;
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

out vec3 toFSVec3FragPos;
#ifdef USE_NORMAL
out vec3 toFSVec3Normal;
#endif
#ifdef USE_TEXCOORD
out vec2 toFSVec2TexCoord;
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
	    mat4 pose0 = GetAnimationPoseUseTightTexture(aJoints.x, instanceIndex);
	    mat4 pose1 = GetAnimationPoseUseTightTexture(aJoints.y, instanceIndex);
	    mat4 pose2 = GetAnimationPoseUseTightTexture(aJoints.z, instanceIndex);
	    mat4 pose3 = GetAnimationPoseUseTightTexture(aJoints.w, instanceIndex);
	    //mat4 pose0 = GetAnimationPose(aJoints.x, instanceIndex);
	    //mat4 pose1 = GetAnimationPose(aJoints.y, instanceIndex);
	    //mat4 pose2 = GetAnimationPose(aJoints.z, instanceIndex);
	    //mat4 pose3 = GetAnimationPose(aJoints.w, instanceIndex);
        skinMatrix = aWeights.x*10 * pose0 +
                          aWeights.y *10* pose1 +
                          aWeights.z *10* pose2 +
                          aWeights.w*10 * pose3;
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
    toFSVec3FragPos = worldPos.xyz;
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

#ifdef USE_TEXCOORD
    toFSVec2TexCoord = aTexCoord;
#endif
}
)";

#if defined(DEBUG)
    FMLOG(shaderCode.c_str());
#elif defined(WASM)
    printf(shaderCode.c_str());
#endif
    return shaderCode;
}


//from grok but didnt woks
//-----------------------------------

std::string GenerateFragmentShaderWithFVF(int64 e_i64FVFFlags)
{
    std::string l_strDefine;
    // Define macros based on FVF flags
    if (e_i64FVFFlags & FVF_TEX0_FLAG)
        l_strDefine += "#define USE_TEXCOORD\n";
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



    std::string shaderCode = "//this is framgent shader\n";
#if defined(WIN32)
    shaderCode += "#version 330 core\n";
#else
    shaderCode += R"(#version 300 es"
precision mediump float;
precision highp int;
precision highp sampler2D;
)";
#endif
    shaderCode += l_strDefine;
    shaderCode += R"(
flat in int toFSInstanceID; // Flat qualifier for instance ID
in vec3 toFSVec3FragPos;
#ifdef USE_NORMAL
in vec3 toFSVec3Normal;
#endif
#ifdef USE_TEXCOORD
in vec2 toFSVec2TexCoord;
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
uniform sampler2D utextureDiffuse;
uniform vec4 uBaseColorFactor;
#ifdef USE_NORMAL_MAP
uniform sampler2D utextureNormal;
#endif
#ifdef USE_OCCLUSION
uniform sampler2D utextureOcclusion;
uniform float uocclusionStrength;
#endif
#ifdef USE_METALLIC_ROUGHNESS
uniform sampler2D utextureMetallicRoughness;
#endif
uniform float uMetallicFactor;
uniform float uRoughnessFactor;
#ifdef USE_EMISSIVE
uniform sampler2D uTextureEmissive;
uniform vec3 uEmissiveFactor;
#endif

// Light data structure matching std140 layout
struct Light
{
    vec3 position;       // Aligned to 16 bytes
    float intensity;     // Aligned to 16 bytes (vec4 padding)
    vec3 direction;      // Aligned to 16 bytes
    float range;         // Aligned to 16 bytes (vec4 padding)
    vec3 color;          // Aligned to 16 bytes
    float innerConeAngle;// Aligned to 16 bytes (vec4 padding)
    float outerConeAngle;// Aligned to 4 bytes
    int type;            // Aligned to 4 bytes
    int Enable;          // Aligned to 4 bytes
    float pad;           // Padding to align to 16 bytes
};

layout(std140) uniform uLightBlock
{
    Light lights[8];   // Array of light data (maximum 8 lights)
    int numLights;     // Number of lights
    vec3 pad;          // Padding to align to 16 bytes
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

vec3 CalculatePBRLighting(vec3 N, vec3 V, vec3 L, vec3 lightColor, float lightIntensity, vec3 albedo, float roughness, float metallic, float occlusion)
{
    vec3 H = normalize(V + L);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
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
#if defined(USE_NORMAL_MAP) && defined(USE_TANGENT) && defined(USE_NORMAL) && defined(USE_TEXCOORD)
    vec3 tangent = normalize(toFSVec3Tangent);
    vec3 normal = normalize(toFSVec3Normal);
#ifdef USE_BINORMAL
    vec3 bitangent = normalize(toFSVec3Binormal);
#else
    vec3 bitangent = normalize(cross(normal, tangent));
#endif
    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 normalMap = texture(utextureNormal, toFSVec2TexCoord).rgb * 2.0 - 1.0;
    return normalize(TBN * normalMap);
#else
#ifdef USE_NORMAL
    return normalize(toFSVec3Normal);
#else
    return vec3(0.0, 0.0, 1.0); // Default normal if none provided
#endif
#endif
}

void main()
{
    vec3 N = GetNormalFromMap();
    vec3 V = normalize(uVec3ViewPosition - toFSVec3FragPos);
    vec3 color = vec3(0.0);
#if defined(USE_TEXCOORD) && defined(FVF_BASE_COLOR_TEXTURE_FLAG)
    //vec3 albedo = texture(utextureDiffuse, toFSVec2TexCoord).rgb;
    vec3 albedo = pow(texture(utextureDiffuse, toFSVec2TexCoord).rgb, vec3(2.0)) * uBaseColorFactor.rgb;
#else
    vec3 albedo = uBaseColorFactor.rgb;
#endif

#ifdef USE_OCCLUSION
    float occlusion = texture(utextureOcclusion, toFSVec2TexCoord).r * uocclusionStrength;
#else
    float occlusion = 1.0;
#endif

#ifdef USE_METALLIC_ROUGHNESS
    float metallic = texture(utextureMetallicRoughness, toFSVec2TexCoord).b * uMetallicFactor;
    float roughness = texture(utextureMetallicRoughness, toFSVec2TexCoord).g * uRoughnessFactor;
#else
    float metallic = uMetallicFactor;
    float roughness = uRoughnessFactor;
#endif

#ifdef USE_EMISSIVE
    vec3 emissive = pow(texture(uTextureEmissive, toFSVec2TexCoord).rgb, vec3(2.0)) * uEmissiveFactor;
#else
    vec3 emissive = vec3(0.0);
#endif

    for (int i = 0; i < numLights; i++)
    {
        if (lights[i].Enable == 0)
        {
            continue;
        }
        vec3 L;
        float attenuation = 1.0;
        if (lights[i].type == 0) // Directional light
        {
            L = -normalize(lights[i].direction);
        }
        else if (lights[i].type == 3) // Ambient light
        {
            color += lights[i].color * albedo;
            continue;
        }
        else // Point or spot
        {
            L = normalize(lights[i].position - toFSVec3FragPos);
            float distance = length(lights[i].position - toFSVec3FragPos);
            if (lights[i].range > 0.0)
            {
                attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
            }
            // Apply spot light cone if applicable
            if (lights[i].type == 2) // Spot light
            {
                float theta = dot(-L, normalize(lights[i].direction));
                float epsilon = lights[i].innerConeAngle - lights[i].outerConeAngle;
                float spotIntensity = clamp((theta - lights[i].outerConeAngle) / epsilon, 0.0, 1.0);
                attenuation *= spotIntensity;
            }
        }
        //float diff = max(dot(N, L), 0.0)*lights[i].intensity;
        //color += (diff*albedo);
        color += CalculatePBRLighting(N, V, L, lights[i].color, lights[i].intensity * attenuation, albedo, roughness, metallic, occlusion);
    }

    color += emissive;
    //FragColor = vec4(color,1);
    FragColor = vec4(pow(color, vec3(1.0 / 2.2)), 1.0); // Gamma correction)";
#ifdef DEBUG
    shaderCode += R"(
    if(toFSInstanceID %10 == 0)
    {
        FragColor = vec4(1.0, 1.0, 0.0, 1.0)*FragColor; // Debug color for every 100th instance   
    })";
#endif
    shaderCode += R"(
}
)";

#if defined(DEBUG)// || defined(WASM)
    FMLOG(shaderCode.c_str());
#elif defined(WASM)
    printf(shaderCode.c_str());
#endif
    return shaderCode;
}


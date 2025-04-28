#include "../AllCoreInclude.h"
#include "LazyShaderGenerator.h"


std::string GenerateVertexShaderWithFVF(int64 e_i64FVFFlags, int e_iNumMorphTarget)
{
    std::string l_strDefine;
    // Define flags for conditional compilation
    if (e_i64FVFFlags & FVF_POS_FLAG)
        l_strDefine += "#define USE_POSITION\n";
    if (e_i64FVFFlags & FVF_NORMAL_FLAG)
        l_strDefine += "#define USE_NORMAL\n";
    if (e_i64FVFFlags & FVF_TANGENT_FLAG)
        l_strDefine += "#define USE_TANGENT\n";
    if (e_i64FVFFlags & FVF_BITAGENT_FLAG)
        l_strDefine += "#define USE_BINORMAL\n";
    if (e_i64FVFFlags & FVF_SKINNING_WEIGHT_FLAG)
        l_strDefine += "#define USE_WEIGHTS\n";
    if (e_i64FVFFlags & FVF_SKINNING_BONE_INDEX_FLAG)
        l_strDefine += "#define USE_JOINTS\n";
    if (e_i64FVFFlags & FVF_TEX0_FLAG)
        l_strDefine += "#define USE_TEXCOORD\n";

    std::string shaderCode = R"(
    #version 330 core
#ifdef GL_ES
    precision mediump float;
#endif
)" + l_strDefine + R"(

#ifdef USE_POSITION
    layout(location = 0) in vec3 aPosition;
#endif
#ifdef USE_NORMAL
    layout(location = 1) in vec3 aNormal;
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
)";

    // Morph target attributes
    if (e_iNumMorphTarget > 0)
    {
        for (int i = 0; i < e_iNumMorphTarget; i++)
        {
            shaderCode += "\n\tlayout(location = " + std::to_string(i + FVF_MORPHING_TARGET_POS1) + ") in vec3 aMorphTarget" + std::to_string(i) + ";";
        }
        shaderCode += "\n\tuniform float uMorphWeights[" + std::to_string(e_iNumMorphTarget) + "];";
    }

    shaderCode += R"(
    uniform mat4 inMat4Model;
    uniform mat4 inMat4View;
    uniform mat4 inMat4Projection;
    uniform vec3 inVec3LightPosition;

#ifdef USE_TEXCOORD
    out vec2 toFSVec2TexCoord;
#endif
#ifdef USE_NORMAL
    out vec3 toFSVec3Normal;
#endif
    out vec3 toFSVec3FragPos;
    out vec3 toFSVec3LightDir;
#ifdef USE_TANGENT
    out vec3 toFSVec3Tangent;
#endif
#ifdef USE_BINORMAL
    out vec3 toFSVec3Binormal;
#endif
)";

    // Skinning uniforms
    shaderCode += R"(
#ifdef USE_WEIGHTS
    const int MAX_BONES = 100;
    uniform mat4 uBoneTransforms[MAX_BONES];
#endif
)";

    // Main function
    shaderCode += R"(
    void main() 
    {
)";
    if (e_iNumMorphTarget > 0)
    {
        shaderCode += R"(
        vec3 morphedPosition = aPosition;
)";
        for (int i = 0; i < e_iNumMorphTarget; i++)
        {
            shaderCode += "\tmorphedPosition += uMorphWeights[" + std::to_string(i) + "] * aMorphTarget" + std::to_string(i) + ";\n";
        }
        shaderCode += R"(
        vec4 worldPosition = inMat4Model * vec4(morphedPosition, 1.0);
        toFSVec3FragPos = worldPosition.xyz;
)";
    }
    else
    {
        shaderCode += R"(
        vec4 worldPosition = inMat4Model * vec4(aPosition, 1.0);
        toFSVec3FragPos = worldPosition.xyz;
)";
    }

    // Skinning transformation
    shaderCode += R"(
#ifdef USE_WEIGHTS
#ifdef USE_JOINTS
        mat4 skinMatrix = aWeights.x * uBoneTransforms[aJoints.x] +
                          aWeights.y * uBoneTransforms[aJoints.y] +
                          aWeights.z * uBoneTransforms[aJoints.z] +
                          aWeights.w * uBoneTransforms[aJoints.w];
        vec4 skinnedPosition = skinMatrix * vec4(aPosition, 1.0);
        worldPosition = inMat4Projection * inMat4View * inMat4Model * skinnedPosition;
#endif
#endif
)";

    shaderCode += R"(
        toFSVec3LightDir = normalize(inVec3LightPosition - toFSVec3FragPos);
)";

    shaderCode += R"(
#ifdef USE_TANGENT
        toFSVec3Tangent = mat3(transpose(inverse(inMat4Model))) * aTangent;
#endif
)";

    shaderCode += R"(
#ifdef USE_BINORMAL
        toFSVec3Binormal = mat3(transpose(inverse(inMat4Model))) * aBinormal;
#endif
)";

    shaderCode += R"(
#ifdef USE_TEXCOORD
        toFSVec2TexCoord = aTexCoord;
#endif
)";

    // Final position and normal
    shaderCode += R"(
#ifdef USE_WEIGHTS
#ifdef USE_JOINTS
        gl_Position = worldPosition;
#else
        gl_Position = inMat4Projection * inMat4View * worldPosition;
#endif
#else
        gl_Position = inMat4Projection * inMat4View * worldPosition;
#endif
#ifdef USE_NORMAL
        toFSVec3Normal = mat3(transpose(inverse(inMat4Model))) * aNormal;
#endif
    }
)";

#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}

std::string GenerateFragmentShaderWithFVF(int64 e_i64FVFFlags)
{
    std::string l_strDefine;

    if (e_i64FVFFlags & FVF_TEX0_FLAG)
        l_strDefine += "#define USE_TEXCOORD\n";
    if (e_i64FVFFlags & FVF_NORMAL_FLAG)
        l_strDefine += "#define USE_NORMAL\n";
    if (e_i64FVFFlags & FVF_TANGENT_FLAG)
        l_strDefine += "#define USE_TANGENT\n";
    if (e_i64FVFFlags & FVF_BITAGENT_FLAG)
        l_strDefine += "#define USE_BINORMAL\n";
    if (e_i64FVFFlags & FVF_NORMAL_MAP_TEXTURE_FLAG)
        l_strDefine += "#define USE_NORMAL_MAP\n";

    // Define ONE of these depending on the shader type you want to build:
    if (e_i64FVFFlags & FVF_TEX0_FLAG)
        l_strDefine += "#define USE_DIFFUSE_ONLY\n";
    if (e_i64FVFFlags & FVF_NORMAL_MAP_TEXTURE_FLAG)
        l_strDefine += "#define USE_DIFFUSE_NORMAL\n";
    if (e_i64FVFFlags & FVF_HAS_PBR_TEXTURE_FLAG)
        l_strDefine += "#define USE_PBR\n";

    std::string shaderCode = R"(
    #version 330 core
#ifdef GL_ES
    precision mediump float;
#endif
)" + l_strDefine + R"(

#ifdef USE_TEXCOORD
    in vec2 toFSVec2TexCoord;
#endif
#ifdef USE_NORMAL
    in vec3 toFSVec3Normal;
#endif
    in vec3 toFSVec3FragPos;
    in vec3 toFSVec3LightDir;
#ifdef USE_TANGENT
    in vec3 toFSVec3Tangent;
#endif
#ifdef USE_BINORMAL
    in vec3 toFSVec3Binormal;
#endif

    out vec4 FragColor;

    uniform vec3 inVec3LightColor;
    uniform vec3 inVec3LightPosition;
    uniform vec3 inVec3ViewPosition;

#ifdef USE_DIFFUSE_ONLY
    uniform sampler2D textureDiffuse;
#endif

#ifdef USE_DIFFUSE_NORMAL
    uniform sampler2D textureDiffuse;
    uniform sampler2D textureNormal;
#endif

#ifdef USE_PBR
    uniform sampler2D textureAlbedo;
    uniform sampler2D textureRoughness;
    uniform sampler2D textureMetallic;
    uniform sampler2D textureNormal;
#endif

vec3 GetNormalFromMap()
{
#ifdef USE_NORMAL_MAP
#ifdef USE_BINORMAL
    vec3 tangent = normalize(toFSVec3Tangent);
    vec3 bitangent = normalize(toFSVec3Binormal);
    vec3 normal = normalize(toFSVec3Normal);
    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 normalMap = texture(textureNormal, toFSVec2TexCoord).rgb;
    normalMap = normalMap * 2.0 - 1.0;
    return normalize(TBN * normalMap);
#else
    return normalize(toFSVec3Normal);
#endif
#else
    return normalize(toFSVec3Normal);
#endif
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
#ifdef USE_DIFFUSE_ONLY
    vec3 N = normalize(toFSVec3Normal);
    vec3 L = normalize(inVec3LightPosition - toFSVec3FragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 diffuseColor = texture(textureDiffuse, toFSVec2TexCoord).rgb;
    vec3 result = diffuseColor * inVec3LightColor * diff;
    FragColor = vec4(result, 1.0);
#elif defined(USE_DIFFUSE_NORMAL)
    vec3 N = GetNormalFromMap();
    vec3 L = normalize(inVec3LightPosition - toFSVec3FragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 diffuseColor = texture(textureDiffuse, toFSVec2TexCoord).rgb;
    vec3 result = diffuseColor * inVec3LightColor * diff;
    FragColor = vec4(result, 1.0);
#elif defined(USE_PBR)
    vec3 albedo = pow(texture(textureAlbedo, toFSVec2TexCoord).rgb, vec3(2.2));
    float roughness = texture(textureRoughness, toFSVec2TexCoord).r;
    float metallic = texture(textureMetallic, toFSVec2TexCoord).r;

    vec3 N = GetNormalFromMap();
    vec3 V = normalize(inVec3ViewPosition - toFSVec3FragPos);
    vec3 L = normalize(inVec3LightPosition - toFSVec3FragPos);
    vec3 H = normalize(V + L);

    float NDF = pow(max(dot(N, H), 0.0), (2.0 / pow(roughness + 0.001, 4.0)) - 2.0);
    float G = 1.0; // Simplified for performance
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float NdotL = max(dot(N, L), 0.0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.001) * max(dot(N, L), 0.001);
    vec3 specular = numerator / max(denominator, 0.001);

    vec3 irradiance = inVec3LightColor;
    vec3 diffuse = kD * albedo / 3.14159;

    vec3 color = (diffuse + specular) * irradiance * NdotL;

    FragColor = vec4(pow(color, vec3(1.0 / 2.2)), 1.0);
#else
    // No texture, no normal map, no PBR -> basic diffuse lighting
    vec3 N = normalize(toFSVec3Normal);
    vec3 L = normalize(inVec3LightPosition - toFSVec3FragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 diffuseColor = vec3(1.0, 1.0, 1.0); // White diffuse color
    vec3 result = diffuseColor * inVec3LightColor * diff;
    FragColor = vec4(result, 1.0);
#endif
}

)";
#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}

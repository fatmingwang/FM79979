#include "../AllCoreInclude.h"
#include "LazyShaderGenerator.h"


std::string GenerateVertexShaderWithFVF(int64 e_i64FVFFlags, int e_iNumMorphTarget)
{
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
)";

	if (e_iNumMorphTarget > 0)
	{
		for (int i = 0; i < e_iNumMorphTarget; i++)
		{
			shaderCode += "\nlayout(location = " + std::to_string(i + FVF_MORPHING_TARGET_POS1) + ") in vec3 aMorphTarget" + std::to_string(i) + ";";
		}
		shaderCode += "\nuniform float uMorphWeights[" + std::to_string(e_iNumMorphTarget) + "];";
	}

	shaderCode += R"(
uniform mat4 inMat4Model;
uniform mat4 inMat4View;
uniform mat4 inMat4Projection;

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
    mat4 skinMatrix = aWeights.x * uBoneTransforms[aJoints.x] +
                      aWeights.y * uBoneTransforms[aJoints.y] +
                      aWeights.z * uBoneTransforms[aJoints.z] +
                      aWeights.w * uBoneTransforms[aJoints.w];
    worldPos = skinMatrix * worldPos;
#endif
#endif

    worldPos = inMat4Model * worldPos;
    toFSVec3FragPos = worldPos.xyz;
    gl_Position = inMat4Projection * inMat4View * worldPos;

#ifdef USE_NORMAL
    toFSVec3Normal = mat3(transpose(inverse(inMat4Model))) * aNormal;
#endif
#ifdef USE_TANGENT
    toFSVec3Tangent = mat3(transpose(inverse(inMat4Model))) * aTangent;
#endif
#ifdef USE_BINORMAL
    toFSVec3Binormal = mat3(transpose(inverse(inMat4Model))) * aBinormal;
#endif
#ifdef USE_TEXCOORD
    toFSVec2TexCoord = aTexCoord;
#endif
}
)";

#ifdef DEBUG
	FMLOG(shaderCode.c_str());
#endif
	return shaderCode;
}



//std::string GenerateFragmentShaderWithFVF(int64 e_i64FVFFlags)
//{
//    std::string l_strDefine;
//
//    // Define macros based on FVF flags
//    if (e_i64FVFFlags & FVF_TEX0_FLAG)
//        l_strDefine += "#define USE_TEXCOORD\n";
//    if (e_i64FVFFlags & FVF_NORMAL_FLAG)
//        l_strDefine += "#define USE_NORMAL\n";
//    if (e_i64FVFFlags & FVF_TANGENT_FLAG)
//        l_strDefine += "#define USE_TANGENT\n";
//    if (e_i64FVFFlags & FVF_BITAGENT_FLAG)
//        l_strDefine += "#define USE_BINORMAL\n";
//    if (e_i64FVFFlags & FVF_NORMAL_MAP_TEXTURE_FLAG)
//        l_strDefine += "#define USE_NORMAL_MAP\n";
//    //if (e_i64FVFFlags & FVF_HAS_PBR_TEXTURE_FLAG)
//      //  l_strDefine += "#define USE_PBR\n";
//
//    // Start building the shader code
//    std::string shaderCode = R"(
//        #version 450
//        #ifdef GL_ES
//        precision mediump float;
//        #endif
//    )" + l_strDefine + R"(
//
//        in vec3 toFSVec3FragPos;
//        #ifdef USE_NORMAL
//        in vec3 toFSVec3Normal;
//        #endif
//        #ifdef USE_TEXCOORD
//        in vec2 toFSVec2TexCoord;
//        #endif
//        #ifdef USE_TANGENT
//        in vec3 toFSVec3Tangent;
//        #endif
//        #ifdef USE_BINORMAL
//        in vec3 toFSVec3Binormal;
//        #endif
//
//        out vec4 FragColor;
//
//        uniform vec3 inVec3ViewPosition;
//
//        // Light data structure matching std140 layout
//        struct Light
//        {
//            vec3 position;       // Aligned to 16 bytes
//            float intensity;     // Aligned to 16 bytes (vec4 padding)
//
//            vec3 direction;      // Aligned to 16 bytes
//            float range;         // Aligned to 16 bytes (vec4 padding)
//
//            vec3 color;          // Aligned to 16 bytes
//            float innerConeAngle; // Aligned to 16 bytes (vec4 padding)
//
//            float outerConeAngle; // Aligned to 4 bytes
//            int type;             // Aligned to 4 bytes
//            int Enable;          //
//            float pad;           // Padding to align the structure to 16 bytes
//        };
//
//        layout(std140) uniform LightBlock 
//        {
//            Light lights[8];   // Array of light data (maximum 8 lights)
//            int numLights;       // Number of lights
//            vec3 pad;            // Padding to align to 16 bytes
//        };
//
//        layout(std140) uniform TestingBlock
//        {
//            Light TestingLight;            // Padding to align to 16 bytes
//        };
//
//        #ifdef USE_PBR
//        uniform sampler2D textureAlbedo;
//        uniform sampler2D textureRoughness;
//        uniform sampler2D textureMetallic;
//        uniform sampler2D textureNormal;
//        #else
//        uniform sampler2D textureDiffuse;
//        #ifdef USE_NORMAL_MAP
//        uniform sampler2D textureNormal;
//        #endif
//        #endif
//
//        // Function to get the normal from the normal map or fallback to the interpolated normal
//        vec3 GetNormalFromMap()
//        {
//        #ifdef USE_NORMAL_MAP
//            vec3 tangent = normalize(toFSVec3Tangent);
//            vec3 bitangent = normalize(toFSVec3Binormal);
//            vec3 normal = normalize(toFSVec3Normal);
//            mat3 TBN = mat3(tangent, bitangent, normal);
//            vec3 normalMap = texture(textureNormal, toFSVec2TexCoord).rgb * 2.0 - 1.0;
//            return normalize(TBN * normalMap);
//        #else
//            return normalize(toFSVec3Normal);
//        #endif
//        }
//
//        // Function to calculate the diffuse color
//        vec3 CalculateDiffuseColor(vec3 lightColor, float diff)
//        {
//        #ifdef USE_TEXCOORD
//            vec3 diffuseColor = texture(textureDiffuse, toFSVec2TexCoord).rgb;
//            return diffuseColor * lightColor * diff;
//        #else
//            return lightColor * diff;
//        #endif
//        }
//
//        // Function to calculate PBR lighting
//        vec3 CalculatePBRLighting(vec3 N, vec3 V, vec3 L, vec3 lightColor, vec3 albedo, float roughness, float metallic)
//        {
//            vec3 H = normalize(V + L);
//            vec3 F0 = mix(vec3(0.04), albedo, metallic);
//            vec3 F = F0 + (1.0 - F0) * pow(1.0 - max(dot(H, V), 0.0), 5.0);
//            float NDF = pow(max(dot(N, H), 0.0), (2.0 / pow(roughness + 0.001, 4.0)) - 2.0);
//            float G = 1.0;
//
//            float NdotV = max(dot(N, V), 0.001);
//            float NdotL = max(dot(N, L), 0.001);
//            float denom = 4.0 * NdotV * NdotL;
//            vec3 spec = NDF * G * F / max(denom, 0.001);
//            vec3 kS = F;
//            vec3 kD = vec3(1.0) - kS;
//            kD *= 1.0 - metallic;
//            vec3 irradiance = lightColor;
//            vec3 diffuse = kD * albedo / 3.14159;
//            return (diffuse + spec) * irradiance * NdotL;
//        }
//
//        void main()
//        {
//            vec3 N = GetNormalFromMap();
//            vec3 V = normalize(inVec3ViewPosition - toFSVec3FragPos);
//            vec3 color = vec3(0.0);
//        //#ifdef USE_TEXCOORD
//        //    vec3 diffuseColor = texture(textureDiffuse, toFSVec2TexCoord).rgb;
//        //    return diffuseColor * lightColor * diff;
//        //#else
//            for (int i = 0; i < numLights; ++i)
//            {
//                vec3 L;
//                float distance = 1.0;
//                float attenuation = 1.0;
//                if (lights[i].Enable == 0)
//                {
//                    continue;
//                }
//                if (lights[i].type == 0) 
//                { // Directional light
//                    L = -normalize(lights[i].direction);
//                }
//                else
//                if (lights[i].type == 3) 
//                { //ambient
//                    //texture(textureDiffuse, toFSVec2TexCoord).rgb should move to start
//#ifdef USE_TEXCOORD
//                    vec3 diffuseColor = texture(textureDiffuse, toFSVec2TexCoord).rgb;
//                    color += (lights[i].color*diffuseColor);
//#else
//                    color += lights[i].color;
//#endif
//                    continue;
//                }
//                else
//                { // Point or spot light
//                    L = normalize(lights[i].position - toFSVec3FragPos);
//                    distance = length(lights[i].position - toFSVec3FragPos);
//                    if (lights[i].range > 0.0) 
//                    {
//                        attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
//                    }
//                }
//                float diff = max(dot(N, L), 0.0);
//
//                #ifdef USE_PBR
//                vec3 albedo = pow(texture(textureAlbedo, toFSVec2TexCoord).rgb, vec3(2.2));
//                float roughness = texture(textureRoughness, toFSVec2TexCoord).r;
//                float metallic = texture(textureMetallic, toFSVec2TexCoord).r;
//                color += CalculatePBRLighting(N, V, L, lights[i].color * lights[i].intensity, albedo, roughness, metallic);
//                #else
//                color += CalculateDiffuseColor(lights[i].color * lights[i].intensity*attenuation, diff);
//                #endif
//            }
//            FragColor = vec4(pow(color, vec3(1.0 / 2.2)), 1.0); // Gamma correction
//        }
//    )";
//
//#ifdef DEBUG
//    FMLOG(shaderCode.c_str());
//#endif
//    return shaderCode;
//}





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

    // Start building the shader code
    std::string shaderCode = R"(
#version 330 core
#ifdef GL_ES
precision mediump float;
#endif
)" + l_strDefine + R"(

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

uniform vec3 inVec3ViewPosition;

// Material uniforms
uniform sampler2D textureDiffuse;
uniform vec4 baseColorFactor;
#ifdef USE_NORMAL_MAP
uniform sampler2D textureNormal;
#endif
#ifdef USE_OCCLUSION
uniform sampler2D textureOcclusion;
uniform float occlusionStrength;
#endif
#ifdef USE_METALLIC_ROUGHNESS
uniform sampler2D textureMetallicRoughness;
#endif
uniform float metallicFactor;
uniform float roughnessFactor;
#ifdef USE_EMISSIVE
uniform sampler2D textureEmissive;
uniform vec3 emissiveFactor;
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

layout(std140) uniform LightBlock 
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
    vec3 normalMap = texture(textureNormal, toFSVec2TexCoord).rgb * 2.0 - 1.0;
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
    vec3 V = normalize(inVec3ViewPosition - toFSVec3FragPos);
    vec3 color = vec3(0.0);
#if defined(USE_TEXCOORD) && defined(FVF_BASE_COLOR_TEXTURE_FLAG)
    vec3 albedo = texture(textureDiffuse, toFSVec2TexCoord).rgb;
#else
    vec3 albedo = baseColorFactor.rgb;
#endif
//#if defined(USE_TEXCOORD) && defined(FVF_BASE_COLOR_TEXTURE_FLAG)
//    vec3 albedo = pow(texture(textureDiffuse, toFSVec2TexCoord).rgb, vec3(2.0)) * baseColorFactor.rgb;
//#else
//    vec3 albedo = baseColorFactor.rgb;
//#endif

#ifdef USE_OCCLUSION
    float occlusion = texture(textureOcclusion, toFSVec2TexCoord).r * occlusionStrength;
#else
    float occlusion = 1.0;
#endif

#ifdef USE_METALLIC_ROUGHNESS
    float metallic = texture(textureMetallicRoughness, toFSVec2TexCoord).b * metallicFactor;
    float roughness = texture(textureMetallicRoughness, toFSVec2TexCoord).g * roughnessFactor;
#else
    float metallic = metallicFactor;
    float roughness = roughnessFactor;
#endif

#ifdef USE_EMISSIVE
    vec3 emissive = pow(texture(textureEmissive, toFSVec2TexCoord).rgb, vec3(2.0)) * emissiveFactor;
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
        float diff = max(dot(N, L), 0.0)*lights[i].intensity;
        color += (diff*albedo);
        color += CalculatePBRLighting(N, V, L, lights[i].color, lights[i].intensity * attenuation, albedo, roughness, metallic, occlusion);
    }

    color += emissive;
    //FragColor = vec4(color,1);
    FragColor = vec4(pow(color, vec3(1.0 / 2.2)), 1.0); // Gamma correction
}
)";

#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}


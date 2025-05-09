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

    if (e_i64FVFFlags & FVF_HAS_PBR_TEXTURE_FLAG)
        l_strDefine += "#define USE_PBR\n";

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

        // Light data structure matching std140 layout
        struct Light {
            vec3 position;       // Aligned to 16 bytes
            float intensity;     // Aligned to 16 bytes (vec4 padding)

            vec3 direction;      // Aligned to 16 bytes
            float range;         // Aligned to 16 bytes (vec4 padding)

            vec3 color;          // Aligned to 16 bytes
            float innerConeAngle; // Aligned to 16 bytes (vec4 padding)

            float outerConeAngle; // Aligned to 4 bytes
            int type;             // Aligned to 4 bytes
            float pad1;           // Padding to align the structure to 16 bytes
            float pad2;           // Padding to align the structure to 16 bytes
        };

        layout(std140) uniform LightBlock {
            int numLights;       // Number of lights
            vec3 pad;            // Padding to align to 16 bytes
            Light lights[256];   // Array of light data (maximum 256 lights)
        };

        #ifdef USE_PBR
        uniform sampler2D textureAlbedo;
        uniform sampler2D textureRoughness;
        uniform sampler2D textureMetallic;
        uniform sampler2D textureNormal;
        #else
        uniform sampler2D textureDiffuse;
        #ifdef USE_NORMAL_MAP
        uniform sampler2D textureNormal;
        #endif
        #endif

        vec3 GetNormalFromMap()
        {
        #ifdef USE_NORMAL_MAP
            vec3 tangent = normalize(toFSVec3Tangent);
            vec3 bitangent = normalize(toFSVec3Binormal);
            vec3 normal = normalize(toFSVec3Normal);
            mat3 TBN = mat3(tangent, bitangent, normal);
            vec3 normalMap = texture(textureNormal, toFSVec2TexCoord).rgb * 2.0 - 1.0;
            return normalize(TBN * normalMap);
        #else
            return toFSVec3Normal;
        #endif
        }

        void main()
        {
            vec3 N = GetNormalFromMap();
            vec3 V = normalize(inVec3ViewPosition - toFSVec3FragPos);
            vec3 color = vec3(0.1);

            for (int i = 0; i < numLights; ++i)
            {
                vec3 L = normalize(lights[i].position - toFSVec3FragPos);
                vec3 H = normalize(V + L);
                float diff = max(dot(N, L), 0.0);

                #ifdef USE_PBR
                vec3 albedo = pow(texture(textureAlbedo, toFSVec2TexCoord).rgb, vec3(2.2));
                float roughness = texture(textureRoughness, toFSVec2TexCoord).r;
                float metallic = texture(textureMetallic, toFSVec2TexCoord).r;

                vec3 F0 = mix(vec3(0.04), albedo, metallic);
                vec3 F = F0 + (1.0 - F0) * pow(1.0 - max(dot(H, V), 0.0), 5.0);
                float NDF = pow(max(dot(N, H), 0.0), (2.0 / pow(roughness + 0.001, 4.0)) - 2.0);
                float G = 1.0;

                float NdotV = max(dot(N, V), 0.001);
                float NdotL = max(dot(N, L), 0.001);
                float denom = 4.0 * NdotV * NdotL;
                vec3 spec = NDF * G * F / max(denom, 0.001);
                vec3 kS = F;
                vec3 kD = vec3(1.0) - kS;
                kD *= 1.0 - metallic;
                vec3 irradiance = lights[i].color * lights[i].intensity;
                vec3 diffuse = kD * albedo / 3.14159;
                color += (diffuse + spec) * irradiance * NdotL;
                #else
                vec3 lightColor = lights[i].color * lights[i].intensity;
                #ifdef USE_TEXCOORD
                vec3 diffuseColor = texture(textureDiffuse, toFSVec2TexCoord).rgb;
                color += diffuseColor * lightColor * diff;
                #else
                color += lightColor * diff;
                #endif
                #endif
            }

            FragColor = vec4(pow(color, vec3(1.0 / 2.2)), 1.0);
        }
    )";

#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}



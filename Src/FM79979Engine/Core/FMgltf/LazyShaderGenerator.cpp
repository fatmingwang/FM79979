#include "../AllCoreInclude.h"
#include "LazyShaderGenerator.h"

std::string GenerateVertexShaderWithFVF(int64 e_i64FVFFlags, int e_iNumMorphTarget)
{
    std::string shaderCode = R"(
        #version 330 core
        #ifdef GL_ES
        precision mediump float;
        #endif
    )";

    // Define flags for conditional compilation
    if (e_i64FVFFlags & FVF_POS_FLAG)
        shaderCode += "#define USE_POSITION\n";
    if (e_i64FVFFlags & FVF_NORMAL_FLAG)
        shaderCode += "#define USE_NORMAL\n";
    if (e_i64FVFFlags & FVF_TANGENT_FLAG)
        shaderCode += "#define USE_TANGENT\n";
    if (e_i64FVFFlags & FVF_BITAGENT_FLAG)
        shaderCode += "#define USE_BINORMAL\n";
    if (e_i64FVFFlags & FVF_SKINNING_WEIGHT_FLAG)
        shaderCode += "#define USE_WEIGHTS\n";
    if (e_i64FVFFlags & FVF_SKINNING_BONE_INDEX_FLAG)
        shaderCode += "#define USE_JOINTS\n";
    if (e_i64FVFFlags & FVF_TEX0_FLAG)
        shaderCode += "#define USE_TEXCOORD\n";

    // Input attributes with conditional compilation
    shaderCode += R"(
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
            shaderCode += "\n\t\tlayout(location = " + std::to_string(i + FVF_MORPHING_TARGET_POS1) + ") in vec3 aMorphTarget" + std::to_string(i) + ";";
        }
        shaderCode += "\n\t\tuniform float uMorphWeights[" + std::to_string(e_iNumMorphTarget) + "];";
    }

    // Uniforms and outputs
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
    if (e_i64FVFFlags & FVF_SKINNING_WEIGHT_FLAG)
    {
        shaderCode += R"(
        const int MAX_BONES = 100;
        uniform mat4 uBoneTransforms[MAX_BONES];
        )";
    }

    // Main function
    if (e_iNumMorphTarget > 0)
    {
        shaderCode += R"(
        void main() 
        {
            vec3 morphedPosition = aPosition;
        )";
        for (int i = 0; i < e_iNumMorphTarget; i++)
        {
            shaderCode += "\tmorphedPosition += uMorphWeights[" + std::to_string(i) + "] * aMorphTarget" + std::to_string(i) + ";\n\t\t";
        }
        shaderCode += R"(
            vec4 worldPosition = inMat4Model * vec4(morphedPosition, 1.0);
            toFSVec3FragPos = worldPosition.xyz;
        )";
    }
    else
    {
        shaderCode += R"(
        void main() 
        {
            vec4 worldPosition = inMat4Model * vec4(aPosition, 1.0);
            toFSVec3FragPos = worldPosition.xyz;
        )";
    }

    // Skinning transformation
    if ((e_i64FVFFlags & FVF_SKINNING_WEIGHT_FLAG) && (e_i64FVFFlags & FVF_SKINNING_BONE_INDEX_FLAG))
    {
        shaderCode += R"(
            mat4 skinMatrix = aWeights.x * uBoneTransforms[aJoints.x] +
                              aWeights.y * uBoneTransforms[aJoints.y] +
                              aWeights.z * uBoneTransforms[aJoints.z] +
                              aWeights.w * uBoneTransforms[aJoints.w];
            vec4 skinnedPosition = skinMatrix * vec4(aPosition, 1.0);
            worldPosition = inMat4Projection * inMat4View * inMat4Model * skinnedPosition;
        )";
    }

    // Light direction and attribute transformations
    shaderCode += R"(
            toFSVec3LightDir = normalize(inVec3LightPosition - toFSVec3FragPos);
    )";

    if (e_i64FVFFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += R"(
            #ifdef USE_TANGENT
            toFSVec3Tangent = mat3(transpose(inverse(inMat4Model))) * aTangent;
            #endif
        )";
    }

    if (e_i64FVFFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += R"(
            #ifdef USE_BINORMAL
            toFSVec3Binormal = mat3(transpose(inverse(inMat4Model))) * aBinormal;
            #endif
        )";
    }

    if (e_i64FVFFlags & FVF_TEX0_FLAG)
    {
        shaderCode += R"(
            #ifdef USE_TEXCOORD
            toFSVec2TexCoord = aTexCoord;
            #endif
        )";
    }

    // Final position and normal
    if ((e_i64FVFFlags & FVF_SKINNING_WEIGHT_FLAG) && (e_i64FVFFlags & FVF_SKINNING_BONE_INDEX_FLAG))
    {
        shaderCode += R"(
            gl_Position = worldPosition;
        )";
    }
    else
    {
        shaderCode += R"(
            gl_Position = inMat4Projection * inMat4View * worldPosition;
            #ifdef USE_NORMAL
            toFSVec3Normal = mat3(transpose(inverse(inMat4Model))) * aNormal;
            #endif
        )";
    }

    shaderCode += R"(
        }
    )";

#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}

std::string GenerateFragmentShaderWithFVF(int64 e_i64FVFFlags)
{
    std::string shaderCode = R"(
        #version 330 core
        #ifdef GL_ES
        precision mediump float;
        #endif
    )";

    // Define flags for conditional compilation
    if (e_i64FVFFlags & FVF_TEX0_FLAG)
        shaderCode += "#define USE_TEXCOORD\n";
    if (e_i64FVFFlags & FVF_NORMAL_FLAG)
        shaderCode += "#define USE_NORMAL\n";
    if (e_i64FVFFlags & FVF_TANGENT_FLAG)
        shaderCode += "#define USE_TANGENT\n";
    if (e_i64FVFFlags & FVF_BITAGENT_FLAG)
        shaderCode += "#define USE_BINORMAL\n";
    if (e_i64FVFFlags & FVF_NORMAL_MAP_TEXTURE_FLAG)
        shaderCode += "#define USE_NORMAL_MAP\n";

    // Input and output declarations
    shaderCode += R"(
        out vec4 FragColor;
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
    )";

    // Uniforms and lighting functions
    shaderCode += R"(
        uniform sampler2D normalMap;
        uniform sampler2D texture1;
        uniform sampler2D textureRoughness;
        uniform sampler2D textureMetallic;

        uniform vec3 inVec3LightColor;
        uniform vec3 inVec3LightPosition;
        uniform vec3 inVec3ViewPosition;

        uniform vec3 dirLightDirection;
        uniform vec3 dirLightColor;

        vec3 ComputeLighting(vec3 normal, vec3 lightDir, vec3 viewDir) 
        {
            vec3 ambient = 0.1 * inVec3LightColor;
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * inVec3LightColor;
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = spec * inVec3LightColor;
            return ambient + diffuse + specular;
        }

        vec3 ComputeDirectionalLight(vec3 normal, vec3 viewDir) 
        {
            vec3 ambient = 0.1 * dirLightColor;
            float diff = max(dot(normal, -dirLightDirection), 0.0);
            vec3 diffuse = diff * dirLightColor;
            vec3 reflectDir = reflect(dirLightDirection, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = spec * dirLightColor;
            return ambient + diffuse + specular;
        }
    )";

    // Main function
    if (e_i64FVFFlags & FVF_TEX0_FLAG)
    {
        shaderCode += R"(
        void main() 
        {
            vec3 color = texture(texture1, toFSVec2TexCoord).rgb;
        )";
    }
    else
    {
        shaderCode += R"(
        void main() 
        {
            vec3 color = vec3(0);
        )";
    }

    // Normal handling
    if (e_i64FVFFlags & FVF_NORMAL_MAP_TEXTURE_FLAG)
    {
        shaderCode += R"(
            #ifdef USE_NORMAL_MAP
            vec3 normal = normalize(toFSVec3Normal);
            vec3 normalMapColor = texture(normalMap, toFSVec2TexCoord).rgb;
            normalMapColor = normalize(normalMapColor * 2.0 - 1.0);
            #endif
        )";
        if (e_i64FVFFlags & FVF_BITAGENT_FLAG)
        {
            shaderCode += R"(
                #ifdef USE_BINORMAL
                mat3 TBN = mat3(normalize(toFSVec3Tangent), normalize(toFSVec3Binormal), normalize(toFSVec3Normal));
                normal = normalize(TBN * normalMapColor);
                #endif
            )";
        }
    }
    else
    {
        if (e_i64FVFFlags & FVF_NORMAL_FLAG)
        {
            shaderCode += R"(
            #ifdef USE_NORMAL
            vec3 normal = toFSVec3Normal;
            #endif
            )";
        }
        else
        {
            shaderCode += R"(
            vec3 normal;
            )";
        }
    }

    // Final lighting and output
    shaderCode += R"(
            vec3 viewDir = normalize(inVec3ViewPosition - toFSVec3FragPos);
            vec3 lighting = ComputeLighting(normal, toFSVec3LightDir, viewDir);
            vec3 dirLighting = ComputeDirectionalLight(normal, viewDir);
            FragColor = vec4(color * (lighting + dirLighting), 1.0);
        }
    )";

#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}
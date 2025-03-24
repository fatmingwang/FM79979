#include "LazyShaderGenerator.h"
#include "../AllCoreInclude.h"

std::string GenerateVertexShaderWithFVF(unsigned int fvfFlags, int e_iNumMorphTarget)
{
    std::string shaderCode = R"(
        #version 330 core
        #ifdef GL_ES
        precision mediump float;
        #endif

        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aNormal;
        layout(location = 7) in vec2 aTexCoord;
    )";
    if (e_iNumMorphTarget>0)
    {
        // Generate morph target attributes dynamically
        for (int i = 0; i < e_iNumMorphTarget; i++)
        {
            shaderCode += "\tlayout(location = " + std::to_string(i + FVF_MORPHING_TARGET_POS1) + ") in vec3 aMorphTarget" + std::to_string(i) + ";\n\t";
        }
        // Generate weight uniforms
        shaderCode += "\tuniform float morphWeights[" + std::to_string(e_iNumMorphTarget) + "];\n";

    }
    // Adding tangents if necessary
    if (fvfFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += R"(
        layout(location = 3) in vec3 aTangent;)";
    }

    // Adding binormals if necessary
    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += R"(
        layout(location = 4) in vec3 aBinormal;)";
    }

    // Adding joint indices and weights if necessary
    if (fvfFlags & FVF_SKINNING_WEIGHT_FLAG)
    {
        shaderCode += R"(
        layout(location = 5) in vec4 aWeights;)";
    }

    if (fvfFlags & FVF_SKINNING_BONE_INDEX_FLAG)
    {
        shaderCode += R"(
        layout(location = 6) in ivec4 aJoints;)";
    }

    // Normalizing the vectors and transforming to view space
    shaderCode += R"(
        uniform mat4 inMat4Model;
        uniform mat4 inMat4View;
        uniform mat4 inMat4Projection;
        uniform vec3 inVec3LightPosition;

        out vec2 toFSVec2TexCoord;
        out vec3 toFSVec3Normal;
        out vec3 toFSVec3FragPos;
        out vec3 toFSVec3LightDir;
    )";

    if (fvfFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += R"(
        out vec3 toFSVec3Tangent;)";
    }

    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += R"(
        out vec3 toFSVec3Binormal;)";
    }
    if (fvfFlags & FVF_SKINNING_WEIGHT_FLAG)
    {
        shaderCode += R"(
            const int MAX_BONES = 100;
            uniform mat4 uBoneTransforms[MAX_BONES];
        )";
    }

    if (e_iNumMorphTarget)
    {
        shaderCode += R"(
        void main() 
        {
            vec3 morphedPosition = aPosition;
        )";
        for (int i = 0; i < e_iNumMorphTarget; i++)
        {
            shaderCode += "\tmorphedPosition += morphWeights[" + std::to_string(i) + "] * aMorphTarget" + std::to_string(i) + ";\n\t\t";
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

    if ((fvfFlags & FVF_SKINNING_WEIGHT_FLAG) && (fvfFlags & FVF_SKINNING_BONE_INDEX_FLAG))
    {
        shaderCode += R"(
            // Skinning transformation
            mat4 skinMatrix = aWeights.x * uBoneTransforms[aJoints.x] +
                              aWeights.y * uBoneTransforms[aJoints.y] +
                              aWeights.z * uBoneTransforms[aJoints.z] +
                              aWeights.w * uBoneTransforms[aJoints.w];
            vec4 skinnedPosition = skinMatrix * vec4(aPosition, 1.0);;
            worldPosition = inMat4Projection*inMat4View*inMat4Model* skinnedPosition;

        )";
    }

    shaderCode += R"(
            // Passing light direction to fragment shader
            toFSVec3LightDir = normalize(inVec3LightPosition - toFSVec3FragPos);
    )";


    if (fvfFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += R"(
            // Transform tangent if present
            toFSVec3Tangent = mat3(transpose(inverse(inMat4Model))) * aTangent;
        )";
    }

    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += R"(
            // Transform binormal if present
            toFSVec3Binormal = mat3(transpose(inverse(inMat4Model))) * aBinormal;
        )";
    }

    shaderCode += R"(
            // Pass texture coordinates
            toFSVec2TexCoord = aTexCoord;
        )";

    if ((fvfFlags & FVF_SKINNING_WEIGHT_FLAG) && (fvfFlags & FVF_SKINNING_BONE_INDEX_FLAG))
    {
        shaderCode += R"(    gl_Position = worldPosition;
        )";
    }
    else
    {
        shaderCode += R"(
            gl_Position = inMat4Projection * inMat4View * worldPosition;
            // Transform normal to world space and then to view space
            toFSVec3Normal = mat3(transpose(inverse(inMat4Model))) * aNormal;
        )";
    }
    shaderCode += R"(
        })";
#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}


std::string GenerateFragmentShaderWithFVF(unsigned int fvfFlags)
{
    std::string shaderCode = R"(
        #version 330 core
        #ifdef GL_ES
        precision mediump float;
        #endif

        out vec4 FragColor;
        in vec2 toFSVec2TexCoord;
        in vec3 toFSVec3Normal;
        in vec3 toFSVec3FragPos;
        in vec3 toFSVec3LightDir;)";
    if (fvfFlags & FVF_TANGENT_FLAG)
    {
        shaderCode += R"(
        in vec3 toFSVec3Tangent;)";
    }

    if (fvfFlags & FVF_BITAGENT_FLAG)
    {
        shaderCode += R"(
        in vec3 toFSVec3Binormal;)";
    }
    shaderCode += R"(
        // Declare the uniform for fvfFlags passed from C++ 
        uniform uint fvfFlags;

        // Define constants for flags, from shader.h
        const uint FVF_TANGENT_FLAG = 1u << 4;
        const uint FVF_BITAGENT_FLAG = 1u << 5;
        const uint FVF_NORMAL_FLAG = 1u << 1;  // Flag for normal
        const uint FVF_NORMAL_MAP = 1u << 6;  // Flag for normal map

        // Add normal map if necessary
        uniform sampler2D normalMap;
        uniform sampler2D texture1; // Base color texture
        uniform sampler2D textureRoughness; // Roughness texture
        uniform sampler2D textureMetallic; // Metallic texture

        // Light properties
        uniform vec3 inVec3LightColor;
        uniform vec3 inVec3LightPosition;
        uniform vec3 inVec3ViewPosition;

        // Directional light properties
        uniform vec3 dirLightDirection;
        uniform vec3 dirLightColor;

        // Phong lighting model
        vec3 ComputeLighting(vec3 normal, vec3 lightDir, vec3 viewDir) 
        {
            vec3 ambient = 0.1 * inVec3LightColor;  // Ambient lighting

            // Diffuse shading (Lambert's cosine law)
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * inVec3LightColor;

            // Specular reflection (Blinn-Phong)
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // Hardcoded shininess
            vec3 specular = spec * inVec3LightColor;

            return ambient + diffuse + specular;  // Returning the combined light contribution
        }

        // Directional light calculation
        vec3 ComputeDirectionalLight(vec3 normal, vec3 viewDir) 
        {
            vec3 ambient = 0.1 * dirLightColor;  // Ambient lighting

            // Diffuse shading (Lambert's cosine law)
            float diff = max(dot(normal, -dirLightDirection), 0.0);
            vec3 diffuse = diff * dirLightColor;

            // Specular reflection (Blinn-Phong)
            vec3 reflectDir = reflect(dirLightDirection, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // Hardcoded shininess
            vec3 specular = spec * dirLightColor;

            return ambient + diffuse + specular;  // Returning the combined light contribution
        }
    )";

    shaderCode += R"(
        void main() 
        {
            // Fetch base color texture
            vec3 color = texture(texture1, toFSVec2TexCoord).rgb;
    )";

    if (fvfFlags & FVF_NORMAL_FLAG)
    {
        shaderCode += R"(
            // Normal map processing (if flag is set)
            vec3 normal = normalize(toFSVec3Normal);
            if (bool(fvfFlags & FVF_NORMAL_MAP)) 
            {  // Check if FVF_NORMAL_MAP bit is set
                vec3 normalMapColor = texture(normalMap, toFSVec2TexCoord).rgb;
                normalMapColor = normalize(normalMapColor * 2.0 - 1.0);  // Convert from [0, 1] to [-1, 1]

                        )";
        if (fvfFlags & FVF_BITAGENT_FLAG)
        {
            shaderCode += R"(
                // Transform normal from tangent space to world space
                mat3 TBN = mat3(normalize(toFSVec3Tangent), normalize(toFSVec3Binormal), normalize(toFSVec3Normal));
                normal = normalize(TBN * normalMapColor);
            )";
        }
        shaderCode += R"(
            }
            // Calculate the view direction
            vec3 viewDir = normalize(inVec3ViewPosition - toFSVec3FragPos);

            // Calculate lighting
            vec3 lighting = ComputeLighting(normal, toFSVec3LightDir, viewDir);

            // Calculate directional lighting
            vec3 dirLighting = ComputeDirectionalLight(normal, viewDir);

            // Apply texture and lighting effects
            FragColor = vec4(color * (lighting + dirLighting), 1.0);
        )";
    }
    else
    {
        shaderCode += R"(
            // Skip lighting calculations and directly set FragColor
            FragColor = vec4(color, 1.0);
        )";
    }

    if ((fvfFlags & FVF_TEX0_FLAG) == 0)
    {
        shaderCode += R"(
            FragColor = vec4(1, 1, 0, 1.0);)";
    }
    shaderCode += R"(
        })";
#ifdef DEBUG
    FMLOG(shaderCode.c_str());
#endif
    return shaderCode;
}
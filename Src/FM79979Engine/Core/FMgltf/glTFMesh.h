#pragma once



enum eFVF
{
    FVF_POSITION1 = 1 << 0,   // Position attribute (vec3)
    FVF_NORMAL2 = 1 << 1,     // Normal attribute (vec3)
    FVF_COLOR3 = 1 << 2,      // Color attribute (vec4)
    FVF_TEXCOORD4 = 1 << 3,   // Texture coordinates attribute (vec2)
    FVF_TANGENT5 = 1 << 4,    // Tangent attribute (vec3)
    FVF_BINORMAL6 = 1 << 5,   // Binormal attribute (vec3)
    FVF_NORMAL_MAP = 1 << 6,  // Normal map (for fragment shader)
    FVF_METALLIC = 1 << 7,    // Metallic texture (for PBR)
    FVF_ROUGHNESS = 1 << 8,   // Roughness texture (for PBR)
    FVF_EMISSIVE = 1 << 9,    // Emissive texture (if needed)
    FVF_OCCLUSION = 1 << 10,  // Occlusion texture (for PBR)
    FVF_BASE_COLOR = 1 << 11, // Base color texture (if needed)
    FVF_PBR = FVF_METALLIC | FVF_ROUGHNESS | FVF_OCCLUSION | FVF_BASE_COLOR, // PBR materials group
    FVF_ALL = FVF_POSITION1 | FVF_NORMAL2 | FVF_TEXCOORD4 | FVF_TANGENT5 | FVF_BINORMAL6 // All common attributes
};
//Explanation:
//Position(FVF_POSITION1) : Vertex position(typically vec3).
//Normal(FVF_NORMAL2) : Normal vector(typically vec3).
//Color(FVF_COLOR3) : Color data(typically vec4), useful for vertex colors.
//Texture Coordinates(FVF_TEXCOORD4) : Texture UV coordinates(typically vec2).
//Tangent(FVF_TANGENT5) : Tangent vector(typically vec3), useful for normal mapping.
//Binormal(FVF_BINORMAL6) : Binormal vector(typically vec3), needed for normal mapping(or TBN matrix).
//Normal Map(FVF_NORMAL_MAP) : Flag to enable the use of normal maps in the fragment shader.
//Metallic(FVF_METALLIC) : Metallic texture, typically part of PBR workflows.
//Roughness(FVF_ROUGHNESS) : Roughness texture, also part of PBR workflows.
//Emissive(FVF_EMISSIVE) : Emissive texture for lighting effects like glow.
//Occlusion(FVF_OCCLUSION) : Occlusion texture, typically used for PBR(ambient occlusion).
//Base Color(FVF_BASE_COLOR) : Base color texture(diffuse map or albedo in PBR).
//PBR(FVF_PBR) : Convenience flag for PBR, which combines all the necessary attributes.
//All(FVF_ALL) : Combines all common attributes like position, normal, texture coordinates, tangent, and binormal.
//
//The FVF flags are used to define which attributes are present in your mesh data.For example, if you want to use a normal map in your fragment shader, you would enable the FVF_NORMAL_MAP flag.
//In your cMesh::Draw or cMesh::LoadAttributes function, you can check these flags and dynamically modify the shaders or buffer binding accordingly.
//
//Let's say you want to load a mesh with positions, normals, and texture coordinates, but you don't want tangents or binormals, and you want to include a normal map for the fragment shader.You could set the FVF flags as follows :
//unsigned int fvfFlags = FVF_POSITION1 | FVF_NORMAL2 | FVF_TEXCOORD4 | FVF_NORMAL_MAP;
//This tells the system to only use position, normal, texture coordinates, and normal map in the shader, and the shaders would be generated accordingly.

class cMesh
{
    friend class cScene;
    //no sub mesh because I am lazy to do different FVF fuck.
    //struct SubMesh
    //{
    //    unsigned int fvfFlags;
    //    GLuint shaderProgram;
    //    unsigned int indexOffset;
    //    unsigned int indexCount;
    //};
    //std::vector<SubMesh> subMeshes;  // Store different primitives
private:
    GLuint m_uiVAO;
    GLuint m_uiVBO;
    GLuint m_uiEBO;
    GLenum                  m_DrawIndexType = GL_UNSIGNED_INT;
    std::vector<float>      m_VertexBufferVector;  // All vertex data in one buffer
    std::vector<GLuint>     m_uiTextureIDVector;        // Base color textures (and other types if necessary)
    std::vector<GLuint>     m_uiNormalTextureIDVector;  // Normal maps
    std::vector<GLuint>     m_uiOocclusionTextureIDVector;  // Occlusion maps
    std::vector<GLuint>     m_uiEmissiveTextureIDVector; // Emissive maps (if needed)
    std::vector<uint32_t>   m_IDrawndicesBufferVector;      // Index data for rendering
    unsigned int m_uiVertexStride;          // Total stride for vertices
    unsigned int m_uiFVFFlags;              // Flexible Vertex Format flags
	unsigned int m_uiShaderProgram;         // Shader program for this mesh
public:
    cMesh();
    ~cMesh();

    void InitBuffer();  // Initialize and bind buffers
    void Draw();        // Draw the mesh

    // Function to load vertex attributes and indices
    void LoadAttributes(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal);
    void LoadTextures(const tinygltf::Model& model, const tinygltf::Material& material);
    void logFVFFlags();
};


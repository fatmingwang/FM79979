#pragma once



enum eFVF
{
    FVF_POSITION1 = 1 << 0,
    FVF_NORMAL2 = 1 << 1,
    FVF_COLOR3 = 1 << 2,
    FVF_TEXCOORD4 = 1 << 3,
    FVF_BINORMAL5 = 1 << 4 // <-- No trailing comma
};

class cMesh
{
    public:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    std::vector<float> interleavedData;  // All vertex data in one buffer
    std::vector<GLuint> textures;        // Base color textures (and other types if necessary)
    std::vector<GLuint> normalTextures;  // Normal maps
    std::vector<GLuint> occlusionTextures;  // Occlusion maps
    std::vector<GLuint> emissiveTextures; // Emissive maps (if needed)
    std::vector<uint32_t> indices;      // Index data for rendering
    unsigned int vertexStride;          // Total stride for vertices
    unsigned int fvfFlags;              // Flexible Vertex Format flags

    cMesh();
    ~cMesh();

    void InitBuffer();  // Initialize and bind buffers
    void Draw();        // Draw the mesh

    // Function to load vertex attributes and indices
    void LoadAttributes(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal);
    void LoadTextures(const tinygltf::Model& model, const tinygltf::Material& material);
    void logFVFFlags();
};


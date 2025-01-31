#pragma once

#include "glTFMesh.h"


class cScene
{
    std::string GenerateVertexShader(unsigned int fvfFlags);
    std::string GenerateFragmentShader(unsigned int fvfFlags);
    std::map<unsigned int, GLuint> shaderPrograms; // FVF -> Shader Program Map
    GLuint GetShaderProgram(unsigned int fvfFlags);  // Returns shader based on FVF
    GLuint CreateShader(unsigned int fvfFlags);
public:
    std::map<std::string, cMesh> meshes;
    bool LoadFromGLTF(const std::string& filename,bool e_bCalculateBiNormal = false);
    void InitBuffers();
    void Draw();
};
#pragma once

#include "glTFMesh.h"
#include "glTFAnimationMesh.h"


class cScene
{
    std::string GenerateVertexShader(unsigned int fvfFlags);
    std::string GenerateFragmentShader(unsigned int fvfFlags);
    std::map<unsigned int, GLuint> shaderPrograms; // FVF -> Shader Program Map
    GLuint GetShaderProgram(unsigned int fvfFlags);  // Returns shader based on FVF
    GLuint CreateShader(unsigned int fvfFlags);
    void    PopulateUniform(int e_iProgram);
    void    PopulateAttribute(int e_iProgram);
public:
    std::map<std::string, cMesh*> meshes;
    std::map<std::string, cAnimationMesh*> m_AnimationMeshMap;
    bool LoadFromGLTF(const std::string& filename,bool e_bCalculateBiNormal = false);
    void InitBuffers();
    void Draw();
    void Destory();
};
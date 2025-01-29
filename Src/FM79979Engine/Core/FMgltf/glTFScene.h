#pragma once

#include "glTFMesh.h"


class cScene
{
    public:
    std::map<std::string, cMesh> meshes;
    bool LoadFromGLTF(const std::string& filename,bool e_bCalculateBiNormal = false);
    void InitBuffers();
    void Draw();
};

#include "../AllCoreInclude.h"
#include <vector>
#include <string>
#include <map>
#include "tiny_gltf.h"
#include <iostream>
#include "glTFScene.h"


// Load the scene from a GLTF file
bool cScene::LoadFromGLTF(const std::string& filename, bool e_bCalculateBiNormal)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;

    std::string err;
    std::string warn;
    bool success = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    if (!success)
    {
        // If loading as binary fails, try loading as ASCII
        success = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    }

    if (!success)
    {
        std::cerr << "Error loading GLTF file: " << err << std::endl;
        return false;
    }

    for (const auto& meshPair : model.meshes)
    {
        cMesh mesh;
        for (const auto& primitive : meshPair.primitives)
        {
            mesh.LoadAttributes(model, primitive, e_bCalculateBiNormal);

            // Load textures for each material
            if (primitive.material >= 0 && primitive.material < model.materials.size())
            {
                mesh.LoadTextures(model, model.materials[primitive.material]);
            }
        }
        meshes[meshPair.name] = mesh;
    }

    return true;
}

// Initialize buffers for all meshes
void cScene::InitBuffers()
{
    for (auto& meshPair : meshes)
    {
        meshPair.second.InitBuffer();
    }
}

// Draw the scene
void cScene::Draw()
{
    for (auto& meshPair : meshes)
    {
        meshPair.second.Draw();
    }
}

cScene g_cScene;
int glTFInit()
{
    g_cScene.LoadFromGLTF("glTFModel/Duck.gltf",true);
    g_cScene.InitBuffers();
    return 1;
}

void GlTFRender()
{
    //    DrawModel(model, shaderProgram);
    g_cScene.Draw();
}


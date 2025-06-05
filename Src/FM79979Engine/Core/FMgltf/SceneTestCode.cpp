#include "pch.h"
#include "glTFModel.h"
#include "GameScene.h"

cglTFModel* g_pglTFModel = nullptr;

int glTFInit()
{
    if (!g_pglTFModel)
    {
        g_pglTFModel = new cglTFModel();
    }
    cglTFModel& g_glTFModel = *g_pglTFModel;
    //g_glTFModel.LoadFromGLTF("glTFModel/Duck.gltf",false);
    //g_glTFModel.LoadFromGLTF("glTFModel/Lantern.gltf",true);
    // 
    //g_glTFModel.LoadFromGLTF("glTFModel/Avocado.gltf", true);
    //g_glTFModel.LoadFromGLTF("glTFModel/CesiumMilkTruck.glb", true);
    //g_glTFModel.LoadFromGLTF("glTFModel/Fox.gltf", true);
    //morphing
    //g_glTFModel.LoadFromGLTF("glTFModel/AnimatedMorphCube.glb", true);
    //g_glTFModel.LoadFromGLTF("glTFModel/CarConcept.gltf", false);
    //g_glTFModel.LoadFromGLTF("glTFModel/glTF/ABeautifulGame.gltf", true);

    //g_glTFModel.LoadFromGLTF("glTFModel/SimpleSkin.gltf", true);
    g_glTFModel.LoadFromGLTF("glTFModel/Woman.gltf", true);

    //g_glTFModel.LoadFromGLTF("glTFModel/Buggy.gltf", false);
    //g_glTFModel.LoadFromGLTF("glTFModel/AnimatedCube.gltf", false);
    //g_glTFModel.LoadFromGLTF("glTFModel/BoxAnimated.gltf", false);


    //g_glTFModel.InitBuffers();
    return 1;
}

void GlTFRender()
{
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glCullFace(GL_FRONT);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    cBaseShader* l_pShader = GetCurrentShader();
    if (l_pShader)
    {
        l_pShader->Unuse();
    }
    UseShaderProgram(L"qoo79979");
    cLighController::GetInstance()->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
    cCameraController::GetInstance()->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
    if (g_pglTFModel)
    {
        g_pglTFModel->Update(0.016f);
        g_pglTFModel->Render();
    }
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (g_pglTFModel)
    {
        for (auto& meshPair : g_pglTFModel->m_AnimationMeshMap)
        {
            if (meshPair.second)
            {
                meshPair.second->RenderSkeleton();
            }
        }
    }
}

void GlTFDestory()
{
    //    DrawModel(model, shaderProgram);
    cTextureManager::ClearSharedTextureReferenceMap();
    SAFE_DELETE(g_pglTFModel);
}


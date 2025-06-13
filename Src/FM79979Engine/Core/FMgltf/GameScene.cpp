#include "pch.h"
#include "GameScene.h"
#include <memory>
#include <string>


cglTFScene::cglTFScene()
{
    // Create root frame
    m_pRootFrame = std::make_unique<cRenderObject>();
    m_pRootFrame->SetName(L"RootFrame");
}

cglTFScene::~cglTFScene()
{
    cCameraController::DestroyInstance();
	cLighController::DestroyInstance();
}

Frame* cglTFScene::GetRootFrame() const
{
    return m_pRootFrame.get();
}
//cCameraController* cglTFScene::GetCameraController() const
//{
//    return m_pCameraController.get();
//}
//cLighController* cglTFScene::GetLightController() const
//{
//    return m_pLightController;
//}

// Recursive update
void cglTFScene::Update(float e_fElpaseTime)
{
    
    g_fLightControllerUpdate(e_fElpaseTime);
    g_fCameraControllerUpdate(e_fElpaseTime);
    if (m_pRootFrame)
    {
		m_pRootFrame->UpdateNodes(e_fElpaseTime);
    }
        //m_pRootFrame-Update(e_fElpaseTime);
}

// Recursive render
void cglTFScene::Render()
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
    if (m_pRootFrame)
    {
		m_pRootFrame->RenderNodes();
    }

    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //g_fCameraDebugRender();
}

// Find Frame by name (recursive)
Frame* cglTFScene::FindFrameByName(const std::wstring& name) const
{
    return FindFrameByNameRecursive(m_pRootFrame.get(), name);
}


Frame* cglTFScene::FindFrameByNameRecursive(Frame* pFrame, const std::wstring& name) const
{
    if (!pFrame)
        return nullptr;
    if (pFrame->GetName() == name)
        return pFrame;
    for (Frame* child = pFrame->GetFirstChild(); child; child = child->GetNextSibling())
    {
        Frame* found = FindFrameByNameRecursive(child, name);
        if (found)
            return found;
    }
    return nullptr;
}
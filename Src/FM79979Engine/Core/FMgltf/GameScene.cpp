#include "pch.h"
#include "GameScene.h"
#include <memory>
#include <string>
#include "../../imgui/ImGuiRender.h"
#include "../../imgui/imgui.h"
#include "glTFModel.h"


cglTFScene::cglTFScene()
{
    // Create root frame
    m_pRootFrame = std::make_unique<cglTFNodeData>();
    m_pRootFrame->SetName(L"RootFrame");
    ImGui_ImplOpenGL3_Init(cGameApp::m_spOpenGLRender->m_Handle, nullptr, 1);
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    //ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().LogFilename = nullptr;
    ImGui::GetIO().FontGlobalScale = 1.5f;
}

cglTFScene::~cglTFScene()
{
    DestoryWithChildren();
}

Frame* cglTFScene::GetRootFrame() const
{
    return m_pRootFrame.get();
}

// Recursive update
void cglTFScene::Update(float e_fElpaseTime)
{
    g_fLightControllerUpdate(e_fElpaseTime);
    if (m_pRootFrame)
    {
		m_pRootFrame->UpdateNodes(e_fElpaseTime);
    }
    g_fCameraControllerUpdate(e_fElpaseTime);
        //m_pRootFrame-Update(e_fElpaseTime);
}

// Recursive render
void cglTFScene::Render()
{
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);     // glTF uses counter-clockwise winding
    //glCullFace(GL_FRONT);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glClearColor(1.f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cBaseShader* l_pShader = GetCurrentShader();
    if (l_pShader)
    {
        l_pShader->Unuse();
    }
    UseShaderProgram(L"qoo79979");

    //for (auto& light : cLighController::GetInstance()->GetLights())
    //{
    //    if (light->m_0Type1Enable[0] == (int)eLightType::eLT_DIRECTIONAL || light->m_0Type1Enable[0] == (int)eLightType::eLT_SPOT)
    //    {
    //        m_ShadowMap.BindForWriting();
    //        glClear(GL_DEPTH_BUFFER_BIT);
    //        //cMatrix44 lightViewProj = ComputeLightViewProjMatrix(light); // You need to implement this
    //        //m_pRootFrame->RenderNodesShadowPass(lightViewProj, shadowShaderProgram);
    //        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //    }
    //}
    if (m_pRootFrame)
    {
		m_pRootFrame->RenderNodes();
    }
    UseShaderProgram();
    auto l_matWVP = cCameraController::GetInstance()->GetCurrentCamera()->GetWorldViewProjection();
    //FATMING_CORE::SetupShaderViewProjectionMatrix(m_Projection.GetMatrix() * this->GetWorldView(), true);
    FATMING_CORE::SetupShaderViewProjectionMatrix(l_matWVP, true);
    cLighController::GetInstance()->DebugRender();
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //g_fCameraDebugRender();    
    if (m_pRootFrame)
    {
        //m_pRootFrame->DebugRenderNodes();
    }
    ImGui_StartFrame();
    cLighController::GetInstance()->RenderImGUILightControllerUI();
    GlobalRenderObjectGoThoughAllFrameFromaFirstToEnd(
        [](Frame* e_pFrame)
        {
            if(e_pFrame->Type() == cglTFModel::TypeID)
            {
                cglTFModel* l_pModel = dynamic_cast<cglTFModel*>(e_pFrame);
                if(l_pModel)
                {
                    l_pModel->RenderImGUI();
                }
			}
        }
    , dynamic_cast<Frame*>(m_pRootFrame.get()));
    GlobalRenderObjectGoThoughAllFrameFromaFirstToEnd(
        [](Frame* e_pFrame)
        {
            if (e_pFrame->Type() == cglTFModel::TypeID)
            {
                cglTFModel* l_pModel = dynamic_cast<cglTFModel*>(e_pFrame);
                if (l_pModel)
                {
                    l_pModel->RenderAnimationImGUI();
                }
            }
        }
    , dynamic_cast<Frame*>(m_pRootFrame.get()));
    
    ImGui_EndFrame();
    
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
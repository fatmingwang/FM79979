#include "pch.h"
#include "GameScene.h"
#include <memory>
#include <string>
#include "../../imgui/ImGuiRender.h"
#include "../../imgui/imgui.h"
#include "glTFModel.h"
#include "LazyShaderGenerator.h"

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
	m_pShadowMap = new cShadowMap();
    m_pShadowMap->Init(1920, 1080);
    m_pShadowMap->InitShadowMapProgram(); // Initialize shadow map shader program
}

cglTFScene::~cglTFScene()
{
    SAFE_DELETE(m_pShadowMap);
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
    auto l_matWVP = cCameraController::GetInstance()->GetCurrentCamera()->GetWorldViewProjection();
    // Shadow pass for each directional/spot light
    if (m_pShadowMap)
    {
        // Set cull face to front for shadow map pass
        glCullFace(GL_FRONT);
        for (auto& light : cLighController::GetInstance()->GetLights())
        {
            if (light->m_0Type1Enable[0] == (int)eLightType::eLT_DIRECTIONAL || light->m_0Type1Enable[0] == (int)eLightType::eLT_SPOT)
            {
                cMatrix44 lightViewProj;
                if (cLighController::GetInstance()->GetLightViewProjectionMatrixByIndex(lightViewProj))
                {
                    m_pShadowMap->BindForWriting();
                    glClear(GL_DEPTH_BUFFER_BIT);
                    glClearDepth(1.0f);
                    GLuint shadowShaderProgram = m_pShadowMap->GetShadowMapProgram();
                    m_pRootFrame->RenderNodesShadowPass(lightViewProj, shadowShaderProgram);
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                }
            }
        }
        // Restore cull face to back after shadow map pass
        glCullFace(GL_BACK);
    }

    if (m_pRootFrame)
    {
		m_pRootFrame->RenderNodes();
    }
    UseShaderProgram();
    FATMING_CORE::SetupShaderViewProjectionMatrix(l_matWVP, true);
    cLighController::GetInstance()->DebugRender();
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    if (m_pRootFrame)
    {
        //m_pRootFrame->DebugRenderNodes();
    }
    if (m_pShadowMap)
    {
		m_pShadowMap->RenderFrameBufferAs2DImage(Vector2(0, 0), Vector2(1920/4,1080/4));
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
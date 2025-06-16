#include "pch.h"
#include "glTFModel.h"
#include "GameScene.h"

cglTFScene*g_pglTFScene = nullptr;

cglTFModel*LazyAddModel(Frame*e_pFrame,const char*e_strFileName,int e_iInstanceValue = 0)
{
    cglTFModel*l_pglTFModel = new cglTFModel();;
    l_pglTFModel->LoadFromGLTF(e_strFileName, true,e_iInstanceValue);
    //e_pFrame->AddChild(l_pglTFModel);
    return l_pglTFModel;
}

std::shared_ptr<class cAnimationInstanceManager>g_spAnimationInstanceManager = nullptr;
std::shared_ptr<sAniamationInstanceData> g_spAniamationInstanceData = nullptr;

void TestRenderFunction(GLuint e_uiProgramID)
{
    g_spAnimationInstanceManager->Render(e_uiProgramID, g_spAniamationInstanceData);
}
class cSkinningAnimTestClass
{
    std::shared_ptr<class cAnimationInstanceManager>m_spAnimationInstanceManager;
    GLuint m_uiProgramID;
    std::shared_ptr<sAniamationInstanceData> m_spAniamationInstanceData;
public:
    void    SetData(std::vector<std::shared_ptr<class cAnimationInstanceManager>>&e_Data)
    {
        if (e_Data.size())
        {
            m_spAnimationInstanceManager = e_Data[0];
            std::tuple<std::shared_ptr<sAniamationInstanceData>, GLuint > l_TupleData = e_Data[0]->GetAnimationInstanceData("Running");
            //std::tuple<std::shared_ptr<sAniamationInstanceData>, GLuint > l_TupleData = e_Data[0]->GetAnimationInstanceData("");
            m_spAniamationInstanceData = std::get<0>(l_TupleData);
            m_uiProgramID = std::get<1>(l_TupleData);
            auto l_uiSize = m_spAniamationInstanceData->m_AnimationFrameAndTimeVector.size();
            for (auto i = 0; i < l_uiSize; ++i)
            {
                auto l_pCurrentData = m_spAniamationInstanceData->m_AnimationFrameAndTimeVector[i];
                l_pCurrentData->Update(frand(0, 3.f));
                m_spAniamationInstanceData->m_FrameIndexVector[i].iCurrent = l_pCurrentData->m_iCurrentFrame;
                m_spAniamationInstanceData->m_FrameIndexVector[i].iCurrent = l_pCurrentData->m_iNextFrame;
                m_spAniamationInstanceData->m_ToNextLerpTime[i] = l_pCurrentData->m_fNextFrameLerpTimes;
            }
        }
        g_spAnimationInstanceManager = m_spAnimationInstanceManager;
        g_spAniamationInstanceData = m_spAniamationInstanceData;

    }
    void Update(float e_fElpaseTime)
    {
        if (m_spAniamationInstanceData)
        {
            auto l_uiSize = m_spAniamationInstanceData->m_AnimationFrameAndTimeVector.size();
            for (auto i = 0; i < l_uiSize; ++i)
            {
                auto l_pCurrentData = m_spAniamationInstanceData->m_AnimationFrameAndTimeVector[i];
                l_pCurrentData->Update(e_fElpaseTime);
                m_spAniamationInstanceData->m_FrameIndexVector[i].iCurrent = l_pCurrentData->m_iCurrentFrame;
                m_spAniamationInstanceData->m_FrameIndexVector[i].iNext = l_pCurrentData->m_iNextFrame;
                m_spAniamationInstanceData->m_ToNextLerpTime[i] = l_pCurrentData->m_fNextFrameLerpTimes;
            }
        }
    }
    void Render()
    {
        //if (m_spAnimationInstanceManager)
        //{
        //    glUseProgram(this->m_uiProgramID);
        //    m_spAnimationInstanceManager->Render(this->m_uiProgramID, this->m_spAniamationInstanceData);
        //}
    }
    void Clear()
    {
        m_spAnimationInstanceManager = nullptr;
        m_spAniamationInstanceData = nullptr;
        g_spAnimationInstanceManager = nullptr;
        g_spAniamationInstanceData = nullptr;
    }
};

cSkinningAnimTestClass g_SkinningAnimTestClass;

int glTFInit()
{
	g_pglTFScene = new cglTFScene();
    auto l_pRootFrame = g_pglTFScene->GetRootFrame();
    {
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/Duck.gltf");
        auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/Woman.gltf",100);
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/SimpleSkin.gltf", 100);
        g_SkinningAnimTestClass.SetData(l_pDuck->GetAnimationInstanceManagerVector());
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/SpecularTest.glb");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/Avocado.gltf");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/AnimatedMorphCube.glb");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/fox.gltf");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/MosquitoInAmber.glb");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/Buggy.glb");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/VirtualCity.glb");          
        
        {
            //auto l_pDuck2 = LazyAddModel(l_pRootFrame, "glTFModel/CesiumMilkTruck.glb");
            //l_pRootFrame->AddChild(l_pDuck2);
        }
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/Lantern.gltf");// 
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/glTF/ABeautifulGame.gltf",1);
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/SimpleInstancing.glb");// 
        // 
        // 
        l_pRootFrame->AddChild(l_pDuck);
        
        // 
        //l_pRootFrame->AddChild(l_pDuck2);
        //l_pDuck2->SetLocalPosition(Vector3(5, 0, 0));
        //cglTFModelRenderNode* l_pToRenderNode1 = l_pDuck2->ToRenderNode();
        int l_iNumCloneNodex = 0;
        for(int i=0;i< l_iNumCloneNodex;i++)
        {
            cglTFModelRenderNode* l_pToRenderNode1 = l_pDuck->ToRenderNode();
            l_pToRenderNode1->SetName(UT::ComposeMsgByFormat(L"Car",ValueToString(i).c_str()));
            l_pToRenderNode1->SetLocalPosition(Vector3(i*2.f, 0, 0) + Vector3(0, 0, 0));
            l_pToRenderNode1->Update(i*0.5f);
            l_pRootFrame->AddChild(l_pToRenderNode1);// 
		}
        //auto l_pLatern = LazyAddModel(l_pRootFrame, "glTFModel/Lantern.gltf");
        //auto l_pWoman = LazyAddModel(l_pRootFrame, "glTFModel/Woman.gltf");
        //l_pWoman->SetLocalPosition(Vector3(-5, 0, 0));
        //auto l_pSimpleSkin = LazyAddModel(l_pRootFrame, "glTFModel/SimpleSkin.gltf");
        //l_pSimpleSkin->SetLocalPosition(Vector3(3, 0, -2));
        //LazyAddModel(l_pRootFrame, "glTFModel/CesiumMilkTruck.gltf");
        //LazyAddModel(l_pRootFrame, "glTFModel/Avocado.gltf");
        //LazyAddModel(l_pRootFrame, "glTFModel/CarConcept.gltf");
        //LazyAddModel(l_pRootFrame, "glTFModel/glTF/ABeautifulGame.gltf");
        //g_glTFModel.LoadFromGLTF("glTFModel/Avocado.gltf", true);
        // 
        //g_glTFModel.LoadFromGLTF("glTFModel/Lantern.gltf",true);
    }
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
    //g_glTFModel.LoadFromGLTF("glTFModel/Buggy.gltf", false);
    //g_glTFModel.LoadFromGLTF("glTFModel/AnimatedCube.gltf", false);
    //g_glTFModel.LoadFromGLTF("glTFModel/BoxAnimated.gltf", false);


    //g_glTFModel.InitBuffers();
    return 1;
}

void GlTFUpdate(float e_fElpaseTime)
{
    float l_fElpaseTime = cGameApp::m_sTimeAndFPS.fElpaseTime;
    l_fElpaseTime = cGameApp::m_sbSpeedControl ? l_fElpaseTime * cGameApp::m_sfGameSpeedValue : l_fElpaseTime;
    g_fLightControllerUpdate(l_fElpaseTime);
    g_fCameraControllerUpdate(l_fElpaseTime);
    if (g_pglTFScene)
    {
		g_pglTFScene->Update(l_fElpaseTime);
        g_SkinningAnimTestClass.Update(l_fElpaseTime);
    }
}

void GlTFRender()
{
    if (g_pglTFScene)
    {
		g_pglTFScene->Render();
        g_SkinningAnimTestClass.Render();
    }
    
}

void GlTFDestory()
{
    //    DrawModel(model, shaderProgram);
    g_SkinningAnimTestClass.Clear();
    SAFE_DELETE(g_pglTFScene);
    cglTFModel::DeleteCachedFiles();
    cTextureManager::ClearSharedTextureReferenceMap();
    //SAFE_DELETE(g_pglTFModel);
}


//cglTFModel* g_pglTFModel = nullptr;
//
//int glTFInit()
//{
//    if (!g_pglTFModel)
//    {
//        g_pglTFModel = new cglTFModel();
//    }
//    cglTFModel& g_glTFModel = *g_pglTFModel;
//    //g_glTFModel.LoadFromGLTF("glTFModel/Duck.gltf",false);
//    //g_glTFModel.LoadFromGLTF("glTFModel/Lantern.gltf",true);
//    // 
//    //g_glTFModel.LoadFromGLTF("glTFModel/Avocado.gltf", true);
//    //g_glTFModel.LoadFromGLTF("glTFModel/CesiumMilkTruck.glb", true);
//    //g_glTFModel.LoadFromGLTF("glTFModel/Fox.gltf", true);
//    //morphing
//    //g_glTFModel.LoadFromGLTF("glTFModel/AnimatedMorphCube.glb", true);
//    //g_glTFModel.LoadFromGLTF("glTFModel/CarConcept.gltf", false);
//    //g_glTFModel.LoadFromGLTF("glTFModel/glTF/ABeautifulGame.gltf", true);
//
//    //g_glTFModel.LoadFromGLTF("glTFModel/SimpleSkin.gltf", true);
//    g_glTFModel.LoadFromGLTF("glTFModel/Woman.gltf", true);
//
//    //g_glTFModel.LoadFromGLTF("glTFModel/Buggy.gltf", false);
//    //g_glTFModel.LoadFromGLTF("glTFModel/AnimatedCube.gltf", false);
//    //g_glTFModel.LoadFromGLTF("glTFModel/BoxAnimated.gltf", false);
//
//
//    //g_glTFModel.InitBuffers();
//    return 1;
//}
//
//void GlTFRender()
//{
//    // Enable backface culling
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    //glCullFace(GL_FRONT);
//    // Enable depth testing
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    cBaseShader* l_pShader = GetCurrentShader();
//    if (l_pShader)
//    {
//        l_pShader->Unuse();
//    }
//    UseShaderProgram(L"qoo79979");
//    float l_fElpaseTime = cGameApp::m_sTimeAndFPS.fElpaseTime;
//    g_fLightControllerUpdate(l_fElpaseTime);
//    g_fCameraControllerUpdate(l_fElpaseTime);
//    if (g_pglTFModel)
//    {
//        g_pglTFModel->Update(0.016f);
//        g_pglTFModel->Render();
//    }
//    glDisable(GL_CULL_FACE);
//    glDisable(GL_DEPTH_TEST);
//    glUseProgram(0);
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    g_fRenderSkeleton(g_pglTFModel);
//    g_fCameraDebugRender();
//
//}
//
//void GlTFDestory()
//{
//    //    DrawModel(model, shaderProgram);
//    cTextureManager::ClearSharedTextureReferenceMap();
//    SAFE_DELETE(g_pglTFModel);
//}
//

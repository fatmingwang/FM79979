#include "pch.h"
#include "glTFModel.h"
#include "GameScene.h"

#include <iostream>
#include <string>
bool g_bWait = true;
// Macro to check for OpenGL errors
#define FORCE_CHECK_GL_ERROR2() \
    do { \
        GLenum err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            std::cerr << "OpenGL Error: " << err << " at line " << __LINE__ << std::endl; \
        } \
    } while(0)

cglTFScene*g_pglTFScene = nullptr;

cglTFModel*LazyAddModel(Frame*e_pFrame,const char*e_strFileName,int e_iInstanceValue = 0)
{
    cglTFModel*l_pglTFModel = new cglTFModel();;
    l_pglTFModel->LoadFromGLTF(e_strFileName, true,e_iInstanceValue);
    return l_pglTFModel;
}

int glTFInit()
{
	g_pglTFScene = new cglTFScene();
    auto l_pRootFrame = g_pglTFScene->GetRootFrame();
    bool l_bDoAnimTexture = false;
    {
        cglTFModel* l_pModel = nullptr;
        l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/glTF/ABeautifulGame.gltf");
        if (l_pModel)
        {
            auto l_pNodeVector = l_pModel->GetNodeVector();
            auto l_pChessBoard = l_pNodeVector->GetObject(L"Chessboard");
            if (l_pChessBoard)
            {
                l_pChessBoard->SetIgnoreShadowPass(true);
            }
        }
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/CarConcept.gltf");
        
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/SpecularTest.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/Mixamo_WalkingGuy.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/glTF/leshark.glb",100);
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/Flamingo.glb");
//        l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/sponza.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/glTF/headtest2__1_.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/glTF/falling_cube.glb");
        
        
        
        
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/Avocado.gltf");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/AnimatedMorphCube.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/fox.gltf");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/fox.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/burger-default_compression-mats_notex.-2glb.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/drone.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/DamagedHelmet.glb");
        
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/FromAdobe/result.gltf");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/MosquitoInAmber.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/Buggy.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/VirtualCity.glb");
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/CesiumMilkTruck.glb");        
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/Duck.gltf");
        if (!l_pModel)
        {
            l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/Woman.gltf", 100);
        }
        //l_pModel = LazyAddModel(l_pRootFrame, "glTFModel/SimpleSkin.gltf");
        if (!l_pModel)
        {
            FMLOG("parse model filed");
            return 0;
        }
        if (l_pModel && l_bDoAnimTexture)
        {
            cSkinningAnimInstanceClass* l_pSkinningAnimTestClassl1 = new cSkinningAnimInstanceClass();
            //l_pSkinningAnimTestClassl1->SetData(l_pModel->GetAnimationInstanceManagerVector(), "PickUp");
            l_pSkinningAnimTestClassl1->SetData(l_pModel->GetAnimationInstanceManagerVector(), "Walking");
            l_pRootFrame->AddChild(l_pSkinningAnimTestClassl1);
            if (1)
            {
                cSkinningAnimInstanceClass* l_pSkinningAnimTestClassl2 = new cSkinningAnimInstanceClass();
                l_pSkinningAnimTestClassl2->SetLocalPosition(Vector3(0, 0, -15));
                l_pSkinningAnimTestClassl2->SetData(l_pModel->GetAnimationInstanceManagerVector(), "Jump2");
                //g_SkinningAnimTestClass.SetData(l_pModel->GetAnimationInstanceManagerVector(), "Idle");
                //g_SkinningAnimTestClass.SetData(l_pModel->GetAnimationInstanceManagerVector(), "");
                l_pRootFrame->AddChild(l_pSkinningAnimTestClassl2);
            }
        }
        {
            
            l_pRootFrame->AddChild(l_pModel);
        }
        if(0)
        {
            auto l_pModel2 = LazyAddModel(l_pRootFrame, "glTFModel/Lantern.gltf");// 
            l_pModel2->SetWorldPosition(Vector3(20, 0, 0));
            l_pRootFrame->AddChild(l_pModel2);
        }        
        int l_iNumCloneNodex = 0;
        for(int i=0;i< l_iNumCloneNodex;i++)
        {
            cglTFModelRenderNode* l_pToRenderNode1 = l_pModel->ToRenderNode();
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
        //LazyAddModel(l_pRootFrame, "glTFModel/CarConcept.gltf");
        //LazyAddModel(l_pRootFrame, "glTFModel/glTF/ABeautifulGame.gltf");
        //g_glTFModel.LoadFromGLTF("glTFModel/Avocado.gltf", true);
        // 
        //g_glTFModel.LoadFromGLTF("glTFModel/Lantern.gltf",true);
    }
    //g_glTFModel.LoadFromGLTF("glTFModel/Duck.gltf",false);
    //g_glTFModel.LoadFromGLTF("glTFModel/Lantern.gltf",true);
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
    g_bWait = false;
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
    }
}

void GlTFRender()
{
    if (g_pglTFScene && !g_bWait)
    {
		g_pglTFScene->Render();
    }
    
}

void GlTFDestory()
{
    SAFE_DELETE(g_pglTFScene);
    cglTFModel::DeleteCachedFiles();
    cCameraController::DestroyInstance();
    cLighController::DestroyInstance();
    cTextureManager::ClearSharedTextureReferenceMap();
}

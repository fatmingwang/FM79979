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

class GLTestDrawElements
{
    public:
    GLTestDrawElements()
    {
        FMLOG("GLTestDrawElements start");
        // Vertex Shader source (GLSL ES 3.00)
        const char* vertexShaderSource = R"(#version 300 es
            in vec3 aPos;
            void main() {
                gl_Position = vec4(aPos, 1.0);
            }
        )";

        // Fragment Shader source (GLSL ES 3.00)
        const char* fragmentShaderSource = R"(#version 300 es
            precision mediump float;
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            }
        )";

        // Compile vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        FORCE_CHECK_GL_ERROR2();
        glCompileShader(vertexShader);
        FORCE_CHECK_GL_ERROR2();

        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "Vertex Shader Compilation Failed: " << infoLog << std::endl;
        }

        // Compile fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        FORCE_CHECK_GL_ERROR2();
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        FORCE_CHECK_GL_ERROR2();
        glCompileShader(fragmentShader);
        FORCE_CHECK_GL_ERROR2();

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "Fragment Shader Compilation Failed: " << infoLog << std::endl;
        }

        // Link shader program
        shaderProgram = glCreateProgram();
        FORCE_CHECK_GL_ERROR2();
        glAttachShader(shaderProgram, vertexShader);
        FORCE_CHECK_GL_ERROR2();
        glAttachShader(shaderProgram, fragmentShader);
        FORCE_CHECK_GL_ERROR2();
        glLinkProgram(shaderProgram);
        FORCE_CHECK_GL_ERROR2();

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cerr << "Shader Program Linking Failed: " << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        FORCE_CHECK_GL_ERROR2();
        glDeleteShader(fragmentShader);
        FORCE_CHECK_GL_ERROR2();

        // Vertex data for a square (two triangles)
        float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
           -0.5f, -0.5f, 0.0f,  // bottom left
           -0.5f,  0.5f, 0.0f   // top left 
        };

        unsigned int indices[] = {
            0, 1, 3,  // first triangle
            1, 2, 3   // second triangle
        };

        // Setup VAO, VBO, EBO
        glGenVertexArrays(1, &VAO);
        FORCE_CHECK_GL_ERROR2();
        glGenBuffers(1, &VBO);
        FORCE_CHECK_GL_ERROR2();
        glGenBuffers(1, &EBO);
        FORCE_CHECK_GL_ERROR2();

        glBindVertexArray(VAO);
        FORCE_CHECK_GL_ERROR2();

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        FORCE_CHECK_GL_ERROR2();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        FORCE_CHECK_GL_ERROR2();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        FORCE_CHECK_GL_ERROR2();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        FORCE_CHECK_GL_ERROR2();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        FORCE_CHECK_GL_ERROR2();
        glEnableVertexAttribArray(0);
        FORCE_CHECK_GL_ERROR2();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        FORCE_CHECK_GL_ERROR2();
        glBindVertexArray(0);
        FORCE_CHECK_GL_ERROR2();
    }

    ~GLTestDrawElements()
    {
        glDeleteVertexArrays(1, &VAO);
        FORCE_CHECK_GL_ERROR2();
        glDeleteBuffers(1, &VBO);
        FORCE_CHECK_GL_ERROR2();
        glDeleteBuffers(1, &EBO);
        FORCE_CHECK_GL_ERROR2();
        glDeleteProgram(shaderProgram);
        FORCE_CHECK_GL_ERROR2();
    }

    void render()
    {
        glUseProgram(shaderProgram);
        FORCE_CHECK_GL_ERROR2();
        glBindVertexArray(VAO);
        FORCE_CHECK_GL_ERROR2();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        FORCE_CHECK_GL_ERROR2();
        glBindVertexArray(0);
        FORCE_CHECK_GL_ERROR2();
        //FMLOG("GLTestDrawElements Render");
    }

    private:
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
    unsigned int vertexShader, fragmentShader;
};
cglTFScene*g_pglTFScene = nullptr;

cglTFModel*LazyAddModel(Frame*e_pFrame,const char*e_strFileName,int e_iInstanceValue = 0)
{
    cglTFModel*l_pglTFModel = new cglTFModel();;
    l_pglTFModel->LoadFromGLTF(e_strFileName, true,e_iInstanceValue);
    return l_pglTFModel;
}
//GLTestDrawElements* renderer;
int glTFInit()
{
    //renderer = new GLTestDrawElements();
    FMLOG("check error");
	g_pglTFScene = new cglTFScene();
    auto l_pRootFrame = g_pglTFScene->GetRootFrame();
    bool l_bDoAnimTexture = true;
    {
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/SpecularTest.glb");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/Avocado.gltf");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/AnimatedMorphCube.glb");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/fox.gltf");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/MosquitoInAmber.glb");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/Buggy.glb");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/VirtualCity.glb");
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/CesiumMilkTruck.glb");        
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/Duck.gltf");
        auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/Woman.gltf",1000);
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/SimpleSkin.gltf");
        if (!l_pDuck)
        {
            FMLOG("parse model filed");
            return 0;
        }
        if (l_pDuck && l_bDoAnimTexture)
        {
            cSkinningAnimTestClass* l_pSkinningAnimTestClassl1 = new cSkinningAnimTestClass();
            l_pSkinningAnimTestClassl1->SetData(l_pDuck->GetAnimationInstanceManagerVector(), "PickUp");
            cSkinningAnimTestClass* l_pSkinningAnimTestClassl2 = new cSkinningAnimTestClass();
            l_pSkinningAnimTestClassl2->SetLocalPosition(Vector3(0, 0, -15));
            l_pSkinningAnimTestClassl2->SetData(l_pDuck->GetAnimationInstanceManagerVector(), "Jump2");
            //g_SkinningAnimTestClass.SetData(l_pDuck->GetAnimationInstanceManagerVector(), "Idle");
            //g_SkinningAnimTestClass.SetData(l_pDuck->GetAnimationInstanceManagerVector(), "");
            l_pRootFrame->AddChild(l_pSkinningAnimTestClassl1);
            l_pRootFrame->AddChild(l_pSkinningAnimTestClassl2);
        }
        {
            
            l_pRootFrame->AddChild(l_pDuck);
        }
        if(0)
        {
            auto l_pDuck2 = LazyAddModel(l_pRootFrame, "glTFModel/Lantern.gltf");// 
            l_pDuck2->SetWorldPosition(Vector3(20, 0, 0));
            l_pRootFrame->AddChild(l_pDuck2);
        }
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/glTF/ABeautifulGame.gltf",1);
        //auto l_pDuck = LazyAddModel(l_pRootFrame, "glTFModel/SimpleInstancing.glb");// 
        // 
        // 
        //l_pRootFrame->AddChild(l_pDuck);
        
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
    //renderer->render();;
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

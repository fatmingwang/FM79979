#include "tiny_gltf.h"
#include "glTFCamera.h"
#include "glTFNode.h"
#include "glTFModel.h"


TYPDE_DEFINE_MARCO(cCameraFrameData);
TYPDE_DEFINE_MARCO(cCameraController);
TYPDE_DEFINE_MARCO(cglTFCamera);

cglTFCamera::~cglTFCamera()
{
    cCameraController::GetInstance()->RemoveglTFCamera(this);
}

void cglTFCamera::LoadCamerasFromGLTF(const tinygltf::Model& model, std::map<int,cglTFNodeData*>* e_pNideIndexAndNodeDataMap)
{
    cCameraController::GetInstance()->AddglTFCamera(this);
    m_CameraVector.clear();

    for (const auto& gltfCam : model.cameras)
    {
        sCamera cam;
        cam.name = gltfCam.name;
        cam.znear = 0.0f;
        cam.zfar = 0.0f;
        cam.aspectRatio = 0.0f;
        cam.yfov = 0.0f;
        cam.xmag = 0.0f;
        cam.ymag = 0.0f;

        if (gltfCam.type == "perspective")
        {
            cam.type = eCameraType::PERSPECTIVE;
            cam.znear = static_cast<float>(gltfCam.perspective.znear);
            cam.zfar = static_cast<float>(gltfCam.perspective.zfar);
            cam.aspectRatio = static_cast<float>(gltfCam.perspective.aspectRatio);
            cam.yfov = static_cast<float>(gltfCam.perspective.yfov);
        }
        else if (gltfCam.type == "orthographic")
        {
            cam.type = eCameraType::ORTHOGRAPHIC;
            cam.znear = static_cast<float>(gltfCam.orthographic.znear);
            cam.zfar = static_cast<float>(gltfCam.orthographic.zfar);
            cam.xmag = static_cast<float>(gltfCam.orthographic.xmag);
            cam.ymag = static_cast<float>(gltfCam.orthographic.ymag);
        }
        else
        {
            cam.type = eCameraType::UNKNOWN;
        }

        m_CameraVector.push_back(std::move(cam));
    }
    // Link cameras to nodes and compute transform
    for (size_t nodeIdx = 0; nodeIdx < model.nodes.size(); ++nodeIdx)
    {
        const auto& node = model.nodes[nodeIdx];
        if (node.camera >= 0 && node.camera < static_cast<int>(m_CameraVector.size()))
        {
            m_CameraVector[node.camera].nodeIndex = static_cast<int>(nodeIdx);
            if (e_pNideIndexAndNodeDataMap)
            {
                auto l_IT = e_pNideIndexAndNodeDataMap->find(m_CameraVector[node.camera].nodeIndex);
				if (l_IT != e_pNideIndexAndNodeDataMap->end())
				{
					cglTFNodeData* pNodeData = l_IT->second;
					if (pNodeData)
					{
						m_CameraVector[node.camera].m_StartNodeTransform = pNodeData->m_StartNodeTransform;
                        if (m_CameraVector[node.camera].nodeIndex != -1)
                        {
                            m_CameraVector[node.camera].m_pglTFNodeData = pNodeData;
                        }
					}
				}
            }
        }
    }
}


size_t cglTFCamera::GetCameraCount() const
{
    return m_CameraVector.size();
}

const cglTFCamera::sCamera* cglTFCamera::GetCameraByIndex(size_t index) const
{
    if (index >= m_CameraVector.size()) return nullptr;
    return &m_CameraVector[index];
}

const cglTFCamera::sCamera* cglTFCamera::GetCameraByName(const std::string& name) const
{
    for (const auto& cam : m_CameraVector)
    {
        if (cam.name == name)
            return &cam;
    }
    return nullptr;
}

bool cglTFCamera::IsCameraExists(const tinygltf::Model& model)
{
	return model.cameras.size() > 0?true:false;
}

cCameraFrameData::cCameraFrameData(cglTFCamera::sCamera e_CameraData)
{
    // Initialize m_Camera as a shared pointer to a new cFrameCamera instance
    m_Camera = std::make_shared<cFrameCamera>();

    // Configure the projection based on the camera type
    Projection proj;
    if (e_CameraData.type == cglTFCamera::eCameraType::PERSPECTIVE)
    {
        // Set perspective projection using field of view, aspect ratio, near, and far planes
        proj.SetFovYAspect(e_CameraData.yfov, e_CameraData.aspectRatio, e_CameraData.znear, e_CameraData.zfar);
    }
    else if (e_CameraData.type == cglTFCamera::eCameraType::ORTHOGRAPHIC)
    {
        // Set orthographic projection using width, height, near, and far planes
        float width = e_CameraData.xmag * 2.0f; // Assuming xmag represents half-width
        float height = e_CameraData.ymag * 2.0f; // Assuming ymag represents half-height
        proj.SetOrthographic(width, height, e_CameraData.znear, e_CameraData.zfar);
    }

    // Apply the configured projection to the camera
    m_Camera->SetProjection(proj);

    // Set the initial transform of the camera
    m_Camera->SetLocalTransform(e_CameraData.m_StartNodeTransform);
}

cCameraController::cCameraController()
{
    m_pCameraBehaveByMouseBehave = new cCameraBehaveByMouseBehave(nullptr);
}
cCameraController::~cCameraController()
{
    SAFE_DELETE(m_pCameraBehaveByMouseBehave);
    m_CameraVector.clear();
    m_NameAndglTFCameraMap.clear();
}

bool cCameraController::AddglTFCamera(cglTFCamera* e_pglTFCamera)
{
    m_NameAndglTFCameraMap[e_pglTFCamera->GetCharName()] = e_pglTFCamera;
    return false;
}

bool cCameraController::RemoveglTFCamera(cglTFCamera* e_pglTFCamera)
{
	m_NameAndglTFCameraMap.erase(e_pglTFCamera->GetCharName());
    return false;
}

bool cCameraController::AddCamera(std::shared_ptr<cFrameCamera> camera)
{
    if (!camera)
    {
        return false;
    }

    // Check if the camera already exists (pointer comparison)
    for (const auto& cam : m_CameraVector)
    {
        if (cam == camera)
        {
            // Already exists, do not add again
            return false;
        }
    }

    m_CameraVector.push_back(camera);

    if (m_CurrentCameraIndex == -1)
    {
        m_CurrentCameraIndex = 0;
    }
    return true;
}

void cCameraController::RemoveCamera(std::shared_ptr<cFrameCamera> camera)
{
    auto it = std::remove(m_CameraVector.begin(), m_CameraVector.end(), camera);
    if (it != m_CameraVector.end())
    {
        m_CameraVector.erase(it, m_CameraVector.end());
    }
    if (m_CameraVector.empty())
    {
        m_CurrentCameraIndex = -1;
    }
    else
    if (m_CurrentCameraIndex >= (int)m_CameraVector.size())
    {
        m_CurrentCameraIndex = 0;
    }
}

void cCameraController::ClearCameras()
{
    if (m_pCameraBehaveByMouseBehave)
    {
        m_pCameraBehaveByMouseBehave->SetCamera(nullptr);
    }
    m_CameraVector.clear();
    m_CurrentCameraIndex = -1;
}

size_t cCameraController::GetCameraCount() const
{
    return m_CameraVector.size();
}

std::shared_ptr<cFrameCamera> cCameraController::GetCamera(size_t idx) const
{
    if (idx < m_CameraVector.size())
    {
        return m_CameraVector[idx];
    }
    return nullptr;
}

std::shared_ptr<cFrameCamera> cCameraController::GetCurrentCamera() const
{
    if (m_CurrentCameraIndex >= 0 && m_CurrentCameraIndex < (int)m_CameraVector.size())
    {
        return m_CameraVector[m_CurrentCameraIndex];
    }
    return nullptr;
}

bool cCameraController::SwitchCamera(size_t idx)
{
    if (idx < m_CameraVector.size())
    {
        m_CurrentCameraIndex = (int)idx;
        if (this->m_bEnableCotrolCameraByMouse)
        {
            if (m_pCameraBehaveByMouseBehave)
            {
				m_pCameraBehaveByMouseBehave->SetCamera(m_CameraVector[m_CurrentCameraIndex].get());
            }
        }
        return true;
    }
    if (m_pCameraBehaveByMouseBehave)
    {
        m_pCameraBehaveByMouseBehave->SetCamera(nullptr);
    }
    return false;
}

bool cCameraController::SwitchCamera(std::shared_ptr<cFrameCamera> camera)
{
    for (size_t i = 0; i < m_CameraVector.size(); ++i)
    {
        if (m_CameraVector[i] == camera)
        {
            return SwitchCamera(i);
        }
    }
    return false;
}


void cCameraController::Update(float e_fElpaseTime)
{
    if (m_bEnableCotrolCameraByMouse)
    {
        // Assumes cGameApp::m_sbMouseClickStatus is accessible and of type sMouseState
        if (m_pCameraBehaveByMouseBehave)
        {
            if (cGameApp::m_sucKeyData['R'])
            {
                auto l_pCamera = this->GetCurrentCamera();
                if (l_pCamera)
                {
                    l_pCamera->SetLocalPosition(Vector3(0, 8, 22));
                    l_pCamera->SetLocalRotation(Vector3(0, 0, 0));
                    m_pCameraBehaveByMouseBehave->SetAngleX(0);
                    m_pCameraBehaveByMouseBehave->SetAngleY(0);
                }
            }
            else
            if (cGameApp::m_sucKeyData[107])//+
            {
                m_pCameraBehaveByMouseBehave->SetMouseMoveSpeed(m_pCameraBehaveByMouseBehave->GetMouseMoveSpeed() * 1.1f);
            }
            else
            if (cGameApp::m_sucKeyData[109])//-
            {
                m_pCameraBehaveByMouseBehave->SetMouseMoveSpeed(m_pCameraBehaveByMouseBehave->GetMouseMoveSpeed() / 1.1f);
            }
            m_pCameraBehaveByMouseBehave->CameraUpDateByMouse(cGameApp::m_sbMouseClickStatus[0], cGameApp::m_sbMouseClickStatus[1], cGameApp::m_sMouseWhellDelta,
                                                              cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y, e_fElpaseTime);
        }
    }
    auto l_pCamera = GetCurrentCamera();
    if (!l_pCamera)
    {
        this->CreateDefault3DCamera();
        l_pCamera = GetCurrentCamera();
    }
    if (m_NameAndglTFCameraMap.size())
    {
        //only support cglTFModel not clone node
        if (cGameApp::m_sucKeyUpData['T'])
        {
            cGameApp::m_sucKeyUpData['T'] = false;
            ++m_iDefaultModelCameraIndex;
            auto l_IT = m_NameAndglTFCameraMap.begin();
            if (m_iDefaultModelCameraIndex >= l_IT->second->m_CameraVector.size())
            {
                m_iDefaultModelCameraIndex = -1;
            }
        }
        if (m_iDefaultModelCameraIndex != -1)
        {
            auto l_IT = m_NameAndglTFCameraMap.begin();
            if (l_IT->second->m_CameraVector.size())
            {
                if (l_IT->second->m_CameraVector[m_iDefaultModelCameraIndex].m_pglTFNodeData)
                {
                    auto l_mat = l_IT->second->m_CameraVector[m_iDefaultModelCameraIndex].m_pglTFNodeData->GetWorldTransform();
                    l_pCamera->SetLocalTransform(l_mat);
                }
            }
        }
    }
}

void cCameraController::Render(GLuint e_uiProgramID, float* e_pMatrix)
{
    if (m_iLastUsedProgram == e_uiProgramID)
    {
		//return; // No need to re-render if the same program is used
    }
	m_iLastUsedProgram = e_uiProgramID;
    auto l_pCamera = GetCurrentCamera();
    if (l_pCamera)
    {
        auto l_Matrix = l_pCamera->GetWorldViewglTFProjection();
        // Set model, view, projection matrices
        GLuint viewLoc = glGetUniformLocation(e_uiProgramID, "inMat4View");
        GLuint projLoc = glGetUniformLocation(e_uiProgramID, "inMat4Projection");
        //cMatrix44 viewMatrix = cMatrix44::LookAtMatrix(Vector3(0, -0, l_fCameraZPosition), Vector3(0, 0, 0), Vector3(0, 1, 0));
		cMatrix44 viewMatrix = l_pCamera->GetWorldView();
        if(e_pMatrix)
        {
            viewMatrix = e_pMatrix;
		}
        auto l_Proj = l_pCamera->GetProjection();
        auto l_matProjectionMatrix = l_Proj.GetMatrix();// .GetglTFPerspectiveRH();
        //auto l_matProjectionMatrix = projectionMatrix.GetglTFPerspectiveRH();
        //auto l_matProjectionMatrix2 = projectionMatrix.GetMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, l_matProjectionMatrix);

    }
}

void cCameraController::RenderGrid()
{
    auto l_pCamera = GetCurrentCamera();
    if (l_pCamera)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
		UseShaderProgram();
        l_pCamera->Render();
        cMatrix44	l_mat = cMatrix44::XAxisRotationMatrix(D3DXToRadian(90));
        RenderPlane(cMatrix44::TranslationMatrix(Vector3(0, 0, 0)) * l_mat, 10, 10, 1, 1, 1, Vector4(0.5, 1, 0, 1));
        auto l_vPos = l_pCamera->GetWorldPosition();
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable2D(1920, 1080);
        std::wstring l_strCameraNodeName;
        if (m_iDefaultModelCameraIndex != -1)
        {
            auto l_IT = m_NameAndglTFCameraMap.begin();
            if (l_IT->second->m_CameraVector.size())
            {
                l_strCameraNodeName = l_IT->second->m_CameraVector[m_iDefaultModelCameraIndex].m_pglTFNodeData->GetName();
            }
        }
        cGameApp::RenderFont(1000, 100, UT::ComposeMsgByFormat(L"Camera Pos: %.2f,%.2f,%.2f\nModelCameraIndex:%d %s", l_vPos.x, l_vPos.y, l_vPos.z, m_iDefaultModelCameraIndex, l_strCameraNodeName.c_str()).c_str());
        cBaseShader* l_pShader = GetCurrentShader();
        if (l_pShader)
        {
            l_pShader->Unuse();
        }
	}
}

void cCameraController::CreateDefault3DCamera(bool e_bEnableControleByMouse)
{
    if (m_CameraVector.empty())
    {
        m_CameraVector.push_back(std::make_shared<cFrameCamera>());
        m_CurrentCameraIndex = 0;
        auto l_pCamera = GetCurrentCamera();
        if (l_pCamera)
        {
            Projection proj;
            //proj.SetFovYAspect(45.0f, 16.f/9.f, 0.1f, 100.0f);
            //proj.SetFovYAspect(60.0f, 16.f / 9.f, 0.1f, 100.0f);
            //projectionMatrix.SetFovYAspect(XM_PIDIV4, (float)1920 / (float)1080, 0.1f, 10000.0f);
            //projectionMatrix.SetFovYAspect(XM_PIDIV4, 4/3, 0.1f, 10000.0f);
            //projectionMatrix.SetFovYAspect(XM_PIDIV4*2, 21 / 9, 0.1f, 10000.0f);
            proj.SetFovYAspect(XM_PIDIV4, (float)1920 / (float)1080, 0.1f, 10000.0f);
            l_pCamera->SetProjection(proj);
            l_pCamera->SetLocalTransform(cMatrix44::Identity); // Identity transform
            if (1)
            {
                l_pCamera->SetLocalPosition(Vector3(1, 4, 10));
            }
        }
        this->m_bEnableCotrolCameraByMouse = e_bEnableControleByMouse;
        this->SwitchCamera(m_CurrentCameraIndex);
    }
}

int cCameraController::GetCurrentCameraIndex() const
{
    return m_CurrentCameraIndex;
}

void cCameraFrameData::Update(float e_fElpaseTime)
{

}

void cCameraFrameData::Render()
{
    if(!this->m_bAddIntocCameraController)
    {
		this->m_bAddIntocCameraController = true;
		cCameraController::GetInstance()->AddCamera(m_Camera);
    }
    if(this->m_bDoUseThisCamera)
    {
        m_iOriginalCameraIndex = cCameraController::GetInstance()->GetCurrentCameraIndex();
        cCameraController::GetInstance()->SwitchCamera(m_Camera);
    }
}
void cCameraFrameData::EndRender()
{
    if(m_iOriginalCameraIndex != -1)
    {
        cCameraController::GetInstance()->SwitchCamera(m_iOriginalCameraIndex);
	}
}

void g_fSetCameraUniform(GLuint e_uiProgramID, float* e_pMatrix)
{
    cCameraController::GetInstance()->Render(e_uiProgramID,e_pMatrix);
}

void g_fCameraDebugRender()
{
    cCameraController::GetInstance()->RenderGrid();
}

void g_fCameraControllerUpdate(float e_fElpaseTime)
{
	cCameraController::GetInstance()->Update(e_fElpaseTime);
}
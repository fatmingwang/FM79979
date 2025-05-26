#include "tiny_gltf.h"
#include "glTFCamera.h"
#include "glTFNode.h"



TYPDE_DEFINE_MARCO(cCameraFrameData);
TYPDE_DEFINE_MARCO(cCameraController);
TYPDE_DEFINE_MARCO(cglTFCamera);

void cglTFCamera::LoadCamerasFromGLTF(const tinygltf::Model& model, std::map<int,cglTFNodeData*>* e_pNideIndexAndNodeDataMap)
{
    cameras.clear();

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

        cameras.push_back(std::move(cam));
    }
    // Link cameras to nodes and compute transform
    for (size_t nodeIdx = 0; nodeIdx < model.nodes.size(); ++nodeIdx)
    {
        const auto& node = model.nodes[nodeIdx];
        if (node.camera >= 0 && node.camera < static_cast<int>(cameras.size()))
        {
            cameras[node.camera].nodeIndex = static_cast<int>(nodeIdx);
            if (e_pNideIndexAndNodeDataMap)
            {
                auto l_IT = e_pNideIndexAndNodeDataMap->find(cameras[node.camera].nodeIndex);
				if (l_IT != e_pNideIndexAndNodeDataMap->end())
				{
					cglTFNodeData* pNodeData = l_IT->second;
					if (pNodeData)
					{
						cameras[node.camera].m_StartNodeTransform = pNodeData->m_StartNodeTransform;
					}
				}
            }
        }
    }
}

size_t cglTFCamera::GetCameraCount() const
{
    return cameras.size();
}

const cglTFCamera::sCamera* cglTFCamera::GetCameraByIndex(size_t index) const
{
    if (index >= cameras.size()) return nullptr;
    return &cameras[index];
}

const cglTFCamera::sCamera* cglTFCamera::GetCameraByName(const std::string& name) const
{
    for (const auto& cam : cameras)
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
}
cCameraController::~cCameraController()
{
    m_CameraVector.clear();
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
        return true;
    }
    return false;
}

bool cCameraController::SwitchCamera(std::shared_ptr<cFrameCamera> camera)
{
    for (size_t i = 0; i < m_CameraVector.size(); ++i)
    {
        if (m_CameraVector[i] == camera)
        {
            m_CurrentCameraIndex = (int)i;
            return true;
        }
    }
    return false;
}

void cCameraController::Render(GLuint e_uiProgramID, float* e_pMatrix)
{
    if (m_iLastUsedProgram == e_uiProgramID)
    {
		//return; // No need to re-render if the same program is used
    }
	m_iLastUsedProgram = e_uiProgramID;
    auto l_pCamera = GetCurrentCamera();
    if(!l_pCamera)
    {
        this->CreateDefault3DCamera();
        l_pCamera = GetCurrentCamera();
    }
    if (l_pCamera)
    {
        auto l_Matrix = l_pCamera->GetWorldViewglTFProjection();
        // Set model, view, projection matrices
        GLuint viewLoc = glGetUniformLocation(e_uiProgramID, "inMat4View");
        GLuint projLoc = glGetUniformLocation(e_uiProgramID, "inMat4Projection");
        //cMatrix44 viewMatrix = cMatrix44::LookAtMatrix(Vector3(0, -0, l_fCameraZPosition), Vector3(0, 0, 0), Vector3(0, 1, 0));
		cMatrix44 viewMatrix = l_pCamera->GetWorldTransform();
        if(e_pMatrix)
        {
            viewMatrix = e_pMatrix;
		}
        else
        {
            //lazy for now.
            viewMatrix.GetTranslation().z *= -1;
        }
        Projection projectionMatrix;
		//180/4 = 45 degree FOV
		//radians = 45 * (XM_PI / 180.0f);
        projectionMatrix.SetFovYAspect(XM_PIDIV4, (float)1920 / (float)1080, 0.1f, 10000.0f);
        auto l_matProjectionMatrix = projectionMatrix.GetglTFPerspectiveRH();
        auto l_matProjectionMatrix2 = projectionMatrix.GetMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, l_matProjectionMatrix);

    }
}

void cCameraController::CreateDefault3DCamera()
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
            proj.SetFovYAspect(60.0f, 16.f / 9.f, 0.1f, 100.0f);
            l_pCamera->SetProjection(proj);
            l_pCamera->SetLocalTransform(cMatrix44::Identity); // Identity transform
        }
}
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
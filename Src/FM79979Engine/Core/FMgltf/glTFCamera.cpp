#include "tiny_gltf.h"
#include "glTFCamera.h"
#include "glTFNode.h"

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

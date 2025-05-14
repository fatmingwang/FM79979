#pragma once

#pragma once
#include <string>
#include <vector>
#include "../AllCoreInclude.h"

class cglTFCamera:public NamedTypedObject
{
    public:
    DEFINE_TYPE_INFO();
    enum class eCameraType
    {
        PERSPECTIVE,
        ORTHOGRAPHIC,
        UNKNOWN
    };

    struct sCamera
    {
        std::string name;
        eCameraType type;
        float       yfov;        // Perspective
        float       aspectRatio; // Perspective
        float       znear;
        float       zfar;
        float       xmag;        // Orthographic
        float       ymag;        // Orthographic
        int         nodeIndex = -1;
        cMatrix44   m_StartNodeTransform = cMatrix44::Identity;
    };

    // List of cameras loaded from glTF
    std::vector<sCamera> cameras;

    // Load all camera data from glTF model
    void LoadCamerasFromGLTF(const tinygltf::Model& model, std::map<int, class cglTFNodeData*>*e_pNideIndexAndNodeDataMap);
    // Accessors
    size_t GetCameraCount() const;
    const sCamera* GetCameraByIndex(size_t index) const;
    const sCamera* GetCameraByName(const std::string& name) const;
    static bool    IsCameraExists(const tinygltf::Model& model);
};



// Controller for managing multiple cameras
class cCameraController : public NamedTypedObject, public cSingltonTemplate<cCameraController>
{
    std::shared_ptr<cFrameCamera> m_Camera;
    cCameraController();
    virtual ~cCameraController();
public:
    DEFINE_TYPE_INFO();
    SINGLETON_BASIC_FUNCTION(cCameraController);
    void SetCamera(std::shared_ptr<cFrameCamera> e_CameraData)
    {
        m_Camera = e_CameraData;
	}
    void Render(GLuint e_uiProgramID);
};

// Frame-specific camera data
class cCameraFrameData : public Frame
{
    std::shared_ptr<cFrameCamera> m_Camera;
public:
    DEFINE_TYPE_INFO();
    cCameraFrameData(cglTFCamera::sCamera e_CameraData);
    virtual ~cCameraFrameData(){}
    virtual void Render() override;
    virtual void EndRender() override;
    std::shared_ptr<cFrameCamera> GetCameraData()
    {
        return m_Camera;
    }
};
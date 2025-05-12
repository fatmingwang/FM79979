#pragma once

#pragma once
#include <string>
#include <vector>
#include "../AllCoreInclude.h"

class cglTFCamera
{
    public:
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

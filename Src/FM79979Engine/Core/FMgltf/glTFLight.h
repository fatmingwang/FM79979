#pragma once
#include <string>
#include "../AllCoreInclude.h"
enum class eLightType
{
    eLT_DIRECTIONAL = 0,
    eLT_POINT,
    eLT_SPOT,
    eLT_MAX
};

class cglTFLight:public NamedTypedObject
{
public:
    // Light type enumeration
    enum class LightType
    {
        Directional,
        Point,
        Spot,
        Unknown
    };

    // Structure to hold light properties
    struct sLightData
    {
        eLightType  m_eType;
        float       m_fIntensity = 1.0f;
        float       m_fRange = 0.0f;         // Only for point and spot
        float       m_fInnerConeAngle = 0.0f; // Only for spot
        float       m_fOuterConeAngle = 0.0f;
        Vector3     m_vPosition = Vector3::Zero;
        Vector3     m_vDirection = Vector3(0.0f, 0.0f, -1.0f); // Default down
        Vector3     m_vColor = Vector3(1.0f, 1.f, 1.f);
    };
private:
        std::vector< sLightData> m_LightDataVector;
public:
    void        LoadLightsFromGLTF(const tinygltf::Model& model);
	void        CreateDefaulights();
    const std::vector<sLightData>& GetLights() const;
};
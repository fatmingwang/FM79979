#include "tiny_gltf.h"
#include "glTFLight.h"

TYPDE_DEFINE_MARCO(cglTFLight);


//"extensions": {
//    "KHR_lights_punctual": {
//        "lights": [
//        {
//            "type": "point",
//                "color" : [1.0, 1.0, 1.0] ,
//                "intensity" : 10.0,
//                "range" : 5.0,
//                "name" : "MyLight"
//        }
//        ]
//    }
//}

void cglTFLight::LoadLightsFromGLTF(const tinygltf::Model& model)
{
    const auto& extLights = model.extensions.find("KHR_lights_punctual");
    if (extLights == model.extensions.end()) return;

    const auto& lightsArray = extLights->second.Get("lights");
    for (size_t i = 0; i < lightsArray.ArrayLen(); ++i)
    {
        const tinygltf::Value& lightDef = lightsArray.Get(i);

        sLightData light;
        std::string l_strName = lightDef.Get("name").Get<std::string>();
        if (lightDef.Has("type"))
        {
            std::string type = lightDef.Get("type").Get<std::string>();
            if (type == "directional")
            {
                light.m_eType = eLightType::eLT_DIRECTIONAL;
            }
            else
            if (type == "spot")
            {
                light.m_eType = eLightType::eLT_SPOT;
            }
            else
            if (type == "point")
            {
                light.m_eType = eLightType::eLT_POINT;
            }
            else
            {
                light.m_eType = eLightType::eLT_POINT;
            }
        }

        if (lightDef.Has("color"))
        {
			float l_fColor[3] = { 1.f,1.f,1.f };
            for (int j = 0; j < 3; ++j)
            {
                l_fColor[j] = (float)lightDef.Get("color").Get(j).Get<double>();
            }
			light.m_vColor = Vector3(l_fColor[0], l_fColor[1], l_fColor[2]);
        }
        if (lightDef.Has("intensity"))
        {
            light.m_fIntensity = (float)lightDef.Get("intensity").Get<double>();
        }

        if (lightDef.Has("range"))
        {
            light.m_fRange = (float)lightDef.Get("range").Get<double>();
        }

        if (light.m_eType == eLightType::eLT_SPOT)
        {
            auto& spot = lightDef.Get("spot");
            if (spot.Has("innerConeAngle"))
            {
                light.m_fInnerConeAngle = (float)spot.Get("innerConeAngle").Get<double>();
            }
                
            if (spot.Has("outerConeAngle"))
            {
                light.m_fOuterConeAngle = (float)spot.Get("outerConeAngle").Get<double>();
            }
        }
        m_LightDataVector.push_back(light);
    }

    // Position/direction come from node transforms
    for (const auto& node : model.nodes)
    {
        if (!node.extensions.count("KHR_lights_punctual")) continue;
        int lightIndex = node.extensions.at("KHR_lights_punctual").Get("light").Get<int>();
        if (lightIndex >= 0 && lightIndex < m_LightDataVector.size())
        {
            cMatrix44 l_Mat = cMatrix44::Identity;
            if (node.matrix.size() == 16)
            {
                l_Mat = cMatrix44(&node.matrix[0]);
            }
            else
            {
                Vector3 l_vTranslation = Vector3::Zero;
                Quaternion l_vRotation = Quaternion::Identity;
                Vector3 l_vScale = Vector3::One;
                if (node.translation.size() == 3)
                {
                    l_vTranslation = Vector3((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
                }
                if (node.rotation.size() == 4)
                {
                    l_vRotation = Quaternion((float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]);
                }
                if (node.scale.size() == 3)
                {
                    l_vScale = Vector3((float)node.scale[0], (float)node.scale[1], (float)node.scale[2]);
                }
				l_Mat = cMatrix44::TranslationMatrix(l_vTranslation)*l_vRotation.ToMatrix()* cMatrix44::ScaleMatrix(l_vScale);
            }
            // You must convert node.translation and rotation to world-space
            if (node.translation.size() == 3)
            {
                m_LightDataVector[lightIndex].m_vPosition = Vector3((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
                m_LightDataVector[lightIndex].m_vDirection = Vector3(&l_Mat.m[2][0]);
            }
        }
    }
    if (m_LightDataVector.size() == 0)
    {
        CreateDefaulights();
    }
}

void cglTFLight::CreateDefaulights()
{
	sLightData light;
	light.m_eType = eLightType::eLT_DIRECTIONAL;
	light.m_fIntensity = 1.0f;
	light.m_fRange = 0.0f;
	light.m_fInnerConeAngle = 0.0f;
	light.m_fOuterConeAngle = 0.0f;
	light.m_vPosition = Vector3(0.f, 0.f, 0.f);
	light.m_vDirection = Vector3(0.f, -1.f, 0.f);
	light.m_vColor = Vector3(1.f, 1.f, 1.f);
	m_LightDataVector.push_back(light);
}

const std::vector<sLightData>& cLighCollector::GetLights() const
{
        return m_LightDataVector;
}

void cLighCollector::SetLight(int e_iIndex, sLightData e_sLightData)
{
	if (e_iIndex >= 0 && e_iIndex < m_LightDataVector.size())
	{
		m_LightDataVector[e_iIndex] = e_sLightData;
	}
}

void cLighCollector::AddLight(sLightData e_sLightData)
{
	m_LightDataVector.push_back(e_sLightData);
}

bool cglTFLight::IsLightExists(const tinygltf::Model& model)
{
    const auto& extLights = model.extensions.find("KHR_lights_punctual");
    if (extLights == model.extensions.end())
    {
        return false;
    }

    const auto& lightsArray = extLights->second.Get("lights");
    if (lightsArray.ArrayLen())
    {
        return true;
    }
    return false;
}

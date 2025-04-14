#include "tiny_gltf.h"
#include "glTFLight.h"


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

void cglTFLight::LoadLightsFromGLTF(const tinygltf::Model& model, std::vector<cglTFLight>& lights)
{
    const auto& extLights = model.extensions.find("KHR_lights_punctual");
    if (extLights == model.extensions.end()) return;

    const auto& lightsArray = extLights->second.Get("lights");
    for (size_t i = 0; i < lightsArray.ArrayLen(); ++i)
    {
        const tinygltf::Value& lightDef = lightsArray.Get(i);

        cglTFLight light;
        std::string l_strName = lightDef.Get("name").Get<std::string>();
		//light.name = 
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

        lights.push_back(light);
    }

    // Position/direction come from node transforms
    for (const auto& node : model.nodes)
    {
        if (!node.extensions.count("KHR_lights_punctual")) continue;
        int lightIndex = node.extensions.at("KHR_lights_punctual").Get("light").Get<int>();
        if (lightIndex >= 0 && lightIndex < lights.size())
        {
            // You must convert node.translation and rotation to world-space
            if (node.translation.size() == 3)
            {
				lights[lightIndex].m_vPosition = Vector3((float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);
            }
            // Direction can be derived from node.rotation if needed
        }
    }
}
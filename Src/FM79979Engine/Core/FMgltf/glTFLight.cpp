#include "tiny_gltf.h"
#include "glTFLight.h"
#include "glTFAnimation.h"
#include "../../imgui/imgui.h"

TYPDE_DEFINE_MARCO(cglTFLight);
TYPDE_DEFINE_MARCO(cLighController);
TYPDE_DEFINE_MARCO(cLighFrameData);
//"extensions": {
//    "KHR_lights_punctual": {
//        "lights": [
//        {
//            "type" : "point",
//                "color" : [1.0, 1.0, 1.0] ,
//                "intensity" : 10.0,
//                "range" : 5.0,
//                "name" : "MyLight"
//        }
//        ]
//    }
//}
// Forward declarations for debug draw helpers


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
                light.m_0Type1Enable[0] = (int)eLightType::eLT_DIRECTIONAL;
            }
            else
            if (type == "spot")
            {
                light.m_0Type1Enable[0] = (int)eLightType::eLT_SPOT;
            }
            else
            if (type == "point")
            {
                light.m_0Type1Enable[0] = (int)eLightType::eLT_POINT;
            }
            else
            {
                light.m_0Type1Enable[0] = (int)eLightType::eLT_POINT;
            }
        }

        if (lightDef.Has("color"))
        {
			float l_fColor[3] = { 1.f,1.f,1.f };
            for (int j = 0; j < 3; ++j)
            {
                l_fColor[j] = (float)lightDef.Get("color").Get(j).Get<double>();
            }
			light.m_vColor = Vector4(l_fColor[0], l_fColor[1], l_fColor[2],1.f);
        }
        if (lightDef.Has("intensity"))
        {
            light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x = (float)lightDef.Get("intensity").Get<double>();
        }

        if (lightDef.Has("range"))
        {
            light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.y = (float)lightDef.Get("range").Get<double>();
        }

        if (light.m_0Type1Enable[0] == (int)eLightType::eLT_SPOT)
        {
            auto& spot = lightDef.Get("spot");
            if (spot.Has("innerConeAngle"))
            {
                light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.z = (float)spot.Get("innerConeAngle").Get<double>();
            }
                
            if (spot.Has("outerConeAngle"))
            {
                light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.w = (float)spot.Get("outerConeAngle").Get<double>();
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
}

std::shared_ptr<sLightData> cglTFLight::CreateDirectionLight()
{
    sLightData light;
    light.m_0Type1Enable[0] = (int)eLightType::eLT_DIRECTIONAL; // Directional light
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x = 10.0f; // Increased intensity for visibility
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x = 10.0f; // Increased intensity for visibility
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.y = 0.0f;                           // Not used for directional lights
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.z = 0.0f;                  // Not used for directional lights
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.w = 0.0f;                  // Not used for directional lights
    light.m_vPosition = Vector3(0.f, 10.f, 10.f);    // Arbitrary position (not used for directional lights)
    light.m_vDirection = Vector3(0.f, -1.f, -1.f);   // Light direction (pointing toward the origin)
    light.m_vColor = Vector4(1.f, 1.f, 1.f,1.f);         // White light
	light.m_0Type1Enable[1] = 1;                            // Light is enabled
	std::shared_ptr<sLightData>l_Light = std::make_shared<sLightData>(light);
    return l_Light;
}

std::shared_ptr<sLightData> cglTFLight::CreateAmbientLight()
{
    sLightData l_AmbientLight;
    l_AmbientLight.m_0Type1Enable[0] = (int)eLightType::eLT_AMBIENT;
    l_AmbientLight.m_vColor = Vector4(0.3f, 0.3f, 0.3f,1.f); // Increased ambient color for more fill light
    l_AmbientLight.m_vPosition = Vector4(0.f, 0.f, 0.f,0.f); // Position of the ambient light
	l_AmbientLight.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x = 1.0f; // Intensity of the ambient light
    l_AmbientLight.m_0Type1Enable[1] = 1;
    return std::make_shared<sLightData>(l_AmbientLight);
}

std::shared_ptr<sLightData> cglTFLight::CreateSpotLight()
{
    sLightData light;
    light.m_0Type1Enable[0] = (int)eLightType::eLT_SPOT; // Spot light
    light.m_vPosition = Vector4(0.f, 10.f, 0.f, 1.f); // Default position
    light.m_vDirection = Vector4(0.f, -1.f, 0.f, 0.f); // Default direction
    light.m_vColor = Vector4(1.f, 0.f, 1.f, 1.f); // White color
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x = 10.0f; // Intensity
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.y = 20.0f; // Range
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.z = 0.3f; // Inner cone angle (radians)
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.w = 0.7f; // Outer cone angle (radians)
    light.m_0Type1Enable[1] = 1; // Enabled
    return std::make_shared<sLightData>(light);
}

std::shared_ptr<sLightData> cglTFLight::CreatePointLight()
{
    sLightData light;
    light.m_0Type1Enable[0] = (int)eLightType::eLT_POINT; // Point light
    light.m_vPosition = Vector4(0.f, 5.f, 5.f, 1.f); // Default position
    light.m_vDirection = Vector4(0.f, -1.f, 0.f, 0.f); // Not used for point, but set to default
    light.m_vColor = Vector4(1.f, 1.f, 1.f, 1.f); // White color
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x = 5.0f; // Intensity
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.y = 15.0f; // Range
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.z = 0.0f; // Not used
    light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.w = 0.0f; // Not used
    light.m_0Type1Enable[1] = 1; // Enabled
    return std::make_shared<sLightData>(light);
}

cLighController::cLighController()
{
    glGenBuffers(1, &m_uiLightUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uiLightUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(sLightBlock), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); // Unbind after allocation
    int l_iNumLights = static_cast<int>(m_LightDataVector.size());
    if (l_iNumLights == 0)
    {
        m_LightDataVector.push_back(cglTFLight::CreateSpotLight());
        //m_LightDataVector.push_back(cglTFLight::CreateDirectionLight());
        
        //m_LightDataVector.push_back(cglTFLight::CreateAmbientLight());
        //l_iNumLights = (int)m_LightDataVector.size();
        //auto l_TestDirectionLight = m_LightDataVector[0];
        //l_TestDirectionLight->m_0Type1Enable[1] = 1;
        //static float angle = 0.0f; // Angle for dynamic movement
        //angle +=  4; // Adjust speed based on frame time

        //l_TestDirectionLight->m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x = 10.0f; // Increased intensity for visibility
        //// Update the light's position in a circular path
        ////Vector3 l_vLightDirection = Vector3(10.0f * cos(angle), 10.0f, -10.0f * sin(angle));
        //Vector3 l_vLightDirection = Vector3(0, 0, -10.0f);
        ////Vector3 l_vLightDirection = Vector3(0, -1.0f,0);
        //l_TestDirectionLight->m_vPosition = Vector3(0, 0, -100);

        //// Update the light's direction to point toward the origin
        //l_TestDirectionLight->m_vDirection = l_vLightDirection.Normalize();
        //l_TestDirectionLight->m_vColor = Vector4(1.0f, 1.f, 1.0f, 1.f);
    }
    else
    {
        //auto l_TestDirectionLight = m_LightDataVector[0];
        //l_TestDirectionLight->m_0Type1Enable[1] = 1;
        //static float angle = 0.0f; // Angle for dynamic movement
        //angle += e_fElpaseTime/4; // Adjust speed based on frame time

        //l_TestDirectionLight->m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x = 10.0f; // Increased intensity for visibility
        //// Update the light's position in a circular path
        ////Vector3 l_vLightDirection = Vector3(10.0f * cos(angle), 10.0f, -10.0f * sin(angle));
        //Vector3 l_vLightDirection = Vector3(0,0, -10.0f);
        ////Vector3 l_vLightDirection = Vector3(0, -1.0f,0);
        //l_TestDirectionLight->m_vPosition = Vector3(0, 0, -100);

        //// Update the light's direction to point toward the origin
        //l_TestDirectionLight->m_vDirection = l_vLightDirection.Normalize();
        //l_TestDirectionLight->m_vColor = Vector4(1.0f,1.f,1.0f,1.f);
        //// Change the light's color over time for a dynamic effect
        ////l_TestDirectionLight->m_vColor = Vector3(
        ////    (sin(angle) + 1.0f) * 1.5f, // Red oscillates
        ////    (cos(angle) + 1.0f) * 1.5f, // Green oscillates
        ////    0.5f                        // Blue remains constant
        ////);
    }
}

cLighController::~cLighController()
{
    if (m_uiLightUBO != -1)
    {
        glDeleteBuffers(1, &m_uiLightUBO); // Delete UBO
        m_uiLightUBO = -1;
    }
}

const std::vector<std::shared_ptr<sLightData>>& cLighController::GetLights() const
{
        return m_LightDataVector;
}

void cLighController::SetLight(int e_iIndex, std::shared_ptr<sLightData> e_LightData)
{
	if (e_iIndex >= 0 && e_iIndex < m_LightDataVector.size())
	{
		m_LightDataVector[e_iIndex] = e_LightData;
	}
}

void cLighController::AddLight(std::shared_ptr<sLightData> e_LightData)
{
	m_LightDataVector.push_back(e_LightData);
}

void cLighController::RemoveLight(std::shared_ptr<sLightData> e_LightData)
{
	auto it = std::remove(m_LightDataVector.begin(), m_LightDataVector.end(), e_LightData);
	if (it != m_LightDataVector.end())
	{
		m_LightDataVector.erase(it, m_LightDataVector.end());
	}
}

void cLighController::RenderBegin()
{
    m_iLastUsedProgram = -1;
}


void  cLighController::Update(float e_fElpaseTime)
{

}

void  cLighController::DebugRender()
{
    for (const auto& lightPtr : m_LightDataVector)
    {
        if (!lightPtr) continue;
        const sLightData& light = *lightPtr;
        Vector3 pos(light.m_vPosition.x, light.m_vPosition.y, light.m_vPosition.z);
        Vector3 dir(light.m_vDirection.x, light.m_vDirection.y, light.m_vDirection.z);
        Vector4 color = light.m_vColor;
        int type = light.m_0Type1Enable[0];
        float intensity = light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x;
        float range = light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.y;
        float innerCone = light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.z;
        float outerCone = light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.w;
        // Always use the light's position and direction
        switch ((eLightType)type)
        {
        case eLightType::eLT_DIRECTIONAL:
            // Draw cone for direction at the light's position
            DrawCone(pos, dir.Normalize(), 2.0f, 0.5f, color);
            DrawLine(pos, pos + dir.Normalize() * 5.0f, color);
            break;
        case eLightType::eLT_POINT:
            DrawSphere(pos, range, color); // Draw a 3D sphere for point light
            // Optionally, draw lines from center to sphere surface for visual effect
            // for (int i = 0; i < 8; ++i)
            // {
            //     float angle = (float)i / 8.0f * 2.0f * 3.1415926f;
            //     Vector3 dirLine = Vector3(cos(angle), 0, sin(angle));
            //     DrawLine(pos, pos + dirLine * range, color);
            // }
            break;
        case eLightType::eLT_SPOT:
            // Draw cone for spot at the light's position, in the light's direction
            {
                Vector3 spotDir = -dir.Normalize();
                DrawCone(pos, spotDir, range, tan(outerCone) * range, color);
                DrawLine(pos, pos + spotDir * range, color);
            }
            break;
        case eLightType::eLT_AMBIENT:
            // Optionally draw a small sphere at the position
            // DrawSphere(pos, 0.3f, color);
            break;
        default:
            break;
        }
    }
}

void cLighController::RenderImGUILightControllerUI()
{
    if (ImGui::Begin("Light Controller"))
    {
        int numLights = static_cast<int>(m_LightDataVector.size());
        int prevNumLights = numLights;
        ImGui::SliderInt("Num Lights", &numLights, 0, MAX_LIGHT);
        if (numLights != prevNumLights)
        {
            if (numLights > prevNumLights)
            {
                for (int i = prevNumLights; i < numLights; ++i)
                {
                    m_LightDataVector.push_back(cglTFLight::CreateDirectionLight());
                }
            }
            else if (numLights < prevNumLights)
            {
                m_LightDataVector.resize(numLights);
            }
        }
        ImGui::Text("Num Lights: %d", numLights);
        for (int i = 0; i < numLights; ++i)
        {
            ImGui::PushID(i);
            sLightData& light = *m_LightDataVector[i];
            int prevType = light.m_0Type1Enable[0];
            if (ImGui::CollapsingHeader((std::string("Light ") + std::to_string(i)).c_str()))
            {
                ImGui::InputFloat3("Position", (float*)&light.m_vPosition);
                ImGui::ColorEdit3("Color", (float*)&light.m_vColor);
                ImGui::InputFloat3("Direction", (float*)&light.m_vDirection);
                ImGui::InputFloat("Intensity", &light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x);
                ImGui::InputFloat("Range", &light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.y);
                ImGui::InputFloat("Inner Cone Angle", &light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.z);
                ImGui::InputFloat("Outer Cone Angle", &light.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.w);
                int type = light.m_0Type1Enable[0];
                if (ImGui::Combo("Type", &type, "Directional\0Point\0Spot\0Ambient\0"))
                {
                    light.m_0Type1Enable[0] = type;
                    std::shared_ptr<sLightData> newLight;
                    switch ((eLightType)type)
                    {
                        case eLightType::eLT_DIRECTIONAL:
                            newLight = cglTFLight::CreateDirectionLight();
                            break;
                        case eLightType::eLT_POINT:
                            newLight = cglTFLight::CreatePointLight();
                            break;
                        case eLightType::eLT_SPOT:
                            newLight = cglTFLight::CreateSpotLight();
                            break;
                        case eLightType::eLT_AMBIENT:
                            newLight = cglTFLight::CreateAmbientLight();
                            break;
                        default:
                            newLight = cglTFLight::CreateDirectionLight();
                            break;
                    }
                    light = *newLight;
                }
                ImGui::Checkbox("Enable", (bool*)&light.m_0Type1Enable[1]);
            }
            ImGui::PopID();
        }
    }
    ImGui::End();
}

void cLighController::Render(GLuint e_uiProgramID)
{
    if(m_iLastUsedProgram == e_uiProgramID)
    {
        //return;
	}
    m_iLastUsedProgram = e_uiProgramID;
    // Ensure we don't exceed the maximum number of lights
    int numLights = static_cast<int>(m_LightDataVector.size());
    if (numLights > MAX_LIGHT)
    {
        numLights = MAX_LIGHT;
    }
	m_LightBlock.m_iNumLights[0] = numLights;
    for(int i = 0; i < m_LightBlock.m_iNumLights[0]; ++i)
    {
        m_LightBlock.m_Lights[i] = *m_LightDataVector[i].get();
	}

    if(0)
    {
        sLightData l_ssTestingBlock;
        l_ssTestingBlock.m_vColor = Vector3(1,0.5, 0.5);
		l_ssTestingBlock.m_0Type1Enable[0] = (int)eLightType::eLT_DIRECTIONAL;
		l_ssTestingBlock.m_vPosition = Vector3(0, 0, 0);
        l_ssTestingBlock.m_vLightData_xIntensityyRangezInnerConeAngelwOutterConeAngel.x = 1;
        GLuint blockIndex = glGetUniformBlockIndex(e_uiProgramID, "TestingBlock");
        if (blockIndex != GL_INVALID_INDEX)
        {
            glUniformBlockBinding(e_uiProgramID, blockIndex, 0);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uiLightUBO); // Bind UBO to binding point 0
        }
        // Bind and upload data to the UBO
        glBindBuffer(GL_UNIFORM_BUFFER, m_uiLightUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(sLightData), &l_ssTestingBlock);

        // Unbind the UBO
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    
    else
    {
        // Bind and upload data to the UBO
        FORCE_CHECK_GL_ERROR("1");
        GLuint blockIndex = glGetUniformBlockIndex(e_uiProgramID, "uLightBlock");
        if (blockIndex != GL_INVALID_INDEX)
        {
            auto l_uiSize = sizeof(sLightBlock);
            glBindBuffer(GL_UNIFORM_BUFFER, m_uiLightUBO);
            glUniformBlockBinding(e_uiProgramID, blockIndex, 0);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uiLightUBO); // Bind UBO to binding point 0
            FORCE_CHECK_GL_ERROR("2");
            glBufferSubData(GL_UNIFORM_BUFFER, 0, l_uiSize, &m_LightBlock);
            FORCE_CHECK_GL_ERROR("4");
            // Unbind the UBO
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            FORCE_CHECK_GL_ERROR("5");
        }
    }

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

cLighFrameData::~cLighFrameData()
{
    cLighController::GetInstance()->RemoveLight(m_LightData);
}

void cLighFrameData::Render()
{
    if (m_bDataChanged)
    {
        cLighController::GetInstance()->AddLight(m_LightData);
    }
}

void cLighFrameData::EndRender()
{
    
}

void g_fSetLightUniform(GLuint e_uiProgramID)
{
    cLighController::GetInstance()->Render(e_uiProgramID);
}

void g_fLightControllerUpdate(float e_fElpaseTime)
{
    cLighController::GetInstance()->Update(e_fElpaseTime);
}

//void AddDefaultDirectionAndAmbientLight()
//{
//    cLighController::GetInstance()->AddLight(cglTFLight::CreateDirectionLight());
//    cLighController::GetInstance()->AddLight(cglTFLight::CreateAmbientLight());
//}

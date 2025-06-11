#include "tiny_gltf.h"
#include "glTFLight.h"

TYPDE_DEFINE_MARCO(cglTFLight);
TYPDE_DEFINE_MARCO(cLighController);
TYPDE_DEFINE_MARCO(cLighFrameData);
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
                light.m_eType = (int)eLightType::eLT_DIRECTIONAL;
            }
            else
            if (type == "spot")
            {
                light.m_eType = (int)eLightType::eLT_SPOT;
            }
            else
            if (type == "point")
            {
                light.m_eType = (int)eLightType::eLT_POINT;
            }
            else
            {
                light.m_eType = (int)eLightType::eLT_POINT;
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

        if (light.m_eType == (int)eLightType::eLT_SPOT)
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
}

std::shared_ptr<sLightData> cglTFLight::CreateDirectionLight()
{
    sLightData light;
    light.m_eType = (int)eLightType::eLT_DIRECTIONAL; // Directional light
    light.m_fIntensity = 1.0f;                       // Intensity of the light
    light.m_fRange = 0.0f;                           // Not used for directional lights
    light.m_fInnerConeAngle = 0.0f;                  // Not used for directional lights
    light.m_fOuterConeAngle = 0.0f;                  // Not used for directional lights
    light.m_vPosition = Vector3(0.f, 10.f, 10.f);    // Arbitrary position (not used for directional lights)
    light.m_vDirection = Vector3(0.f, -1.f, -1.f);   // Light direction (pointing toward the origin)
    light.m_vColor = Vector3(1.f, 1.f, 1.f);         // White light
	light.m_iEnable = 1;                            // Light is enabled
	std::shared_ptr<sLightData>l_Light = std::make_shared<sLightData>(light);
    return l_Light;
}

std::shared_ptr<sLightData> cglTFLight::CreateAmbientLight()
{
    sLightData l_AmbientLight;
    l_AmbientLight.m_eType = (int)eLightType::eLT_AMBIENT;
    l_AmbientLight.m_vColor = Vector3(0.1f, 0.1f, 0.1f); // Ambient light color 
    l_AmbientLight.m_vPosition = Vector3(0.f, 0.f, 0.f); // Position of the ambient light
	l_AmbientLight.m_fIntensity = 1.0f; // Intensity of the ambient light
    l_AmbientLight.m_iEnable = 1;
    return std::make_shared<sLightData>(l_AmbientLight);
}

cLighController::cLighController()
{
    glGenBuffers(1, &m_uiLightUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uiLightUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlock), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); // Unbind after allocation
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
    int l_iNumLights = static_cast<int>(m_LightDataVector.size());
    if (l_iNumLights == 0)
    {
        m_LightDataVector.push_back(cglTFLight::CreateDirectionLight());
        m_LightDataVector.push_back(cglTFLight::CreateAmbientLight());
        l_iNumLights = (int)m_LightDataVector.size();
    }
    else
    {
        auto l_TestDirectionLight = m_LightDataVector[0];
        l_TestDirectionLight->m_iEnable = 1;
        static float angle = 0.0f; // Angle for dynamic movement
        float deltaTime = 0.016f;  // Replace with actual frame time
        angle += 0.5f * deltaTime; // Adjust speed based on frame time

        l_TestDirectionLight->m_fIntensity = 1.f;
        // Update the light's position in a circular path
        l_TestDirectionLight->m_vPosition = Vector3(10.0f * cos(angle), 5.0f, 10.0f * sin(angle));

        // Update the light's direction to point toward the origin
        l_TestDirectionLight->m_vDirection = -l_TestDirectionLight->m_vPosition.Normalize();

        // Change the light's color over time for a dynamic effect
        l_TestDirectionLight->m_vColor = Vector3(
            (sin(angle) + 1.0f) * 1.f, // Red oscillates
            (cos(angle) + 1.0f) * 1.f, // Green oscillates
            1.f                        // Blue remains constant
        );
    }
}

void cLighController::Render(GLuint e_uiProgramID)
{
    if(m_iLastUsedProgram == e_uiProgramID)
    {
        //return;
	}
    m_iLastUsedProgram = e_uiProgramID;
    GLuint l_inVec3ViewPosition = glGetUniformLocation(e_uiProgramID, "inVec3ViewPosition");
    if (l_inVec3ViewPosition != GL_INVALID_INDEX)
    {
        Vector3 l_vCameraPos(0, 0, -99999);
        glUniform3fv(l_inVec3ViewPosition, 1, l_vCameraPos);
    }
    // Ensure we don't exceed the maximum number of lights
    int numLights = static_cast<int>(m_LightDataVector.size());
    if (numLights > MAX_LIGHT)
    {
        numLights = MAX_LIGHT;
    }
	m_LightBlock.m_iNumLights = numLights;
    for(int i = 0; i < m_LightBlock.m_iNumLights; ++i)
    {
        m_LightBlock.m_Lights[i] = *m_LightDataVector[i].get();
	}

    if(0)
    {
        sLightData l_ssTestingBlock;
        l_ssTestingBlock.m_vColor = Vector3(1,0.5, 0.5);
		l_ssTestingBlock.m_eType = (int)eLightType::eLT_DIRECTIONAL;
		l_ssTestingBlock.m_vPosition = Vector3(0, 0, 0);
        l_ssTestingBlock.m_fIntensity = 1;
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
        GLuint blockIndex = glGetUniformBlockIndex(e_uiProgramID, "LightBlock");
        if (blockIndex != GL_INVALID_INDEX)
        {
            glUniformBlockBinding(e_uiProgramID, blockIndex, 0);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uiLightUBO); // Bind UBO to binding point 0
        }
        // Bind and upload data to the UBO
        glBindBuffer(GL_UNIFORM_BUFFER, m_uiLightUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlock), &m_LightBlock);

        // Unbind the UBO
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        int a = 0;
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

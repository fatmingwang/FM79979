#include "glTFNode.h"
#include "glTFAnimationMesh.h"
#include "glTFModel.h"
#include "glTFAnimation.h"
#include "../../imgui/imgui.h"

TYPDE_DEFINE_MARCO(cglTFNodeData);

cMatrix44 sSRT::GetMatrix()
{
    cMatrix44 l_Mat = cMatrix44::Identity;
    if (iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    {
        l_Mat *= cMatrix44::TranslationMatrix(vTranslation);
    }
    if (iSRTFlag & SRT_ROTATION_FLAG) // Rotation
    {
        l_Mat *= qRotation.ToMatrix();
    }
    if (iSRTFlag & SRT_SCALE_FLAG) // Scale
    {
        l_Mat *= cMatrix44::ScaleMatrix(vScale);
    }
    return l_Mat;
}

sSRT sSRT::Blend(sSRT& e_Source1, sSRT& e_Source2, float e_fFactor)
{
    Quaternion l_Quaternion = e_Source2.qRotation;
    if (Quaternion::Dot(e_Source1.qRotation, l_Quaternion) < 0.0f)
    {
        l_Quaternion = ~l_Quaternion;
    }
    sSRT l_SRT;
    l_SRT.iSRTFlag = e_Source1.iSRTFlag | e_Source2.iSRTFlag;
    l_SRT.vScale = Vector3Lerp(e_Source1.vScale, e_Source2.vScale, e_fFactor);
    l_SRT.qRotation = Quaternion::Slerp(e_Source1.qRotation, l_Quaternion, e_fFactor);
    l_SRT.vTranslation = Vector3Lerp(e_Source1.vTranslation, e_Source2.vTranslation, e_fFactor);
    return l_SRT;
}

void ConvertSRTMapToMatrixMap(const FloatToSRTMap& srtMap, FloatTocMatrix44Map& matrixMap)
{
    matrixMap.clear();

    for (const auto& [time, srt] : srtMap)
    {
        cMatrix44 mat = cMatrix44::Identity;
        if (srt.iSRTFlag & SRT_TRANSLATION_FLAG)
        {
            // Construct translation matrix
            float l_fTransArray[] =
            {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                srt.vTranslation.x, srt.vTranslation.y, srt.vTranslation.z, 1
            };
            cMatrix44 transMat(l_fTransArray);
            mat = transMat;
        }
        if (srt.iSRTFlag & SRT_ROTATION_FLAG)
        {
            // Construct rotation matrix from quaternion
            float x = srt.qRotation.x, y = srt.qRotation.y, z = srt.qRotation.z, w = srt.qRotation.w;
            float l_fTotationArray[]
            {
                1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w), 0,
                2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w), 0,
                2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y), 0,
                0, 0, 0, 1
            };
            cMatrix44 rotMat(l_fTotationArray);
            mat *= rotMat;
        }
        // Construct scale matrix
        if (srt.iSRTFlag & SRT_SCALE_FLAG)
        {
            float l_fScaleArray[] =
            {
                srt.vScale.x, 0, 0, 0,
                0, srt.vScale.y, 0, 0,
                0, 0, srt.vScale.z, 0,
                0, 0, 0, 1
            };
            cMatrix44 scaleMat(l_fScaleArray);
            mat *= scaleMat;
        }
        matrixMap[time] = mat;
    }
}
cglTFNodeData::cglTFNodeData(const tinygltf::Node& e_Node, int e_iNodeIndex)
{
    m_bIgnoreShadowPass = false;
    if (e_Node.name.length())
    {
        SetName(e_Node.name.c_str());
    }
    else
    {
        std::string l_strName = "NodeIndex:" + ValueToString(e_iNodeIndex);
        SetName(l_strName.c_str());
    }
    m_iNodeIndex = e_iNodeIndex;
}

cglTFNodeData::cglTFNodeData(cglTFNodeData* e_pglTFNodeData)
{
    m_bIgnoreShadowPass = false;
    //for debug
    auto l_mat = e_pglTFNodeData->GetLocalTransform();
    this->SetLocalTransform(l_mat);
    m_iJointIndex = e_pglTFNodeData->m_iJointIndex;
    m_iNodeIndex = e_pglTFNodeData->m_iNodeIndex;
    //assign later ensure mesh only exists once.
    m_pMesh = nullptr;
    //
    m_StartNodeWorldTransform = e_pglTFNodeData->m_StartNodeWorldTransform;
    m_StartNodeTransform = e_pglTFNodeData->m_StartNodeTransform;
    m_StartSRT = e_pglTFNodeData->m_StartSRT;
    m_Instance = std::make_shared<cMeshInstance>(e_pglTFNodeData->m_Instance.get());
}

cglTFNodeData::~cglTFNodeData()
{
}

void cglTFNodeData::SetMesh(cMesh* e_pMesh, const tinygltf::Node& e_pNode, const tinygltf::Model& model, int e_iInstanceValue)
{
    m_pMesh = e_pMesh;
    if (!m_pMesh)
    {
        return;
    }
    this->m_Instance = LoadInstance(e_pNode, model);
    if (!this->m_Instance)
    {
        if (e_iInstanceValue > 1)
        {
            this->m_Instance = cMeshInstance::CreateInstance();
            std::vector<cMatrix44>l_matVector;
            int gridSize = static_cast<int>(std::ceil(std::sqrt(static_cast<float>(e_iInstanceValue))));
            auto l_fLength = e_pMesh->m_vMaxBounds.Length();
            for (int i = 0; i < e_iInstanceValue; ++i)
            {
                int row = i / gridSize;
                int col = i % gridSize;
                cMatrix44 l_matRotation = cMatrix44::RotationMatrix(Vector3(0,frand(0,360),0));
                cMatrix44 l_matTranslation = cMatrix44::TranslationMatrix(Vector3(col * l_fLength, row * l_fLength, 0));
                //cMatrix44 l_matTranslation = cMatrix44::TranslationMatrix(Vector3(col * 5.f, row * 5.f, 0));// 
                //l_matVector.push_back(l_matTranslation* l_matRotation);
                l_matVector.push_back(l_matTranslation);
            }
            this->m_Instance->SetInstanceTransforms(l_matVector);
        }
    }
}

void cglTFNodeData::SetMesh(cMesh* e_pMesh, std::shared_ptr<cMeshInstance>e_Instance)
{
    m_pMesh = e_pMesh;
    m_Instance = e_Instance;
}

cglTFNodeData* cglTFNodeData::FinChildByName(const wchar_t* e_strBoneName)
{
    if (!wcscmp(e_strBoneName, GetName()))
        return this;
    if (this->GetNextSibling() != nullptr)
    {
        return (dynamic_cast<cglTFNodeData*>(GetNextSibling()))->FinChildByName(e_strBoneName);
    }
    if (GetFirstChild() != nullptr)
    {
        return (dynamic_cast<cglTFNodeData*>(GetFirstChild()))->FinChildByName(e_strBoneName);
    }
    return nullptr;
}




void cglTFNodeData::ApplySRT(const sSRT& srt, bool e_bSetChildBonesDirty)
{
    cMatrix44 localTransform = cMatrix44::Identity;
    sSRT l_SRT = srt;
    if (l_SRT.iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    {
        localTransform *= cMatrix44::TranslationMatrix(l_SRT.vTranslation);
    }
    if (l_SRT.iSRTFlag & SRT_ROTATION_FLAG) // Rotation
    {
        localTransform *= l_SRT.qRotation.ToMatrix();
    }
    if (l_SRT.iSRTFlag & SRT_SCALE_FLAG) // Scale
    {
        localTransform *= cMatrix44::ScaleMatrix(l_SRT.vScale);
    }
    //if (l_SRT.iSRTFlag & SRT_SCALE_FLAG) // Scale
    //{
    //    localTransform *= cMatrix44::ScaleMatrix(l_SRT.scale);
    //}
    //if (l_SRT.iSRTFlag & SRT_ROTATION_FLAG) // Rotation
    //{
    //    localTransform *= l_SRT.rotation.ToMatrix();
    //}
    //if (l_SRT.iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    //{
    //    localTransform *= cMatrix44::TranslationMatrix(l_SRT.translation);
    //}
    //localTransform = this->m_StartNodeTransform* localTransform;
    SetLocalTransform(localTransform , e_bSetChildBonesDirty);
}

void cglTFNodeData::Update(float e_fElpaseTime)
{
    if (this->m_pMesh)
    {
        this->m_pMesh->Update(e_fElpaseTime);
    }
}

void cglTFNodeData::Render()
{
    if (this->m_pMesh)
    {
        auto l_mat = this->GetWorldTransform();
        this->m_pMesh->SetWorldTransform(l_mat);
        if (m_Instance)
        {
            this->m_pMesh->Render(m_Instance.get());
        }
        else
        {
            //auto l_str = std::wstring(this->m_pMesh->GetName());
            //if (std::string::npos != l_str.find(L"BodyHoodInterior01"))
            {
                this->m_pMesh->Render();
            }
            
        }
    }
}

void   cglTFNodeData::RenderNodesShadowPass(const cMatrix44& lightViewProj, GLuint shadowShaderProgram)
{
    if (m_bIgnoreShadowPass)
    {
        return;
    }
    if (m_pMesh)
    {
        cMatrix44 modelMatrix = GetWorldTransform();
        m_pMesh->RenderShadow(lightViewProj, modelMatrix, shadowShaderProgram);

    }
    for (Frame* child = GetFirstChild(); child; child = child->GetNextSibling())
    {
        cglTFModel*childObj = dynamic_cast<cglTFModel*>(child);
        if (childObj)
        {
            childObj->RenderNodesShadowPass(lightViewProj, shadowShaderProgram);
        }
        else
        {
            cglTFNodeData*l_pglTFNodeData = dynamic_cast<cglTFNodeData*>(child);
            if (l_pglTFNodeData)
            {
                l_pglTFNodeData->RenderNodesShadowPass(lightViewProj, shadowShaderProgram);
            }
        }
            
    }
}

void		cglTFNodeData::DebugRender()
{
    if (this->m_pMesh)
    {
        auto l_mat = this->GetWorldTransform();
        this->m_pMesh->SetWorldTransform(l_mat);
        this->m_pMesh->DebugRender();
    }
}

void  cglTFNodeData::RenderImGUI()
{
    std::string l_UniqueID = std::to_string(this->GetUniqueID());
    ImGui::PushID(l_UniqueID.c_str());
    std::string l_strNodeName = this->GetCharName();
    if (ImGui::TreeNode(l_strNodeName.c_str()))
    {
        Vector3 worldPos = this->GetWorldPosition();
        Vector3 localPos = this->GetLocalPosition();    
        ImGui::Text("World Position");
        if (ImGui::InputFloat3((l_strNodeName+"_WorldPos").c_str(), worldPos, "%.3f", 0))
        {
            this->SetWorldPosition(Vector3(worldPos[0], worldPos[1], worldPos[2]));
        }
        //ImGui::Text("Local Position");
        //if (ImGui::InputFloat3((l_strNodeName +"_LocalPos").c_str(), localPos, "%.3f", 0))
        //{
        //    this->SetLocalPosition(Vector3(localPos[0], localPos[1], localPos[2]));
        //}
        // Add local rotation display and edit
        auto l_matWorldMatrix = this->GetWorldTransform();
        Vector3 scale, rotation, translation;
        float inverted = 0;
		l_matWorldMatrix.Decompose(scale, rotation, translation, inverted);
        ImGui::Text("World Rotation");
        if (ImGui::InputFloat3((l_strNodeName + "_LocalRot").c_str(), rotation, "%.3f", 0))
        {
			l_matWorldMatrix.Recompose(scale, rotation, translation, inverted);
            this->SetWorldTransform(l_matWorldMatrix);
        }



        if (m_pMesh)
        {
            m_pMesh->RenderImGUI();
        }
        // Render children as tree nodes
        Frame* child = this->GetFirstChild();
        while (child)
        {
            cglTFNodeData* childNode = dynamic_cast<cglTFNodeData*>(child);
            if (childNode)
            {
                childNode->RenderImGUI();
            }
            child = child->GetNextSibling();
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}

bool cglTFNodeData::ContainInstanceExtension(const tinygltf::Node& e_pNode)
{
    auto extIt = e_pNode.extensions.find("EXT_mesh_gpu_instancing");
    if (extIt != e_pNode.extensions.end())
    {
        return true;
    }
    return false;
}

std::shared_ptr<cMeshInstance> cglTFNodeData::LoadInstance(const tinygltf::Node& e_pNode, const tinygltf::Model& model)
{
    if (!cglTFNodeData::ContainInstanceExtension(e_pNode))
    {
        return nullptr;
    }
    auto extIt = e_pNode.extensions.find("EXT_mesh_gpu_instancing");
    const auto& ext = extIt->second;
    if (!ext.Has("attributes")) return nullptr;
    const auto& attributes = ext.Get("attributes");

    // Determine instance count from one of the accessors
    size_t count = 0;
    if (attributes.Has("TRANSLATION"))
    {
        int accessorIdx = attributes.Get("TRANSLATION").Get<int>();
        count = model.accessors[accessorIdx].count;
    }
    else if (attributes.Has("MATRIX"))
    {
        int accessorIdx = attributes.Get("MATRIX").Get<int>();
        count = model.accessors[accessorIdx].count;
    }
    else
    {
        // No instancing data
        return nullptr;
    }

    std::vector<sSRT> srtVector(count);
    std::vector<cMatrix44> matrices(count);
    bool hasMatrix = false;

    // Read per-instance attributes
    if (attributes.Has("TRANSLATION"))
    {
        int accessorIdx = attributes.Get("TRANSLATION").Get<int>();
        const auto& accessor = model.accessors[accessorIdx];
        const auto& bufferView = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bufferView.buffer];
        const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        for (size_t i = 0; i < count; ++i)
        {
            srtVector[i].vTranslation = Vector3(&data[i * 3]);
            srtVector[i].iSRTFlag |= SRT_TRANSLATION_FLAG;
        }
    }
    if (attributes.Has("ROTATION"))
    {
        int accessorIdx = attributes.Get("ROTATION").Get<int>();
        const auto& accessor = model.accessors[accessorIdx];
        const auto& bufferView = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bufferView.buffer];
        const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        for (size_t i = 0; i < count; ++i)
        {
            srtVector[i].qRotation = Quaternion(&data[i * 4]);
            srtVector[i].iSRTFlag |= SRT_ROTATION_FLAG;
        }
    }
    if (attributes.Has("SCALE"))
    {
        int accessorIdx = attributes.Get("SCALE").Get<int>();
        const auto& accessor = model.accessors[accessorIdx];
        const auto& bufferView = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bufferView.buffer];
        const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        for (size_t i = 0; i < count; ++i)
        {
            srtVector[i].vScale = Vector3(&data[i * 3]);
            srtVector[i].iSRTFlag |= SRT_SCALE_FLAG;
        }
    }
    if (attributes.Has("MATRIX"))
    {
        hasMatrix = true;
        int accessorIdx = attributes.Get("MATRIX").Get<int>();
        const auto& accessor = model.accessors[accessorIdx];
        const auto& bufferView = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bufferView.buffer];
        const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        for (size_t i = 0; i < count; ++i)
        {
            matrices[i] = cMatrix44(&data[i * 16]);
        }
    }

    // Compose transforms
    std::vector<cMatrix44> transforms;
    for (size_t i = 0; i < count; ++i)
    {
        if (hasMatrix)
        {
            transforms.push_back(matrices[i]);
        }
        else
        {
            transforms.push_back(srtVector[i].GetMatrix());
        }
    }
    auto instance = std::make_shared<cMeshInstance>();
    instance->SetInstanceTransforms(transforms);
    return instance;
}
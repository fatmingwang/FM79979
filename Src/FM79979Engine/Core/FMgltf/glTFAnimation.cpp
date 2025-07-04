#include "../AllCoreInclude.h"
#include "glTFAnimation.h"
#include "glTFAnimationMesh.h"
#include "glTFModel.h"
#include "AnimationTexture.h"

//const int g_iTextureColorComponentCount = 4; // RGBA

sAnimationData::sAnimationData(cNamedTypedObjectVector<cglTFNodeData>* e_pglTFNodeDataVector, const tinygltf::Model& e_Model, const tinygltf::Animation&e_Animation)
{
    m_BoneIDAndAnimationData = std::make_shared<std::map<int, FloatToSRTMap>>();
    m_pBoneVector = e_pglTFNodeDataVector;
    //l_pAnimationData->m_fMinKeyTime = FLT_MAX;
    //l_pAnimationData->m_fMaxKeyTime = FLT_MIN;
    m_fCurrentTime = 0.0f;
    m_fStartTime = 0.0f;
    m_fEndTime = 0.0f;

    for (const auto& channel : e_Animation.channels)
    {
        const auto& sampler = e_Animation.samplers[channel.sampler];
        cglTFNodeData* bone = (*m_pBoneVector)[channel.target_node];
        //"STEP" are allowd just do not do interpolation,but I am lazy to implement it.
        if (sampler.interpolation != "LINEAR")
        {
            FMLOG("animation only support lineat format");
            continue;
        }
        const auto& inputAccessor = e_Model.accessors[sampler.input];
        const auto& inputBufferView = e_Model.bufferViews[inputAccessor.bufferView];
        const auto& inputBuffer = e_Model.buffers[inputBufferView.buffer];
        const float* inputData = reinterpret_cast<const float*>(inputBuffer.data.data() + inputBufferView.byteOffset + inputAccessor.byteOffset);

        const auto& outputAccessor = e_Model.accessors[sampler.output];
        const auto& outputBufferView = e_Model.bufferViews[outputAccessor.bufferView];
        const auto& outputBuffer = e_Model.buffers[outputBufferView.buffer];
        const float* outputData = reinterpret_cast<const float*>(outputBuffer.data.data() + outputBufferView.byteOffset + outputAccessor.byteOffset);
        m_iTargetNodeIndex = channel.target_node;
        //morph
        if (channel.target_path == "weights")
        {
            size_t l_uiFrameCount = inputAccessor.count;
            size_t l_MorphTargetCount = outputAccessor.count / l_uiFrameCount;
            std::map<float, std::vector<float>>& l_Map = m_TimaAndMorphWeightMap;
            for (int i = 0; i < l_uiFrameCount; ++i)
            {
                float time = inputData[i];
                std::vector<float> l_WeightVector(l_MorphTargetCount);
                std::memcpy(l_WeightVector.data(), outputData + i * l_MorphTargetCount, l_MorphTargetCount * sizeof(float));
                l_Map[time] = l_WeightVector;
            }
        }
        else
        {
            for (size_t i = 0; i < inputAccessor.count; ++i)
            {
                float time = inputData[i];
                FloatToSRTMap& keyframes = (*m_BoneIDAndAnimationData)[bone->m_iNodeIndex];
                sSRT& srt = keyframes[time];
                if (channel.target_path == "translation")
                {
                    srt.vTranslation = Vector3(outputData + (i * 3));
                    srt.iSRTFlag |= SRT_TRANSLATION_FLAG; // Set translation flag
                }
                else if (channel.target_path == "rotation")
                {
                    srt.qRotation = Quaternion(outputData + (i * 4));
                    //srt.rotation.Normalize();
                    srt.iSRTFlag |= SRT_ROTATION_FLAG; // Set rotation flag
                }
                else if (channel.target_path == "scale")
                {
                    srt.vScale = Vector3(outputData + (i * 3));
                    srt.iSRTFlag |= SRT_SCALE_FLAG; // Set scale flag
                }
                else if (channel.target_path == "matrix")
                {
                    int a = 0;
                }
            }
        }

        if (inputAccessor.count)
        {
            float l_fBeginTime = inputData[0];
            float l_fEndTime = inputData[inputAccessor.count - 1];
            m_fMinKeyTime = min(m_fMinKeyTime, l_fBeginTime);
            m_fMaxKeyTime = max(m_fMaxKeyTime, l_fEndTime);
            m_fStartTime = m_fMinKeyTime;
            m_fEndTime = m_fMaxKeyTime;
        }
    }
}

bool    cAnimationClip::SampleToTime(float e_fTime, bool e_bAssignToBone, std::vector<sSRT>* e_pSRTVector)
{
    if (!this->m_pCurrentAnimationData)
    {
        return false;
    }
    std::vector<sSRT>* l_pSRTVector = &m_SRTVector;
    if (e_pSRTVector)
    {
        l_pSRTVector = e_pSRTVector;
    }
    for (auto l_IT : *m_pCurrentAnimationData->m_BoneIDAndAnimationData)
    {
        auto l_pBone = (*m_pCurrentAnimationData->m_pBoneVector)[l_IT.first];
        //auto l_pBone = (*m_pCurrentAnimationData->m_pBoneVector)[214];
        UpdateNode(l_pBone, e_fTime, (*l_pSRTVector)[l_IT.first], e_bAssignToBone);
    }
    if (m_pCurrentAnimationData->m_TimaAndMorphWeightMap.size() && m_pCurrentAnimationData->m_pTargetMesh)
    {
        m_pCurrentAnimationData->m_pTargetMesh->m_CurrentAnimationMorphPrimitiveWeightsVector = m_pCurrentAnimationData->GetInterpolatedWeights(e_fTime);
    }
    return true;
}

void cAnimationClip::UpdateNode(cglTFNodeData* e_pBone, float e_fTime, sSRT& e_SRT, bool e_bAssignToBone)
{
    FloatToSRTMap* l_pFloatToSRTMap = &(*this->m_pCurrentAnimationData->m_BoneIDAndAnimationData)[e_pBone->m_iNodeIndex];
    const sSRT& l_RestSRT = e_pBone->m_StartSRT; // <-- get rest pose
    if (l_pFloatToSRTMap->empty())
    {
        e_SRT = l_RestSRT;
        if (e_bAssignToBone)
        {
            e_pBone->SetLocalTransform(e_SRT.GetMatrix());
        }
        return;
    }
    // Find the keyframes surrounding the current time
    auto it = l_pFloatToSRTMap->lower_bound(e_fTime);

    if (it == l_pFloatToSRTMap->begin()) // If time is before the first keyframe
    {
        e_SRT = it->second;
        return;
    }
    if (it == l_pFloatToSRTMap->end()) // If time is after the last keyframe
    {
        it = std::prev(l_pFloatToSRTMap->end());
    }

    auto nextIt = it;
    auto prevIt = std::prev(it); // Get the previous keyframe


    float prevTime = prevIt->first;
    float nextTime = nextIt->first;

    if (prevTime == nextTime) // Avoid division by zero
    {
        e_SRT = prevIt->second;
        return;
    }

    // Calculate the interpolation factor (clamped between 0 and 1)
    float factor = (e_fTime - prevTime) / (nextTime - prevTime);
    factor = std::clamp(factor, 0.0f, 1.0f);

    // Interpolate SRT values
    const sSRT& prevSRT = prevIt->second;
    const sSRT& nextSRT = nextIt->second;
    sSRT l_CurrentSRT = l_RestSRT;

    // Translation Interpolation
    if (prevSRT.iSRTFlag & SRT_TRANSLATION_FLAG)
    {
        l_CurrentSRT.vTranslation = prevSRT.vTranslation * (1.0f - factor) + nextSRT.vTranslation * factor;
        l_CurrentSRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
    }

    // Rotation Interpolation using SLERP
    if (prevSRT.iSRTFlag & SRT_ROTATION_FLAG)
    {
        l_CurrentSRT.qRotation = Quaternion::Slerp(prevSRT.qRotation, nextSRT.qRotation, factor);
        l_CurrentSRT.iSRTFlag |= SRT_ROTATION_FLAG;
    }

    // Scale Interpolation
    if (prevSRT.iSRTFlag & SRT_SCALE_FLAG)
    {
        l_CurrentSRT.vScale = prevSRT.vScale * (1.0f - factor) + nextSRT.vScale * factor;
        l_CurrentSRT.iSRTFlag |= SRT_SCALE_FLAG;
    }
    e_SRT = l_CurrentSRT;
    if (e_bAssignToBone)
    {
        e_pBone->SetLocalTransform(e_SRT.GetMatrix());
    }
}

void	cAnimationClip::SetBoneAndAnimationData(class cglTFModelRenderNode* e_pglTFModel)
{
    m_pNodesVector = &e_pglTFModel->m_NodesVector;
    m_pNodeIndexAndBoneMap = &e_pglTFModel->m_NodeIndexAndBoneMap;
    m_pNameAndAnimationMap = &e_pglTFModel->m_NameAndAnimationMap;
    m_SRTVector.clear();
    for (auto l_pBone : *e_pglTFModel->m_NodesVector.GetList())
    {
        sSRT l_SRT;
        l_SRT.iSRTFlag = SRT_SCALE_FLAG | SRT_ROTATION_FLAG | SRT_TRANSLATION_FLAG;
        m_SRTVector.push_back(l_SRT);
    }
}
void cAnimationClip::SetBoneAndAnimationData(cglTFModel* e_pglTFModel)
{
    m_pNodesVector = &e_pglTFModel->m_NodesVector;
    m_pNodeIndexAndBoneMap = &e_pglTFModel->m_NodeIndexAndBoneMap;
    m_pNameAndAnimationMap = &e_pglTFModel->m_NameAndAnimationMap;
    m_SRTVector.clear();
    for (auto l_pBone : *e_pglTFModel->m_NodesVector.GetList())
    {
        sSRT l_SRT;
        l_SRT.iSRTFlag = SRT_SCALE_FLAG | SRT_ROTATION_FLAG | SRT_TRANSLATION_FLAG;
        m_SRTVector.push_back(l_SRT);
    }
}

bool cAnimationClip::SetAnimation(const char* e_strAnimationName, bool e_bLoop, float e_fTargetTime)
{
    if (!m_pNameAndAnimationMap || !m_pNodeIndexAndBoneMap)
    {
        return false;
    }
    auto it = m_pNameAndAnimationMap->find(e_strAnimationName);
    if (it != m_pNameAndAnimationMap->end())
    {
        m_strAnimationName = e_strAnimationName;
        m_pCurrentAnimationData = it->second;
        m_pCurrentAnimationData->m_fCurrentTime = e_fTargetTime;
        m_pCurrentAnimationData->m_bLoop = e_bLoop;
        if (!m_pCurrentAnimationData->m_pTargetMesh)
        {
            auto l_pBoneIT = this->m_pNodeIndexAndBoneMap->find(m_pCurrentAnimationData->m_iTargetNodeIndex);
            if (l_pBoneIT != this->m_pNodeIndexAndBoneMap->end())
            {
                m_pCurrentAnimationData->m_pTargetMesh = l_pBoneIT->second->GetMesh();
            }
            else
            {
                FMLOG("animation %s don't has target mesh!? how come!!?", e_strAnimationName);
            }
        }
        return true;
    }
    return false;
}

void cAnimationClip::UpdateToTargetTime(float e_fTime, bool e_bAssignToBone)
{
    SampleToTime(e_fTime, true);
}

void    cAnimationClip::BlendClips(float e_fTime, const char* e_strAnimationName1, const char* e_strAnimationName2, bool e_bAssignToBone, bool e_bLoop, float e_fTargetFactor)
{
    float l_fPreviousTime = m_fBlendingTime;
    m_fBlendingTime += e_fTime;
    std::vector<sSRT>   l_SRTVector1 = m_SRTVector;
    std::vector<sSRT>   l_SRTVector2 = m_SRTVector;
    if (!SetAnimation(e_strAnimationName1, e_bLoop, l_fPreviousTime))
    {
        return;
    }
    this->m_pCurrentAnimationData->Update(e_fTime);
    this->SampleToTime(this->m_pCurrentAnimationData->m_fCurrentTime, false, &l_SRTVector1);
    if (!SetAnimation(e_strAnimationName2, e_bLoop, l_fPreviousTime))
    {
        return;
    }
    this->m_pCurrentAnimationData->Update(e_fTime);
    this->SampleToTime(this->m_pCurrentAnimationData->m_fCurrentTime, false, &l_SRTVector2);

    auto l_iSize = m_pNodesVector->Count();
    for (int i = 0; i < l_iSize; ++i)
    {
        m_SRTVector[i] = sSRT::Blend(l_SRTVector1[i], l_SRTVector2[i], e_fTargetFactor);
        (*m_pNodesVector)[i]->SetLocalTransform(m_SRTVector[i].GetMatrix());
    }
}

void cAnimationClip::Update(float e_fElpaseTime)
{
    if (this->m_pCurrentAnimationData)
    {
        m_pCurrentAnimationData->Update(e_fElpaseTime);
        this->SampleToTime(this->m_pCurrentAnimationData->m_fCurrentTime, true);
    }
}

sAnimationData* cAnimationClip::GetAnimationData(std::string e_strName)
{
    return MapFind(*m_pNameAndAnimationMap, e_strName);
}

void sAnimationData::Update(float e_fElpaseTime)
{
    float l_fNewTime = m_fCurrentTime + e_fElpaseTime;
    if (l_fNewTime > m_fEndTime)
    {
        if (m_bLoop)
        {
            l_fNewTime = std::fmod(l_fNewTime, m_fEndTime);
        }
        else
        {
            l_fNewTime = m_fEndTime;
        }
    }
    m_fCurrentTime = l_fNewTime;
}


std::vector<float> sAnimationData::GetInterpolatedWeights(float e_fTime)
{
    if (m_TimaAndMorphWeightMap.empty())
    {
        return {}; // No morph animation data
    }

    // If exact match found, return weights directly
    auto exact = m_TimaAndMorphWeightMap.find(e_fTime);
    if (exact != m_TimaAndMorphWeightMap.end())
    {
        return exact->second;
    }

    // Find closest keyframes before and after e_fTime
    auto upper = m_TimaAndMorphWeightMap.upper_bound(e_fTime);
    if (upper == m_TimaAndMorphWeightMap.begin())
    {
        return upper->second; // Before first keyframe, return first weights
    }
    if (upper == m_TimaAndMorphWeightMap.end())
    {
        return std::prev(upper)->second; // After last keyframe, return last weights
    }

    // Interpolate between two closest keyframes
    auto lower = std::prev(upper);
    float t1 = lower->first;
    float t2 = upper->first;
    float alpha = (e_fTime - t1) / (t2 - t1); // Interpolation factor

    const std::vector<float>& weights1 = lower->second;
    const std::vector<float>& weights2 = upper->second;
    std::vector<float> interpolatedWeights(weights1.size());

    for (size_t i = 0; i < weights1.size(); i++)
    {
        interpolatedWeights[i] = (1.0f - alpha) * weights1[i] + alpha * weights2[i];
    }

    return interpolatedWeights;
}

sAnimationData* sAnimationData::CloneFromModel(cglTFModelRenderNode* e_pglTFModelRenderNode, sAnimationData* e_pSource)
{
	sAnimationData* l_pAnimationData = new sAnimationData();
    //is this safe?
    *l_pAnimationData = *e_pSource;
    l_pAnimationData->m_pBoneVector = &e_pglTFModelRenderNode->m_NodesVector;
    return l_pAnimationData;
}




sAnimationFrameAndTime::sAnimationFrameAndTime(float e_fEndTime)
{
    m_fCurrentPlayTime = 0.f;
    m_iNextFrame = 0;
    m_iCurrentFrame = 1;
    m_fAnimationEndTime = e_fEndTime;
    float l_fStepTime = ANIMATION_FRAME_STEP_TIME;
    m_iLastFrame = (int)(e_fEndTime / l_fStepTime) + 1;
    m_bLoop = true;
}
void sAnimationFrameAndTime::Update(float e_fElpaseTime)
{
    m_fCurrentPlayTime += e_fElpaseTime;
    if (m_fCurrentPlayTime >= m_fAnimationEndTime)
    {
        if (m_bLoop)
        {
            m_fCurrentPlayTime = UT::GetFloatModulus(m_fCurrentPlayTime, m_fAnimationEndTime);
        }
        else
        {
            m_fCurrentPlayTime = m_fAnimationEndTime;
        }
    }
    m_iCurrentFrame = (int)(m_fCurrentPlayTime / ANIMATION_FRAME_STEP_TIME);
    m_iNextFrame = m_iCurrentFrame + 1;
    m_fNextFrameLerpTimes = (m_fCurrentPlayTime - (m_iCurrentFrame * ANIMATION_FRAME_STEP_TIME)) / ANIMATION_FRAME_STEP_TIME;
    if (m_iCurrentFrame >= m_iLastFrame)
    {
        m_iCurrentFrame = 0;
    }
    if (m_iNextFrame >= m_iLastFrame)
    {
        m_iNextFrame = 0;
    }
}


sAniamationInstanceData::sAniamationInstanceData(cAnimationClip* e_pAnimationClip, const char* e_strAnimationName, int e_iNumInstanceData,cSkinningMesh* e_pSkinningMesh)
{
    this->SetName((std::string(e_strAnimationName)+std::string("_AniamationInstanceData")).c_str());
    auto l_pAnimationData = e_pAnimationClip->GetAnimationData(e_strAnimationName);
    if (!l_pAnimationData)
    {
        FMLOG("can't find animation %s", e_strAnimationName);
        assert("no animation data");
        return;
    }
    m_FrameIndexVector.resize(e_iNumInstanceData);
    m_ToNextLerpTime.resize(e_iNumInstanceData);;
    m_spAnimTexture = std::make_shared<cAnimTexture>(*e_pAnimationClip, e_strAnimationName, e_pSkinningMesh);
    for (int i = 0; i < e_iNumInstanceData; ++i)
    {
        std::shared_ptr<sAnimationFrameAndTime>l_spData = std::make_shared<sAnimationFrameAndTime>(l_pAnimationData->m_fEndTime);
        m_AnimationFrameAndTimeVector.push_back(l_spData);
    }
}


cAnimationInstanceManager::cAnimationInstanceManager(cAnimationClip* e_pAnimationClip, std::shared_ptr<class cMeshInstance> e_spMeshInstance, GLuint e_uiProgramID, cSkinningMesh* e_pTargetMesh)
{
    m_pTargetMesh = e_pTargetMesh;
    m_pAnimationClip = e_pAnimationClip;
    m_spMeshInstance = e_spMeshInstance;
    m_uiProgramID = e_uiProgramID;
}

cAnimationInstanceManager::~cAnimationInstanceManager()
{
}


void cAnimationInstanceManager::Render(GLuint e_uiProgramID, std::shared_ptr<sAniamationInstanceData>e_spAniamationInstanceDataVector)
{
    if (!e_spAniamationInstanceDataVector || !e_spAniamationInstanceDataVector->m_spAnimTexture)
    {
        return;
    }
    int l_iSize = (int)e_spAniamationInstanceDataVector->m_AnimationFrameAndTimeVector.size();
    GLuint l_uiuNumAnimationModelID = glGetUniformLocation(e_uiProgramID, "uNumAnimationModel");
    if (l_uiuNumAnimationModelID != GL_INVALID_INDEX)
    {
        glUniform1i(l_uiuNumAnimationModelID, l_iSize);
    }
    const int l_iTestID = 8;
    GLuint l_uiuAnimTexture = glGetUniformLocation(e_uiProgramID, "uAnimTexture");
    if (l_uiuAnimTexture != GL_INVALID_INDEX)
    {
        e_spAniamationInstanceDataVector->m_spAnimTexture->Set(l_uiuAnimTexture, l_iTestID);
    }
    GLuint l_uiuCurrentAndNextFrameIndex = glGetUniformLocation(e_uiProgramID, "uCurrentAndNextFrameIndex");
    GLuint l_uiuAnimationLerpTime = glGetUniformLocation(e_uiProgramID, "uAnimationLerpTime");
    if (l_uiuCurrentAndNextFrameIndex != GL_INVALID_INDEX)
    {
        glUniform2iv(l_uiuCurrentAndNextFrameIndex, l_iSize, (GLint*)&e_spAniamationInstanceDataVector->m_FrameIndexVector[0]);
    }
    if (l_uiuAnimationLerpTime != GL_INVALID_INDEX)
    {
        glUniform1fv(l_uiuAnimationLerpTime,l_iSize,(GLfloat*)&e_spAniamationInstanceDataVector->m_ToNextLerpTime[0]);
    }
    GLuint l_uiuNumBones = glGetUniformLocation(e_uiProgramID, "uNumBones");
    if (l_uiuNumBones != GL_INVALID_INDEX)
    {
        int l_iNumJoints = (int)this->m_pTargetMesh->GetNodeInversePoseMatrixVector()->size();
        glUniform1i(l_uiuNumBones, l_iNumJoints);
    }
    GLuint l_uiTextureSize = glGetUniformLocation(e_uiProgramID, "uTextureSize");
    if (l_uiTextureSize != GL_INVALID_INDEX)
    {
        glUniform1i(l_uiTextureSize, e_spAniamationInstanceDataVector->m_spAnimTexture->m_TextureSquareSize);
    }
}
void cAnimationInstanceManager::GenerateAnimationNameAndAniamationInstanceDataMap(int e_iNumInstanceData, const char* e_strAnimationName)
{
    //if (m_AnimationNameAndAniamationInstanceDataMap.size() == 0)
    {
        if (e_iNumInstanceData >= MAX_INSTANCES)
        {
            e_iNumInstanceData = MAX_INSTANCES;
			FMLOG("cAnimationInstanceManager::GenerateAnimationNameAndAniamationInstanceDataMap e_iNumInstanceData is too big, set to %d", MAX_INSTANCES);
        }
        auto l_pValue = MapFind(m_AnimationNameAndAniamationInstanceDataMap, e_strAnimationName);
        if (!l_pValue)
        {
            std::shared_ptr<sAniamationInstanceData>l_spData = std::make_shared<sAniamationInstanceData>(this->m_pAnimationClip, e_strAnimationName,e_iNumInstanceData,this->m_pTargetMesh);
            m_AnimationNameAndAniamationInstanceDataMap[e_strAnimationName] = l_spData;
        }
        //if (this->m_pAnimationClip->m_pNameAndAnimationMap->size())
        //{
        //    for (auto l_IT : *this->m_pAnimationClip->m_pNameAndAnimationMap)
        //    {
        //        std::shared_ptr<sAniamationInstanceData>l_spData = std::make_shared<sAniamationInstanceData>(this->m_pAnimationClip, l_IT.first.c_str(),e_iNumInstanceData);
        //        m_AnimationNameAndAniamationInstanceDataMap[l_IT.first] = l_spData;
        //    }
        //}
    }
}
std::tuple<std::shared_ptr<sAniamationInstanceData>, GLuint > cAnimationInstanceManager::GetAnimationInstanceData(const char* e_strAnimationName)
{
    GenerateAnimationNameAndAniamationInstanceDataMap((int)m_spMeshInstance->GetTransforms()->size(), e_strAnimationName);
    return { MapFind(m_AnimationNameAndAniamationInstanceDataMap, e_strAnimationName),m_uiProgramID };
}
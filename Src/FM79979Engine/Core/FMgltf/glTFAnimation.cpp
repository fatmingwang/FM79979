#include "../AllCoreInclude.h"
#include "glTFAnimation.h"
#include "glTFModel.h"
#include <fstream>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

const int g_iTextureColorComponentCount = 4; // RGBA

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
    if (l_pFloatToSRTMap->empty())
    {
        sSRT l_sSRT;
        e_SRT = l_sSRT;
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
    sSRT l_CurrentSRT;

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


// Utility function to get next power of two
unsigned int NextPowerOfTwo(unsigned int v)
{
    if (v == 0) return 1;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}
cAnimTexture::cAnimTexture(cAnimationClip& e_AnimationClip, const char* e_strAnimationName)
{
    m_pfTextureData = nullptr;
    m_TextureSquareSize = 0;
    e_AnimationClip.SetAnimation(e_strAnimationName, true, 0.0f);
    auto l_pCurrentAnimationData = e_AnimationClip.GetCurrentAnimationData();
    if (l_pCurrentAnimationData)
    {
        fs::path filePath(e_AnimationClip.GetCharName());
        std::string filenameWithoutExtension = filePath.stem().string();
        std::string l_strPath = filenameWithoutExtension + "/" + std::string(e_strAnimationName) + ".AnimTexture";
        //if (this->Load(l_strPath.c_str()))
        {
            //return;
        }
        // 30 fps
        bool l_bDoLazyAlign = true;
        float l_fStepTime = ANIMATION_FRAME_STEP_TIME;
        int l_iNumFrames = (int)(l_pCurrentAnimationData->m_fEndTime / l_fStepTime) + 1;
        int l_iNumBones = l_pCurrentAnimationData->m_pBoneVector->Count();
        assert(l_iNumBones <= 100 && "now shader is 100");
        const int l_iNumRowsPerMatrix = 4; // mat4 has 4 rows
        int l_TotalMatrices = l_iNumFrames * l_iNumBones;
        int l_TotalTexels = l_TotalMatrices * l_iNumRowsPerMatrix;
        int l_SQRT = (int)sqrt(l_TotalTexels) + 1;
        int l_iTextureSize = NextPowerOfTwo(l_SQRT);
        if (l_bDoLazyAlign)
        {
            l_iTextureSize = 512;
        }
        this->Resize(l_iTextureSize);
        for (int l_iFrameIndex = 0; l_iFrameIndex < l_iNumFrames; ++l_iFrameIndex)
        {
            e_AnimationClip.UpdateToTargetTime(l_fStepTime * l_iFrameIndex, true);
            for (int l_iJointIndex = 0; l_iJointIndex < l_iNumBones; ++l_iJointIndex)
            {
                cglTFNodeData* l_pBone = (*l_pCurrentAnimationData->m_pBoneVector)[l_iJointIndex];
                cMatrix44 l_mat = l_pBone->GetWorldTransform();
                //cMatrix44 l_matTranspose = l_mat.Transposed();
                // Store each row of the matrix as a vec4
                if (l_bDoLazyAlign)
                {
                    const int l_ciMatrixFloatCount = 16;
                    int l_iX = l_iJointIndex * l_ciMatrixFloatCount;
                    int l_iY = (l_iFrameIndex * l_iTextureSize)* g_iTextureColorComponentCount;
                    int l_iDataIndex = l_iX + l_iY;
                    memcpy(&m_pfTextureData[l_iDataIndex], l_mat, sizeof(cMatrix44));
                }
                else
                {
                    int linearIndex = (l_iFrameIndex * l_iNumBones + l_iJointIndex) * l_iNumRowsPerMatrix;
                    for (int row = 0; row < l_iNumRowsPerMatrix; ++row)
                    {
                        int l_IndexForMatrix = linearIndex + row;
                        int x = l_IndexForMatrix % m_TextureSquareSize;
                        int y = l_IndexForMatrix / m_TextureSquareSize;
                        VECTOR4 rowVec = l_mat.r[row]; // Assumes GetRow returns Vector4
                        SetTexel(x, y, Vector4(rowVec.x, rowVec.y, rowVec.z, rowVec.w));
                        //auto l_TransposeRow = l_matTranspose.r[row];
                        //SetTexel(x, y, Vector4(l_TransposeRow.x, l_TransposeRow.y, l_TransposeRow.z, l_TransposeRow.w));
                    }
                }
            }
        }
        glGenTextures(1, &m_uiOpenglTecxtureID);
        UploadTextureDataToGPU();
        //Save(l_strPath.c_str());
    }
}

cAnimTexture::cAnimTexture(const cAnimTexture& other)
{
    m_pfTextureData = nullptr;
    m_TextureSquareSize = 0;
    glGenTextures(1, &m_uiOpenglTecxtureID);
    *this = other;
}

cAnimTexture& cAnimTexture::operator=(const cAnimTexture& other)
{
    if (this == &other)
    {
        return *this;
    }

    m_TextureSquareSize = other.m_TextureSquareSize;
    if (m_pfTextureData != 0)
    {
        delete[] m_pfTextureData;
    }
    m_pfTextureData = new float[m_TextureSquareSize * m_TextureSquareSize * g_iTextureColorComponentCount];
    memcpy(m_pfTextureData, other.m_pfTextureData, sizeof(float) * (m_TextureSquareSize * m_TextureSquareSize * g_iTextureColorComponentCount));

    return *this;
}

cAnimTexture::cAnimTexture(const char* path)
{
    m_pfTextureData = nullptr;
    m_TextureSquareSize = 0;
    glGenTextures(1, &m_uiOpenglTecxtureID);
}

cAnimTexture::cAnimTexture(unsigned int size)
{
    glGenTextures(1, &m_uiOpenglTecxtureID);
    m_TextureSquareSize = size;
    m_pfTextureData = new float[size * size * g_iTextureColorComponentCount];
}

cAnimTexture::~cAnimTexture()
{
    SAFE_DELETE(m_pfTextureData);
    if (m_uiOpenglTecxtureID != -1)
    {
        glDeleteTextures(1, &m_uiOpenglTecxtureID);
    }
}

bool cAnimTexture::Load(const char* path)
{
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        //std::cout << "Couldn't open " << path << " to read from\n";
		FMLOG("Couldn't open %s to read from", path);
        return false;
    }

    file >> m_TextureSquareSize;
    if (m_TextureSquareSize == 0)
    {
        //std::cout << "Trying to read Animation Texture With Size 0\n";
		FMLOG("Trying to read Animation Texture With Size 0");
        return false;
    }

    m_pfTextureData = new float[m_TextureSquareSize * m_TextureSquareSize * g_iTextureColorComponentCount];
    file.read((char*)m_pfTextureData, sizeof(float) * (m_TextureSquareSize * m_TextureSquareSize * g_iTextureColorComponentCount));
    file.close();
    UploadTextureDataToGPU();
    return true;
}

bool cAnimTexture::Save(const char* path)
{
    fs::path filePath(path);
    fs::path dir = filePath.parent_path();
    if (!dir.empty() && !fs::exists(dir))
    {
        fs::create_directories(dir);
    }
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        //std::cout << "Couldn't open " << path << " to write to\n";
		FMLOG("Couldn't open %s to write to", path);
        return false;
    }

    file << m_TextureSquareSize;
    if (m_TextureSquareSize != 0)
    {
        file.write((char*)m_pfTextureData, sizeof(float) * (m_TextureSquareSize * m_TextureSquareSize * g_iTextureColorComponentCount));
    }
    else
    {
        //std::cout << "Trying to write Animation Texture With Size 0\n";
		FMLOG("Trying to write Animation Texture With Size 0");

    }
    file.close();
    if (m_TextureSquareSize <= 0)
    {
        return false;
    }
    return true;
}

void cAnimTexture::UploadTextureDataToGPU()
{
    glBindTexture(GL_TEXTURE_2D, m_uiOpenglTecxtureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_TextureSquareSize, m_TextureSquareSize, 0, GL_RGBA, GL_FLOAT, m_pfTextureData);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int cAnimTexture::Size()
{
    return m_TextureSquareSize;
}

void cAnimTexture::Resize(unsigned int newSize)
{
    if (m_pfTextureData != 0)
    {
        delete[] m_pfTextureData;
    }
    m_TextureSquareSize = newSize;
    m_pfTextureData = new float[m_TextureSquareSize * m_TextureSquareSize * g_iTextureColorComponentCount];
}

unsigned int cAnimTexture::GetHandle()
{
    return m_uiOpenglTecxtureID;
}

void cAnimTexture::SetTexel(unsigned int x, unsigned int y, const Vector4& e_Value)
{
    unsigned int index = (y * m_TextureSquareSize * g_iTextureColorComponentCount) + (x * g_iTextureColorComponentCount);

    m_pfTextureData[index + 0] = e_Value.x;
    m_pfTextureData[index + 1] = e_Value.y;
    m_pfTextureData[index + 2] = e_Value.z;
    m_pfTextureData[index + 3] = e_Value.w;
}

void cAnimTexture::SetTexel(unsigned int x, unsigned int y, const Vector3& e_Value)
{
    unsigned int index = (y * m_TextureSquareSize * g_iTextureColorComponentCount) + (x * g_iTextureColorComponentCount);

    m_pfTextureData[index + 0] = e_Value.x;
    m_pfTextureData[index + 1] = e_Value.y;
    m_pfTextureData[index + 2] = e_Value.z;
    m_pfTextureData[index + 3] = 0.0f;
}

void cAnimTexture::Set(unsigned int uniformIndex, unsigned int textureIndex)
{
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, m_uiOpenglTecxtureID);
    glUniform1i(uniformIndex, textureIndex); // Bind uniform X to sampler Y
}
void cAnimTexture::UnSet(unsigned int textureIndex)
{
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

float* cAnimTexture::GetData()
{
    return m_pfTextureData;
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


sAniamationInstanceData::sAniamationInstanceData(cAnimationClip* e_pAnimationClip, const char* e_strAnimationName, int e_iNumInstanceData)
{
    auto l_pAnimationData = e_pAnimationClip->GetAnimationData(e_strAnimationName);
    if (!l_pAnimationData)
    {
        FMLOG("can't find animation %s", e_strAnimationName);
        assert("no animation data");
        return;
    }
    m_FrameIndexVector.resize(e_iNumInstanceData);
    m_ToNextLerpTime.resize(e_iNumInstanceData);;
    m_spAnimTexture = std::make_shared<cAnimTexture>(*e_pAnimationClip, e_strAnimationName);
    for (int i = 0; i < e_iNumInstanceData; ++i)
    {
        std::shared_ptr<sAnimationFrameAndTime>l_spData = std::make_shared<sAnimationFrameAndTime>(l_pAnimationData->m_fEndTime);
        m_AnimationFrameAndTimeVector.push_back(l_spData);
    }
}


cAnimationInstanceManager::cAnimationInstanceManager(cAnimationClip* e_pAnimationClip, std::shared_ptr<class cMeshInstance> e_spMeshInstance, GLuint e_uiProgramID)
{
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
        //glUniform1i(l_uiuNumAnimationModelID,0);
        glUniform1i(l_uiuNumAnimationModelID, l_iSize);
    }
    const int l_iTestID = 3;
    GLuint l_uiuAnimTexture = glGetUniformLocation(e_uiProgramID, "uAnimTexture");
    if (l_uiuAnimTexture != GL_INVALID_INDEX)
    {
        e_spAniamationInstanceDataVector->m_spAnimTexture->Set(l_uiuAnimTexture, l_iTestID);
    }
    GLuint l_uiuCurrentAndNextFrameIndex = glGetUniformLocation(e_uiProgramID, "uCurrentAndNextFrameIndex");
    GLuint l_uiuAnimationLerpTime = glGetUniformLocation(e_uiProgramID, "uAnimationLerpTime");
    if (l_uiuCurrentAndNextFrameIndex != GL_INVALID_INDEX)
    {
        //int l_iArray[100 * 2];
        //memcpy(l_iArray, &e_spAniamationInstanceDataVector->m_FrameIndexVector[0], sizeof(int) * 100 * 2);
        //glUniform2iv(l_uiuCurrentAndNextFrameIndex, l_iSize, (GLint*)l_iArray);
        glUniform2iv(l_uiuCurrentAndNextFrameIndex, l_iSize, (GLint*)&e_spAniamationInstanceDataVector->m_FrameIndexVector[0]);
    }
    if (l_uiuAnimationLerpTime != GL_INVALID_INDEX)
    {
        glUniform1fv(l_uiuAnimationLerpTime,l_iSize,(GLfloat*)&e_spAniamationInstanceDataVector->m_ToNextLerpTime[0]);
    }
    GLuint l_uiuNumBones = glGetUniformLocation(e_uiProgramID, "uNumBones");
    if (l_uiuNumBones != GL_INVALID_INDEX)
    {
        auto l_iNumJoints = this->m_pAnimationClip->m_pNodesVector->Count();
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
        
        auto l_pValue = MapFind(m_AnimationNameAndAniamationInstanceDataMap, e_strAnimationName);
        if (!l_pValue)
        {
            std::shared_ptr<sAniamationInstanceData>l_spData = std::make_shared<sAniamationInstanceData>(this->m_pAnimationClip, e_strAnimationName,e_iNumInstanceData);
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
    GenerateAnimationNameAndAniamationInstanceDataMap((int)m_spMeshInstance->GetTransforms().size(), e_strAnimationName);
    return { MapFind(m_AnimationNameAndAniamationInstanceDataMap, e_strAnimationName),m_uiProgramID };
}
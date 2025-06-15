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
    mData = 0;
    mSize = 0;
    glGenTextures(1, &mHandle);
	e_AnimationClip.SetAnimation(e_strAnimationName, true, 0.0f);
    auto l_pCurrentAnimationData = e_AnimationClip.GetCurrentAnimationData();
    if (l_pCurrentAnimationData)
    {
        fs::path filePath(e_AnimationClip.GetCharName());
        std::string filenameWithoutExtension = filePath.stem().string();
        std::string l_strPath = filenameWithoutExtension + "/" + std::string(e_strAnimationName)+".AnimTexture";
        if (this->Load(l_strPath.c_str()))
        {
            return;
        }
        //30 fps
        float l_fStepTime = 0.032f;
        int l_iNumStep = (int)(l_pCurrentAnimationData->m_fEndTime / l_fStepTime) + 1;
        int l_iBneCount = l_pCurrentAnimationData->m_pBoneVector->Count();
        int l_iRequireSize = l_iNumStep * 16 * l_iBneCount;
        int l_SQRT = (int)sqrt(l_iRequireSize)+1;
        int l_iSize = NextPowerOfTwo(l_SQRT);
		this->Resize(l_iSize);
        for(int i=0; i < l_iNumStep; ++i)
        {
            e_AnimationClip.UpdateToTargetTime(l_fStepTime*i, true);
            for(int j = 0; j < l_iBneCount; ++j)
            {
                cglTFNodeData* l_pBone = (*l_pCurrentAnimationData->m_pBoneVector)[j];
                auto l_mat = l_pBone->GetWorldTransform();
                int matrixIdx = i * l_iBneCount + j;
                SetTexel(matrixIdx, l_mat);
			}
		}
        Save(l_strPath.c_str());
    }
}

cAnimTexture::cAnimTexture(const cAnimTexture& other)
{
    mData = 0;
    mSize = 0;
    glGenTextures(1, &mHandle);
    *this = other;
}

cAnimTexture& cAnimTexture::operator=(const cAnimTexture& other)
{
    if (this == &other)
    {
        return *this;
    }

    mSize = other.mSize;
    if (mData != 0)
    {
        delete[] mData;
    }
    mData = new float[mSize * mSize * g_iTextureColorComponentCount];
    memcpy(mData, other.mData, sizeof(float) * (mSize * mSize * g_iTextureColorComponentCount));

    return *this;
}

cAnimTexture::cAnimTexture(const char* path)
{
    mData = 0;
    mSize = 0;
    glGenTextures(1, &mHandle);
}

cAnimTexture::cAnimTexture(unsigned int size)
{
    glGenTextures(1, &mHandle);
    mSize = size;
    mData = new float[size * size * g_iTextureColorComponentCount];
}

cAnimTexture::~cAnimTexture()
{
    if (mData != 0)
    {
        delete[] mData;
    }
    glDeleteTextures(1, &mHandle);
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

    file >> mSize;
    if (mSize == 0)
    {
        //std::cout << "Trying to read Animation Texture With Size 0\n";
		FMLOG("Trying to read Animation Texture With Size 0");
        return false;
    }

    mData = new float[mSize * mSize * g_iTextureColorComponentCount];
    file.read((char*)mData, sizeof(float) * (mSize * mSize * g_iTextureColorComponentCount));
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

    file << mSize;
    if (mSize != 0)
    {
        file.write((char*)mData, sizeof(float) * (mSize * mSize * g_iTextureColorComponentCount));
    }
    else
    {
        //std::cout << "Trying to write Animation Texture With Size 0\n";
		FMLOG("Trying to write Animation Texture With Size 0");

    }
    file.close();
    if (mSize <= 0)
    {
        return false;
    }
    return true;
}

void cAnimTexture::UploadTextureDataToGPU()
{
    glBindTexture(GL_TEXTURE_2D, mHandle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mSize, mSize, 0, GL_RGBA, GL_FLOAT, mData);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int cAnimTexture::Size()
{
    return mSize;
}

void cAnimTexture::Resize(unsigned int newSize)
{
    if (mData != 0)
    {
        delete[] mData;
    }
    mSize = newSize;
	//512*4/16 = 128
    //width size 512 bone limit is 128
    mData = new float[mSize * mSize * g_iTextureColorComponentCount];
}

unsigned int cAnimTexture::GetHandle()
{
    return mHandle;
}

void cAnimTexture::Set(unsigned int uniformIndex, unsigned int textureIndex)
{
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, mHandle);
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
    return mData;
}

void   cAnimTexture::SetTexel(unsigned int x, unsigned int y, const cMatrix44& e_Mat)
{
	const int l_ciMatrixSize = 16; // 4x4 matrix has 16 elements
    // Each matrix occupies 4 texels (16 floats)
    unsigned int matrixIndex = y * mSize + x;
    unsigned int floatIndex = matrixIndex * l_ciMatrixSize;
    memcpy(&mData[floatIndex], e_Mat, sizeof(float) * l_ciMatrixSize);

}
void   cAnimTexture::SetTexel(unsigned int matrixIdx, const cMatrix44& e_Mat)
{
    // matrixIdx: 0 .. (mSize * mSize / 4) - 1
    unsigned int floatIndex = matrixIdx * 16; // 16 floats per matrix
    if (floatIndex + 16 > mSize * mSize * g_iTextureColorComponentCount)
    {
        // Out of bounds, do not write
        return;
    }
    memcpy(&mData[floatIndex], e_Mat, sizeof(float) * 16);
}
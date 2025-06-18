#pragma once

#include "glTFAnimation.h"
#include "Common.h"

class cAnimTexture :public NamedTypedObject
{
    friend class cAnimationInstanceManager;
    Vector4* m_pvTextureData;
    unsigned int m_TextureSquareSize = -1;
    unsigned int m_uiOpenglTecxtureID = -1;
    public:
    cAnimTexture(cAnimationClip& e_AnimationClip, const char* e_strAnimationName, class cSkinningMesh* e_pSkinningMesh);
    cAnimTexture(const cAnimTexture&);
    cAnimTexture& operator=(const cAnimTexture&);
    cAnimTexture(const char* path);
    cAnimTexture(unsigned int size);
    ~cAnimTexture();

    bool Load(const char* path);
    bool Save(const char* path);
    void UploadTextureDataToGPU();

    unsigned int Size();
    //make it square
    void Resize(unsigned int e_uiWidthAndHeight);
    float* GetData();

    void Set(unsigned int uniformIndex, unsigned int textureIndex);
    void UnSet(unsigned int textureIndex);
    unsigned int GetHandle();
    void        SetTexel(unsigned int x, unsigned int y, const Vector4& e_Value);
    void        SetTexel(unsigned int x, unsigned int y, const Vector3& e_Value);
};
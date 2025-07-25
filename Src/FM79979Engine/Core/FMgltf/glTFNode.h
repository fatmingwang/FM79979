#pragma once
//#include "gltfMesh.h"
#include <map>
#include <string>
#include "../AllCoreInclude.h"
#include "tiny_gltf.h"

struct sSRT
{
    Vector3             vScale = Vector3::One;
    Quaternion          qRotation = Quaternion::Identity;
    Vector3             vTranslation = Vector3::Zero;
    int                 iSRTFlag = 0;//S 1<<1 R 1<< 2 T 1<<3
    cMatrix44           GetMatrix();
    static sSRT         Blend(sSRT& e_Source1, sSRT& e_Source2,float e_fFactor);
};
#define SRT_SCALE_FLAG          (1<<1)
#define SRT_ROTATION_FLAG       (1<<2)
#define SRT_TRANSLATION_FLAG    (1<<3)

using FloatToSRTMap = std::map<float, sSRT>;
typedef std::map<float, cMatrix44> FloatTocMatrix44Map;
void    ConvertSRTMapToMatrixMap(const FloatToSRTMap& srtMap, FloatTocMatrix44Map& matrixMap);

//=====================================
class cMeshInstance;
class cglTFNodeData : public cRenderObject
{
    class cMesh* m_pMesh = nullptr;
    // Instance manager
    std::shared_ptr<cMeshInstance>  m_Instance;
    std::shared_ptr<cMeshInstance>  LoadInstance(const tinygltf::Node& e_pNode, const tinygltf::Model& model);
public:
    DEFINE_TYPE_INFO();
    LAZY_CLONE_FUNCTION(cglTFNodeData);
    //for debug
    int                 m_iJointIndex = -1;
    int                 m_iNodeIndex = -1;
    //
    cMatrix44   m_StartNodeWorldTransform;
    //rest pose
    cMatrix44   m_StartNodeTransform;
    sSRT        m_StartSRT;

    cglTFNodeData(const tinygltf::Node&e_Node,int e_iNodeIndex);
    cglTFNodeData(cglTFNodeData*e_pglTFNodeData);
    virtual ~cglTFNodeData();
    class cMesh* GetMesh()
    {
        return m_pMesh;
    }
	void SetMesh(class cMesh* e_pMesh, const tinygltf::Node& e_pNode, const tinygltf::Model& model,int e_iInstanceValue);
    void SetMesh(class cMesh* e_pMesh, std::shared_ptr<cMeshInstance>e_Instance);
    cglTFNodeData* FinChildByName(const wchar_t* e_strBoneName);
    void ApplySRT(const sSRT& srt, bool e_bSetChildBonesDirty);

    virtual	void        Update(float e_fElpaseTime)override;
    virtual	void        Render()override;
    virtual	void		DebugRender()override;
    std::shared_ptr<cMeshInstance>  GetMeshInstance()
    {
        return m_Instance;
    }
    static bool ContainInstanceExtension(const tinygltf::Node& e_pNode);
};




//#define	FORCE_CHECK_GL_ERROR(p) {}//ForceGlErrorTest(p,__FILE__, __LINE__)
#define	FORCE_CHECK_GL_ERROR(p) ForceGlErrorTest(p,__FILE__, __LINE__)

inline void	ForceGlErrorTest(const char* e_strMessage, const char* e_strFileName, int32_t e_iCodeLine)
{
    int l_iTestCont = 5;
    int	l_iError = glGetError();
    while (l_iError != 0 && l_iTestCont > 0)
    {
        --l_iTestCont;
        std::string l_str = GET_FILENAME_AND_LINE(e_strFileName, e_iCodeLine);
        l_str += "   ";
        l_str += e_strMessage;
        l_str += "   glGetError:";
        const char* errorString = 0;
        switch (l_iError)
        {
            case GL_INVALID_ENUM: l_str += "GL_INVALID_ENUM"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: l_str += "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_INVALID_VALUE: l_str += "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: l_str += "GL_INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY: l_str += "GL_OUT_OF_MEMORY"; break;
            default: l_str += FATMING_CORE::ValueToString(l_iError); break;
        }
        FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
        l_iError = glGetError();
    }
}
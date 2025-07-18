#include "glTFNode.h"
#include "Common.h"

void	DumpBoneIndexDebugInfo(cglTFNodeData* e_pBone, bool e_bDoNextSibling, bool e_bRoot)
{
    Frame* l_pParentNode = e_pBone->GetParent();
    static int	l_siCount = 0;
    ++l_siCount;
    std::string l_strDebugInfo;
    while (l_pParentNode)
    {
        l_strDebugInfo += "-----";
        //FMLog::LogWithFlag(L"-----",false,false);
        l_pParentNode = l_pParentNode->GetParent();
    }
    l_strDebugInfo += "Joint:";
    l_strDebugInfo += ValueToString(e_pBone->m_iJointIndex);
    l_strDebugInfo += ",";
    l_strDebugInfo += "Node:";
    l_strDebugInfo += ValueToString(e_pBone->m_iNodeIndex);
    l_strDebugInfo += ",Name:";
    l_strDebugInfo += e_pBone->GetCharName();
    l_strDebugInfo += ",Matrix:";
    l_strDebugInfo += ValueToString(e_pBone->GetWorldTransform());
    //FMLog::LogWithFlag(l_str, CORE_LOG_FLAG);
    FMLog::Log(l_strDebugInfo.c_str(), false);
    if (e_pBone->GetFirstChild())
    {
        DumpBoneIndexDebugInfo(dynamic_cast<cglTFNodeData*>(e_pBone->GetFirstChild()), true, false);
    }

    if (e_bDoNextSibling && e_pBone->GetNextSibling())
    {
        DumpBoneIndexDebugInfo(dynamic_cast<cglTFNodeData*>(e_pBone->GetNextSibling()), e_bDoNextSibling, false);
    }
    if (e_bRoot)
    {
        FMLOG("Total count:%d", l_siCount);
        l_siCount = 0;
    }
}
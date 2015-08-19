#include "StdAfx.h"
#include "ColladaVisualScene.h"
#include "AnimationMesh.h"
#include <string>
const wchar_t*	cLibraryVisualScenes::TypeID( L"cLibraryVisualScenes" );


sVisualSceneLibrary::sSkeletonNode*		sVisualSceneLibrary::GetSkeletonNode(const WCHAR*e_strSkeletonName)
{
	size_t	l_uiSekletonSize = AllSkeletonNodeVector.size();
	for( size_t i=0;i<l_uiSekletonSize;++i )
	{
		if(AllSkeletonNodeVector[i]->pRootNode->IsContain(e_strSkeletonName))
			return AllSkeletonNodeVector[i];
	}
	return 0;
}

void	sVisualSceneLibrary::DumpNodeInfo(sVisualSceneLibrary::sNode*e_pNode,int *e_piCount)
{
	++(*e_piCount);
	sVisualSceneLibrary::sNode*l_pParentNode = e_pNode->pParent;
	int	l_iLevel = 0;
	while( l_pParentNode )
	{
		cGameApp::OutputDebugInfoString(L"-----");
		l_pParentNode = l_pParentNode->pParent;
		l_iLevel++;
	}
	WCHAR	l_str[MAX_PATH];
	if( e_pNode->strName )
		swprintf(l_str,MAX_PATH,L"Count:%d   %d:Name:%ls\n",*e_piCount,l_iLevel,e_pNode->strName);
	else
	if( e_pNode->strID )
		swprintf(l_str,MAX_PATH,L"Count:%d   %d:ID:%ls\n",*e_piCount,l_iLevel,e_pNode->strID);
	else
	if( e_pNode->strSID )
		swprintf(l_str,MAX_PATH,L"Count:%d   %d:SID:%ls\n",*e_piCount,l_iLevel,e_pNode->strSID);

	cGameApp::OutputDebugInfoString(l_str);
	
	if( e_pNode->pFirstChild )
	{
		DumpNodeInfo(e_pNode->pFirstChild,e_piCount);
	}

	if( e_pNode->pSibling )
	{
		DumpNodeInfo(e_pNode->pSibling,e_piCount);
	}
}

sVisualSceneLibrary::sSkeletonNode::sSkeletonNode()
{
	pRootNode = 0;
	pRootBone = 0;
}
sVisualSceneLibrary::sSkeletonNode::~sSkeletonNode()
{

}

void	sVisualSceneLibrary::sSkeletonNode::ContainThisNodeName(sNode*e_pNode,WCHAR*e_strName,bool*e_pbValue)
{
	if( *e_pbValue )
		return;
	//const WCHAR*l_strSID = 0;
	//const WCHAR*l_strName = 0;
	if( !wcscmp(e_pNode->strID,e_strName)||!wcscmp(e_pNode->strName,e_strName) )
	{
		*e_pbValue = true;
		return;
	}
	sNode*l_pCurrentNode = e_pNode->pFirstChild;
	if( l_pCurrentNode )
	{
		ContainThisNodeName(l_pCurrentNode,e_strName,e_pbValue);
	}
	l_pCurrentNode = e_pNode->pSibling;
	if( l_pCurrentNode )
	{
		ContainThisNodeName(l_pCurrentNode,e_strName,e_pbValue);
	}
}
bool	sVisualSceneLibrary::sSkeletonNode::ContainThisNodeName(const WCHAR*e_strName)
{
	bool l_b = false;
	ContainThisNodeName(this->pRootNode,(WCHAR*)e_strName,&l_b);
	return l_b;
}

void	DumpBoneInfo(cBone*e_pBone)
{
	cBone*l_pParentNode = (cBone*)e_pBone->GetParent();
	int	l_iLevel = 0;
	while( l_pParentNode )
	{
		cGameApp::OutputDebugInfoString(L"-----");
		l_pParentNode = (cBone*)l_pParentNode->GetParent();
		l_iLevel++;
	}
	WCHAR	l_str[MAX_PATH];
	swprintf(l_str,MAX_PATH,L"Name:%ls\n",e_pBone->GetName());
	cGameApp::OutputDebugInfoString(l_str);
	
	if( e_pBone->GetFirstChild() )
	{
		DumpBoneInfo((cBone*)e_pBone->GetFirstChild());
	}

	if( e_pBone->GetNextSibling() )
	{
		DumpBoneInfo((cBone*)e_pBone->GetNextSibling());
	}
}

cBone*	sVisualSceneLibrary::sSkeletonNode::AddChildBone(sNode*e_pMeshNode,cBone*e_pParentBone)
{
	assert(e_pMeshNode->NodeType == sNode::eNT_JOINT );
	const WCHAR*l_strName = e_pMeshNode->strName;
	if( l_strName == 0 )
		l_strName = L"NoName!?";
	cBone*l_pBone = new cBone(l_strName);
	l_pBone->SetDestroyConnectionWhileDestoruction(false);			
	//e_pParentBone->AddChild(l_pBone);//should set sibling is faster,not only add child add parent
	l_pBone->SetParent(e_pParentBone);
	l_pBone->SetLocalTransform(e_pMeshNode->mat);
	m_AllBoneData.AddObjectNeglectExist(l_pBone);
	AllNodesOfSkeletonNodeVector.push_back(e_pMeshNode);
#ifdef DEBUG
	if( e_pMeshNode->strSID )
		l_pBone->m_strSID = e_pMeshNode->strSID;
#endif
	return l_pBone;
}

cBone*	sVisualSceneLibrary::sSkeletonNode::AddSiblingBone(sNode*e_pMeshNode,cBone*e_pPreviousBone)
{
	//it's possible a node iwth has children under it which are joints....fucling weired.
	//assert(e_pMeshNode->NodeType == sNode::eNT_JOINT );
	const WCHAR*l_strName = e_pMeshNode->strName;
	if( l_strName == 0 )
		l_strName = L"NoName!?";
	cBone*l_pBone = new cBone(l_strName);
	e_pPreviousBone->SetNextSibling(l_pBone);
	l_pBone->SetLocalTransform(e_pMeshNode->mat);
	l_pBone->SetDestroyConnectionWhileDestoruction(false);
	m_AllBoneData.AddObjectNeglectExist(l_pBone);
	AllNodesOfSkeletonNodeVector.push_back(e_pMeshNode);
#ifdef DEBUG
	if( e_pMeshNode->strSID )
		l_pBone->m_strSID = e_pMeshNode->strSID;
#endif
	return l_pBone;
}

void	sVisualSceneLibrary::sSkeletonNode::GenerateSkeleton()
{
	m_AllBoneData.Destroy();
	//assert(pRootNode->NodeType == sVisualSceneLibrary::sNode::eNT_JOINT&&"not a joint");
	sNode*l_pSibling = pRootNode->pSibling;
	//just search it's childrent
	pRootNode->pSibling = 0;
	AllNodesOfSkeletonNodeVector.push_back(pRootNode);
	const WCHAR*l_strSID = 0;
	const WCHAR*l_strName = 0;
	if(pRootNode->strName)
		l_strName = pRootNode->strName;
	else
		l_strName = L"";
	if(pRootNode->strSID)
		l_strSID = pRootNode->strSID;
	else
		l_strSID = L"";
	assert( pRootBone == 0 && "GenerateSkeleton has called!!" );
	pRootBone = new cBone(l_strName);
	pRootBone->SetLocalTransform(pRootNode->mat);
	m_AllBoneData.AddObjectNeglectExist(pRootBone);
	GenerateSkeleton(pRootNode,pRootBone);
	pRootNode->pSibling = l_pSibling;
	//kill all bone is not joint
#ifdef DEBUG
	if( pRootNode->strSID )
		pRootBone->m_strSID = pRootNode->strSID;
	else
		pRootBone->m_strSID = L"NoData!!";
	//DumpBoneInfo(pRootBone);
#endif
	for(int i=0;i<m_AllBoneData.Count();++i)
	{
		if(wcslen(m_AllBoneData[i]->GetName()) == 0)
		{
			m_AllBoneData.RemoveObject(i);
			--i;
		}
	}
}

void	sVisualSceneLibrary::sSkeletonNode::GenerateSkeleton(sVisualSceneLibrary::sNode*e_pNode,cBone*e_pParent)//via pMeshNode to generate hierachy for children
{
	const WCHAR*l_strSID = 0;
	const WCHAR*l_strName = 0;
	sNode*l_pCurrentNode = e_pNode->pFirstChild;
	if( l_pCurrentNode )
	{
		//fuck it should have SID here,but I will still keep the data as bone,but once the bone is not store in the controller's source(NameArray)
		//do as follow
		//if( !l_pCurrentNode->strSID )
		//{
			//assert(!l_pCurrentNode->pFirstChild&&"this is not a joint");
			//assert(0&&"");
		//}
		bool	l_bContainJoint = false;
		IsNodeContainJoints(l_pCurrentNode,&l_bContainJoint);
		if( l_bContainJoint )
		{
			l_pCurrentNode->NodeType = sNode::eNT_JOINT;
			assert(l_pCurrentNode->strInstanceCamera == 0);
		}
		if( l_pCurrentNode->NodeType == sNode::eNT_JOINT )
		{
			GenerateSkeleton(l_pCurrentNode,AddChildBone(l_pCurrentNode,e_pParent));
		}
		else
		{
			assert(l_pCurrentNode->pFirstChild == 0&&"this node should't have child in the skeleton system");
			if(l_pCurrentNode->pSibling)
			{
				GenerateSkeleton(l_pCurrentNode->pSibling,AddChildBone(l_pCurrentNode->pSibling,e_pParent));
			}
		}
	}
	l_pCurrentNode = e_pNode->pSibling;
	if( l_pCurrentNode )
	{
		if( l_pCurrentNode->NodeType == sNode::eNT_JOINT || l_pCurrentNode->pFirstChild )
		{//it's a node but...it also has joint under it......fuck...why it's a node
			l_pCurrentNode->NodeType = sNode::eNT_JOINT;
			GenerateSkeleton(l_pCurrentNode,AddSiblingBone(l_pCurrentNode,e_pParent));
		}
		else
		{
			//assert(l_pCurrentNode->pFirstChild == 0&&"this node should't have child in the skeleton system");
			if(l_pCurrentNode->pSibling)
			{
				GenerateSkeleton(l_pCurrentNode->pSibling,AddSiblingBone(l_pCurrentNode->pSibling,e_pParent));
			}
		}
	}
}

//void	sAllVisualSceneNodeData::GetRelateatedBoneCount(sVisualSceneLibrary::sNode*e_pNode,int*e_piNumeBones)
//{
//	if( e_pNode->pFirstChild )
//	{
//		GetRelateatedBoneCount(e_pNode->pFirstChild,e_piNumeBones);
//	}
//
//	if( e_pNode->pSibling )
//	{
//		GetRelateatedBoneCount(e_pNode->pSibling,e_piNumeBones);
//	}
//	++(*e_piNumeBones);
//}
//
//sVisualSceneLibrary::sNode*		sAllVisualSceneNodeData::FindUsableRootNode()
//{
//	sVisualSceneLibrary::sNode*l_pCurrentNode = pRootNode;
//	assert(!this->pRootNode->pSibling);
//	assert(!this->pRootNode->pParent);
//	while( l_pCurrentNode )
//	{
//		if( l_pCurrentNode->pFirstChild->pSibling )
//			return l_pCurrentNode;
//		l_pCurrentNode = l_pCurrentNode->pFirstChild;
//	}
//	return 0;
//}
void	sVisualSceneLibrary::IsNodeContainJoints(sNode*e_pNode,bool *e_pbValue)
{
	if(e_pNode->NodeType == sNode::eNT_JOINT)
	{
		*e_pbValue = true;
		return;
	}
	sNode*l_pCurrentNode = e_pNode->pFirstChild;
	if( l_pCurrentNode&& *e_pbValue == false )
	{
		IsNodeContainJoints(l_pCurrentNode,e_pbValue);
	}
	l_pCurrentNode = e_pNode->pSibling;
	if( l_pCurrentNode&& *e_pbValue == false )
	{
		IsNodeContainJoints(l_pCurrentNode,e_pbValue);
	}
}
//=============================================================
//remove main root bone's sibling,we just wanna a skelton system here
//=============================================================
void	sVisualSceneLibrary::GenerateSkeltonRootBone()
{
	sNode*l_pCurrentNode = pRootNode->pFirstChild;
	if( !l_pCurrentNode->pSibling )
		l_pCurrentNode = l_pCurrentNode->pFirstChild;
	while(l_pCurrentNode)
	{
		bool	l_bIsJoint = false;
		if( l_pCurrentNode->pFirstChild )
			IsNodeContainJoints(l_pCurrentNode->pFirstChild,&l_bIsJoint);
		else
		if( l_pCurrentNode->NodeType == sNode::eNT_JOINT  )
		{
			l_bIsJoint = true;
		}
		if( l_bIsJoint )
		{
			sSkeletonNode*l_pSkeletonNode = new sSkeletonNode;
			l_pSkeletonNode->pRootNode = l_pCurrentNode;
			l_pSkeletonNode->GenerateSkeleton();
			this->AllSkeletonNodeVector.push_back(l_pSkeletonNode);
		}
		l_pCurrentNode = l_pCurrentNode->pSibling;
	}
#ifdef DEBUG
	for( UINT i=0;i<AllInstanceNodesVector.size();++i )
	{
		if(AllInstanceNodesVector[i]->pInstance->IndtanceType == sVisualSceneLibrary::sNode::eIT_CONTROLLER)
			assert(AllSkeletonNodeVector.size()>0&&"search skeleton failed!!Not only search the main root's first child's silbling");
	}
#endif
}
//
//
//
void	cLibraryVisualScenes::ParseData(TiXmlElement*e_pTiXmlElement)
{
	m_VisualSceneData.fEndTime = -1.f;
	m_VisualSceneData.fStartTime = -1.f;
	m_VisualSceneData.fFPrameRate = 0.f;
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene]))
		{
			Parsevisual_scene(l_pChildTiXmlElement);
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
	this->m_VisualSceneData.GenerateSkeltonRootBone();
}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	m_VisualSceneData.pRootNode = new sVisualSceneLibrary::sNode(0);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
			m_VisualSceneData.strVisualSceneID = l_strValue;
			m_VisualSceneData.pRootNode->strSID = m_VisualSceneData.strVisualSceneID;
		}
		else
		if(!wcscmp(l_strName,L"name"))
		{
			m_VisualSceneData.strVisualSceneName = l_strValue;
			m_VisualSceneData.pRootNode->strName = m_VisualSceneData.strVisualSceneName;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	//here is a little bit stupid,but it works :(
	//haha......fuck
	m_VisualSceneData.AllNodesVector.push_back(m_VisualSceneData.pRootNode);
	sVisualSceneLibrary::sNode*l_pPreviousVisualSceneLibrary = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node]))
		{
			sVisualSceneLibrary::sNode*l_pNode = new sVisualSceneLibrary::sNode(m_VisualSceneData.pRootNode);
			if( !m_VisualSceneData.pRootNode->pFirstChild )
				m_VisualSceneData.pRootNode->pFirstChild = l_pNode;
			m_VisualSceneData.AllNodesVector.push_back(l_pNode);
			if( l_pPreviousVisualSceneLibrary )
				l_pPreviousVisualSceneLibrary->pSibling = l_pNode;
			l_pPreviousVisualSceneLibrary = l_pNode;
			Parsevisual_scene_node( l_pChildTiXmlElement,l_pNode );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_extra]))
		{
			Parsevisual_scene_extra(l_pChildTiXmlElement);
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
#ifdef DEBUG
	int	l_iNum = 0;
	//m_VisualSceneData.DumpNodeInfo(m_VisualSceneData.pRootNode,&l_iNum);
#endif
}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
#ifdef DEBUG
	bool	l_bShowDebugInfo = true;
#else
	bool	l_bShowDebugInfo = false;
#endif
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"name"))
		{
			e_pParentNode->strName = l_strValue;
		if( l_bShowDebugInfo )
		{
			cGameApp::OutputDebugInfoString(l_strValue);
			cGameApp::OutputDebugInfoString(L"\n");
		}
		}
		else
		if(!wcscmp(l_strName,L"type"))
		{
			if( !wcscmp(l_strValue,L"JOINT") )
				e_pParentNode->NodeType = sVisualSceneLibrary::sNode::eNT_JOINT;
			else
			if( !wcscmp(l_strValue,L"NODE") )
				e_pParentNode->NodeType = sVisualSceneLibrary::sNode::eNT_NODE;
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			e_pParentNode->strID = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"sid"))
		{
			e_pParentNode->strSID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
//=========================================
//debug info
	if( l_bShowDebugInfo )
	if( g_strParsingDataList )
	{
		std::string	l_str;
		if( e_pParentNode->NodeType == sVisualSceneLibrary::sNode::eNT_JOINT )
		{
			l_str.append("Type:JOINT ");
			l_str.append(UT::WcharToChar(e_pParentNode->strName));
		}
		else
		if( e_pParentNode->NodeType == sVisualSceneLibrary::sNode::eNT_NODE )
		{
			l_str.append("Type:Node ");
			if( e_pParentNode->strName )
				l_str.append(WcharToChar(e_pParentNode->strName));
			else
				l_str.append(WcharToChar(e_pParentNode->strID));
		}
		else
		{
			l_str.append("Type:None ");
			if( e_pParentNode->strName )
			{
				l_str.append(WcharToChar(e_pParentNode->strName));
			}
			else
				l_str.append("!?");
		}
		if( e_pParentNode->strSID )
		{
			l_str.append("   SID:");
			l_str.append(WcharToChar(e_pParentNode->strSID));
		}
		else
		{
			l_str.append("   SID:");
			l_str.append("no SID");		
		}
		l_str.append("\n");
		g_strParsingDataList->push_back(l_str);
	}
//=========================================
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_matrix]))
		{
			Parsevisual_scene_node_matrix( l_pChildTiXmlElement,e_pParentNode );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node]))
		{
//======================================================
//debug info
		sVisualSceneLibrary::sNode*l_pNode2 = e_pParentNode->pParent;
		if( l_bShowDebugInfo )
		while( l_pNode2 )
		{
			cGameApp::OutputDebugInfoString(L"---");
			l_pNode2 = l_pNode2->pParent;
		}
//======================================================
			sVisualSceneLibrary::sNode*l_pNode = new sVisualSceneLibrary::sNode(e_pParentNode);
			m_VisualSceneData.AllNodesVector.push_back(l_pNode);
			//add into firest child if it's emtpy,or find appropriate child and set new node as his sibling
			if(!e_pParentNode->pFirstChild)
			{
				e_pParentNode->pFirstChild = l_pNode;
				if( e_pParentNode  == l_pNode )
					assert(0);
			}
			else
			{
				sVisualSceneLibrary::sNode*l_pDestNode = e_pParentNode->pFirstChild;
				while( l_pDestNode->pSibling )
				{
					l_pDestNode = l_pDestNode->pSibling;
				}
				l_pDestNode->pSibling = l_pNode;
				if( l_pDestNode  == l_pNode )
					assert(0);
			}
			Parsevisual_scene_node( l_pChildTiXmlElement,l_pNode );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_geometry])||
			!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_controller]))
		{
			assert(e_pParentNode->pInstance == 0);
			e_pParentNode->pInstance = new sVisualSceneLibrary::sNode::sInstance();
			if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_geometry]))
				e_pParentNode->pInstance->IndtanceType = sVisualSceneLibrary::sNode::eIT_GEOMETRY;
			else
			if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_controller]))
				e_pParentNode->pInstance->IndtanceType = sVisualSceneLibrary::sNode::eIT_CONTROLLER;
			else
				assert(0);
			Parsevisual_scene_node_instance( l_pChildTiXmlElement,e_pParentNode );
			m_VisualSceneData.AllInstanceNodesVector.push_back(e_pParentNode);
		}
		else
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_extra]))
		{
			if( l_bShowDebugInfo )
				cGameApp::OutputDebugInfoString(L" visual scene extra data have not implement yet\n ");
		}
		else
		if(	!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_translate])||
			!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_scale])||
			!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_rotate]))
		{
			if( l_bShowDebugInfo )
				cGameApp::OutputDebugInfoString(L" SRT current not implement yet,checked bake marix while export\n ");
		}
		else
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_light]))
		{
			Parsevisual_scene_node_instance_light(l_pChildTiXmlElement,e_pParentNode);
		}
		else
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_camera]))
		{
			Parsevisual_scene_node_instance_camera(l_pChildTiXmlElement,e_pParentNode);
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node_matrix(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"sid"))
		{
			e_pParentNode->strMatrixSid = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	//if( e_pParentNode->strMatrixSid )
	{
		//assert( e_pParentNode->NodeType == sVisualSceneLibrary::sNode::eNT_JOINT||e_pParentNode->NodeType == sVisualSceneLibrary::sNode::eNT_INSTANCE );
		const WCHAR*l_strText = e_pTiXmlElement->GetText();
		assert(l_strText);
		e_pParentNode->mat = GetMatrix(l_strText);
	}

}

//void	cLibraryVisualScenes::Parsevisual_scene_node_instance_Geometry(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
//{
//
//}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node_instance(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"url"))
		{
			e_pParentNode->pInstance->strUrl = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_skeleton]))
		{
			Parsevisual_scene_node_instance_skeleton( l_pChildTiXmlElement,e_pParentNode );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_bind_material]))
		{
			Parsevisual_scene_node_instance_bind_material( l_pChildTiXmlElement,e_pParentNode );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node_instance_skeleton(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);
	e_pParentNode->pInstance->strSkeletonVector.push_back(l_strText);

}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node_instance_bind_material(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_bind_material_technique_common]))
		{
			Parsevisual_scene_node_instance_bind_material_technique_common( l_pChildTiXmlElement,e_pParentNode );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node_instance_bind_material_technique_common(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_bind_material_technique_common_instance_material]))
		{
			Parsevisual_scene_node_instance_bind_material_technique_common_instance_material( l_pChildTiXmlElement,e_pParentNode );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node_instance_bind_material_technique_common_instance_material(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"symbol"))
		{
			e_pParentNode->pInstance->strMaterialStmbol = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"target"))
		{
			e_pParentNode->pInstance->strMaterialTarget = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_node_instance_bind_material_technique_common_instance_material_bind_vertex_input]))
		{
			Parsevisual_scene_node_instance_bind_material_technique_common_instance_material_bind_vertex_input( l_pChildTiXmlElement,e_pParentNode );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node_instance_bind_material_technique_common_instance_material_bind_vertex_input(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"semantic"))
		{
			e_pParentNode->pInstance->BineVertexInput.strSemantic = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"input_semantic"))
		{
			e_pParentNode->pInstance->BineVertexInput.strInputSemantic = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"input_set"))
		{
			e_pParentNode->pInstance->BineVertexInput.strInputSet = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node_instance_camera(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"url"))
		{
			e_pParentNode->strInstanceCamera = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_node_instance_light(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"url"))
		{
			//l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_extra(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_extra_technique]))
		{
			Parsevisual_scene_extra_technique( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_extra_technique(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"profile"))
		{
			//l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_extra_technique_start_time]))
		{
			Parsevisual_scene_extra_technique_start_time( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_extra_technique_end_time]))
		{
			Parsevisual_scene_extra_technique_end_time( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryVisualScenesElement[eLVS_visual_scene_extra_technique_frame_rate]))
		{
			Parsevisual_scene_extra_technique_frame_rate( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_extra_technique_start_time(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);
	m_VisualSceneData.fStartTime = (float)_wtof(l_strText);
}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_extra_technique_end_time(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);
	m_VisualSceneData.fEndTime = (float)_wtof(l_strText);

}
//
//
//
void	cLibraryVisualScenes::Parsevisual_scene_extra_technique_frame_rate(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);
	m_VisualSceneData.fFPrameRate = (float)_wtof(l_strText);

}

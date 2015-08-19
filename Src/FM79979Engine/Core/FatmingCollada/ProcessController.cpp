#include "StdAfx.h"
#include "ColladaParser.h"
#include "AnimationMesh.h"

void	cColladaParser::ProcessController()
{
	if( !m_pAllAnimationMesh )
		m_pAllAnimationMesh = new cNamedTypedObjectVector<cAnimationMesh>;
	//search all skeleton node,and fetch controller then assign coorespond data
	UINT	l_iNumInstanceNode = m_LibraryVisualScenes.m_VisualSceneData.AllInstanceNodesVector.size();
	for( UINT i=0;i<l_iNumInstanceNode;++i )
	{
		sVisualSceneLibrary::sNode*l_pNode = m_LibraryVisualScenes.m_VisualSceneData.AllInstanceNodesVector[i];
		sVisualSceneLibrary::sNode::sInstance*l_pInstance = l_pNode->pInstance;
		if( !l_pInstance )
			continue;
		if( l_pInstance->IndtanceType == sVisualSceneLibrary::sNode::eIT_CONTROLLER )//find skeleton data
		{
			WCHAR	l_strName[MAX_PATH];
			memset(l_strName,0,sizeof(WCHAR)*MAX_PATH);
			int	l_iLastSlash = FindChacterIndexFromBackBuffer((WCHAR*)l_pNode->pInstance->strUrl,L'-');
			if( l_iLastSlash != -1 )
				memcpy(l_strName,&l_pNode->pInstance->strUrl[1],sizeof(WCHAR)*(l_iLastSlash-1));
			sController*l_pController = this->m_LibraryControllers.GetController((WCHAR*)&l_pInstance->strUrl[1]);
			if( !l_pController )
			{
				cGameApp::OutputDebugInfoString(L"fatal error!!! void	cColladaParser::ProcessController()");
				continue;
			}
			cMesh*l_pMesh = 0;
			sColladaGeomatries*l_pColladaGeomatries = m_LibraryGeometries.GetGeometryByID(l_strName);
			if( l_pColladaGeomatries )
			{
				//it should only has one mesh
				assert(l_pColladaGeomatries->pGeometryMesh->AllMeshIndexList.size() == 1);
				l_pMesh = l_pColladaGeomatries->pGeometryMesh->AllMeshIndexList[0];
			}
			cAnimationMesh*l_pAnimationMesh = new cAnimationMesh(l_pMesh,l_pController->Skin.matBindShapeMatrix);
			if( !l_pMesh )
				l_pAnimationMesh->SetName(l_pController->strID);
			m_pAllAnimationMesh->AddObject(l_pAnimationMesh);

			assert(wcscmp(l_pController->strID,&l_pInstance->strUrl[1])==0);
			sController::sSkin::sSource*l_pBindPoseSource = l_pController->Skin.GetSourceByType(sController::sSkin::eCT_BINDPOSE);
			sController::sSkin::sSource*l_pJointsSource = l_pController->Skin.GetSourceByType(sController::sSkin::eCT_JOINT);


			size_t	l_uiNumSkeletonName = l_pInstance->strSkeletonVector.size();
			cNamedTypedObjectVector<cBone>*l_pAllBoneList = new cNamedTypedObjectVector<cBone>;
			std::vector<sVisualSceneLibrary::sNode*>		l_AllBoneData;
			for( size_t l_iSkeletonCount=0;l_iSkeletonCount<l_uiNumSkeletonName;++l_iSkeletonCount )
			{
				sVisualSceneLibrary::sSkeletonNode*l_pSkeletonNode = m_LibraryVisualScenes.m_VisualSceneData.GetSkeletonNode(&l_pInstance->strSkeletonVector[l_iSkeletonCount][1]);
				assert(l_pSkeletonNode);
				l_pAllBoneList->AddListToMe(&l_pSkeletonNode->m_AllBoneData,true,true);
				//has been dumped so clear it
				l_pSkeletonNode->m_AllBoneData.GetList()->clear();
				size_t	l_AllNodesOfSkeletonNodeVectorSize = l_pSkeletonNode->AllNodesOfSkeletonNodeVector.size();
				for( size_t l_iNumNodes = 0;l_iNumNodes<l_AllNodesOfSkeletonNodeVectorSize;++l_iNumNodes )
				{
					l_AllBoneData.push_back(l_pSkeletonNode->AllNodesOfSkeletonNodeVector[l_iNumNodes]);
				}
			}
			//cNamedTypedObjectVector<cBone>*l_pAllBoneList = &l_pSkeletonNode->m_AllBoneData;
			cNamedTypedObjectVector<cBone> l_BoneListByOrder;//have to take a order by name array,so joints influnce index would be righ
			if( l_pAllBoneList->Count() == 0 )
			{
				this->m_pAllAnimationMesh->RemoveObject(l_pAnimationMesh->GetName());
				l_pAnimationMesh = 0;
				if( l_pMesh )
				{
					cAnimationMesh*l_pExistsAnimationMesh = this->m_pAllAnimationMesh->GetObject(l_pMesh->GetName());
					if( l_pExistsAnimationMesh )
						l_pAnimationMesh = new cAnimationMesh(l_pExistsAnimationMesh);
					else
					{
						UT::ErrorMsg(l_pMesh->GetName(),L"can't find such animation mesh data!!");
						cGameApp::OutputDebugInfoString(l_pMesh->GetName());
						cGameApp::OutputDebugInfoString(L" is not exists!!");
					}
				}
				continue;
			}
			assert(l_pAllBoneList->Count()>=l_pJointsSource->iCount&&"two controller use same skeleton?");
			for(int k=0;k<l_pJointsSource->iCount;++k)
			{
				std::wstring l_strName = l_pJointsSource->strAllJointName[k];
				//int	l_iIndex = l_pAllBoneList->GetObjectIndexByName(l_strName);
				int	l_iIndex = -1;
				//if( l_iIndex == -1 )
				{
					int	l_iAllNodesOfSkeletonNodeVectorSize = (int)l_AllBoneData.size();
					for( int l = 0;l<l_iAllNodesOfSkeletonNodeVectorSize;++l )
					{
						const WCHAR*l_strID = l_AllBoneData[l]->strSID;
						if( !l_strID )
							continue;
						std::wstring	l_strDebug2 = l_strID;
						if(wcscmp(l_strName.c_str(),l_strID) == 0)
						{
							l_iIndex = l;
							break;
						}
					}
#ifdef DEBUG
					if(	l_iIndex == -1||l_iIndex>=l_pAllBoneList->Count())
					{
						int a=0;
					}
#endif
				}
				assert(l_iIndex<=l_pAllBoneList->Count());
				cBone*l_pBone = l_pAllBoneList->GetObject(l_iIndex);
				if( !l_pBone )
					int a=0;
				assert(l_pBone&&"bone is not exist!");
				l_pBone->SetInvBindPose(l_pBindPoseSource->pMatrcies[k]);
				l_BoneListByOrder.AddObject(l_pBone);
			}
			//the bone are not need,so delete them
			//because I am stupid enough to delete those unnecessary node,so I got wrong world matrix...
			//I am fucking stupid...
			//if( l_pAllBoneList->Count() != l_BoneListByOrder.Count() )
			//{
			//	int	l_iTotalBone = l_pAllBoneList->Count();
			//	cBone**l_ppBoneList = &(*l_pAllBoneList->GetList())[0];
			//	for( int l_iCurrentBoneIndex=0;l_iCurrentBoneIndex<l_iTotalBone;++l_iCurrentBoneIndex )
			//	{
			//		cBone*l_pCurrentBoneForDelete = l_ppBoneList[l_iCurrentBoneIndex];
			//		cBone*l_pFirstChildBone = (cBone*)l_pCurrentBoneForDelete->GetFirstChild();
			//		if(!l_BoneListByOrder.GetObject(l_pCurrentBoneForDelete->GetName()))
			//		{//it is not need, delete it.
			//			if( l_pCurrentBoneForDelete->GetParent() )
			//			{
			//				//l_pCurrentBoneForDelete->GetParent()->DisconnectFromParent();
			//				cGameApp::OutputDebugInfoString(l_pCurrentBoneForDelete->GetName());
			//				cGameApp::OutputDebugInfoString(L"        !!!WHAT??fuck....why artist make this dummy joints?\n");
			//				//assert(l_pCurrentBoneForDelete->GetFirstChild() == 0&&"fuck....why artist make this dummy joints?");
			//				if( l_pFirstChildBone )
			//				{//ensure the parent only has this child or it's too complcated...
			//					Frame*l_pParentFrame = l_pCurrentBoneForDelete->GetParent();
			//					if( l_pParentFrame )
			//					{
			//						l_pFirstChildBone->SetParent(l_pParentFrame);
			//					}
			//					else
			//					{
			//						l_pFirstChildBone->SetLocalTransform(l_pFirstChildBone->GetWorldTransform());
			//					}
			//				}
			//				l_pCurrentBoneForDelete->SetParent(0);
			//			}
			//			else
			//			{
			//				if(l_pFirstChildBone)
			//				{
			//					Frame *pSrch, *pLast;
			//					pLast = nullptr;
			//					for( pSrch = l_pFirstChildBone; pSrch;pSrch = pSrch->GetNextSibling())
			//					{
			//						pSrch->SetLocalTransform(pSrch->GetWorldTransform());
			//					}
			//					for( pSrch = l_pFirstChildBone; pSrch;pSrch = pSrch->GetNextSibling())
			//					{
			//						Frame*l_pFrame = pSrch->GetNextSibling();
			//						pSrch->SetParent(0);
			//						if( l_pFrame )
			//							pSrch->SetNextSibling(l_pFrame);
			//					}
			//				}
			//			}
			//			SAFE_DELETE(l_pCurrentBoneForDelete);
			//		}
			//	}
			//}
			//clear all vector data without delete,because above has delete them,and l_BoneListByOrder store necessary.
			l_BoneListByOrder[0]->SetNextSibling(0);//if nex sibling is exiss,it's unnecessary.
			l_pAnimationMesh->SetMainRootBone(l_BoneListByOrder[0]);
			l_pAllBoneList->DumpListPointer(l_pAnimationMesh->GetAllBoneVector());
			SAFE_DELETE(l_pAllBoneList);
			l_BoneListByOrder.DumpListPointer(l_pAnimationMesh->GetSkinningBoneVector());
			l_pAnimationMesh->RefreshAnimationData();
			if( l_pAnimationMesh&&!l_pAnimationMesh->GetMainRootBone() )
			{
				m_pAllAnimationMesh->RemoveObject(l_pAnimationMesh);
			}
		}
		//else
		//if( l_pInstance->IndtanceType == sVisualSceneLibrary::sNode::eIT_GEOMETRY )//find geometry mesh
		//{
		//
		//}
	}
}
#pragma once

#include <list>
#include <map>
#include <vector>
//
//class cObjectForRenderLayer: public cRenderObject
//{
//	GET_SET_DEC(int, m_iGameObjectID, GetGameObjectID, SetGameObjectID);
//	GET_SET_DEC(int,m_iRenderLayerIndex,GetRenderLayerIndex,SetRenderLayerIndex);
//	GET_SET_DEC(bool,m_bAnimationDone, IsAnimationDone,SetAnimationDone);
//	GET_SET_DEC(int, m_iRequiredPlayerID, GetRequiredPlayerID, SetRequiredPlayerID);
//public:
//	cObjectForRenderLayer();
//	~cObjectForRenderLayer();
//};
//
//
//class cRenderLayer:public std::vector
//{
//	bool	m_bNeedToUpdateAnimationDoneObject;
//public:
//}; 
//
//class cRenderLayerManager:public cRenderObject
//{
//	//first is game object ID,second is the object created.
//	std::map<uint64,std::list<cRenderObject*>* >	m_AllObjects;
//	std::vector<int>				m_AvailableLayerIndex;
//	std::list< cRenderLayerBase* >*	m_RenderLayerListArray;
//public:
//	bool			AddObject(int e_iGameObjectID, cRenderObject*e_pRenderObject,int e_iCount);
//};
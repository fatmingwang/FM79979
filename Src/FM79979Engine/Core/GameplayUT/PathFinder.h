#ifndef _PATH_FINDER_H_
#define _PATH_FINDER_H_
namespace FATMING_CORE
{
//thanks feeling software inspite me this method
//bool FCDSceneNode::FindPathDFS(FCDSceneNode* from, FCDSceneNodeList& path) const
//{
//	path.push_back(from);
//
//	if (from == this)
//		return true;
//
//	size_t childrenCount = from->GetChildrenCount();
//	for (size_t i = 0; i < childrenCount; ++i)
//	{
//		FCDSceneNode* child = from->GetChild(i);
//		if (FindPathDFS(child, path))
//			return true;
//	}
//
//	size_t instanceCount = from->GetInstanceCount();
//	for (size_t i = 0; i < instanceCount; ++i)
//	{
//		FCDEntityInstance* inst = from->GetInstance(i);
//		if (inst->GetEntityType() == FCDEntity::SCENE_NODE || inst->GetEntityType() == FCDEntity::SCENE_NODE_LOD)		
//		{
//			FCDSceneNode* child = (FCDSceneNode*)inst->GetEntity();
//			if (FindPathDFS(child, path))
//				return true;
//		}
//	}
//
//	path.pop_back();
//	return false;
//}
//============
//first set all connected nodes
//and indiciate the start point
//then it will search all possible way to get result path.
//ex:
//
//a		b		c		d
//
//e		f		g		h
//
//i		j		k		l
//
//now b has linked a,f,c
//    c            b,g,d
//	  a			   e,b
//	  f			   e,b,j,g
//
//please set those link first then u could find a path to the target
//or just set a hirechy to speed up search time
//ex:
//		only set a direction for its children
//		f only has b,j,g
//		g          c,k,h
//		so    we could have a bit speed result for it now
//
//		
//============
	class	cLinkedNodes:public cNamedTypedObjectVector<cLinkedNodes>
	{
		GET_SET_DEC(POINT,m_Coordinate,GetCoordinate,SetCoordinate);
		//this could optmize for just input the shorest path count,but I am lazy...
		bool			FindTargetNode(cLinkedNodes*e_pTarget,cNamedTypedObjectVector<cLinkedNodes>*ParentPath,cNamedTypedObjectVector<cNamedTypedObjectVector<cLinkedNodes> >*e_pResultPathes,bool e_bFindShoest = false);
	public:
		//while the points of path is bigger than this value skip this path
		static	int		m_siMaxAllowCount;
		cLinkedNodes();
		cLinkedNodes(POINT e_Coordinate){ m_Coordinate = e_Coordinate; }
		virtual ~cLinkedNodes();
		void			RemoveObject(POINT e_Coordinate);
		cLinkedNodes*	GetObjectByCoordinate(POINT e_Coordinate);
		bool			IsSameCoordinate(POINT e_Coordinate);
		bool			FindTargetNode(cLinkedNodes*e_pTarget,cNamedTypedObjectVector<cNamedTypedObjectVector<cLinkedNodes> >*e_pResultPathes,bool e_bFindShoest = false);
	};

	//==================
	//here is a rectange map to demonstrator this theory
	//==================
	class	cLinkedMapManager:public cLinkedNodes
	{
		POINT	m_Size;
	public:
		//if e_bOptmizeFromLeftToRight is true,t will only add linked object on his right side
		cLinkedMapManager(int e_iRow,int e_iColumn,bool e_bColumnMajor = false,bool e_bOptmizeFromLeftToRight = false);
		virtual ~cLinkedMapManager();
		//optmize should be a direction not just right or left
		void	SetupLinkData(int e_iRow,int e_iColumn,bool e_bColumnMajor = false,bool e_bOptmizeFromLeftToRight = false);
		//it will disconnect relative data
		void	SetBlock(POINT e_Coordinate);
		//
		bool	FindTargetNode(POINT e_Point1,
							   POINT e_Point2,
							   cNamedTypedObjectVector<cNamedTypedObjectVector<cLinkedNodes> >*e_pResultPathes,bool e_bFindShoest = false);
	};
	//sample
	//how to find way to go.
	//cGameApp::OutputDebugInfoString(L"=====================\n");
	//cNamedTypedObjectVector<cNamedTypedObjectVector<cLinkedNodes>> l_ResultPathes;
	//cLinkedMapManager	l_LinkedMapManager(4,4,false,false);
	//POINT	l_Point1 = {1,1};
	//POINT	l_Point2 = {1,2};
	//POINT	l_Point3 = {2,1};
	//POINT	l_Point4 = {2,2};
	//l_LinkedMapManager.SetBlock(l_Point1);
	//l_LinkedMapManager.SetBlock(l_Point2);
	//l_LinkedMapManager.SetBlock(l_Point3);
	//l_LinkedMapManager.SetBlock(l_Point4);
	//cLinkedNodes*l_pLinkedNodes = l_LinkedMapManager.GetObject(0);
	//l_pLinkedNodes->FindTargetNode(l_LinkedMapManager.GetObject(3),&l_ResultPathes);
	//cGameApp::OutputDebugInfoString(L"ResultCount:");
	//cGameApp::OutputDebugInfoString(ValueToStringW(l_ResultPathes.Count()));
	//cGameApp::OutputDebugInfoString(L"\n");
	//for(int	i = 0;i<l_ResultPathes.Count();++i)
	//{
	//	cGameApp::OutputDebugInfoString(L"Path");
	//	cGameApp::OutputDebugInfoString(ValueToStringW(i));
	//	cGameApp::OutputDebugInfoString(L":");
	//	cNamedTypedObjectVector<cLinkedNodes>*l_pPath = l_ResultPathes.GetObject(i);
	//	for(int	j = 0;j<l_pPath->Count();++j)
	//	{
	//		cGameApp::OutputDebugInfoString(l_pPath->GetObject(j)->GetName());
	//		cGameApp::OutputDebugInfoString(L" __ ");
	//	}
	//	cGameApp::OutputDebugInfoString(L"\n");
	//}
	//

	//l_LinkedMapManager.Clear();
	//cGameApp::OutputDebugInfoString(L"=====================\n");
}
#endif
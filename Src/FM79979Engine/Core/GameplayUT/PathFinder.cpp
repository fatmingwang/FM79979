#include "../stdafx.h"
#include "PathFinder.h"
namespace FATMING_CORE
{

	int		cLinkedNodes::m_siMaxAllowCount = 79979;

	cLinkedNodes::cLinkedNodes()
	{
		this->m_bFromResource = true;
	}
	cLinkedNodes::~cLinkedNodes()
	{
		this->Destroy();
	}

	bool	cLinkedNodes::IsSameCoordinate(POINT e_Coordinate)
	{
		if(this->m_Coordinate.x == e_Coordinate.x &&
			this->m_Coordinate.y == e_Coordinate.y )
			return true;
		return false;
	}

	void	cLinkedNodes::RemoveObject(POINT e_Coordinate)
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			if(this->m_ObjectList[i]->IsSameCoordinate(e_Coordinate))
			{
				int	l_iIndex = this->GetObjectIndexByPointer(m_ObjectList[i]);
				cNamedTypedObjectVector<cLinkedNodes>::RemoveObject(l_iIndex);
			}
		}	
	}

	cLinkedNodes*	cLinkedNodes::GetObjectByCoordinate(POINT e_Coordinate)
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			if(this->m_ObjectList[i]->IsSameCoordinate(e_Coordinate))
				return m_ObjectList[i];
		}
		return 0;
	}
	bool	cLinkedNodes::FindTargetNode(cLinkedNodes*e_pTarget,cNamedTypedObjectVector<cLinkedNodes>*e_pParentPath,cNamedTypedObjectVector<cNamedTypedObjectVector<cLinkedNodes> >*e_pResultPathes,bool e_bFindShoest)
	{
		e_pParentPath->AddObjectNeglectExist(this);
		//only need shorest path so compare it
		if( e_bFindShoest && e_pResultPathes->Count() )
		{
			int	l_iCount = e_pResultPathes->GetObject(0)->Count();
			if( e_pParentPath->Count() > l_iCount )
				return false;
		}

		if( this == e_pTarget )
		{//u could compare count if the count is bigger than the path we found,ignore it,so we could find the smallest one

			if( e_bFindShoest  )
			{
				if( e_pResultPathes->Count() )
				{
					int	l_iCount = e_pResultPathes->GetObject(0)->Count();
					int	l_iNewPathCount = e_pParentPath->Count();
					//only need shorest
					if( l_iNewPathCount > l_iCount )
						return false;
					if( l_iNewPathCount < l_iCount )
					{//clear old data just need new one.
						e_pResultPathes->Destroy();
					}
					//if same count
					//do nothing and add it again
				}
			}
			cNamedTypedObjectVector<cLinkedNodes>*l_pResult = new cNamedTypedObjectVector<cLinkedNodes>();
			l_pResult->SetFromResource(true);
			l_pResult->CopyListPointer(e_pParentPath);
			e_pResultPathes->AddObjectNeglectExist(l_pResult);
			return true;
		}
		if(  e_pParentPath->Count() >= m_siMaxAllowCount )
		{
			return false;		
		}

		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cLinkedNodes*l_pChild = this->m_ObjectList[i];
			if( e_pParentPath->GetObjectIndexByPointer(l_pChild) != -1 )
				continue;
			cNamedTypedObjectVector<cLinkedNodes>l_ObjectListByName;
			l_ObjectListByName.SetFromResource(true);
			l_ObjectListByName.CopyListPointer(e_pParentPath);
			if(l_pChild->FindTargetNode(e_pTarget,&l_ObjectListByName,e_pResultPathes,e_bFindShoest))
			{

			}
		}
		return false;
	}

	bool	cLinkedNodes::FindTargetNode(cLinkedNodes*e_pTarget,cNamedTypedObjectVector<cNamedTypedObjectVector<cLinkedNodes> >*e_pResultPathes,bool e_bFindShoest)
	{
		bool	l_bFind = false;
		//linked object
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cLinkedNodes*l_pChild = this->m_ObjectList[i];
			cNamedTypedObjectVector<cLinkedNodes> l_ObjectListByName;
			l_ObjectListByName.SetFromResource(true);
			l_ObjectListByName.AddObjectNeglectExist(this);
			if( l_pChild->FindTargetNode(e_pTarget,&l_ObjectListByName,e_pResultPathes,e_bFindShoest) )
			{
				l_bFind = true;
			}
		}
		return l_bFind;
	}

	//if e_bOptmizeFromLeftToRight is true,t will only add linked object on his right side
	cLinkedMapManager::cLinkedMapManager(int e_iRow,int e_iColumn,bool e_bColumnMajor,bool e_bOptmizeFromLeftToRight)
	{
		this->m_bFromResource = false;
		SetupLinkData(e_iRow,e_iColumn,e_bColumnMajor,e_bOptmizeFromLeftToRight);
	}

	cLinkedMapManager::~cLinkedMapManager()
	{
		this->Destroy();
	}

	void	cLinkedMapManager::SetBlock(POINT e_Coordinate)
	{
		int	l_iIndexForData = (e_Coordinate.y*m_Size.x)+e_Coordinate.x;
		cLinkedNodes*l_pTargetLinkedNodes = this->GetObject(l_iIndexForData);
		RECT	l_rc = {0,0,m_Size.x-1,m_Size.y-1};
		POINT	l_Link[4] = {{e_Coordinate.x-1,e_Coordinate.y},{e_Coordinate.x,e_Coordinate.y-1},{e_Coordinate.x+1,e_Coordinate.y},{e_Coordinate.x,e_Coordinate.y+1}};
		for(int k=0 ;k<4;++k )
		{
			if( UT::RectCollidePoint(l_rc,l_Link[k]) )
			{
				int	l_iTagetIndex = l_Link[k].y*m_Size.x+l_Link[k].x;
				cLinkedNodes*l_pReleativeLinkedNodes = m_ObjectList[l_iTagetIndex];
				int	l_iIndex = l_pReleativeLinkedNodes->GetObjectIndexByPointer(l_pTargetLinkedNodes);
				//if -1 SetBlock might be called or optmize parameter is set as true
				if( l_iIndex != -1 )
				{
					l_pReleativeLinkedNodes->RemoveObjectWithoutDelete( l_iIndex );
				}
				l_iIndex = l_pTargetLinkedNodes->GetObjectIndexByPointer(l_pReleativeLinkedNodes);
				if( l_iIndex != -1 )
					l_pTargetLinkedNodes->RemoveObjectWithoutDelete(l_iIndex);
			}
		}
	}

	void	cLinkedMapManager::SetupLinkData(int e_iRow,int e_iColumn,bool e_bColumnMajor,bool e_bOptmizeFromLeftToRight)
	{
		this->Destroy();
		if( !e_bColumnMajor )
		{
			int	l_iRow = e_iRow;
			int	l_iColumn = e_iColumn;
			e_iRow = l_iColumn;
			e_iColumn = l_iRow;
		}
		this->m_ObjectList.reserve(e_iRow*e_iColumn);
		m_Size.x = e_iRow;
		m_Size.y = e_iColumn;
		for( int i=0;i<e_iRow;++i )
		{
			for( int j=0;j<e_iColumn;++j )	
			{
				POINT	l_Coordinate = {j,i};
				int	l_iIndexForData = (i*m_Size.x)+j;
				cLinkedNodes*l_pLinkedNodes = new cLinkedNodes();
				l_pLinkedNodes->SetCoordinate(l_Coordinate);
				l_pLinkedNodes->SetName(ValueToStringW(l_Coordinate));
				this->AddObjectNeglectExist(l_pLinkedNodes);
			}
		}
		RECT	l_rc = {0,0,e_iColumn-1,e_iRow-1};
		for( int i=0;i<e_iRow;++i )	
		{
			for( int j=0;j<e_iColumn;++j )
			{
				int	l_iIndexForData = (i*m_Size.x)+j;
				POINT	l_Link[4] = {{j-1,i},{j,i-1},{j+1,i},{j,i+1}};
				int	k=0;
				//skip left side?
				if( e_bOptmizeFromLeftToRight )
					k=1;
				for( ;k<4;++k )
				{
					if( UT::RectCollidePoint(l_rc,l_Link[k]) )
					{
						int	l_iTagetIndex = l_Link[k].y*m_Size.x+l_Link[k].x;
						this->m_ObjectList[l_iIndexForData]->AddObjectNeglectExist(m_ObjectList[l_iTagetIndex]);
					}
				}
			}
		}
	}

	bool	cLinkedMapManager::FindTargetNode(POINT e_Point1,POINT e_Point2,cNamedTypedObjectVector<cNamedTypedObjectVector<cLinkedNodes> >*e_pResultPathes,bool e_bFindShoest)
	{
		if( !e_pResultPathes )
			return false;
		int	l_iIndexForData1 = (e_Point1.y*m_Size.x)+e_Point1.x;
		int	l_iIndexForData2 = (e_Point2.y*m_Size.x)+e_Point2.x;
		int	l_iCount = this->Count();
		if( l_iIndexForData1>=l_iCount||l_iIndexForData1>=l_iCount )
			return false;
		cLinkedNodes*l_pPoint1 = this->m_ObjectList[l_iIndexForData1];
		cLinkedNodes*l_pPoint2 = this->m_ObjectList[l_iIndexForData2];
		l_pPoint1->FindTargetNode(l_pPoint2,e_pResultPathes,e_bFindShoest);
		e_pResultPathes->SetFromResource(false);
		return e_pResultPathes->Count()>0?true:false;
	}
}
#include "stdafx.h"
#include "LevelLayerData.h"
namespace FATMING_CORE
{
	bool	cLayer::m_sbIsEditorMode = false;
	cLayer::cLayer()
	{
		m_bInGameRender = true;
		m_bInEditorRender = true;
		m_bIsLayerOptmizedForSameTexture = false;
	}

	cLayer::cLayer(cLayer*e_pLayer):cNamedTypedObjectVectorWithData<cLevelLayerGridData>(e_pLayer)
	{
		m_bInGameRender = e_pLayer->m_bInGameRender;
		m_bInEditorRender = e_pLayer->m_bInEditorRender;
		m_bIsLayerOptmizedForSameTexture = e_pLayer->m_bIsLayerOptmizedForSameTexture;
	}

	cLayer::~cLayer()
	{
	
	}


	void	cLayer::Render()
	{
		bool	l_bRender = false;
		if( m_sbIsEditorMode && m_bInEditorRender )
		{
			l_bRender = true;
		}
		else
		{
			l_bRender = m_bInGameRender;
		}
		if( l_bRender)
		{
			//lazy to do this right now.
			//if( m_bIsLayerOptmizedForSameTexture == false )
			{
				int	l_iCount = this->Count();
				for (int i = 0; i < l_iCount; i++)
				{
					this->GetObject(i)->Render();
				}
			}
			//else
			//{
			//
			//}
		}

	}

	void	cLayer::Update(float e_fElpaseTime)
	{
		int	l_iCount = this->Count();
		for (int i = 0; i < l_iCount; i++)
		{
			this->GetObject(i)->Update(e_fElpaseTime);
		}
	}

	bool	cLayer::IsOptimzeWithSameTextureLayer()
	{
		bool	l_bOptmizeForSameTexture = true;
		cTexture*l_pTexture = nullptr;
		int	l_iCount = this->Count();
		if( l_iCount < 2 )
			return false;
		for (int i = 0; i < l_iCount; i++)
		{
			cRenderObject*l_pRenderObject = GetObject(i)->GetImage();
			if(cPuzzleImageUnit::TypeID == l_pRenderObject->Type() ||
				cBaseImage::TypeID == l_pRenderObject->Type())
			{
				cBaseImage*l_pBaseImage = reinterpret_cast<cBaseImage*>(l_pRenderObject);
				if( l_pTexture == nullptr )
				{
					l_pTexture = l_pBaseImage->GetTexture();
				}
				else
				if( l_pTexture != l_pBaseImage->GetTexture() )
					return false;
			}
			else
			//for mpdi or else...but now I am lazy to do this.
			{
				return false;
			}
		}
		return true;
	}

	cLevelLayerList::cLevelLayerList()
	{
		m_vAngle = Vector3::Zero;
		//m_fAngleX = 45.f;
		m_vPos.x = 0.f;
		m_vPos.y = 0.f;
		m_GridSize.x = 10;
		m_GridSize.y = 10;
		m_vSize.x = 100.f;
		m_vSize.y = 100.f;
		m_bUsingGridToAssgnData = true;
		m_bSelectedObject = 0;
		m_v2DPos = Vector2::Zero;
	}
	//this one should only be called in the editor!?
	cLevelLayerList::cLevelLayerList(cLevelLayerList*e_pLevelLayerData):cObjectListWithItsChildren<cLayer,cLevelLayerGridData>(e_pLevelLayerData)
	{
		m_v2DPos = e_pLevelLayerData->m_v2DPos;
		m_vAngle = e_pLevelLayerData->m_vAngle;
		m_bSelectedObject = 0;
		this->SetName(e_pLevelLayerData->GetName());
		m_GridSize = e_pLevelLayerData->GetGridSize();
		m_vSize = e_pLevelLayerData->GetSize();
		m_bUsingGridToAssgnData = e_pLevelLayerData->IsUsingGridToAssgnData();
		m_vPos = e_pLevelLayerData->GetPos();
		//int	l_iCount = e_pLevelLayerData->Count();
		//for( int i=0;i<l_iCount;++i )
		//{
		//	cLayer*l_pOriginalData = e_pLevelLayerData->GetObject(i);
		//	//cNamedTypedObjectVectorWithData<cLevelLayerGridData>*l_pCellData = new cNamedTypedObjectVectorWithData<cLevelLayerGridData>();
		//	//l_pCellData->SetName(l_pOriginalData->GetName());
		//	//for( int j=0;j<l_pOriginalData->Count();++j )
		//	//{
		//	//	cLevelLayerGridData*l_pMap_Cell_ObjectData = new cLevelLayerGridData(l_pOriginalData->GetObject(j));
		//	//	l_pCellData->AddObjectNeglectExist(l_pMap_Cell_ObjectData);
		//	//}
		//	//cNamedTypedObjectVectorWithData<cLevelLayerGridData>*l_pCellData = new cNamedTypedObjectVectorWithData<cLevelLayerGridData>(l_pOriginalData);
		//	cLayer*l_pCellData = NewTemplateList<cLayer,cLevelLayerGridData>(l_pOriginalData,false);
		//	AddObjectNeglectExist(l_pCellData);
		//	
		//}
	}

	cLevelLayerList::~cLevelLayerList()
	{

	}

	void	cLevelLayerList::RenderGridLine()
	{
		int	l_iRow = m_GridSize.x*2;
		int	l_iColumn = m_GridSize.y*2;
		float	l_fEndDown = this->m_vSize.y*m_GridSize.y;
		float	l_fEndRight = this->m_vSize.x*m_GridSize.x;
		float	l_fStartPosX = -l_fEndRight/2;
		float	l_fStartPosY = -l_fEndDown/2;
		if( l_iRow<=1||l_iColumn<=1 )
			return;
		int	l_iTotalCount = l_iRow+l_iColumn;
		//Vector2 is 2 float
		m_LineTempVertex.SetCount(l_iTotalCount*2);
		Vector2*l_vAllVertices = (Vector2*)m_LineTempVertex.pVertex;
		for( int i=0;i<l_iRow/2;++i )
		{
			l_vAllVertices[i*2] = Vector2(l_fStartPosX+i*m_vSize.x,l_fStartPosY);
			l_vAllVertices[i*2+1] = Vector2(l_fStartPosX+i*m_vSize.x,l_fEndDown+l_fStartPosY);
		}
		for( int j=0;j<l_iColumn/2;++j )
		{
			l_vAllVertices[l_iRow+j*2] = Vector2(l_fStartPosX,j*m_vSize.y+l_fStartPosY);
			l_vAllVertices[l_iRow+j*2+1] = Vector2(l_fEndRight+l_fStartPosX,j*m_vSize.y+l_fStartPosY);
		}
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(m_vPos.x+l_fEndRight/2,m_vPos.y+l_fEndDown/2,0.f))*Quaternion::EulerRotationQuaternion(D3DXToRadian(m_vAngle.x),D3DXToRadian(m_vAngle.y),D3DXToRadian(m_vAngle.z)).ToMatrix();
		GLRender::RenderLine((float*)&l_vAllVertices[0],l_iTotalCount,Vector4(0.f,0.5f,0.f,1.f),2,(float*)&l_mat);
		Vector4	l_vColor(1,0.6f,1,1);
		RenderRectangle(l_fEndRight,l_fEndDown,l_mat,Vector4::One);
	}


	void	cLevelLayerList::RenderGrid(POINT e_Index)
	{
		RenderGrid(e_Index.x+e_Index.y*this->m_GridSize.x);
	}

	void	cLevelLayerList::RenderGrid(int e_iIndex)
	{
		if( e_iIndex == -1 )
			return;
		float	l_fEndDown = this->m_vSize.y*m_GridSize.y;
		float	l_fEndRight = this->m_vSize.x*m_GridSize.x;
		int	l_iIndexY = e_iIndex/this->m_GridSize.x;
		int	l_iIndexX = e_iIndex-(l_iIndexY*this->m_GridSize.x);
		//because rotation so minus half width and half height
		float	l_fStartPosX = -l_fEndRight/2+(l_iIndexX*this->m_vSize.x);
		float	l_fStartPosY = -l_fEndDown/2+(l_iIndexY*this->m_vSize.y);
		Vector2	l_fLinePoints[4] = {
		//leftup
			Vector2(l_fStartPosX,l_fStartPosY),
		//rightup
		Vector2(l_fStartPosX+this->m_vSize.x,l_fStartPosY),
		//leftdown
		Vector2(l_fStartPosX,l_fStartPosY+this->m_vSize.y),
		//rightdown
		Vector2(l_fStartPosX+this->m_vSize.x,l_fStartPosY+this->m_vSize.y)};
		Vector2	l_fFinallines[8] = {
			l_fLinePoints[0],l_fLinePoints[1],
			l_fLinePoints[2],l_fLinePoints[3],
			l_fLinePoints[0],l_fLinePoints[2],
			l_fLinePoints[1],l_fLinePoints[3]
		};
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(m_vPos.x+l_fEndRight/2,m_vPos.y+l_fEndDown/2,0.f))*
			Quaternion::EulerRotationQuaternion(D3DXToRadian(m_vAngle.x),D3DXToRadian(m_vAngle.y),D3DXToRadian(m_vAngle.z)).ToMatrix();
		GLRender::RenderLine((float*)l_fFinallines,8,Vector4(1.f,1.f,1.f,1.f),2,(float*)&l_mat);
	}

	void	cLevelLayerList::RenderAllObject()
	{
		int	l_iCount = Count();
		for(int i=0;i<l_iCount;++i)
		{
			cLayer*l_pLayer = (*this)[i];
			int	l_iCount2 = l_pLayer->Count();
			for(int j=0;j<l_iCount2;++j)
			{
				l_pLayer->GetObject(j)->Render();
			}
		}
	}

	void	cLevelLayerList::RenderAllLayer()
	{
		int	l_iCount = Count();
		for(int j=0;j<l_iCount;++j)
		{
			GetObject(j)->Render();
		}	
	}

	void	cLevelLayerList::RenderLayer(int e_iLayIndex)
	{
		cLayer*l_pLayer = GetObject(e_iLayIndex);
		if( l_pLayer )
		{
			l_pLayer->Render();
		}
	}

	int		cLevelLayerList::Collide(Vector2 e_vPos)
	{
		if( !m_pCurrentList )
			return -1;
		int	l_iNum = m_pCurrentList->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cLevelLayerGridData*l_pMap_Cell_ObjectData = m_pCurrentList->GetObject(i);
			if( l_pMap_Cell_ObjectData->IsCollide(e_vPos) )
			{
				return i;
			}
		}
		return -1;
	}

	int		cLevelLayerList::CollideForSamePosition(Vector3 e_vPos)
	{
		return CollideForSamePosition(Vector2(e_vPos.x,e_vPos.y));
	}

	int		cLevelLayerList::CollideForSamePosition(Vector2 e_vPos)
	{
		if( !m_pCurrentList )
			return -1;
		int	l_iNum = m_pCurrentList->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cLevelLayerGridData*l_pMap_Cell_ObjectData = m_pCurrentList->GetObject(i);
			Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
			if( e_vPos.x == l_vPos.x&&e_vPos.y == l_vPos.y )
			{
				return i;
			}
		}
		return -1;
	}

	int		cLevelLayerList::Collide(Vector3 e_vPos)
	{
		return Collide(Vector2(e_vPos.x,e_vPos.y));
	}

	int		cLevelLayerList::CollideWithoutSelected(Vector2 e_vPos)
	{
		if( !m_pCurrentList )
			return -1;
		int	l_iNum = m_pCurrentList->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cLevelLayerGridData*l_pMap_Cell_ObjectData = m_pCurrentList->GetObject(i);
			if( l_pMap_Cell_ObjectData == m_bSelectedObject )
				continue;
			if(l_pMap_Cell_ObjectData->IsCollide(e_vPos))
			{
				return i;
			}
		}
		return -1;
	}

	int		cLevelLayerList::CollideForSamePositionWithoutSelected(Vector3 e_vPos)
	{
		return CollideForSamePositionWithoutSelected(Vector2(e_vPos.x,e_vPos.y));
	}

	int		cLevelLayerList::CollideForSamePositionWithoutSelected(Vector2 e_vPos)
	{
		if( !m_pCurrentList )
			return -1;
		int	l_iNum = m_pCurrentList->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cLevelLayerGridData*l_pMap_Cell_ObjectData = m_pCurrentList->GetObject(i);
			if( l_pMap_Cell_ObjectData == m_bSelectedObject )
				continue;
			Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
			if( e_vPos.x == l_vPos.x && e_vPos.y ==l_vPos.y )
			{
				return i;
			}
		}
		return -1;
	}

	Vector3	cLevelLayerList::GetFiledSize()
	{
		return Vector3(this->m_vSize.x*this->m_GridSize.x,this->m_vSize.y*this->m_GridSize.y,0.f);
	}

	bool	cLevelLayerList::IsGridIndexLegal(POINT e_Index)
	{
		if( m_GridSize.x>e_Index.x&&m_GridSize.y>e_Index.y )
		{
			//if( e_Index.x>-1&&e_Index>-1 )
				return true;
		}
		return false;
	}

	bool	cLevelLayerList::IsPositionLegal(Vector3 e_vPos)
	{
		POINT	l_HitIndex = this->GetHitIndex(e_vPos);
		if( l_HitIndex.x == -1 || l_HitIndex.y == -1)
		{
			return false;
		}
		return true;
	}

	void	cLevelLayerList::SetGridSize(POINT e_GridSize)
	{
		m_GridSize = e_GridSize;
		Set2DPos(m_v2DPos);
	}

	void	cLevelLayerList::SetSize(Vector2 e_vSize)
	{
		m_vSize = e_vSize;
		Set2DPos(m_v2DPos);
	}

	void	cLevelLayerList::SetPos(Vector3 e_vPos)
	{
		this->m_vPos = e_vPos;
		this->m_v2DPos = this->GetTransformedPos();
	}

	Vector3	cLevelLayerList::GetPosbyGridIndex(POINT e_Index)
	{
		Vector3	l_vNewRight,l_vNewUp,l_vNewRightUP,l_vNewLeftDown;
		Vector3 l_vNewLeftUp = GetTransformedPos(true,&l_vNewRight,&l_vNewUp,&l_vNewRightUP,&l_vNewLeftDown);
		Vector3	l_vPos = l_vNewLeftUp+((float)e_Index.x*l_vNewRight*m_vSize.x);
		float	l_fPosX = l_vPos.x;
		l_vPos += ((float)e_Index.y*l_vNewUp*m_vSize.y);
		return l_vPos;
	}

	void	cLevelLayerList::Set2DPos(Vector3 e_vPos)
	{
		//because its orthorgnal
		m_vPos = Vector3::Zero;
		this->m_v2DPos = e_vPos;
		Vector3	l_vPos1 = GetTransformedPos();
		m_vPos.x = e_vPos.x-l_vPos1.x;
		m_vPos.y = e_vPos.y-l_vPos1.y;
	}

	Vector3	cLevelLayerList::GetTransformedPos(bool e_bStripZ,Vector3*e_pvRight,Vector3*e_pvUp,Vector3*e_pvRightUpPos,Vector3*e_pvLeftDownPos)
	{
		//1st,find new start point
		//2nd,get new touch point
		//3rd collide new rectangle with touch point.
		Vector3	l_vSize(m_vSize.x*this->m_GridSize.x,m_vSize.y*this->m_GridSize.y,0.f);
		Vector3	l_vHalfSize = l_vSize/2.f;
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(m_vPos)*
							Quaternion::EulerRotationQuaternion(D3DXToRadian(m_vAngle.x),D3DXToRadian(m_vAngle.y),D3DXToRadian(m_vAngle.z)).ToMatrix();
		//get new rect position
		Vector3 l_vNewLeftUp = l_mat.TransformCoordinate(-l_vHalfSize);
		l_vNewLeftUp += l_vHalfSize;
		if( e_pvRight )
		{
			Vector3	l_vNewRight = l_mat.GetAxis(MyMath::X);
			Vector3	l_vNewUp = l_mat.GetAxis(MyMath::Y);
			Vector3	l_vNewRightUP = l_vNewLeftUp+l_vNewRight*l_vSize.x;
			Vector3	l_vNewLeftDown = l_vNewLeftUp+(l_vNewUp*l_vSize.y);
			//Vector3	l_vNewRightDown = l_vNewRightUP+(l_vNewUp*l_vSize.y);
			*e_pvRight = l_vNewRight;
			*e_pvUp = l_vNewUp;
			*e_pvLeftDownPos = l_vNewLeftDown;
			*e_pvRightUpPos = l_vNewRightUP;
			if( e_bStripZ )
			{
				e_pvRightUpPos->z = e_pvLeftDownPos->z = 0.f;
			}
		}
		if( e_bStripZ )
			l_vNewLeftUp.z = 0.f;
		return l_vNewLeftUp;
	}

	POINT	cLevelLayerList::GetHitIndex(Vector3 e_vPos)
	{
		POINT	l_HitIndex = {-1,-1};
		Vector3	l_vNewRight;
		Vector3	l_vNewUp;
		Vector3	l_vNewRightUP;
		Vector3	l_vNewLeftDown;
		Vector3 l_vNewLeftUp = GetTransformedPos(true,&l_vNewRight,&l_vNewUp,&l_vNewRightUP,&l_vNewLeftDown);
		cLine	l_LineLU_To_RU(Vector3(l_vNewLeftUp.x,l_vNewLeftUp.y,0.f),Vector3(l_vNewRightUP.x,l_vNewRightUP.y,0.f));
		cLine	l_LineLU_To_LD(l_LineLU_To_RU.m_vBegine,Vector3(l_vNewLeftDown.x,l_vNewLeftDown.y,0.f));
		//just need a 2D coordinate.
		l_vNewUp.z = 0.f;
		l_vNewRight.z = 0.f;
		cLine	l_TouchToUPLine(e_vPos,e_vPos-(l_vNewUp*99999));
		cLine	l_TouchToLeftLine(e_vPos,e_vPos-(l_vNewRight*99999));
		Vector3	l_vUpIntersect;
		Vector3	l_vLeftIntersect;
		if(l_LineLU_To_RU.Intersect(l_TouchToUPLine,&l_vUpIntersect) != cLine::INTERESECTING )
			return l_HitIndex;
		if(l_LineLU_To_LD.Intersect(l_TouchToLeftLine,&l_vLeftIntersect) != cLine::INTERESECTING )
			return l_HitIndex;
		//just need a 2D coordinate.
		l_vLeftIntersect.z = l_vUpIntersect.z = 0.f;
		float	l_fXLength = (l_vNewLeftUp-l_vUpIntersect).Length();
		float	l_fYLength = (l_vNewLeftUp-l_vLeftIntersect).Length();
		//just need a 2D coordinate.
		l_vNewRightUP.z = l_vNewLeftUp.z = l_vNewLeftDown.z = 0.f;
		Vector3	l_vNewSize((l_vNewRightUP-l_vNewLeftUp).Length(),(l_vNewLeftDown-l_vNewLeftUp).Length(),0.f);
		Vector3	l_vNewHalfSize = l_vNewSize/2.f;
		Vector3	l_fNewEachGridSize(l_vNewSize.x/(float)this->m_GridSize.x,l_vNewSize.y/(float)this->m_GridSize.y,0.f);
		l_HitIndex.x = (int)(l_fXLength/l_fNewEachGridSize.x);
		l_HitIndex.y = (int)(l_fYLength/l_fNewEachGridSize.y);

		//m_vHitGridPos = l_LineLU_To_RU.m_vBegine+l_vNewRight*(l_fNewEachGridSize.x*l_HitIndex.x);
		//m_vHitGridPos += l_vNewUp*(l_fNewEachGridSize.y*l_HitIndex.y);
		m_vHitGridPos = l_LineLU_To_RU.m_vBegine+l_vNewRight*(this->m_vSize.x*l_HitIndex.x);
		m_vHitGridPos += l_vNewUp*(m_vSize.y*l_HitIndex.y);
		return l_HitIndex;
	}

	int		cLevelLayerList::CollideWithTransform(Vector3 e_vPos,POINT*e_pXYIndex)
	{
		POINT	l_HitIndex = GetHitIndex(e_vPos);
		if( l_HitIndex.x<0||l_HitIndex.y<0 )
			return  -1;
		if( l_HitIndex.x>this->m_GridSize.x||l_HitIndex.y>this->m_GridSize.y )
			return  -1;
		if( e_pXYIndex )
		{
			e_pXYIndex->x = l_HitIndex.x;
			e_pXYIndex->y = l_HitIndex.y;
		}
		l_HitIndex.y*=m_GridSize.x;
		return l_HitIndex.y+l_HitIndex.x;
	}

	POINT	cLevelLayerList::GetRowAndColumn()
	{
		POINT	l_RowAndColumn = {(int)m_vSize.x/m_GridSize.x,(int)m_vSize.y/m_GridSize.y};
		return l_RowAndColumn;
	}
//end namespace
}
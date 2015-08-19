#ifndef _LEVEL_LAYER_DATA_H_
#define _LEVEL_LAYER_DATA_H_
#include "LevelLayerGridData.h"
namespace FATMING_CORE
{

	//for level editor cMapLayer
	template <class T> class cNamedTypedObjectVectorWithData2:public cNamedTypedObjectVector<T>
	{
		GET_SET_DEC(void*,m_pData,GetData,SetData);
	public:
		cNamedTypedObjectVectorWithData2():cNamedTypedObjectVector<T>()
		{
			m_pData = 0;
		}
		virtual ~cNamedTypedObjectVectorWithData2()
		{
			//SAFE_DELETE(m_pData);
		}

		cNamedTypedObjectVectorWithData2(cNamedTypedObjectVectorWithData2*e_pObjectListByNameWithData):cNamedTypedObjectVector<T>(e_pObjectListByNameWithData)
		{
			m_pData = 0;
		}

		virtual	NamedTypedObject*	Clone(){ return new cNamedTypedObjectVectorWithData2<T>(this); }
	};

	//data is a std::wstring  to fetch xml data.
	typedef cNamedTypedObjectVectorWithData<cLevelLayerGridData>					cLayer;
	typedef cObjectListTree<cLayer,cLevelLayerGridData>						cLevelLayerListTree;
	//==========================
	//it's possible be a root
	//this is the cell that store in the MapData
	//a part of 2D data,just a cell.
	//!!!!!!!!!!!ensure u hvae set current list to set layer!!!!!!!!!!!
	//it's orthongal projection!
	//==========================
	class	cLevelLayerList:public cLevelLayerListTree
	{
		//this one to store the real position in the 2D cooridinate
		Vector3	m_v2DPos;
		//this one and above are bind together,use this object please instead SetPos
		Vector3	m_vPos;
	protected:
		//for get hit data.
		Vector3	GetTransformedPos(bool e_bStripZ = true,Vector3*e_pvRight = 0,Vector3*e_pvUp = 0,Vector3*e_pvRightUpPos = 0,Vector3*e_pvLeftDownPos = 0);
		//
		Vector3	m_vHitGridPos;
		//
		//the width and height,so we do know this cell size,it's not filed's size,it only present each cell's size
		POINT	m_GridSize;
		Vector2	m_vSize;
		//if true the data's position is assign by grid position
		GET_SET_DEC(bool,m_bUsingGridToAssgnData,IsUsingGridToAssgnData,SetUsingGridToAssgnData);
		GET_SET_DEC(Vector3,m_vAngle,GetAngle,SetAngle);
		//if the object is selected
		GET_SET_DEC(cLevelLayerGridData*,m_bSelectedObject,GetSelectedObject,SetSelectedObject);
	public:
		cLevelLayerList();
		//this one should only be called in the editor!?
		cLevelLayerList(cLevelLayerList*e_pMap_CellData);
		~cLevelLayerList();
		CLONE_MYSELF(cLevelLayerList);
		Vector3	GetPos(){ return m_vPos; }
		void	SetPos(Vector3 e_vPos);
		virtual	void	Render();
		//render all layer
		void	RenderAllObject();
		void	RenderLayer(int e_iLayIndex);
		//render specific grid.
		void	RenderGrid(int e_iIndex);
		void	RenderGrid(POINT e_Index);
		//===============================================
		//only CollideWithTransform support xyz angle rotation collided.
		//===============================================
		//if angle is rotated call this function.
		int		CollideWithTransform(Vector3 e_vPos,POINT*e_pXYIndex);
		//
		//if return value is not -1,it collided
		int		Collide(Vector2 e_vPos);
		int		CollideForSamePosition(Vector2 e_vPos);
		int		CollideForSamePosition(Vector3 e_vPos);
		int		Collide(Vector3 e_vPos);
		//it might could be object move mode,so do not collide with selected one(m_bSelectedObject)
		int		CollideWithoutSelected(Vector2 e_vPos);
		int		CollideForSamePositionWithoutSelected(Vector2 e_vPos);
		int		CollideForSamePositionWithoutSelected(Vector3 e_vPos);
		//
		Vector3	GetFiledSize();
		//check index and position is inside
		bool	IsGridIndexLegal(POINT e_Index);
		bool	IsPositionLegal(Vector3 e_vPos);
		void	Set2DPos(Vector3 e_vPos);
		POINT	GetGridSize(){return m_GridSize;}
		Vector2	GetSize(){return m_vSize;}
		//aftern set grid data reset position.
		void	SetGridSize(POINT e_GridSize);
		void	SetSize(Vector2 e_vSize);
		//after call CollideWithTransform and it's collided,it will return a posiition where collided of grid
		Vector3	GetHitGridPos(){return m_vHitGridPos;}
		// e_bForceReturn will return result even grid is not legal
		POINT	GetHitIndex(Vector3 e_vPos);
		//get a transfomred 2D position
		Vector3	GetPosbyGridIndex(POINT e_Index);
		POINT	GetRowAndColumn();
	};
//end namespace FATMING_CORE
}
#endif
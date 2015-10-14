#ifndef _MAGIC_TOWER_DATA_DEFINE_H_
#define _MAGIC_TOWER_DATA_DEFINE_H_

#include "../../Core/FMLevelData/AllLevelDataInclude.h" 
#include "ItemData.h"
#include "MagicTowerDefineDataNode.h"

//========================
//could add some estra function
//ex:while man walk on this grid it will lost HP.
//========================
class	cGridBehavior:public NamedTypedObject
{
protected:
	eMagicTowerGridType		m_eMagicTowerGridType;
	cLevelLayerGridData*	m_pLevelLayerGridData;
	GET_SET_DEC(POINT,m_GridIndex,GetGridIndex,SetGridIndex);
	GET_SET_DEC(bool,m_bWorking,IsWorking,SetWorking);//if false this object no more working
public:
	cGridBehavior(cLevelLayerGridData*e_pLevelLayerGridData,POINT e_GridIndex,eMagicTowerGridType e_eMagicTowerGridType);
	~cGridBehavior(){}
	eMagicTowerGridType		GetMagicTowerGridType(){return m_eMagicTowerGridType;}
	Vector3					GetPos();
	virtual	void			Init();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Render();
	virtual	void			DebugRender();
	cLevelLayerGridData*	GetLevelLayerGridData(){return m_pLevelLayerGridData;}
	static cGridBehavior*	GenerateGridBehavior(cLevelLayerGridData*e_pLevelLayerGridData,POINT e_GridIndex);
};

//=====================================
//Data of cLayer.
//=====================================
class	cGridBehaviorListForLevelLayer:public NamedTypedObject
{
	//get grid max row and column count then generate as many as grids count
	//so fetch grid data could fastly by grid index
	cGridBehavior**		m_ppGridBehaviorGridsData;
public:	
	cGridBehaviorListForLevelLayer(cLevelLayerList*e_pLevelLayerList,int e_iLayerIndex);
	~cGridBehaviorListForLevelLayer();

	cGridBehavior*	GetGridBehavior(POINT e_GridIndex);
	cGridBehavior*	GetGridBehavior(int e_iGridIndex);
	void	RenderLevelLayerGridObject();
	void	RenderLevelLayerGridObject(int e_iIndex);
};

//=======================
//cNamedTypedObjectVector<cGridBehavior> for each layer
//=======================
class	cStoreyData:public cLevelLayerList
{
	POINT	m_RowAncolumn;
public:
	//set all as road to default,prevent data error
	cStoreyData(cLevelLayerList*e_pLevelLayerData);
	~cStoreyData();
	POINT	GetRowAndColumn(){return m_RowAncolumn;}
	void	RenderLevelLayerGridObject();
	void	RenderAllLevelLayerGridObject(POINT e_Index);
	void	RenderAllLevelLayerGridObject(int e_iIndex);
	void	RenderLevelLayerGridObject(POINT e_Index,int e_iLayer);
	void	RenderLevelLayerGridObject(int e_iIndex,int e_iLayer);
	cGridBehavior*	GetGridBehavior(POINT e_GridIndex,int e_iLayer);

};

#endif
#ifndef _LEVEL_LAYER_GRID_DATA_H
#define _LEVEL_LAYER_GRID_DATA_H

namespace FATMING_CORE
{
	class	cEventDataNode;
	class	cLevelData;
	//===============
	//position and image and is selected
	//===============
	class	cLevelLayerGridData:virtual	public NamedTypedObject
	{
		friend class	cLevelData;
	protected:
		cRenderObject*	m_pImage;
		cEventDataNode*	m_pTemplateNode;
		cEventDataNode*	m_pEventDataNode;
		bool	m_bSelected;
	public:
		cLevelLayerGridData();
		cLevelLayerGridData(cLevelLayerGridData*e_pLevelLayerGridData);
		virtual ~cLevelLayerGridData();
		CLONE_MYSELF(cLevelLayerGridData);
		cRenderObject*	GetImage(){ return m_pImage; }
		void			SetImage(cRenderObject*e_pRenderObjectBehavior);
		cEventDataNode*	GetTemplateDataNode();
		void			SetTemplateDataNode(cEventDataNode*e_pEventDataNode);

		cEventDataNode*	GetEventDataNode();
		void			SetEventDataNode(cEventDataNode*e_pEventDataNode);

		virtual	Vector3	GetPos();
		virtual	void	Init();
		virtual	void	SetPos(Vector3 e_vPos);
		virtual	void	SetSelected(bool e_bSelected);
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		virtual	void	DebugRender(float e_fRandomColor);
		bool			IsCollide(Vector3 e_vPos);
		bool			IsCollide(Vector2 e_vPos);
		int				GetWidth();
		int				GetHeight();
		//if two data is same return true
		bool			DataCompare(cLevelLayerGridData*e_pMap_Cell_ObjectData);
	};
//end namespace
}
#endif
#ifndef _LEVEL_EDITOR_H_
#define	_LEVEL_EDITOR_H_


enum	eMouseMode
{
	eMM_AddObject = 0,
	eMM_MoveObject,
	eMM_EraseObject,
	eMM_MAX
};

class	cLevelEditorApp:public cGameApp
{
public:
	//the position has been aligh the frid position
	Vector3		m_vGridAlignPos;
	Vector3		m_vMousePos;
	Vector2		m_vStartPos;
	//check auto aligh if true g_vGridAlignPos or g_vMousePos
	Vector3		m_vCurrentPlacePos;
	Vector3		m_vSelectedObjectOriginalPos;
	Vector2		m_vMapWidth;
	eMouseMode	m_eMoudeMode;
	bool		m_bShowGrid;
	bool		m_bAutoAllignGrid;
	bool		m_bShowResolutionBorder;
	bool		m_bPlaceObjectByImageCenter;
	bool		m_bStageDataChanged;
	bool		m_bAllowSamepositionObject;
public:
	cLevelData*				m_pLevelData;
	cOrthogonalCamera*		m_pOrthogonalCamera;
	cLevelLayerList*		m_pCurrentStageEditData;			//for current stage
	cLevelLayerGridData*	m_pCurrentEditUserPropertyObjectData;
	cBaseImage*m_pBGImage;

	cRenderObject*	m_pCurrentSelectedImage;
	//for new add image in listbox
	cRenderObject*	m_pHintImage;

	std::vector<int>		m_SelectedLayers;
	std::vector<int>		m_SelectedLayerObjects;

	cLevelEditorApp(HWND e_Hwnd);
	virtual ~cLevelEditorApp();
	void					Init();
	void					DestoryOldData();
	void					Update(float e_fElpaseTime);
	void					Render();
	void					MouseDown(int e_iPosX,int e_iPosY);
	void					MouseMove(int e_iPosX,int e_iPosY,GCFORM::ListBox^e_pLayerObjectData_listBox);
	void					MouseUp(int e_iPosX,int e_iPosY,System::Collections::Hashtable^e_UniqueUserDefineDataForMap_Cell_ObjectData,GCFORM::ListBox^e_pLayerObjectData_listBox,GCFORM::ListBox^e_pTemplate_listBox);
	void					MouseDoubleClick(GCFORM::TreeView^e_pTreeView,System::Collections::Hashtable^e_UniqueUserDefineDataForMap_Cell_ObjectData,
							System::Collections::Hashtable^e_TemplateNameAndAttributeNode);
	void					KeyDown(char e_char);
	void					KeyUp(char e_char);

	cOrthogonalCamera*		GetOrthogonalCamera(){return m_pOrthogonalCamera;}
	void					AddObjectOnCell(GCFORM::ListBox^e_pLayerObjectData_listBox,GCFORM::ListBox^e_pTemplate_listBox);
	void					AddObjectIntoCurrentStage(Vector3 e_vPos,GCFORM::ListBox^e_pLayerObjectData_listBox,GCFORM::ListBox^e_pTemplate_listBox);
};

#endif
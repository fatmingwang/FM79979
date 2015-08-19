#ifndef	_GAME_OBJECT_SELECTOR_H_
#define	_GAME_OBJECT_SELECTOR_H_

	//<cGameObjectSelector Name="">
	//	<cButtonDialog Description="">
	//		<cMPDI MPDIList="" MPDI=""/>
	//	</cButtonDialog>
	//	<cGameItemInfoButton>
	//		<cButtonDialog Description="">
	//			<cMPDI MPDIList="" MPDI=""/>
	//		</cButtonDialog>
	//		<cRenderObjectVector>
	//			<cFontWithFrame GlyphRenderName="" Text="" />
	//				<SubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
	//			</cFontWithFrame>
	//			<cFontWithFrame GlyphRenderName="" Text="" />
	//				<SubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
	//			</cFontWithFrame>
	//		</cRenderObjectVector>
	//	<cGameItemInfoButton>
	//</cGameObjectSelector>

	class	cGameItemInfoButton:public cButtonDialog
	{
	protected:
		cRenderObjectVector*	m_pDescriptionFontFrameVector;
	public:
		cGameItemInfoButton();
		cGameItemInfoButton(cGameItemInfoButton*e_pGameItemInfoButton);
		cGameItemInfoButton(TiXmlElement*e_pElement);
		~cGameItemInfoButton();
		static cGameItemInfoButton*GetMe(TiXmlElement*e_pElement);
		virtual	void			Update(float e_fElpaseTime);
		virtual	void			Render();
		void					ChangeText(int e_iIndex,const WCHAR*e_strText);
		void					ChangeText(int e_iIndex,std::wstring e_strText);
		void					ChangeBG(const WCHAR*e_strPuzzeleImageUnitName,const WCHAR*e_strPuzzleImageName = 0);
	};
	typedef void    (*AssignGameObjectInfo)(NamedTypedObject*e_pSrc,cGameItemInfoButton*e_pGameItemInfoButton,int e_iType);
	class	cFishRodAndFishThreadData;

	class	cGameObjectSelector:public cFMWorkingObjectChanger<cGameItemInfoButton>
	{
		cNamedTypedObjectVector<NamedTypedObject>*	m_pGameItemInfoReferenceData;
		cFishRodAndFishThreadData*					m_pFishRodAndFishThreadData;
		//
		TiXmlElement*								m_pTemplateElement;
		TiXmlElement*								m_pGameItemInfoButtonTemplateElement;
		cButtonDialog*								m_pButtonDialog;
	public:
		cGameObjectSelector(TiXmlElement*e_pTemplateElement);
		cGameObjectSelector(cGameObjectSelector*e_pGameObjectSelector);
		~cGameObjectSelector();
		CLONE_MYSELF(cGameObjectSelector);
		DEFINE_TYPE_INFO();
		void					SetGameItemInfoReferenceData(cNamedTypedObjectVector<NamedTypedObject>*e_pTargetSource,int e_iType);
		virtual	void			Update(float e_fElpaseTime);
		virtual	void			Render();
		virtual	void			Destroy();
        virtual void			MouseDown(int e_iPosX,int e_iPosY);
        virtual void			MouseMove(int e_iPosX,int e_iPosY);
        virtual void			MouseUp(int e_iPosX,int e_iPosY);
		AssignGameObjectInfo	AssignGameObjectInfoFun;
		bool					IsSatisfiedCondition();
		const WCHAR*			GetSelectItemName();
	};

#endif
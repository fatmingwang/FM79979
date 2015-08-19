#define	IN_HHOUSE_SUB_MEUN_MONEY_PI_FILE_NAME		"NL1/InHouse/UI/SubMenu/SubMenuObject.pi"
#define	IN_HHOUSE_SUB_MEUN_FILE_NAME				"NL1/InHouse/UI/SubMenu/SubMenuTest.mpdi"
		cMPDIList*e_pMPDIList;
		cMouse_TouchWithCancelButton*m_pSubMenu;
		this->m_pSubMenu = ParseFileToSliderBar(e_strSubMenuFileName,IN_HHOUSE_SUB_MEUN_MONEY_PI_FILE_NAME,&e_pMPDIList);
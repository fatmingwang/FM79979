#ifndef MAP_RULE_H
#define MAP_RULE_H

#define	CELL_TYPE_ROAD			1>>0
#define	CELL_TYPE_BLOCK			1>>1
#define	CELL_TYPE_MONSTER		1>>2
#define	CELL_TYPE_MAN			1>>3
#define	CELL_TYPE_ITEM			1>>4

//======================
//
//======================
struct	sBaseCharacterData
{
	WCHAR	strName[MAX_PATH];	//
	int	iHP;					//
	int	iDefence;				//
	int	iStrgeth;				//
	int	iLucky;					//
	int	iAgile;					//
	sBaseCharacterData()
	{
		memset(strName,0,sizeof(WCHAR)*MAX_PAH);
		iHP = 0;
		iDefence = 0;
		iStrgeth = 0;
		iLucky = 0;
		iAgile = 0;
	}
};

//struct	sItemData:public sBaseCharacterData
//{
//	cMultiPathDynamicImage*	pImage;
//	//void	Update(float e_fElpasedTime);
//	void	Render(){ pImage->Render(); }
//	sItemData(){ pImage = 0; }
//};

struct	cMainRoleData:public sBaseCharacterData
{
	int	iCurrentExp;
	int	iAllLevelExpNeed[99];
	int	iCurrentLV;
	sItemData	*psWeapon;
	sItemData	*psShield;
	sItemData	*psDecoration;
};
//========================
//
//========================
class	cCellData
{
	void	int	m_iCellType;
	cBaseImage*m_pImage;
	Vector2	m_vPos;
	//if it filled specific condition,it could become another cell data.
	cCellData*m_pTargetCellData;
public:
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};

//===============================
//total count of cell = (e_vFiledSize.x/e_vCellSize.x)*(e_vFiledSize.y/e_vCellSize.y)
//e_vPos to divide by e_vCellSize
//===============================
POINT	WorldPosTo2ArrayIndex(Vector2 e_vCellSize,Vector2 e_vPos)
{
	float	l_fX = e_vPos.x/e_vCellSize.x;
	float	l_fY = e_vPos.y/e_vCellSize.y;
	int	l_iX = l_fX-(int)l_fX>0?(int)l_fX+1:(int)l_fX;
	int	l_iY = l_fY-(int)l_fY>0?(int)l_fY+1:(int)l_fY;
	POINT	l_ArrayIndex;
	l_ArrayIndex.x = l_iX;
	l_ArrayIndex.y = l_iY;
	return l_ArrayIndex;
}

#endif
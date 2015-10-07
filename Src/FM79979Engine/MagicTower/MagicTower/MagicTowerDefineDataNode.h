#ifndef _MAGIC_TOWER_DEFINE_DATA_NODE_H_
#define _MAGIC_TOWER_DEFINE_DATA_NODE_H_

enum	eMagicTowerGridType
{
	MTGT_ROAD = 0,
	MTGT_BLOCK,
	MTGT_MONSTER,
	MTGT_NPC,
	MTGT_ITEM,
	MTGT_UP,
	MTGT_DOWN,
	MTGT_TELPORTION,
	MTGT_MAX,
};

eMagicTowerGridType		NameToMagicTowerGridType(const WCHAR*e_strName);

extern const WCHAR*g_strMagicTowerCellTypeName[];


struct	sGridData
{
	eMagicTowerGridType	eMagicTowerGridType;
	bool				bMovable;
	void*				pGridTypeData;
};

struct	sMonsterData
{
	cFAICharacterInterface*	pFAICharacterInterface;
	cFAIMachine*			pFAIMachine;
	sMonsterData();
	~sMonsterData();
};

sMonsterData*		GetMonsterData(TiXmlElement*e_pTlement);
//<GridData Type="Road" Moveable="1">
//	<Road />
//</GridData>

//<GridData Type="M">
//	<Road />
//</GridData>

//<GridData Type="Up">

//</GridData>

//<GridData Type="Up">

//</GridData>

//<GridData Type="Monster">
//	<Monster AI="" />
//		<Animation MPDIList="" Battle="" Idle="" SpecialAttack="" Hurt="" />
//	</Monster>
//	<Event EventPackage="" EventObjectInstance="" />
//</GridData>

//if no position player will show as same as the position of current storey

//	<Telportion TargetStorey="" />
//	<Telportion TargetStorey="" Position="" />
struct	sTelportionData
{
	std::wstring	strStoreyName;
	Vector3*		pPos;
	sTelportionData();
	~sTelportionData();
};

sTelportionData*	GetTelportionData(TiXmlElement*e_pTlement);
//end _MAGIC_TOWER_DEFINE_DATA_NODE_H_
#endif
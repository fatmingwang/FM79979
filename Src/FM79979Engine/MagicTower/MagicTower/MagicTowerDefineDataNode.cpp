#include "stdafx.h"
#include "MagicTowerDefineDataNode.h"

const WCHAR*g_strMagicTowerCellTypeName[] = 
{
	L"Road",
	L"Block",
	L"Monster",
	L"Npc",
	L"Item",
	L"Up",
	L"Down",
	L"Telportion",
};


eMagicTowerGridType		NameToMagicTowerGridType(const WCHAR*e_strName)
{
	if(!wcscmp(e_strName,g_strMagicTowerCellTypeName[MTGT_ROAD]))
	{
		return MTGT_ROAD;
	}
	else
	if(!wcscmp(e_strName,g_strMagicTowerCellTypeName[MTGT_BLOCK]))
	{
		return MTGT_BLOCK;
	}
	else
	if(!wcscmp(e_strName,g_strMagicTowerCellTypeName[MTGT_NPC]))
	{
		return MTGT_NPC;
	}
	else
	if(!wcscmp(e_strName,g_strMagicTowerCellTypeName[MTGT_ITEM]))
	{
		return MTGT_ITEM;
	}
	else
	if(!wcscmp(e_strName,g_strMagicTowerCellTypeName[MTGT_UP]))
	{
		return MTGT_UP;
	}
	else
	if(!wcscmp(e_strName,g_strMagicTowerCellTypeName[MTGT_DOWN]))
	{
		return MTGT_DOWN;
	}

	WCHAR*l_strMonsterName = L"Monster";
	int	l_iLength = wcslen(l_strMonsterName);
	bool	l_bSame = true;
	for( int i=0;i<l_iLength;++i )
	{
		if(e_strName[i] != l_strMonsterName[i])
		{
			l_bSame = false;
			break;
		}
	}
	if( l_bSame )
		return MTGT_MONSTER;
	//assert(0);
	return MTGT_MAX;
}

sMonsterData::sMonsterData()
{
	pFAICharacterInterface = 0;
	pFAIMachine = 0;
}

sMonsterData::~sMonsterData()
{
	SAFE_DELETE(pFAICharacterInterface);
	SAFE_DELETE(pFAIMachine);
}

sMonsterData*		GetMonsterData(TiXmlElement*e_pTlement)
{
	sMonsterData*l_pMonsterData = 0;
	return l_pMonsterData;
};

sTelportionData::sTelportionData()
{
	pPos = 0;
}

sTelportionData::~sTelportionData()
{
	SAFE_DELETE(pPos);
}

sTelportionData*	GetTelportionData(TiXmlElement*e_pTlement)
{
	sTelportionData*l_pTelportionData = new sTelportionData;
	l_pTelportionData->pPos = 0;
	return l_pTelportionData;
}
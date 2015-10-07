#include "StdAfx.h"
#include "BattlAttackBase.h"
#include "BattleAttackMoveObject.h"
#include "BattleAttackPointsTouch.h"
#include "BattleAttackTouchSequence.h"

cBattleBase::cBattleBase(eBattleType	e_eBattleType)
{
	m_iObjectLV = 0;
	m_fPower = 0.f;
	m_EnergyRecoveryTime.SetTargetTime(1.f);
	m_eBattleType = m_eBattleType;
}

cBattleBase::~cBattleBase()
{

}

//<BattleAttack Type="MoveObject">
//	<Data>
//	</Data>
//</BattleAttack>

cBattleBase*	cBattleBase::GetBattleBase(eBattleType	e_eBattleType)
{
	eBattleType l_eBattleType = e_eBattleType;
	cBattleBase*l_pBattleBase = 0;
	switch(l_eBattleType)
	{
		case eBT_ATTATK_MOVE_OBJECT:
			l_pBattleBase = new cBattleAttackMoveObject(l_eBattleType);
			break;
		case eBT_ATTATK_POINT_TOUCH:
			l_pBattleBase = new cBattleAttackPointsTouch(l_eBattleType);

			break;
		case eBT_ATTATK_TOUCH_SEQUENCE:
			l_pBattleBase = new cBattleAttackTouchSequence(l_eBattleType);
		break;
		default:

			break;
	}
	//if( wcscmp(e_pTiXmlElement->Value(),L"BattleAttack") )
	//	return 0;
	//const WCHAR*l_strType = e_pTiXmlElement->Attribute(L"Type");
	//if( !wcscmp(l_strType,L"BattleAttackPointsTouch") )
	//{
	//	l_pBattleBase = new cBattleAttackPointsTouch(l_eBattleType);
	//}
	//else
	//if( !wcscmp(l_strType,L"BattleAttackPointsTouch") )
	//{
	//	l_pBattleBase = new cBattleAttackPointsTouch(l_eBattleType);
	//}
	//else
	//if( !wcscmp(l_strType,L"BattleAttackTouchSequence") )
	//{
	//	l_pBattleBase = new cBattleAttackTouchSequence(l_eBattleType);
	//}
	return l_pBattleBase;
}
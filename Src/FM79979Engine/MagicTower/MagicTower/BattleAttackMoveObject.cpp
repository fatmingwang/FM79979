#include "StdAfx.h"
#include "BattleAttackMoveObject.h"

char*BATTLE_ATTACK_MOVE_OBJECT_FILE_NAME = "MagicTower/Battle/BattleAttackMoveObject.xml";

cBattleAttackMoveObject::cBattleAttackMoveObject(eBattleType e_eBattleType):cBattleBase(e_eBattleType)
{
	m_pTwoMovingObjectController = 0;
	cNodeISAX	l_NodeISAX;
	if( l_NodeISAX.ParseDataIntoXMLNode(BATTLE_ATTACK_MOVE_OBJECT_FILE_NAME) )
	{
		TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
		if( l_pTiXmlElement )
			m_pTwoMovingObjectController = new sTwoMovingObjectController(l_pTiXmlElement);
	}
}

cBattleAttackMoveObject::~cBattleAttackMoveObject()
{
	SAFE_DELETE(m_pTwoMovingObjectController);
}

void	cBattleAttackMoveObject::InternalMouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pTwoMovingObjectController )
		m_pTwoMovingObjectController->MouseMove(e_iPosX,e_iPosY);
}

void	cBattleAttackMoveObject::InternalMouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pTwoMovingObjectController )
		m_pTwoMovingObjectController->MouseDown(e_iPosX,e_iPosY);
}

void    cBattleAttackMoveObject::InternalMouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pTwoMovingObjectController )
		m_pTwoMovingObjectController->MouseUp(e_iPosX,e_iPosY);
}

void    cBattleAttackMoveObject::InternalMouseLeave(int e_iPosX,int e_iPosY)
{
	if( m_pTwoMovingObjectController )
		m_pTwoMovingObjectController->MouseLeave(e_iPosX,e_iPosY);
}

void	cBattleAttackMoveObject::Init()
{
	cClickMouseBehavior::Init();
	m_fPower = 0.f;
	if( m_pTwoMovingObjectController )
	{
		m_pTwoMovingObjectController->Init( GetLV()  );
		if( m_pTwoMovingObjectController->pFrameImage )
		{
			Vector2	l_vDrawSize = m_pTwoMovingObjectController->pFrameImage->GetDrawSize();
			Vector3	l_vPos = m_pTwoMovingObjectController->pFrameImage->GetObject(0)->GetPointList()[0];
			this->m_vCollisionRange = Vector4(l_vPos.x,l_vPos.y,l_vPos.x+l_vDrawSize.x,l_vPos.y+l_vDrawSize.y);
		}
	}
}

void	cBattleAttackMoveObject::Update(float e_fElpaseTime)
{
	if( m_pTwoMovingObjectController )
	{
		m_pTwoMovingObjectController->Update(e_fElpaseTime);
		if(m_pTwoMovingObjectController->bIsDone)
		{
			m_fPower = m_pTwoMovingObjectController->fTwoObjectCloseValue;
			m_pTwoMovingObjectController->bIsDone = false;
		}
		else
		{
			m_fPower = -1.f;
		}
	}
}

void	cBattleAttackMoveObject::Render()
{
	if( m_pTwoMovingObjectController )
	{
		m_pTwoMovingObjectController->Render();
		cGameApp::RenderFont(500,100,ValueToString(this->m_eObjectMouseBehavior));

	}
}

void	cBattleAttackMoveObject::DebugRender()
{
	if( m_pTwoMovingObjectController )
	{
		m_pTwoMovingObjectController->DebugRender();
		GLRender::RenderRectangle(Vector2(m_vCollisionRange.x,m_vCollisionRange.y),m_vCollisionRange.Width(),m_vCollisionRange.Height(),Vector4(1,0,0,1));
	}
}
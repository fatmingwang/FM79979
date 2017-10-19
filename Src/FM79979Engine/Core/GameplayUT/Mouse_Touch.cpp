#include "../stdafx.h"
#include "Mouse_Touch.h"
#include "GameApp.h"
namespace FATMING_CORE
{
	float cClickMouseBehavior::m_sfRepeatTime = 0.1f;

    float   sMouseMoveData::DownUpDistance(Vector2 *e_pvDirection)
    {
        Vector2 l_vDirection((float)(UpPos.x-DownPos.x),(float)(UpPos.y-DownPos.y));
        if( e_pvDirection )
            *e_pvDirection = l_vDirection;
        return l_vDirection.Length();
    }

	bool	sMouseMoveData::IsDownUpDistanceSamePoint(Vector2 *e_pvDirection)
	{
		float l_fDistance = DownUpDistance(e_pvDirection);
		if( l_fDistance < DOUBLE_CLICK_RESTRICTION_DISTANCE )
		{
			return true;
		}
		return false;
	}

    float   sMouseMoveData::DownMoveDistance(Vector2 *e_pvDirection)
    {
        Vector2 l_vDirection((float)(MovePos.x-DownPos.x),(float)(MovePos.y-DownPos.y));
        if( e_pvDirection )
            *e_pvDirection = l_vDirection;
        return l_vDirection.Length();
    }
    Vector2   sMouseMoveData::DownUp()
	{
		return Vector2((float)(DownPos.x-UpPos.x),(float)(DownPos.y-UpPos.y)); 
	}

    Vector2   sMouseMoveData::DownMove()
	{
		return Vector2((float)(DownPos.x-MovePos.x),(float)(DownPos.y-MovePos.y)); 
	}
	void	sMouseMoveData::Init()
	{
		fDownToUpElpaseTime = 0.f;
		MouseDownTime = 0;
	}

	float   sMouseMoveData::GetDownElpaseTime()
	{
		if( MouseDownTime == 0 )
			return 0.f;
		return (cGameApp::m_sTimeAndFPS.uiCurrentTime-MouseDownTime)/1000.f;
	}
	void	sMouseMoveData::MouseDown(int e_iPosX,int e_iPosY)
	{
		FrameMovePos.x = FrameMovePos.y = 0;
        DownPos.x = e_iPosX;
        DownPos.y = e_iPosY;
        MovePos = DownPos;
        UpPos = DownPos;
        MouseDownTime = cGameApp::m_sTimeAndFPS.uiCurrentTime;
        fDownToUpElpaseTime = 0.f;		
	}
	void	sMouseMoveData::MouseMove(int e_iPosX,int e_iPosY)
	{
		FrameMovePos.x = e_iPosX-MovePos.x;
		FrameMovePos.y = e_iPosY-MovePos.y;
        MovePos.x = e_iPosX;
        MovePos.y = e_iPosY;
	}

	bool	sMouseMoveData::MouseUp(int e_iPosX,int e_iPosY)
	{
		bool	l_bDoubleclick = false;
        UpPos.x = e_iPosX;
        UpPos.y = e_iPosY;        
        fDownToUpElpaseTime = (cGameApp::m_sTimeAndFPS.uiCurrentTime-MouseDownTime)/1000.f;
		//is first mouse up ?check distance and time.
		if( IsDownUpDistanceSamePoint() )
		{
			if( uiLastMouseUpClickElpaseTime == 0 )//mouse up
			{
				uiLastMouseUpClickElpaseTime = cGameApp::m_sTimeAndFPS.uiCurrentTime;
			}//for double click check
			else
			{
				float	l_fLastMouseUptime = (cGameApp::m_sTimeAndFPS.uiCurrentTime-uiLastMouseUpClickElpaseTime)/1000.f;
				if( l_fLastMouseUptime<=DOUBLE_CLICK_TIME_SPAN )
				{
					l_bDoubleclick = true;
				}
				uiLastMouseUpClickElpaseTime = 0;
			}
		}
		return l_bDoubleclick;
	}
        //cClickMouseBehavior(RECT e_RECT){ m_CollisionRange = e_RECT; m_bCollided = false; m_eObjectMouseBehavior = eOMB_NONE; }
        cClickMouseBehavior::cClickMouseBehavior()
        {
			m_TCForRepeat.SetTargetTime(m_sfRepeatTime);
			m_bEnable = true;
			m_bPressedRepeatMouseUp = false;
			m_bKeyDown = false;
			m_ucKeyData = -1;
            m_bCollided = false;
            m_eObjectMouseBehavior = eOMB_NONE;
			m_vCollisionRange.x = m_vCollisionRange.y = m_vCollisionRange.z = m_vCollisionRange.w = -79979.f;
        }
		cClickMouseBehavior::cClickMouseBehavior(cClickMouseBehavior*e_pClickMouseBehavior)
		{
			m_bEnable = true;
			SetName(e_pClickMouseBehavior->GetName());
			m_TCForRepeat.SetTargetTime(e_pClickMouseBehavior->m_TCForRepeat.fTargetTime);
			m_bPressedRepeatMouseUp = e_pClickMouseBehavior->m_bPressedRepeatMouseUp;
			m_bKeyDown = false;
			m_ucKeyData = e_pClickMouseBehavior->m_ucKeyData;
			m_strExternalFunctionName = e_pClickMouseBehavior->m_strExternalFunctionName;
            m_bCollided = false;
            m_eObjectMouseBehavior = eOMB_NONE;
			m_vCollisionRange = e_pClickMouseBehavior->m_vCollisionRange;
		}
        cClickMouseBehavior::~cClickMouseBehavior()
        {
        
        }

        void    cClickMouseBehavior::MouseLeave(int e_iPosX,int e_iPosY)
        {
			if( !m_bEnable )
				return;
            if( m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO )
            {
                InternalMouseLeave(e_iPosX,e_iPosY);
            }
			m_eObjectMouseBehavior = eOMB_LEAVE;
        }

		void	cClickMouseBehavior::CallExternalFunction(void*e_pData)
		{
			cGameApp::CallExternalFunction(m_strExternalFunctionName,e_pData);
		}

        //if touch point is collided return true or false.
		bool    cClickMouseBehavior::InternalCollide(int e_iPosX,int e_iPosY)
		{
			if( !m_bEnable )
				return false;
			//if( this->m_eObjectMouseBehavior == eOMB_HORVER )
			//	return true;
			Vector3*l_pvPos = this->GetLocalPositionPointer();
			if( !l_pvPos )
				return m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY); 
			e_iPosX -= (int)l_pvPos->x;
			e_iPosY -= (int)l_pvPos->y;
			return m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY); 
		}
		bool    cClickMouseBehavior::IsSatisfiedCondition()
		{
			if( this->m_bPressedRepeatMouseUp && this->m_eObjectMouseBehavior == eOMB_HORVER )
			{
				return this->m_TCForRepeat.bTragetTimrReached;
			}
			if(m_eObjectMouseBehavior == eOMB_UP||	m_eObjectMouseBehavior == eOMB_DOUBLU_CLICK_UP)
				return true;
			return false; 
		}
		void    cClickMouseBehavior::Init()
		{
			this->m_MouseMoveData.Init();
			m_TCForRepeat.Start();
			InternalMouseLeave(-1,-1);
			if( this->m_ucKeyData != -1 && cGameApp::m_sucKeyData[this->m_ucKeyData] )
			{
				m_bKeyDown = true;
			}
			else
			{
				m_bKeyDown = false;
			}
			this->m_bCollided = false;
			if( this->m_bPressedRepeatMouseUp && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ) )
			{
				m_eObjectMouseBehavior = eOMB_HORVER;
			}
			else
			{
				m_eObjectMouseBehavior = eOMB_NONE;
			}
		}
        //first time into
        void    cClickMouseBehavior::MouseDown(int e_iPosX,int e_iPosY)
        {
			if( !m_bEnable )
				return;
			//ensure any object inhireechy u have to call cClickMouseBehavior::Init() while init is called.
			if( m_eObjectMouseBehavior != eOMB_NONE )
				return;
			m_TCForRepeat.Start(0.f);
			m_MouseMoveData.MouseDown(e_iPosX,e_iPosY);
            //ensure first time mouse down
            if(InternalCollide(e_iPosX,e_iPosY))
            {
                m_bCollided = true;
                m_eObjectMouseBehavior = eOMB_FIRST_TIME_INTO;
                InternalMouseDown(e_iPosX,e_iPosY);
            }
            else
            {
                //avoid mouse up error
				m_bCollided = false;
                m_eObjectMouseBehavior = eOMB_NONE;
            }
        }
		//
        void    cClickMouseBehavior::MouseMove(int e_iPosX,int e_iPosY)
        {
			if( !m_bEnable )
				return;
			//must first click then into here!.
			if( m_eObjectMouseBehavior != eOMB_HORVER && m_eObjectMouseBehavior != eOMB_FIRST_TIME_INTO )
				return;
			m_MouseMoveData.MouseMove(e_iPosX,e_iPosY);
            if(InternalCollide(e_iPosX,e_iPosY))
            {
                //m_bCollided is possible be false,because it mouse down at orther place and move to this object!.
                if( m_eObjectMouseBehavior != eOMB_HORVER )
                {
                    m_eObjectMouseBehavior = eOMB_HORVER;
                }
                InternalMouseMove(e_iPosX,e_iPosY);
            }
            else
            {
                if(m_eObjectMouseBehavior == eOMB_HORVER||m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO)
                {
                    MouseLeave(e_iPosX,e_iPosY);
                    m_bCollided = false;
					m_eObjectMouseBehavior = eOMB_NONE;
                }
            }
        }
        //
        void    cClickMouseBehavior::MouseUp(int e_iPosX,int e_iPosY)
        {
			if( !m_bEnable )
				return;
			if( m_eObjectMouseBehavior != eOMB_HORVER && m_eObjectMouseBehavior != eOMB_FIRST_TIME_INTO )
				return;
			bool	l_bDoubleClick = m_MouseMoveData.MouseUp(e_iPosX,e_iPosY);
            if(!m_bCollided)//is it into this area already?
            {
                InternalMouseLeave(e_iPosX,e_iPosY);
                m_eObjectMouseBehavior = eOMB_NONE;
            }
			else
			{
				if( l_bDoubleClick )
				{
					m_eObjectMouseBehavior = eOMB_DOUBLU_CLICK_UP;
					InternalMouseDoubleClickUp(e_iPosX,e_iPosY);
				}
				else
				{
					m_eObjectMouseBehavior = eOMB_UP;
					InternalMouseUp(e_iPosX,e_iPosY);
				}
			}
			m_bCollided = false;
        }
		//
		void	cClickMouseBehavior::ButtonClick()
		{
			if( !m_bEnable )
				return;
			if( m_eObjectMouseBehavior != eOMB_NONE )
				return;
			m_MouseMoveData.MouseDown(0,0);
			m_bKeyDown = true;
			m_eObjectMouseBehavior = eOMB_FIRST_TIME_INTO;
			InternalMouseDown(0,0);
		}

		void	cClickMouseBehavior::ButtonPressed()
		{
			if( !m_bEnable )
				return;
			if( m_eObjectMouseBehavior != eOMB_HORVER && m_eObjectMouseBehavior != eOMB_FIRST_TIME_INTO )
				return;
			m_MouseMoveData.MouseMove(0,0);
			m_eObjectMouseBehavior = eOMB_HORVER;
			InternalMouseMove(0,0);
		}

		void	cClickMouseBehavior::ButtonUp()
		{
			if( !m_bEnable )
				return;
			m_bKeyDown = false;
			if( m_eObjectMouseBehavior != eOMB_HORVER && m_eObjectMouseBehavior != eOMB_FIRST_TIME_INTO )
			{
				m_eObjectMouseBehavior = eOMB_NONE;
				return;
			}
			bool	l_bDoubleClick = m_MouseMoveData.MouseUp(0,0);
			if( l_bDoubleClick )
			{
				m_eObjectMouseBehavior = eOMB_DOUBLU_CLICK_UP;
				InternalMouseDoubleClickUp(0,0);
			}
			else
			{
				m_eObjectMouseBehavior = eOMB_UP;
				InternalMouseUp(0,0);
			}
		}

		void	cClickMouseBehavior::SingnalProcess()
		{
			if( !m_bEnable )
				return;
			if( cGameApp::m_sucKeyData[this->m_ucKeyData] )
			{
				if( !m_bKeyDown )
				{
					ButtonClick();
				}
				else
				{
					ButtonPressed();
				}
				return;
			}
			if( m_bKeyDown )
			{
				ButtonUp();
			}
		}

		void	cClickMouseBehavior::SingnalProcess(unsigned char e_usKey)
		{
			if( !m_bEnable )
				return;
			if( e_usKey == this->m_ucKeyData )
			{
				if( !m_bCollided )
					ButtonClick();
				else
					ButtonPressed();
				return;
			}
			if( m_bCollided )
				ButtonUp();
		}

		void    cClickMouseBehavior::Update(float e_fElpaseTime)
		{
			if( !m_bEnable )
				return;
			if( this->m_bPressedRepeatMouseUp && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ))
			{
				if( m_TCForRepeat.bTragetTimrReached )
				{
					m_TCForRepeat.Start();
					m_eObjectMouseBehavior = eOMB_HORVER;
				}
				else
					this->m_TCForRepeat.Update(e_fElpaseTime);
			}
		}

		void    cClickMouseBehavior::DebugRender()
		{
			if( !m_bEnable )
				GLRender::RenderRectangle(Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.y),m_vCollisionRange.Width(),m_vCollisionRange.Height(),Vector4::HalfBright,0);
			else
				GLRender::RenderRectangle(Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.y),m_vCollisionRange.Width(),m_vCollisionRange.Height(),Vector4::One,0);
		}

		//void	cClickMouseBehavior::SetPos(Vector3 e_vPos)
		//{
		//	Vector2	l_Size = m_vCollisionRange.Size();
		//	m_vCollisionRange = Vector4(e_vPos.x,e_vPos.y,e_vPos.x+l_Size.x,e_vPos.y+l_Size.y);
		//}

		void	cClickMouseBehavior::SetRepeatTime(float e_fRepeatTime)
		{
			this->m_TCForRepeat.SetTargetTime(e_fRepeatTime);
		}

		float	cClickMouseBehavior::GetRepeatTime()
		{
			return this->m_TCForRepeat.fTargetTime;
		}
//end namepsace namespace FATMING_CORE
}
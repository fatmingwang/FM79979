#include "Mouse_Touch.h"
#include "../../../Common/CommonApp.h"
#include "../CommonRender/SimplePrimitive.h"
namespace FATMING_CORE
{
	float cClickMouseBehavior::m_sfRepeatTime = 0.1f;

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
			//cCommonApp::CallExternalFunction(m_strExternalFunctionName,e_pData);
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
			if( this->m_ucKeyData != (unsigned char)-1 && cCommonApp::m_sucKeyData[this->m_ucKeyData] )
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
			if(cCommonApp::m_sucKeyData[this->m_ucKeyData] )
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

		void			cClickMouseBehavior::SingnalProcess(bool e_bSingnalOn)
		{
			if (!m_bEnable)
				return;
			if (e_bSingnalOn)
			{
				if (!m_bKeyDown)
				{
					ButtonClick();
				}
				else
				{
					ButtonPressed();
				}
				return;
			}
			if (m_bKeyDown)
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
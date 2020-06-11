#pragma once
#include "ImageButton.h"
#include "../Parser/AnimationParser.h"
//fuck fon't use this.
namespace FATMING_CORE
{
//===========================
//each option is a MPDI collide
//relative function are mouse move and mouse down!
//all cCueToStartCurveWithTime's first point data,its size must as same as the image size,or collide won't work
//because I am lazy immediately to get correct collision size!.

//MPDI NameRule!!!!
//Name="Selected"
//Name="Horver"
//Name="Idle"
//so we could decide what situation option is by mouse or touch
//===========================

    class   cMultiSelectOptionByMPDI;
    class   cSelectOptionByCueToStartCurveWithTime:public cClickMouseBehavior,public cFMWorkingObjectChanger<cCueToStartCurveWithTime>
    {
		void			AddDefaultObject(cCueToStartCurveWithTime*e_pIdel,cCueToStartCurveWithTime*e_pHorver,cCueToStartCurveWithTime*e_pSelect);
    protected:
        friend class	cMultiSelectOptionByMPDI;
        //if global font render is exists it will show at image\s right side.
        std::wstring    m_strDescription;
        //for state change
        virtual void    InternalMouseMove(int e_iPosX,int e_iPosY)override;
        //first time mouse into
        virtual void    InternalMouseDown(int e_iPosX,int e_iPosY)override;
        //while mouse clicked is up
        virtual void    InternalMouseUp(int e_iPosX,int e_iPosY)override;
        //
        virtual void    InternalMouseLeave(int e_iPosX,int e_iPosY)override;
        //
        virtual bool    InternalCollide(int e_iPosX,int e_iPosY)override;
    public:
        DEFINE_TYPE_INFO()
		//there are 3 ways to seup button here,
		//1st get a MPDIList and get its child by assigned index(specific key words as Idle,Horver,Selected)
		//2nd input Idle,Horver,Selected
		//3rd input Idle,Horver,Selected and set those to new position,then change image to button
		//
		//I preffered 3rd.I would like to make 3 animation and a extract one name as position for each image so as many position I could know the buttons count
		//
        //index indicate which one option is it,e_strDescription is not necessary
        //for ce to get,idle horver and selected MPDI's child(cCueToStartCurveWithTime) by e_iIndex
        cSelectOptionByCueToStartCurveWithTime(cMPDIList*e_pMPDIList,int e_iIndex,wchar_t*e_strDescription = 0);
		//similar above but get cCueToStartCurveWithTime not indicate by specific key words
		cSelectOptionByCueToStartCurveWithTime(cCueToStartCurveWithTime*e_pIdel,cCueToStartCurveWithTime*e_pHorver,cCueToStartCurveWithTime*e_pSelect,wchar_t*e_strDescription = 0);
        //input clone target and set new position.
        //e_pIdel,e_pHorver,e_pSelect is at the MPDI who named Idle,Horver,Selected
        //and those 3 MPDI should only has one child(cCueToStartCurveWithTime)
        //m_pAnimationState[eSOS_SELECTED] this animation can't be animation loop
        cSelectOptionByCueToStartCurveWithTime(cCueToStartCurveWithTime*e_pIdel,cCueToStartCurveWithTime*e_pHorver,cCueToStartCurveWithTime*e_pSelect,Vector3 e_vPos,int e_iPIImageIndex);
		cSelectOptionByCueToStartCurveWithTime(cSelectOptionByCueToStartCurveWithTime*e_pSelectOptionByCueToStartCurveWithTime);
		virtual	cClickMouseBehavior*	Clone()override { return new cSelectOptionByCueToStartCurveWithTime(this); }
        virtual ~cSelectOptionByCueToStartCurveWithTime();

        //inline  virtual void    Init()
        //{
        //    cClickMouseBehavior::Init();
        //    AllAnimationStart();
        //}
        //one of option is mouse up and m_eObjectMouseBehavior is eOMB_UP return true
        virtual bool	IsSatisfiedCondition()override
        {
            //if( cClickMouseBehavior::IsSatisfiedCondition() )
            {
				if( this->m_pCurrentWorkingObject == this->m_ObjectList[eSOS_SELECTED] )
                if(this->m_pCurrentWorkingObject->IsAnimationDone())
                    return true;
            }
            return false;
        }
		//virtual	void	Start(float e_fElpaseTime = 0.f){ cClickMouseBehavior::Start(e_fElpaseTime); }
		virtual	void	Init()override { cClickMouseBehavior::Init(); }
		virtual	void	Destroy()override {}
        virtual void    Update(float e_fElpaseTime)override;
        virtual void    DebugRender()override;
        virtual void    Render()override;
        //enusre m_pCurrentAnimation is not null
        bool    IsAnimationPlayDone(){ return m_pCurrentWorkingObject->IsAnimationDone(); }
        //move animation by relative position.
        inline  virtual void    SetRelativePosition( Vector3 e_vPos )override
        {
            for( int i=0;i<eSOS_MAX;++i )
            {
				this->m_ObjectList[i]->SetPos(this->m_ObjectList[i]->GetPos()+e_vPos);
            }
        }
    };
    //========================
    //resource will keep in this object not a blobal resource manager
    //cMultiSelectOptionByMPDI*g_pMultiSelectOptionByMPDI = 0;
    //g_pMultiSelectOptionByMPDI = new cMultiSelectOptionByMPDI("ColoringBook/SelectionMode/SelectMode.mpdi");;
    //g_pMultiSelectOptionByMPDI->SetName(L"Test");
    //this->m_pPhaseManager->AddObject(g_pMultiSelectOptionByMPDI);
    //this->m_pPhaseManager->SetCurrentCurrentPhase(L"Test");    
    //========================
    class   cMultiSelectOptionByMPDI:public cClickMouseBehaviorVector<cSelectOptionByCueToStartCurveWithTime>
    {
    protected:
        cAnimationParser    m_AnimationParser;
    public:
        DEFINE_TYPE_INFO()
        //cCueToStartCurveWithTime;
        //cMultiPathDynamicImage
        cMultiSelectOptionByMPDI(char*e_strFileName);
        cMultiSelectOptionByMPDI(){}
        virtual ~cMultiSelectOptionByMPDI();
        virtual	void	DebugRender()override;
        inline  virtual void    MouseMove(int e_iPosX,int e_iPosY)override {cClickMouseBehaviorVector<cSelectOptionByCueToStartCurveWithTime>::MouseMove(e_iPosX,e_iPosY);}
        inline  virtual void    MouseDown(int e_iPosX,int e_iPosY)override {cClickMouseBehaviorVector<cSelectOptionByCueToStartCurveWithTime>::MouseDown(e_iPosX,e_iPosY);}
        inline  virtual void    MouseUp(int e_iPosX,int e_iPosY)override {cClickMouseBehaviorVector<cSelectOptionByCueToStartCurveWithTime>::MouseUp(e_iPosX,e_iPosY);}
        inline  virtual bool    IsSatisfiedCondition()override
        {             
            if(this->m_bClickMouseBehaviorSatisfied)
            if(this->m_pCurrentWorkingObject->IsAnimationPlayDone())
            {
                return true;
            }
            return false;
        }
        virtual void    LoadData(char*e_strFileName);        
    };

    //it coube contain cMultiSelectOptionByMPDI or cSelectOptionByCueToStartCurveWithTime list and if mouse down-up distance is
    //begger than the distance we expect,scroll all child data to specific position.
    //here cClickMouseBehavior could be cClickMouseBehavior or cMultiSelectOptionByMPDI
    //if u wanna get MPDI data, please get animation from cSelectOptionByCueToStartCurveWithTime
	//very similar cScroller but must setup MPDI,watch coloring book sample(ImagepackList!?)
    class   cScrollingMSOGroup:public cClickMouseBehaviorVector<cClickMouseBehavior>
    {
        UT::sTimeCounter    m_ScrollingTimeCounter;
        eOrientation m_eOrientation;
        float   m_fRestDistanceToScrolling;
        GET_SET_DEC(float,m_fDistanceToScrolling,GetDistanceToScrolling,SetDistanceToScrolling);
        bool    m_bOnScrolling;
        virtual void    InternalMouseUp(int e_iPosX,int e_iPosY)
        {
            Vector2 l_vDirection;
            if( this->m_MouseMoveData.DownUpDistance(&l_vDirection) >= fabs(m_fDistanceToScrolling) )
            {
                if( !IsEnableToMove(l_vDirection) )
                    return;       
                if(l_vDirection.x>0.f&&m_fDistanceToScrolling<0.f)
                {
                    m_fDistanceToScrolling = -m_fDistanceToScrolling;
                }
                else
                if(l_vDirection.x<0.f&&m_fDistanceToScrolling>0.f)
                {
                    m_fDistanceToScrolling = -m_fDistanceToScrolling;
                }

                m_fRestDistanceToScrolling = (float)fabs(m_fDistanceToScrolling);
                m_bOnScrolling = true;
                m_ScrollingTimeCounter.Start();
            }
        }
        //could it be scrolling?
        virtual bool    IsEnableToMove(Vector2 e_vMoveDirection){ return true; }
        virtual bool    InternalCollide(int e_iPosX,int e_iPosY){ return true; }
    public:
        cScrollingMSOGroup(float e_fDistanceToScrolling,eOrientation e_eOrientation = eO_HORIZONTAL,float e_fScrollingTime = 0.5f);
        virtual ~cScrollingMSOGroup(){}
        bool    IsOnScrolling(){return m_bOnScrolling;}
        //set scrolling position if need
        virtual	void    Update(float e_fElpaseTime);
        inline  virtual void    MouseMove(int e_iPosX,int e_iPosY){if( !m_bOnScrolling )cClickMouseBehaviorVector<cClickMouseBehavior>::MouseMove(e_iPosX,e_iPosY);}
        inline  virtual void    MouseDown(int e_iPosX,int e_iPosY){if( !m_bOnScrolling )cClickMouseBehaviorVector<cClickMouseBehavior>::MouseDown(e_iPosX,e_iPosY);}
        inline  virtual void    MouseUp(int e_iPosX,int e_iPosY){if( !m_bOnScrolling )cClickMouseBehaviorVector<cClickMouseBehavior>::MouseUp(e_iPosX,e_iPosY);}
        cClickMouseBehavior*  GetCurrentWorkingObject()
        {
            if( this->m_pCurrentWorkingObject )
            if(this->m_pCurrentWorkingObject->Type() == cMultiSelectOptionByMPDI::TypeID)
            {
                cMultiSelectOptionByMPDI*l_pMultiSelectOptionByMPDI = dynamic_cast<cMultiSelectOptionByMPDI*>(this->m_pCurrentWorkingObject);
                return l_pMultiSelectOptionByMPDI->GetCurrentWorkingObject();
            }
            return m_pCurrentWorkingObject;
        }
        virtual bool    IsSatisfiedCondition(){ return m_bClickMouseBehaviorSatisfied&&GetCurrentWorkingObject()?true:false;  }
    };
    
    typedef cObjectListWithItsChildren<cMultiSelectOptionByMPDI,cSelectOptionByCueToStartCurveWithTime>    cMSOMPDITree;
//end namespace FATMING_CORE
}
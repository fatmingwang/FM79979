#ifndef _MOUSE_TOUCH_
#define _MOUSE_TOUCH_

#include "../Image/SimplePrimitive.h"
//===========================
//main function is 
//MouseDown
//MouseMove(MouseHorver)    ,m_bCollided is possible be false,because it mouse down at orther place and move to this object!.
//MouseUp                   ,it's possible not mouse down at the object we expect,if it's not first time we into it will call InternalMouseLeave
//MouseLeave                ,it still possible to mouse Leave again and again...
//they all call InternalCollide to detect is it in the range we expect.
//
//main overwrite funciton are InternalMouseXXXXX
//you should overwrite their content 
//
//ex there is a button
//while mouse is click on it or not
//MouseClick is called!!
//if it's in the range InternalMouseDown will be called!.
//then the state will change to eOMB_HORVER
//if mouse up or mouse leave they also call InternalMouseXXXX to implement their goal



//
//cClickMouseBehaviorVector will override write the function
//MouseDown
//MouseMove(MouseHorver)
//MouseUp
//MouseLeave
//because it inherient cNamedTypedObjectVector
//

//===========================
namespace FATMING_CORE
{
	//===========================================
	//the reason why we do want a global sMouseMoveData
	//because the button event just spend a bit calculate works,
	//and we might need to work at multi touch so just keep every mouse has a sMouseMoveData
	//so we could get all touch's data
	//===========================================
    struct  sMouseMoveData
    {
		//the pos for mouse down
        POINT   DownPos;
		//the pos for mouse up
        POINT   UpPos;
		//the pos for mouse move
        POINT   MovePos;
		//the position for how mouse move in each mouse move call.
		POINT	FrameMovePos;
        UINT	MouseDownTime;
        float   fDownToUpElpaseTime;
		//for mouse double click time
		UINT	uiLastMouseUpClickElpaseTime;

        sMouseMoveData(){ MouseDownTime = 0; fDownToUpElpaseTime = 0.f; FrameMovePos.x = FrameMovePos.y = DownPos.x = DownPos.y = UpPos.x = UpPos.y = MovePos.x = MovePos.y = 0; uiLastMouseUpClickElpaseTime = 0; }
        ~sMouseMoveData(){}
		void		Init();
		float		GetDownElpaseTime();
        float		DownUpDistance(Vector2 *e_pvDirection = 0);
        float		DownMoveDistance(Vector2 *e_pvDirection = 0);
        Vector2		DownUp();
        Vector2		DownMove();
		//
		void		MouseDown(int e_iPosX,int e_iPosY);
		void		MouseMove(int e_iPosX,int e_iPosY);
		//if true double click is true or not
		bool		MouseUp(int e_iPosX,int e_iPosY);
		bool		IsDownUpDistanceSamePoint(Vector2 *e_pvDirection = 0);
    };
    //mouse behavior,Mouse Down,Mouse Horver MouseLeave MouseUp
    //basicly it will has a parent cClickMouseBehaviorVector.
    class   cClickMouseBehavior:virtual public Frame
    {
    protected:
		bool		m_bKeyDown;
		//if false mouse won't work
		bool		m_bEnable;
		//for button event.
		GET_SET_DEC(unsigned char,m_ucKeyData,GetKeyData,SetKeyData);
		GET_SET_DEC(bool,m_bPressedRepeatMouseUp,IsPressedRepeatMouseUp,SetPressedRepeatMouseUp);
		UT::sTimeCounter	m_TCForRepeat;
		static	float m_sfRepeatTime;
		//here whoy not use a global share mouse data the reason is:the object could not same coordinate,it's possible use different resolution.
        GET_SET_DEC(sMouseMoveData,m_MouseMoveData,GetMouseMoveData,SetMouseMoveData);
        eObjectMouseBehavior    m_eObjectMouseBehavior;
        //if mouse down is at this object,this object will be true,or false,a member work for the class implement it
		//it's also mean it's in touched
		//please set this at InternalCllided or mouse down or the plac u expaected.
        bool    m_bCollided;
        //the range for vaild collide,simply use for button
        GET_SET_DEC(Vector4,m_vCollisionRange,GetCollisionRange,SetCollisionRange);
        //if mouse up and the position is not similar,it's mean the mouse move from the other,so ignore it
        virtual void			InternalMouseMove(int e_iPosX,int e_iPosY){}
        //first time mouse into
        virtual void			InternalMouseDown(int e_iPosX,int e_iPosY){}
        //while mouse clicked is up
        virtual void    		InternalMouseUp(int e_iPosX,int e_iPosY){}
        //while mouse click and its in the collided,not necessary,most time it call InternalMouseUp
        virtual void    		InternalMouseLeave(int e_iPosX,int e_iPosY){}
        //if mouse double click,if u dud's expect this it will call InternalMouseUp again
		//make sure cGameApp::m_sTimeAndFPS has benn active.
        virtual void    		InternalMouseDoubleClickUp(int e_iPosX,int e_iPosY){ InternalMouseUp(e_iPosX,e_iPosY); }
        //it only call by MouseMove,usually it will set state back to idle...
        virtual void			MouseLeave(int e_iPosX,int e_iPosY);
        //if touch point is collided return true or false.
		virtual bool    		InternalCollide(int e_iPosX,int e_iPosY);
		virtual	void			CallExternalFunction(void*e_pData);
		//whild mouse up and satisfied
		GET_SET_DEC(std::wstring,m_strExternalFunctionName,GetExternalFunctionName,SetExternalFunctionName);
    public:
        //cClickMouseBehavior(RECT e_RECT){ m_CollisionRange = e_RECT; m_bCollided = false; m_eObjectMouseBehavior = eOMB_NONE; }
        cClickMouseBehavior();
		cClickMouseBehavior(cClickMouseBehavior*e_pClickMouseBehavior);
		//CLONE_MYSELF(cClickMouseBehavior);
        virtual ~cClickMouseBehavior();
		bool					IsEnable(){return m_bEnable;}
		virtual	void			SetEnable(bool e_b){ m_bEnable = e_b; }
        eObjectMouseBehavior    GetMouseBehavior(){ return m_eObjectMouseBehavior; }
        //one of option is mouse up and m_eObjectMouseBehavior is eOMB_UP return true
        virtual bool    		IsSatisfiedCondition();
		virtual	void			SetSatisfiedCondition(bool e_b){ if( e_b )m_eObjectMouseBehavior = eOMB_UP;else m_eObjectMouseBehavior = eOMB_NONE; }
        //set state as start.
		//this->m_bCollided = false; m_bKeyDown = false;
		virtual void    		Init();
        //first time into
        virtual void    		MouseDown(int e_iPosX,int e_iPosY);
        //horver and move
        virtual void    		MouseMove(int e_iPosX,int e_iPosY);
        //
        virtual void    		MouseUp(int e_iPosX,int e_iPosY);
		//for keyboard
		virtual void			ButtonClick();
		//for keyboard
		virtual void			ButtonPressed();
		//for keyboard
		virtual void			ButtonUp();
		//for IO
		virtual	void			SingnalProcess(unsigned char e_usKey);
		virtual	void			SingnalProcess();
        //it's possible we want to move the object
        virtual void    		SetRelativePosition( Vector3 e_vPos ){}
        //
        virtual void    		Update(float e_fElpaseTime);
        //
        virtual void    		Render(){}
		//
		virtual void    		DebugRender();
		//
		bool					IsCollided(){ return m_bCollided; }
		//
		bool					IsCollided(int e_iPosX,int e_iPosY){return InternalCollide(e_iPosX,e_iPosY);}

		virtual bool			Collide(int e_iPosX,int e_iPosY){ return InternalCollide(e_iPosX,e_iPosY); }
		void					SetRepeatTime(float e_fRepeatTime);
		float					GetRepeatTime();
		bool					IsKeyDown() { return m_bKeyDown; }
		//for button interface,instead GetLocalPositionPointer()
		//virtual	Vector3*		GetPosPointer(){ return GetLocalPositionPointer(); }
		//instead Frame::SetLocalPosition
		//virtual	void			SetPos(Vector3 e_vPos){ assert(0); }
    };
    //=============================================
    //ensure T has inherient cClickMouseBehavior!
    //a lots object all need to do mouse behavior
    //ex: a bunch of images,we have to decide which one image we selected and if mouse move we have to show the mouse is on
    //the image.
	//if one of children has been collided,mouse behavior will skip rest in the vector.
    //=============================================
    template<class T>   class cClickMouseBehaviorVector:public cNamedTypedObjectVector<T>,public cClickMouseBehavior
    {
		bool					m_bReleaseCurrentWorkingObjectWhileReclickable;//rechoice if true
		bool					m_bReclickable;//rechoice if true
		virtual	void			AddObjectNotification(T* e_t){ if(e_t)this->AddChild(e_t); }
		virtual	void			RemoveObjectNotification(T* e_t){ if(e_t)e_t->SetParent(0); }
    protected:
		//while its children has thing want to lock all mouse or render set working aas true or false
		//
		//GET_SET_DEC(bool,m_bWorking,IsWorking,SetWorking);
		//while mouse is doen and the object is collided,if this is not nullptr,mouse behavior only work for it,if mouse is worked but mouse move is leaved,it won't work again
		T*						m_pCurrentMouseDownCollidedObject;
		//the working object which is satisfied.
        T*  					m_pCurrentWorkingObject;
        //one of option condition is satisfied,so all mouse behavior is done
        bool 					m_bClickMouseBehaviorSatisfied;
        virtual bool    		RenderViewportCollide(cClickMouseBehavior*e_pClickMouseBehavior){ return true; }
    public:
        cClickMouseBehaviorVector(){ this->m_pCurrentMouseDownCollidedObject = 0;this->m_pCurrentWorkingObject = 0; this->m_bClickMouseBehaviorSatisfied = false;	 m_bReleaseCurrentWorkingObjectWhileReclickable = true; m_bReclickable = false; m_bIgnoreChildrenUpdate = true; }
		cClickMouseBehaviorVector(cClickMouseBehaviorVector<T>*e_pClickMouseBehaviorList):cNamedTypedObjectVector<T>(e_pClickMouseBehaviorList),cClickMouseBehavior(e_pClickMouseBehaviorList)
		{
			//ADD_CLONE_DATA_TO_NEW_VECTOR(T,e_pClickMouseBehaviorList,this);
			m_bIgnoreChildrenUpdate = e_pClickMouseBehaviorList->m_bIgnoreChildrenUpdate;
			m_bReclickable = e_pClickMouseBehaviorList->m_bReclickable;
			m_bReleaseCurrentWorkingObjectWhileReclickable = e_pClickMouseBehaviorList->m_bReleaseCurrentWorkingObjectWhileReclickable;
			this->m_pCurrentMouseDownCollidedObject = 0;
			this->m_pCurrentWorkingObject = 0;
			this->m_bClickMouseBehaviorSatisfied = false;
		}
		CLONE_MYSELF(cClickMouseBehaviorVector);
        virtual ~cClickMouseBehaviorVector(){}
		void					SetReleaseCurrentWorkingObjectWhileReclickable(bool e_b){m_bReleaseCurrentWorkingObjectWhileReclickable = e_b;}
		bool					IsReleaseCurrentWorkingObjectWhileReclickable(){return m_bReleaseCurrentWorkingObjectWhileReclickable;}

		void					SetReclickable(bool e_b){m_bReclickable = e_b;}
		bool					IsReclickable(){return m_bReclickable;}
		T*						GetCurrentMouseDownCollidedObject(){ return m_pCurrentMouseDownCollidedObject; }
		void					SetCurrentMouseDownCollidedObject(T*e_pData){ m_pCurrentMouseDownCollidedObject = e_pData; }

        inline  virtual void    Init()
        {
            cClickMouseBehavior::Init();
            this->m_bClickMouseBehaviorSatisfied = false;
			this->m_pCurrentMouseDownCollidedObject = 0;
            this->m_pCurrentWorkingObject = 0;
            int l_iCount = this->Count();
			if( l_iCount > 0 )
			{
				Vector4	l_vDrawRect(FLT_MAX,FLT_MAX,FLT_MIN,FLT_MIN);
				for(int i=0;i<l_iCount;++i  )
				{
					T*l_p = this->GetObject(i);
					if( l_p )
					{
						l_p->Init();
						l_vDrawRect = UT::GetBiggerRect(l_vDrawRect,l_p->GetCollisionRange());
					}
				}
				this->m_vCollisionRange = l_vDrawRect;
			}
        }
        //first time into
        inline  virtual void    MouseDown(int e_iPosX,int e_iPosY)
        {
			m_bCollided = false;
            if( this->m_bClickMouseBehaviorSatisfied && !m_bReclickable )
			{
				//m_pCurrentWorkingObject->MouseDown(e_iPosX,e_iPosY);
                return;
			}
			//we also could set m_pCurrentMouseDownCollidedObject
			if( (!m_pCurrentWorkingObject && !this->m_pCurrentMouseDownCollidedObject) || m_bReclickable )
			{
				int l_iCount = this->Count();
				if( l_iCount != 0 )
				{
					if( m_pCurrentWorkingObject && m_bReleaseCurrentWorkingObjectWhileReclickable )
					{
						m_pCurrentWorkingObject->Init();
						m_pCurrentWorkingObject = 0;
					}
					for(int i=0;i<l_iCount;++i  )
					{
						T*l_pData = this->m_ObjectList[i];
						if( l_pData->GetMouseBehavior() != eOMB_NONE )
						{
							l_pData->Init();
						}
					}
					for(int i=0;i<l_iCount;++i  )
					{
						T*l_pData = this->m_ObjectList[i];
						if(!l_pData->IsEnable())
							continue;
						l_pData->MouseDown(e_iPosX,e_iPosY);
						if(l_pData->IsCollided())
						{
							this->m_pCurrentMouseDownCollidedObject = l_pData;
							m_bCollided = true;
							if( m_pCurrentWorkingObject && m_pCurrentWorkingObject != l_pData )
							{
								m_pCurrentWorkingObject->Init();
							}
							return;
						}
					}
				}
				this->m_pCurrentMouseDownCollidedObject = 0;
				cClickMouseBehavior::MouseDown(e_iPosX,e_iPosY);
			}
			else
				m_bCollided = true;
        }
        inline  virtual void    MouseMove(int e_iPosX,int e_iPosY)
        {
            if( this->m_bClickMouseBehaviorSatisfied && !m_bReclickable )
			{
				//m_pCurrentWorkingObject->MouseMove(e_iPosX,e_iPosY);
                return;
			}
			if( this->m_pCurrentMouseDownCollidedObject )
			{
				this->m_pCurrentMouseDownCollidedObject->MouseMove(e_iPosX,e_iPosY);
				return;
			}
            cClickMouseBehavior::MouseMove(e_iPosX,e_iPosY);
        }
        //
        inline  virtual void    MouseUp(int e_iPosX,int e_iPosY)
        {
            if( this->m_bClickMouseBehaviorSatisfied && !m_bReclickable )
			{
				//m_pCurrentWorkingObject->MouseUp(e_iPosX,e_iPosY);
                return;
			}
			m_bCollided = false;
			if( this->m_pCurrentMouseDownCollidedObject )
			{
				this->m_pCurrentMouseDownCollidedObject->MouseUp(e_iPosX,e_iPosY);
				if( this->m_pCurrentMouseDownCollidedObject->IsSatisfiedCondition() )
					m_pCurrentWorkingObject = this->m_pCurrentMouseDownCollidedObject;
				this->m_pCurrentMouseDownCollidedObject = 0;
				return;
			}
            cClickMouseBehavior::MouseUp(e_iPosX,e_iPosY);
        }
        //if m_pCurrentWorkingObject is not nullptr m_bClickMouseBehaviorSatisfied is true
        T*  					GetCurrentWorkingObject(){return this->m_pCurrentWorkingObject;}
		int  					GetCurrentWorkingObjectIndex(){if( !m_pCurrentWorkingObject )return -1;return this->GetObjectIndexByPointer(m_pCurrentWorkingObject);}
		const wchar_t*			GetCurrentWorkingObjectName(){if( !m_pCurrentWorkingObject )return 0;return m_pCurrentWorkingObject->GetName();}
        virtual bool    		IsSatisfiedCondition(){ return this->m_bClickMouseBehaviorSatisfied;  }
		void					SetSatisfiedCondition(bool e_b){ this->m_bClickMouseBehaviorSatisfied = e_b;}
        inline  virtual void    SetRelativePosition( Vector3 e_vPos )
        {
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->SetRelativePosition(e_vPos);
            }
        }
        virtual void    		Update(float e_fElpaseTime)
        {
            //all mouse event condition is filled so ignore all child update.
            //only update current workingobject
            //for further use please overwrite this function!.
            if( this->m_bClickMouseBehaviorSatisfied )
            {
				if( m_pCurrentWorkingObject )
					this->m_pCurrentWorkingObject->Update(e_fElpaseTime);
				if( !m_bReclickable )
					return;
            }
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->Update(e_fElpaseTime);
                if( this->m_ObjectList[i]->IsSatisfiedCondition() )
                {
                    this->m_bClickMouseBehaviorSatisfied = true;
                    this->m_pCurrentWorkingObject = this->m_ObjectList[i];
                    return;
                }
            }
        }
        virtual     void		Render()
        {
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                if( RenderViewportCollide(this->m_ObjectList[i]) )
                    this->m_ObjectList[i]->Render();
            }
        }
        virtual     void		DebugRender()
        {
			//not cool here but I am lzy to fix this
			//GLRender::RenderRectangle(this->m_vCollisionRange.Width(),m_vCollisionRange.Height(),this->GetWorldTransform(),Vector4::One);

			GLRender::RenderRectangle(Vector2(m_vCollisionRange.x,m_vCollisionRange.y),this->m_vCollisionRange.Width(),m_vCollisionRange.Height(),Vector4::One,0);
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				this->m_ObjectList[i]->DebugRender();
            }
        }
		virtual	int				IsChildrenCollided()
		{
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				if(this->m_ObjectList[i]->IsCollided())
					return i;
            }
			return -1;
		}
		virtual	int				IsChildrenCollided(int e_iPosX,int e_iPosY)
		{
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				if(this->m_ObjectList[i]->IsCollided(e_iPosX,e_iPosY))
					return i;
            }
			return -1;
		}
		virtual	int				IsChildrenSatisfied()
		{
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				if(this->m_ObjectList[i]->IsSatisfiedCondition())
					return i;
            }
			return -1;		
		}
		virtual	const wchar_t*			GetNextPhaseName()
		{
			if( m_pCurrentWorkingObject )
				return m_pCurrentWorkingObject->GetName();
			return 0;
		}

		virtual	void			SingnalProcess()
		{
			//if( m_pCurrentWorkingObject )
			//	return;
			//if( m_pCurrentMouseDownCollidedObject )
			//{
			//	m_pCurrentMouseDownCollidedObject->SingnalProcess();
			//	if( m_pCurrentMouseDownCollidedObject->IsSatisfiedCondition() )
			//	{
			//		m_pCurrentWorkingObject = m_pCurrentMouseDownCollidedObject;
			//		m_pCurrentMouseDownCollidedObject = 0;
			//	}
			//	return ;
			//}
			int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				this->m_ObjectList[i]->SingnalProcess();
				if(this->m_ObjectList[i]->IsCollided())
				{
					//m_pCurrentMouseDownCollidedObject = this->m_ObjectList[i];
					//return;
				}
            }		
		}
		virtual void			SingnalProcess(unsigned char e_usKey)
		{
			//if( m_pCurrentWorkingObject )
			//	return;
			//if( m_pCurrentMouseDownCollidedObject )
			//{
			//	m_pCurrentMouseDownCollidedObject->SingnalProcess();
			//	if( m_pCurrentMouseDownCollidedObject->IsSatisfiedCondition() )
			//	{
			//		m_pCurrentWorkingObject = m_pCurrentMouseDownCollidedObject;
			//		m_pCurrentMouseDownCollidedObject = 0;
			//	}
			//	return ;
			//}
			int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				this->m_ObjectList[i]->SingnalProcess(e_usKey);
				if(this->m_ObjectList[i]->IsCollided())
				{
					//m_pCurrentMouseDownCollidedObject = this->m_ObjectList[i];
					//return;
				}
            }
		}
    };
    //=================
    //T must inhirent cClickMouseBehavior
    //if a simplephase has a lot object and all of them are inhirent cClickMouseBehavior,
    //we could push into cDoWorkingObjecInMouseBehaviorList,then we could just do the mouse behavior for Working Object.
    //while first internal collided is true,should't call internal collided anymore
    //override InternalCollide to find out which one is m_pCurrentWorkingObject.
    //=================
    //template<class T> class   cDoWorkingObjecInMouseBehaviorList:virtual public cFMWorkingObjectChanger<T>,virtual public cClickMouseBehavior
    //{
    //protected:
    //    inline  virtual void    InternalMouseMove(int e_iPosX,int e_iPosY)
    //    {
    //        if( this->m_pCurrentWorkingObject )
    //        {
    //            this->m_pCurrentWorkingObject->MouseMove(e_iPosX,e_iPosY);
    //        }
    //    }
    //    //first time into
    //    inline  virtual void    InternalMouseDown(int e_iPosX,int e_iPosY)
    //    {
    //        if( this->m_pCurrentWorkingObject )
    //        {
    //            this->m_pCurrentWorkingObject->MouseDown(e_iPosX,e_iPosY);
    //        }            
    //    }
    //    //
    //    inline  virtual void    InternalMouseUp(int e_iPosX,int e_iPosY)
    //    {
    //        if( this->m_pCurrentWorkingObject )
    //        {
    //            this->m_pCurrentWorkingObject->MouseUp(e_iPosX,e_iPosY);
    //        }
    //        this->m_pCurrentWorkingObject = 0;
    //    }        
    //public:
    //    cDoWorkingObjecInMouseBehaviorList(){}
    //    virtual ~cDoWorkingObjecInMouseBehaviorList(){}
    //    inline  virtual void    Init()
    //    {
    //        this->m_pCurrentWorkingObject = 0;
    //    }
    //    //if m_pCurrentWorkingObject is not nullptr m_bClickMouseBehaviorSatisfied is true
    //    T*  GetCurrentWorkingObject(){return this->m_pCurrentWorkingObject;}    
    //    void    Update(float e_fElpaseTime)
    //    {
    //        int l_iCount = this->Count();
    //        for( int i=0;i<l_iCount;++i )
    //        {
    //            this->m_ObjectList[i]->Update(e_fElpaseTime);
    //        }
    //    }
    //    void    Render()
    //    {
    //        int l_iCount = this->Count();
    //        for( int i=0;i<l_iCount;++i )
    //        {
    //            this->m_ObjectList[i]->Render();
    //        }
    //    }    
    //};
	//========================
	//just do all the thing
	//========================
    template<class T> class   cDoAllMouseBehaviorList:public cClickMouseBehaviorVector<T>
    {
		GET_SET_DEC(bool,m_bAllObjectsDoneToSatisfied,IsAllObjectsDoneToSatisfied,SetAllObjectsDoneToSatisfied);
    public:
		cDoAllMouseBehaviorList(cDoAllMouseBehaviorList<T>*e_pcDoAllMouseBehaviorList):cClickMouseBehaviorVector<T>(e_pcDoAllMouseBehaviorList){ m_bAllObjectsDoneToSatisfied = e_pcDoAllMouseBehaviorList->m_bAllObjectsDoneToSatisfied; }
        cDoAllMouseBehaviorList(bool e_bAllObjectsDoneToSatisfied = true){m_bAllObjectsDoneToSatisfied = e_bAllObjectsDoneToSatisfied;}
        virtual ~cDoAllMouseBehaviorList(){}
   //     inline  virtual   void    Init()
   //     {
			//cClickMouseBehaviorVector<T>::Init()
   //     }
        inline  virtual   void    Update(float e_fElpaseTime)
        {
			if(!this->m_bEnable)
				return;
			bool	l_bSatisfied  =true;
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->Update(e_fElpaseTime);
				if(!this->m_ObjectList[i]->IsSatisfiedCondition())
				{
					l_bSatisfied = false;
				}
				else
				{
					if( !m_bAllObjectsDoneToSatisfied )
					{
						l_bSatisfied = true;
						break;
					}
				}
            }
			this->m_bClickMouseBehaviorSatisfied = l_bSatisfied;
        }
        inline  virtual   void    Render()
        {
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->Render();
            }
        }
		inline  virtual   void    MouseDown(int e_iPosX,int e_iPosY)
		{
			if(!this->m_bEnable)
				return;
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->MouseDown(e_iPosX,e_iPosY);
				if(this->m_ObjectList[i]->IsCollided())
				{
					this->m_pCurrentMouseDownCollidedObject = this->m_ObjectList[i];
					cClickMouseBehavior::m_bCollided = true;
				}
            }
		}
		inline  virtual   void    MouseMove(int e_iPosX,int e_iPosY)
		{
			if(!this->m_bEnable)
				return;
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->MouseMove(e_iPosX,e_iPosY);
            }
		}
		inline  virtual   void    MouseUp(int e_iPosX,int e_iPosY)
		{
			if(!this->m_bEnable)
				return;
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->MouseUp(e_iPosX,e_iPosY);
				if(this->m_ObjectList[i]->m_eObjectMouseBehavior == eOMB_UP )
				{
					if( cClickMouseBehaviorVector<T>::m_pCurrentMouseDownCollidedObject == cNamedTypedObjectVector<T>::m_ObjectList[i] )
					{
						cClickMouseBehaviorVector<T>::m_pCurrentWorkingObject = cClickMouseBehaviorVector<T>::m_pCurrentMouseDownCollidedObject;
						cClickMouseBehavior::m_bCollided = true;
					}
				}
            }
			this->m_pCurrentMouseDownCollidedObject = 0;
		}
    };
//end namespace FATMING_CORE
}

#endif
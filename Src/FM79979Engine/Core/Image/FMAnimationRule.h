#ifndef	_FATMING_ANIMATION_RULW
#define	_FATMING_ANIMATION_RULW
#include "RenderObject.h"
namespace FATMING_CORE
{
	extern void		SetupShaderViewProjectionMatrix(float*e_pfVPMatrix,bool e_bDoOrientation);
	extern float*	GetCurrentViewProjectionMatrix();
    //=====================
    //a base class for animtion control(LERP)
    //a start time for animation control and a position(matrix is better!?)
    //base render and update interface to be call for any inherit calss
    //three major funciton are internal start,render,update,u have to implement them.
    //some common function have to implement with timing base.
    //=====================
    class	cFMTimeLineAnimationRule:public cRenderObject
    {
		bool			m_bInitCalled;
    protected:
	    //how long we have past
	    float			m_fPastTime;
	    //total time we are going to play
	    float			m_fTotalPlayTime;
		//not exactly need but for quick info
		float			m_fCurrentProgress;
	    //cue time to start
	    GET_SET_DEC(float,m_fStartTime,GetStartTime,SetStartTime);
	    GET_SET_DEC(bool,m_bStart,IsStart,SetStart);						//while the time is over start time the real start is start,not just call start mean it start
		GET_SET_DEC(bool,m_bStayAtLastFrame,IsStayAtLastFrame,SetStayAtLastFrame);
		//how many time we want to play,if the value is -1 it will be loop
	    int				m_iPlayCount;
		int				m_iCurrentPlayCount;
	    //===================================
		void*			m_pUserData;
    private:
	    //the start condition for inherit class to implement
	    virtual	void	InternalInit() = 0;
	    //if m_bAnimationDone is false and start condition is actived,it will be actived.
	    virtual	void	InternalUpdate(float e_fElpaseTime) = 0;
	    //if m_bAnimationDone is false and start condition is actived,it will be actived.
	    virtual	void	InternalRender() = 0;
    public:
	    cFMTimeLineAnimationRule();
	    cFMTimeLineAnimationRule(cFMTimeLineAnimationRule*e_pFatmingAnimationRule);
	    virtual	~cFMTimeLineAnimationRule();
		float*			GetStartTimePointer();
	    virtual void	Init();
	    virtual void	Update(float e_fElpaseTime);
	    virtual void	Render();
	    bool			IsAnimationLoop();
	    virtual bool	IsAnimationDone();
		//
		int				GetCurrentPlayCount();
		void			SetPlayCount(int e_iPlayCount);
	    //the time has been past
	    float			GetPastTime();
		//if true m_iPlayCount -1,or 1
	    virtual	void	SetAnimationLoop(bool e_bLoop);
	    virtual	void	SetAnimationDone(bool e_bAnimationDone);
	    virtual	void	Stop();
		//
	    virtual	void	RearrangeTime(float e_fNewTime) = 0;
		virtual	void	RearrangeTimeByPercent(float e_fPercenttage) = 0;
		virtual	void	UpdateByGlobalTime(float e_fGlobalTime) = 0;
	    virtual	void	RenderByGlobalTime(float e_fTime) = 0;
	    virtual	void	InvertOrder() = 0;
	    //start and last time.if end timeis -1 mean always update
	    virtual	float	GetEndTime() = 0;
		virtual	void	Destroy(){}
		//for extra data
		virtual	void	SetData(void*e_pData,bool e_bForceReplace = true);

		virtual	void	ForAllNodeUpdateByGlobalTime(float e_fGlobalTime);
		virtual	void	ForAllNodeRenderByGlobalTime(float e_fGlobalTime);
    };
    //======================================
    //basicly the TYPE is a inherit class base on cFMTimeLineAnimationRule
    //this is a list to update and render all animation rule objects
    //it also have timing base function for the objects.
    //======================================
    template<class	TYPE>
    class	cFatmingGroupBehaviorList:public cNamedTypedObjectVector<TYPE>,public cFMTimeLineAnimationRule
    {
		cNamedTypedObjectVector<TYPE>*	m_pStayAtLastFrameOrLoopChildrenVector;//some children is stay at last frame or loop so make it keep going
		virtual	void	AddObjectNotification(TYPE* e_t){ if(e_t)this->AddChild(e_t); }
		virtual	void	RemoveObjectNotification(TYPE* e_t){ if(e_t)e_t->SetParent(0); }
    protected:
	    //sometimes the time is up,but the performance is still going,we expected it finished,and stop update
	    //depend what kind performance is it going
	    virtual bool	WaitUntilAllAnimationIsDone(){ return false; }
	    //all path's center
	    Vector3	m_vRotationCenter;
	    //counting number buffer
	    GET_SET_DEC(int,m_iCurrentPlayCount,GetCurrentPlayCount,SetGetCurrentPlayCount);
	    //0 to 1,while progress is i it's end.
	    GET_SET_DEC(float,m_fCurrentProgress,GetCurrentProgress,SetCurrentProgress);
    public:
	    cFatmingGroupBehaviorList();
	    //for clone
	    cFatmingGroupBehaviorList(cFatmingGroupBehaviorList*e_pFatmingGroupBehaviorList);
	    virtual~cFatmingGroupBehaviorList();
		virtual	NamedTypedObject*	Clone(){ return NamedTypedObject::Clone(); }
		virtual	void				Destroy(){ cNamedTypedObjectVector<TYPE>::Destroy(); }
	    virtual	void				InternalInit();
		virtual void				Update(float e_fElpaseTime);
	    virtual	void				InternalUpdate(float e_fElpaseTime);
	    //using global time to update,true while done
	    virtual	void				UpdateByGlobalTime(float e_fGlobalTime);
	    //if u like to change position call SetPosition
		virtual void				Render();
	    virtual	void				InternalRender();
	    virtual	void				RenderByGlobalTime();
		virtual	void				RenderByGlobalTime(float e_fTime){ UpdateByGlobalTime(e_fTime);RenderByGlobalTime(); }
	    //we should call it after AddObject or merge
	    virtual	void				RefreshTotalPlayTime();
	    //start and last time.
		virtual	float				GetEndTime(){ return m_fTotalPlayTime; }
		//do not call this function ,unless you know what are you doing!
		void				SetTotalPlayTime(float e_fEndTime,float e_fStartTime)
		{
			cFMTimeLineAnimationRule::m_fStartTime = e_fStartTime;
			m_fTotalPlayTime = e_fEndTime; 
		}

	    virtual	void				RearrangeTime(float e_fNewTime);
		virtual	void				RearrangeTimeByPercent(float e_fPercenttage);
	    //2 group combine together
	    virtual	void				Merge(cFatmingGroupBehaviorList<TYPE>*e_pTYPEVector);
	    //set point between start time  and last time invert.
	    virtual	void				InvertOrder();
	    //
	    Vector3						GetRotationCenter(){return m_vRotationCenter;}
		int							GetPlayCount();
		//
		virtual	void				SetData(void*e_pData);

	    //if u wanna set done is true,is it done(play done)?
	    //virtual	bool				IsDone(){ return m_fCurrentProgress>=1?true:false; }instead IsAnimationDone
	    //virtual	void				SetLoop(bool e_b){ if(e_b)m_iPlayCount = -1;else m_iPlayCount = 1; }//instead SetAnimationLoop
	    //virtual	bool				IsLoop(){if(m_iPlayCount==-1)return true;return false;}instead IsAnuimationLoop
	    //virtual bool				IsStart(){ return m_fCurrentProgress>0.f?true:false; }
    };

    //======================================
    //
    //======================================
    template<class	TYPE>
    cFatmingGroupBehaviorList<TYPE>::cFatmingGroupBehaviorList()
    {
	    m_iPlayCount = 1;
	    m_fTotalPlayTime = 0.f;
	    m_fCurrentProgress = 0.f;
	    m_iCurrentPlayCount = 0;
	    m_vRotationCenter = Vector3::Zero;
		m_pStayAtLastFrameOrLoopChildrenVector = new cNamedTypedObjectVector<TYPE>();
		m_pStayAtLastFrameOrLoopChildrenVector->SetFromResource(true);
    }
    //======================================
	//
	//======================================
    template<class	TYPE>
	cFatmingGroupBehaviorList<TYPE>::cFatmingGroupBehaviorList(cFatmingGroupBehaviorList<TYPE>*e_pFatmingGroupBehaviorList)
	:cFMTimeLineAnimationRule(e_pFatmingGroupBehaviorList)
	//:cNamedTypedObjectVector<TYPE>(e_pFatmingGroupBehaviorList),cFMTimeLineAnimationRule(e_pFatmingGroupBehaviorList),the reason why not calling cNamedTypedObjectVector<TYPE>(e_pFatmingGroupBehaviorList) is because AddObjectNotification
    {
		this->CloneFromList(e_pFatmingGroupBehaviorList);
		SetName(e_pFatmingGroupBehaviorList->GetName());
	    m_iPlayCount = e_pFatmingGroupBehaviorList->GetPlayCount();
	    m_fTotalPlayTime = e_pFatmingGroupBehaviorList->GetEndTime();
	    m_fCurrentProgress = 0.f;
	    m_iCurrentPlayCount = 0;
	    m_vRotationCenter = e_pFatmingGroupBehaviorList->m_vRotationCenter;
		m_pStayAtLastFrameOrLoopChildrenVector = new cNamedTypedObjectVector<TYPE>();
		m_pStayAtLastFrameOrLoopChildrenVector->SetFromResource(true);
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    cFatmingGroupBehaviorList<TYPE>::~cFatmingGroupBehaviorList()
    {
		SAFE_DELETE(m_pStayAtLastFrameOrLoopChildrenVector);
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::InternalInit()
    {
	    m_iCurrentPlayCount = 0;
	    m_fCurrentProgress = 0.f;
	    for( UINT i=0;i<this->m_ObjectList.size();++i )
		{
		    ((cFMTimeLineAnimationRule*)this->m_ObjectList[i])->Init();
		}
		if( m_pStayAtLastFrameOrLoopChildrenVector->Count() == 0 )
		{
			for( UINT i=0;i<this->m_ObjectList.size();++i )
			{
				//((cFMTimeLineAnimationRule*)this->m_ObjectList[i])->Init();
				if(((cFMTimeLineAnimationRule*)this->m_ObjectList[i])->IsStayAtLastFrame() || ((cFMTimeLineAnimationRule*)this->m_ObjectList[i])->IsAnimationLoop())
				{
					m_pStayAtLastFrameOrLoopChildrenVector->AddObjectNeglectExist(this->m_ObjectList[i]);
				}
			}		
		}
    }
    //======================================
    //
    //======================================
	template<class	TYPE>
	void	cFatmingGroupBehaviorList<TYPE>::Update(float e_fElpaseTime)
	{
		cFMTimeLineAnimationRule::Update(e_fElpaseTime);
		if( cFMTimeLineAnimationRule::IsAnimationDone() )
		{
			int	l_iCount = m_pStayAtLastFrameOrLoopChildrenVector->Count();
			for( int i=0;i<l_iCount;++i )
			    ((cFMTimeLineAnimationRule*)(*m_pStayAtLastFrameOrLoopChildrenVector)[i])->Update(e_fElpaseTime);
		}
	}
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::InternalUpdate(float e_fElpaseTime)
    {
	    if( m_iPlayCount != m_iCurrentPlayCount )
	    {
		    m_fCurrentProgress = this->m_fPastTime/this->m_fTotalPlayTime;
			size_t	l_iCount = this->m_ObjectList.size();
		    for( UINT i=0;i<l_iCount;++i )
			    ((cFMTimeLineAnimationRule*)this->m_ObjectList[i])->Update(e_fElpaseTime);
		    if( m_fCurrentProgress >= 1.f )
		    {
			    ++m_iCurrentPlayCount;
			    if( m_iPlayCount == -1 ||//if loop
				    m_iPlayCount > m_iCurrentPlayCount )
			    {
				    assert(m_fTotalPlayTime>0.f);
				    if(m_fPastTime>=m_fTotalPlayTime)
				    {
					    m_fPastTime = GetFloatModulus(m_fPastTime,m_fTotalPlayTime);
				    }
				    for( UINT i=0;i<l_iCount;++i )
				    {
				        //if(this->m_ObjectList[i]->IsAnimationLoop())
					    //if(!this->m_ObjectList[i]->IsAnimationDone())//here is quit weired to force the loop event to restart so here we skip to start if it's loop.
						//compile do not like me....fuck it must filled a  time or it filled 1.0f autoly
						    ((cFMTimeLineAnimationRule*)this->m_ObjectList[i])->Init();
				    }
				    //update to the time has elpase
				    if( m_fPastTime>0.f )
					    this->Update(m_fPastTime);
			    }
				else
				{
					this->SetAnimationDone(true);
				}
		    }
	    }
	    else//peformance is still going,but the count is reached.
	    if(WaitUntilAllAnimationIsDone())
	    {
		    for( UINT i=0;i<this->m_ObjectList.size();++i )
			    ((cFMTimeLineAnimationRule*)this->m_ObjectList[i])->Update(e_fElpaseTime);
	    }
    }

	template<class	TYPE>
	int	cFatmingGroupBehaviorList<TYPE>::GetPlayCount(){ return m_iPlayCount; }

    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::UpdateByGlobalTime(float e_fGlobalTime)
    {
	    m_fPastTime = e_fGlobalTime;
	    m_fCurrentProgress = this->m_fPastTime/this->m_fTotalPlayTime;
	    if(this->m_iPlayCount == -1)
	    {
		    if( m_fCurrentProgress>1.f )
		    {
			    m_fPastTime = e_fGlobalTime-m_fPastTime;
			    m_fCurrentProgress = this->m_fPastTime/this->m_fTotalPlayTime;
		    }
	    }
    }
	template<class	TYPE>
	void	cFatmingGroupBehaviorList<TYPE>::Render()
	{
		cFMTimeLineAnimationRule::Render();
		if( cFMTimeLineAnimationRule::IsAnimationDone() )
		{
			int	l_iCount = m_pStayAtLastFrameOrLoopChildrenVector->Count();
			for( int i=0;i<l_iCount;++i )
			    ((cFMTimeLineAnimationRule*)(*m_pStayAtLastFrameOrLoopChildrenVector)[i])->Render();
		}
	}
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::InternalRender()
    {
	    if( m_fCurrentProgress >= 1.f && !WaitUntilAllAnimationIsDone() )
		    return;
	    for( UINT i=0;i<this->m_ObjectList.size();++i )
	    {
		    TYPE* l_pTYPE = this->m_ObjectList[i];
		    if( l_pTYPE->IsAnimationDone() )
			    continue;
		    ((cFMTimeLineAnimationRule*)l_pTYPE)->Render();
	    }
    }

    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::RenderByGlobalTime()
    {
	    if( m_fCurrentProgress > 1.f )
		    return;
	    //int	l_iLastImageIndex = -1;
	    for( UINT i=0;i<this->m_ObjectList.size();++i )
	    {
		    this->m_ObjectList[i]->RenderByGlobalTime(this->m_fPastTime);
	    }
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::RefreshTotalPlayTime()
    {
	    this->m_fTotalPlayTime = 0.f;
	    for( UINT i=0;i<this->m_ObjectList.size();++i )
	    {
		    TYPE* l_pType = this->m_ObjectList[i];
		    float	l_fTime = l_pType->GetEndTime();
		    if(m_fTotalPlayTime<l_fTime)
			    m_fTotalPlayTime = l_fTime;
	    }
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::InvertOrder()
    {
	    int	l_iNum = this->Count();
	    for( int i=0;i<l_iNum;++i )
		    (*this)[i]->InvertOrder();
    }

    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::RearrangeTime(float e_fNewTime)
    {
		float	l_fPercenttage = e_fNewTime/this->m_fTotalPlayTime;
		RearrangeTimeByPercent(l_fPercenttage);
    }

	template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::RearrangeTimeByPercent(float e_fPercenttage)
	{
	    if( m_fTotalPlayTime!=0.f )
	    {
			cFMTimeLineAnimationRule::m_fStartTime *= e_fPercenttage;
		    int	l_iNum = this->Count();
		    for( int i=0;i<l_iNum;++i )
		    {
			    TYPE*	l_pType = (*this)[i];
			    l_pType->RearrangeTimeByPercent(e_fPercenttage);
		    }
		    m_fTotalPlayTime *= e_fPercenttage;
	    }	
	}
    //===============================
    //
    //===============================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::Merge(cFatmingGroupBehaviorList<TYPE>* e_pTYPEVector)
    {
		if( e_pTYPEVector && this == e_pTYPEVector )
			return;
	    int	l_iNum = e_pTYPEVector->Count();
	    for(int i =0;i<l_iNum;++i)
	    {
		    TYPE* l_pOriginal = (*e_pTYPEVector)[i];
		    TYPE* l_pNewOne = dynamic_cast<TYPE*>(l_pOriginal->Clone());
		    while(!this->AddObject(l_pNewOne))
		    {
			    static int	l_isSeriesIndex = 0;
			    ++l_isSeriesIndex;
				std::wstring	l_strIDName = l_pOriginal->GetName();
				l_strIDName += ValueToStringW(l_isSeriesIndex);
				l_pNewOne->SetName(l_strIDName.c_str());
		    }
	    }
    }

	template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::SetData(void*e_pData)
	{
	    int	l_iNum = this->Count();
	    for( int i=0;i<l_iNum;++i )
	    {
		    TYPE*	l_pType = (*this)[i];
		    l_pType->SetData(e_pData);
	    }
	}
}
#endif
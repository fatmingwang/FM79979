#ifndef	_EVENT_TRIGGER_H_
#define _EVENT_TRIGGER_H_
//reference
//http://www.codeproject.com/Tips/860245/Trigger-class-for-Cplusplus
//
//to do: GetFunctionAddress is not implement now.
//
template<class INPUT_PARAMETER, class DATA_KEY = std::string>
class EventListTrigger
{
public:
    typedef std::function<void (INPUT_PARAMETER)>										TRIGGER_FUNCTION;
	typedef	 std::map< size_t,TRIGGER_FUNCTION >											OWNER_ID_AND_EVENT_TRIGGER_MAP;//size_t for owner ID,it could be a object or a unique ID.
	typedef std::pair< size_t, TRIGGER_FUNCTION >											OWNER_ID_AND_EVENT_TRIGGER_MAP_PAIR;
    typedef std::map< DATA_KEY, OWNER_ID_AND_EVENT_TRIGGER_MAP >	TRIGGER_MAP;
    typedef std::pair< DATA_KEY, OWNER_ID_AND_EVENT_TRIGGER_MAP >		TRIGGER_PAIR;

    EventListTrigger()
	{
        //m_Triggers.reset(new TRIGGER_MAP);
    };

    virtual ~EventListTrigger()
	{
    };
	//http://stackoverflow.com/questions/18039723/c-trying-to-get-function-address-from-a-stdfunction
	//http://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
	//static size_t GetFunctionAddress(TRIGGER_FUNCTION e_Function)
	size_t GetFunctionAddress(TRIGGER_FUNCTION e_Function)
	{
		//FunctionType ** fnPointer = e_Function.template target<FunctionType*>();
		//return (size_t) *fnPointer;
		return 0;
	}

    //Register a class member trigger function.
    template<class FUNCTION_OWNER>
    bool AddTrigger(DATA_KEY e_Key, TRIGGER_FUNCTION e_Function, FUNCTION_OWNER*e_pFunctionOwner)
	{
		if( e_pFunctionOwner == 0 )
			return false;
		size_t	l_OwnerID = (size_t)e_pFunctionOwner;
		return AddTrigger(e_Key,e_Function,l_OwnerID,GetFunctionAddress(e_Function));
		return 0;
    }

    //Register a trigger function.
    bool AddTrigger(DATA_KEY e_Key, TRIGGER_FUNCTION e_Function,int e_iOwnerID,size_t e_uiFunctionID)
	{
		size_t	l_OwnerID = e_iOwnerID;
		//get by key
        typename TRIGGER_MAP::iterator l_Iterator = m_Triggers.find(e_Key);
		OWNER_ID_AND_EVENT_TRIGGER_MAP	l_EmptyMap;
		OWNER_ID_AND_EVENT_TRIGGER_MAP*	l_pOwnerIDAndEventTriggerEventMap = 0;
		bool	l_bExists = false;
		if( l_Iterator != m_Triggers.end() )
		{
			l_pOwnerIDAndEventTriggerEventMap = &l_Iterator->second;
			l_bExists = true;
		}
		else
		{
			l_pOwnerIDAndEventTriggerEventMap = &l_EmptyMap;
		}
		//check the owner and function is not same,or return false.
		typename OWNER_ID_AND_EVENT_TRIGGER_MAP::iterator	l_OwnerIDAndEventTriggerEventMapIterator;
		l_OwnerIDAndEventTriggerEventMapIterator = l_pOwnerIDAndEventTriggerEventMap->find(l_OwnerID);
		//no this id or function is not same
		if( l_OwnerIDAndEventTriggerEventMapIterator == l_pOwnerIDAndEventTriggerEventMap->end() /*|| e_uiFunctionID != GetFunctionAddress(l_OwnerIDAndEventTriggerEventMapIterator->second) not working now*/)
		{
			l_pOwnerIDAndEventTriggerEventMap->insert(OWNER_ID_AND_EVENT_TRIGGER_MAP_PAIR(l_OwnerID,e_Function));
			if( !l_bExists )
			{
				m_Triggers.insert(TRIGGER_PAIR(e_Key,l_EmptyMap));
			}
			return true;
		}
        return false;
    }
	//======================
	template<class FUNCTION_OWNER>
	bool	RemoveTrigger(DATA_KEY e_Key,FUNCTION_OWNER*e_pFunctionOwner)
	{
		if( !e_pFunctionOwner )
			return false;
		size_t	l_OwnerID = (size_t)e_pFunctionOwner;
		RemoveTrigger(e_Key,l_OwnerID);
	}
	//======================
	bool	RemoveTrigger(DATA_KEY e_Key,size_t e_uiID)
	{
		typename TRIGGER_MAP::iterator l_Iterator = m_Triggers.find(e_Key);
		if( l_Iterator == m_Triggers.end() )
		{
			return false;
		}
		OWNER_ID_AND_EVENT_TRIGGER_MAP*	l_pOwnerIDAndEventTriggerEventMap = &l_Iterator->second;
		typename OWNER_ID_AND_EVENT_TRIGGER_MAP::iterator	l_Iterator2 = l_pOwnerIDAndEventTriggerEventMap->begin();
		for( ;l_Iterator2 != l_pOwnerIDAndEventTriggerEventMap->end();++l_Iterator2 )
		{
			if( l_Iterator2->first == e_uiID )
			{
				l_pOwnerIDAndEventTriggerEventMap->erase(l_Iterator2);
				return true;
			}
		}
		return false;
	}
	//call trigger function
    bool Shot(DATA_KEY e_Key, int e_InputParameter)
	{
		bool	l_bCalled = false;
        typename TRIGGER_MAP::iterator l_Iterator = m_Triggers.find(e_Key);
        if( l_Iterator != m_Triggers.end())
		{
			OWNER_ID_AND_EVENT_TRIGGER_MAP	l_MapData = l_Iterator->second;
			for( typename OWNER_ID_AND_EVENT_TRIGGER_MAP::iterator l_Iterator2 = l_MapData.begin();l_Iterator2 != l_MapData.end(); ++l_Iterator2  )
			{
				l_Iterator2->second(e_InputParameter);
				l_bCalled = true;
			}
        }
		return l_bCalled;
    };

private:
    TRIGGER_MAP	m_Triggers;
};

//sample
//void AddMoneyToUI2(int e_iMoney)
//{
//	int a = 0;
//	//dosomething
//}
//class	cMoneyShowStatus
//{
//		EventListTrigger<int,int>m_EventListTrigger;
//public:
//cMoneyShowStatus()
//{
//		m_EventListTrigger.AddTrigger<cMoneyShowStatus>(2266,std::bind(&cMoneyShowStatus::AddMoneyToUI,this,_1),this);
//}
//~cMoneyShowStatus()
//{
//		ml_EventListTrigger.RemoveTrigger(2266,(size_t)this);
//}
//	void	cMoneyShowStatus::AddMoneyToUI( int e_iData )
//	{
//	
//	}
//
//};
//
//int main(int argc, const char **argv)
//{
//		int	l_iEventID = 79979;
//		cMoneyShowStatus	l_MoneyShowStatus;
//		EventListTrigger<int,int>l_EventListTrigger;
//		l_EventListTrigger.AddTrigger(l_iEventID,&AddMoneyToUI2,(size_t)this);
//		l_EventListTrigger.AddTrigger<cMoneyShowStatus>(l_iEventID,std::bind(&cMoneyShowStatus::AddMoneyToUI,&l_MoneyShowStatus,_1),&l_MoneyShowStatus);
//		l_EventListTrigger.Shot(l_iEventID,0);
//     std::cout << status << std::endl;
//     return 0;
//}

#endif
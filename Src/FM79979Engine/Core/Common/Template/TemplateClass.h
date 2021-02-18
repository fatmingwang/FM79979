#ifndef _TEMPLATE_CLASS_H_
#define _TEMPLATE_CLASS_H_
#include <vector>
#include <assert.h>
#include "../CommonDefine.h"
#include "../NamedTypedObject.h"
#include "../Utility.h"
#include "TempContainer.h"
#include "../Log/FMLog.h"
using namespace std;
////====================
////if the object is not belong to the other,itself must to be delete object or release object
////====================
//template <class T>class ObjectAndName
//{
//public:
//	T	*m_pObject;
//	const char*	m_sName;
//	ObjectAndName(const char*e_pString,T* e_p){ m_sName = e_pString; m_pObject = e_p; };
//	~ObjectAndName(){};
//	const char*	GetName(){ return m_sName; }
//	T*		GetObject(){ return m_pObject; }
//	void	SetObject(T*e_p){ m_pObject = e_p; }
//};

//the T must inherit NamedTypedObject
//T must be a pointer
//void    MyAssert2(bool e_b,char*e_str = "Error")
//{
  //      assert(e_b&&e_str);
//}

//only work for the class has new self
#define	CLONE_VECTOR_TO_NEW_VECTOR(TYPE,SRC_VECTOR,DEST_VECTOR)	\
int	l_iCount = SRC_VECTOR->Count();								\
for(int i=0;i<l_iCount;++i)										\
{																\
	TYPE* l_pData = new TYPE((*SRC_VECTOR)[i]);					\
	DEST_VECTOR->AddObjectNeglectExist(l_pData);				\
}


/*#define	ADD_CLONE_DATA_TO_NEW_VECTOR(TYPE,SRC_VECTOR,DEST_VECTOR)	\
int	l_iCount = SRC_VECTOR->Count();									\
for(int i=0;i<l_iCount;++i)											\
{																	\
	TYPE* l_pData = dynamic_cast<TYPE*>((*SRC_VECTOR)[i]->Clone());	\
	DEST_VECTOR->AddObjectNeglectExist(l_pData);					\
}*/


#define LIST_DO_FOR_ALL_CHILDREN(List,FUNCTION) \
{												\
	int l_iCount = List->Count();               \
	for( int i=0;i<l_iCount;++i )               \
	{                                           \
	    (*List)[i]->FUNCTION;                   \
	}											\
}

#define	VECTOR_CLONE(SrcVector,DestVector,Type)			\
		for(size_t i=0;i<SrcVector.size();++i)			\
		{												\
			Type*l_pObject = new Type(SrcVector[i]);	\
			DestVector.push_back(l_pObject);			\
		}												\

template<class TYPE,class CHILDTYPE>
TYPE*	NewTemplateList(TYPE*e_pSource,bool e_bFromREsource)
{
	TYPE* l_pData = new TYPE();
	l_pData->SetName(e_pSource->GetName());
	if( e_bFromREsource )
	{
		l_pData->CopyListPointer(e_pSource);
	}
	else
	{
		int	l_iCount = e_pSource->Count();
		for( int i=0;i<l_iCount;++i  )
		{
			CHILDTYPE* l_p = new CHILDTYPE( (*e_pSource)[i] );
			l_p->SetName((*e_pSource)[i]->GetName());
			l_pData->AddObjectNeglectExist(l_p);
		}
	}
	return l_pData;
}

//template < class LIST,class CHILDREN_DATA_TYPE > class cObjectListWithItsChildren;
//key function RemoveResourceObject.
template <class T> class cNamedTypedObjectVector:virtual public NamedTypedObject
{
	//friend class	cObjectListWithItsChildren<cNamedTypedObjectVector<T*>,T*>;
	//this one to remove the resource if object has something must to delete
	//ex:AnimationParser or particle or else.....
	virtual	void	RemoveResourceObject(NamedTypedObject*e_pObject){}
	//
	virtual	void	AddObjectNotification(T* e_t){}
	virtual	void	RemoveObjectNotification(T* e_t){}
protected:
	vector < T* > m_ObjectList;
	bool	m_bFromResource;
public:
	cNamedTypedObjectVector(){ m_bFromResource = false; }
	cNamedTypedObjectVector(cNamedTypedObjectVector<T>*e_pObjectListByName)
	{
		CloneFromList(e_pObjectListByName);
		SetName(e_pObjectListByName->GetName());
	}
	virtual	NamedTypedObject*	Clone(){ return new cNamedTypedObjectVector<T>(this); }
	virtual ~cNamedTypedObjectVector(){ Destroy(); }
	//ensure what are u doing while old data is not from resource
    inline  void    CloneFromList(cNamedTypedObjectVector<T>*e_pObjectListByName)
    {
        int l_iCount = e_pObjectListByName->Count();
        for( int i=0;i<l_iCount;++i )
        {
			auto l_pObject = e_pObjectListByName->GetObject(i);
            T*l_p = dynamic_cast<T*>(l_pObject->Clone());
			l_p->SetName(l_pObject->GetName());
			assert(l_p);
            this->AddObjectNeglectExist(l_p);
        }
		m_bFromResource = false;
    }
	bool	IsFromResource(){ return m_bFromResource; }
	void	SetFromResource(bool e_bFromResource){ m_bFromResource = e_bFromResource; }
	virtual	void	Destroy()
	{
		if(!m_bFromResource)
		{
			while( m_ObjectList.size() )
			{
				if(!RemoveObject(0))
					m_ObjectList.erase(m_ObjectList.begin());
				//m_ObjectList[i].~T();
			}
		}
		m_ObjectList.clear();
	}

	T*	GetObjectByUniqueID(uint64 e_uiID)
	{
		for(UINT i=0;i<m_ObjectList.size();++i)
		{
			T* l_t = m_ObjectList[i];
			if(l_t->GetUniqueID() == e_uiID)
				return l_t;
		}
		return 0;	
	}
	inline T*	operator [](const wchar_t*e_pString){return GetObject(e_pString);}

	inline T*	operator [](int e_iIndex){return GetObject(e_iIndex);}

	inline T*	GetObject(int e_i){if( e_i<this->Count() && e_i>-1 )return m_ObjectList[e_i];	return 0;}
	inline T*	GetObject(std::wstring e_pString){return this->GetObject(e_pString.c_str());	}
	inline T*	GetObject(const wchar_t*e_pString){return this->GetObject(this->GetObjectIndexByName(e_pString));	}
	virtual NamedTypedObject* GetObjectByFileName(const char*e_strFileName)
	{
		auto l_strObjectName = UT::GetFileNameWithoutFullPath(e_strFileName);
		return this->GetObject(l_strObjectName.c_str());
	}
	NamedTypedObject* GetObjectByFileNameW(const wchar_t*e_strFileName)
	{
		auto l_strFileName = UT::WcharToChar(e_strFileName);
		return GetObjectByFileName(l_strFileName.c_str());
	}
	inline T*	GetObjectWithWarningMessage(int e_i)
	{
		auto l_pObject = this->GetObject(e_i);
		if (!l_pObject)
		{
			UT::ErrorMsg(this->GetName(), L"index not in range");
		}
		return l_pObject;
	}
	inline T*	GetObjectWithWarningMessage(const wchar_t*e_pString)
	{
		auto l_pObject = this->GetObject(this->GetObjectIndexByName(e_pString)); 
		if (!l_pObject)
		{
			UT::ErrorMsg(e_pString,L"not exist");
		}
		return l_pObject;
	}

	inline T*	GetObject(const char*e_pString)
	{
		if(!e_pString)return nullptr;
		std::wstring	l_strName = UT::CharToWchar(e_pString);
		return this->GetObject(l_strName.c_str());
	}

	inline T*	GetLastObject(){	return this->GetObject((int)m_ObjectList.size()-1);}
	inline bool	IsContain(T*e_pObject)
	{
		if( this->GetObjectIndexByPointer(e_pObject) != -1 || this->GetObjectIndexByName(e_pObject->GetName())!= -1 )
			return true;
		return false;
	}
	inline bool	InsertObject(int e_iIndex,T*e_pObject,bool e_bAllowSameName = false)
	{
		if( e_pObject == nullptr )
			return false;
		if( (int)m_ObjectList.size() < e_iIndex )
		{
			return false;
		}
		if( e_bAllowSameName == false )
		{
			if(IsContain(e_pObject))
				return false;
		}
		if((int)m_ObjectList.size() == e_iIndex)
		{
			this->m_ObjectList.push_back(e_pObject);
		}
		else
		{
			this->m_ObjectList.insert(m_ObjectList.begin()+e_iIndex,e_pObject);
		}
		return true;
	}
	//however data is existed or not remove data and add it
	inline bool	ReplaceObject(int e_iIndex,T* e_T,bool e_bDeleteObject = true)
	{
		assert((int)m_ObjectList.size() > e_iIndex);
		//if( e_T )
		{
			T*l_pObject = this->m_ObjectList[e_iIndex];
			if( e_bDeleteObject )
			{
				RemoveObjectNotification(l_pObject);
				SAFE_DELETE(l_pObject);
			}
			this->m_ObjectList[e_iIndex] = e_T;
			AddObjectNotification(e_T);
			return true;
		}
		return false;
	}

	inline T* GetObjectByType(const wchar_t*e_pString)
	{
		size_t l_uiSize = m_ObjectList.size();
		for(size_t i=0;i<l_uiSize;++i  )
		{
			if( m_ObjectList[i]->Type() == e_pString )
				return m_ObjectList[i];
		}
		return 0;
	}
	
	inline T* GetObjectByTypePointer(T*e_pObject)
	{
		size_t l_uiSize = m_ObjectList.size();
		for (size_t i = 0; i<l_uiSize; ++i)
		{
			if( m_ObjectList[i]->Type() == e_pObject->Type() )
				return m_ObjectList[i];
		}
		return 0;
	}	
	//only need to overwrite RemoveObject(int e_iIndex).
	inline	bool	RemoveObject(const wchar_t*e_pString){return RemoveObject(GetObjectIndexByName(e_pString));}
	inline	bool	RemoveObject(std::wstring e_pString) { return RemoveObject(GetObjectIndexByName(e_pString.c_str())); }
	inline	bool	RemoveObject(NamedTypedObject* e_t){return RemoveObject(GetObjectIndexByName(e_t->GetName()));}
	inline	virtual	bool	RemoveObject(int e_iIndex)
	{
		T*l_p = this->GetObject(e_iIndex);
		RemoveObjectWithoutDelete(e_iIndex);
		if( l_p )
		{
			RemoveResourceObject(l_p);
			delete l_p;
			return true;
		}
		return false;
	}

	bool	RemoveObjectWithoutDelete(int e_iIndex)
	{
		T*l_p = this->GetObject(e_iIndex);
		if( l_p )
		{
			RemoveObjectNotification(l_p);
			l_p->SetOwner(0);
			m_ObjectList.erase(m_ObjectList.begin()+e_iIndex);
			return true;
		}
		return false;
	}
	inline	bool	RemoveObjectWithoutDelete(const wchar_t*e_strName){ return RemoveObjectWithoutDelete((wchar_t*)e_strName); }
	inline	bool	RemoveObjectWithoutDelete(wchar_t*e_strName){int	l_iIndex = GetObjectIndexByName(e_strName);if( l_iIndex != -1 )	return RemoveObjectWithoutDelete(l_iIndex);return false;}
	inline	bool	RemoveObjectWithoutDelete(T*e_pObject){int	l_iIndex = GetObjectIndexByPointer(e_pObject);if( l_iIndex != -1 )	return RemoveObjectWithoutDelete(l_iIndex);return false;}
	inline	bool	AddObject(T* e_t){if( !e_t )return false;if( GetObject(e_t->GetName()) )return false;return AddObjectNeglectExist(e_t);}

	inline	bool	AddObjectWarningIfExists(T*e_pData)
	{
		if(!AddObject(e_pData))
		{
			UT::ErrorMsg(L"object exists!",e_pData->GetName());
			return false;
		}
		return true;
	}

	inline	virtual	bool	AddObjectNeglectExist(T* e_t){AddObjectNotification(e_t);m_ObjectList.push_back(e_t);if(e_t)e_t->SetOwner(this);return true;}

	int	Count(){ return (int)m_ObjectList.size(); }

	inline	int	GetObjectIndexByName(const wchar_t* e_pString){return GetObjectIndexByName((wchar_t*)e_pString);}
	inline	int	GetObjectIndexByName(wchar_t* e_pString)
	{
		if( e_pString )
		{
			size_t l_uiSize = m_ObjectList.size();
			for (size_t i = 0; i<l_uiSize; ++i)
			{
				if( !wcscmp(m_ObjectList[i]->GetName(),e_pString) )
				{
					return (int)i;
				}
			}
		}
		return -1;
	}

	inline	int	GetObjectIndexByPointer(T* e_T)
	{
		size_t l_uiSize = m_ObjectList.size();
		for (size_t i = 0; i<l_uiSize; ++i)
		{
			if( m_ObjectList[i] == e_T )
			{
				return (int)i;
			}
		}
		return -1;
	}

	std::vector<T*>*	GetList(){return &m_ObjectList;}
	//=========================
	//set from resource true and assign pointer to dest
	//get data from dest
	//=========================
	void	CopyListPointer(cNamedTypedObjectVector<T>*e_pList)
	{
		Destroy();
		this->m_bFromResource = true;
		int l_iSize = e_pList->Count();
		for (int i = 0; i<l_iSize; ++i)
		{
			m_ObjectList.push_back((*e_pList)[i]);
		}
	}
	//=========================
	//dump data to dest list and clear self
	//beware while has any data in the dest list it could be occor some problem
	//=========================
	virtual	void	DumpListPointer(cNamedTypedObjectVector<T>*e_pList)
	{
		this->m_bFromResource = false;
		size_t l_uiSize = m_ObjectList.size();
		for (size_t i = 0; i<l_uiSize; ++i)
		{
			bool	l_b = e_pList->AddObject(m_ObjectList[i]);
			assert(l_b&&"such file has already exist");
		}
		e_pList->SetFromResource(false);
		m_ObjectList.clear();
	}
	//=========================
	//ensure m_bFromResource set as well
	//
	//=========================
	virtual	void	AddListToMe(cNamedTypedObjectVector<T>*e_pList,bool e_bIgnoreSameName = false,bool e_bIgnoreSameData = false)
	{
		this->m_bFromResource = true;
		int l_uiSize = e_pList->Count();
		for (int i = 0; i<l_uiSize; ++i)
		{
		    if( !e_bIgnoreSameName )
		    {
		        if(this->GetObject(e_pList->m_ObjectList[i]->GetName()))
		            continue;
		    }
			if( !e_bIgnoreSameData )
			{
				if(GetObjectIndexByPointer(e_pList->m_ObjectList[i]) != -1)
				{
					continue;
				}
			}
			m_ObjectList.push_back(e_pList->m_ObjectList[i]);
	    }
	}
	//==================================
	//swap data index
	//2 objects change index
	//==================================
	void	SwapIndex(int e_iIndex1,int e_iIndex2)
	{
		T* l_1 = this->m_ObjectList[e_iIndex1];
		T* l_2 = this->m_ObjectList[e_iIndex2];
		this->m_ObjectList[e_iIndex1] = l_2;
		this->m_ObjectList[e_iIndex2] = l_1;
	}
	//a vector contain a bunch data,now input a index it is possible bigger than last index or smaller 0,
	//so we have to find the proper one if we want it be loop
	int		GetLoopIndex(int e_iCurrentIndex)
	{
		int	l_iTotalCount = (int)m_ObjectList.size();
		if( e_iCurrentIndex<0 )
			return l_iTotalCount-1;
		if( e_iCurrentIndex+1>=l_iTotalCount )
			return l_iTotalCount-1;
		return e_iCurrentIndex;
	}
	//compare 2list to get different data list
	static cNamedTypedObjectVector<T>	GetDifferentObjectList(cNamedTypedObjectVector<T>*e_pList1,cNamedTypedObjectVector<T>*e_pList2)
	{
		cNamedTypedObjectVector<T>	l_DiffObjectList;
		int	l_iCount1 = e_pList1->Count();
		int	l_iCount2 = e_pList2->Count();
		T*l_pObject = 0;
		bool	l_bSame = false;
		for( int i=0;i<l_iCount1;++i )
		{
			l_pObject = e_pList1->GetObject(i);
			for( int j=0;j<l_iCount2;++j )
			{
				if(e_pList2->GetObject(j) == l_pObject)
				{
					l_bSame = true;
					break;
				}
				if( !l_bSame )
				{
					l_DiffObjectList.AddObjectNeglectExist(l_pObject);
				}
			}
		}
		return l_DiffObjectList;
	}
};
//T must inhirent NamedTypedObject
template<class  T>
class   cFMWorkingObjectChanger:public cNamedTypedObjectVector<T>
{
protected:
    T*  m_pCurrentWorkingObject;
public:
    cFMWorkingObjectChanger(){ m_pCurrentWorkingObject = 0;}
    virtual ~cFMWorkingObjectChanger(){}
	cFMWorkingObjectChanger(cFMWorkingObjectChanger<T>*e_pFMWorkingObjectChanger):cNamedTypedObjectVector<T>(e_pFMWorkingObjectChanger){ m_pCurrentWorkingObject = 0; }

	inline	int		GetCurrentWorkingObjectIndex(){ if( !m_pCurrentWorkingObject )return -1;return this->GetObjectIndexByPointer(m_pCurrentWorkingObject); }
    inline  T*      GetCurrentWorkingObject(){return m_pCurrentWorkingObject; }
    inline  virtual	bool      SetCurrentWorkingObject(int e_iIndex,bool e_bRestart = true)
	{
		m_pCurrentWorkingObject = this->GetObject(e_iIndex);
		if( m_pCurrentWorkingObject )
		{
			if( e_bRestart ) 
			{
				m_pCurrentWorkingObject->Init();
				m_pCurrentWorkingObject->Update(0.f);
			}
			return true;
		}
		return false;
	}
    inline  bool      SetCurrentWorkingObject(const wchar_t*e_strName,bool e_bRestart = true)
	{
		return SetCurrentWorkingObject(this->GetObjectIndexByName(e_strName),e_bRestart);
	}
    inline  void    InitAllObject()
    {
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            this->m_ObjectList[i]->Init();
        }
    }
	virtual	void	Init(){ if( m_pCurrentWorkingObject )m_pCurrentWorkingObject->Init(); }
	//virtual	void	Destroy(){ if( m_pCurrentWorkingObject )m_pCurrentWorkingObject->Destroy(); }
	virtual	void	Update(float e_fElpaseTime){ if( m_pCurrentWorkingObject )m_pCurrentWorkingObject->Update(e_fElpaseTime); }
	virtual	void	Render(){ if( m_pCurrentWorkingObject )m_pCurrentWorkingObject->Render(); }
};


//sample
// 
//	class MyListManager:public cNamedTypedObjectVector<cNamedTypedObjectVector<NamedTypedObject>,NamedTypedObject*>
//	{
//		this one just like tree nodes.
//		a list contain a list and the list which is use to be contained,may contain a list!
//	};
//
//  LIST must inherit cNamedTypedObjectVector or cNamedTypedObjectVector
//
//LIST as parent
//CHILDREN_DATA_TYPE as its child
template < class LIST,class CHILDREN_DATA_TYPE > class cObjectListWithItsChildren:public cNamedTypedObjectVector< LIST >
{
protected:
	LIST*	m_pCurrentList;
public:
	cObjectListWithItsChildren()
	{
		m_pCurrentList = 0;
	}	
	cObjectListWithItsChildren(cObjectListWithItsChildren<LIST,CHILDREN_DATA_TYPE>*e_pObjectListTree):cNamedTypedObjectVector<LIST>(e_pObjectListTree){ m_pCurrentList = 0; }
	virtual ~cObjectListWithItsChildren(){}
	virtual	void	Destroy()
	{
		cNamedTypedObjectVector< LIST >::Destroy();
		m_pCurrentList = 0;
	}
	bool	AddChildObject(wchar_t*e_strListName,CHILDREN_DATA_TYPE e_pObject)
	{
		LIST*l_pTargetList = this->GetObject(e_strListName);
		if( l_pTargetList )
			return l_pTargetList->GetObject(e_strListName)->AddObject(e_pObject);
		return false;
	}

	inline CHILDREN_DATA_TYPE*	GetChildObject(wchar_t*e_strListName,wchar_t*ChildName)
	{
		LIST*l_pTargetList = this->GetObject(e_strListName);
		if( l_pTargetList )
			return l_pTargetList->GetObject(ChildName); 
		return nullptr;
	}
	inline CHILDREN_DATA_TYPE*	GetChildObject(int e_iListIndex,int e_iChildIndex)
	{
		LIST*l_pTargetList = this->GetObject(e_iListIndex);
		if( l_pTargetList )
			return l_pTargetList->GetObject(e_iChildIndex); 
		return nullptr;
	}
	//from current list
	inline CHILDREN_DATA_TYPE*	GetChildObject(wchar_t*e_strChildName){ if(m_pCurrentList)return m_pCurrentList->GetObject(e_strChildName); return nullptr; }
	inline CHILDREN_DATA_TYPE*	GetChildObject(int e_iChildIndex){ if(m_pCurrentList)return m_pCurrentList->GetObject(e_iChildIndex); return nullptr; }

	inline void	SetCurrentList(wchar_t*e_strName){m_pCurrentList = this->GetObject(e_strName);}
	inline void	SetCurrentList(LIST* e_pList){ m_pCurrentList = e_pList; }
	inline void	SetCurrentList(int e_iIndex){ m_pCurrentList = this->m_ObjectList[e_iIndex]; }

	inline LIST*	GetCurrentList(){ return m_pCurrentList; }
	//this one is a sample and it sux....it should considate recursive sitiation(list of list of list...listtree type)
	inline CHILDREN_DATA_TYPE*	GetListChildByUniqueID(uint64 e_uiID)
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			LIST*	l_pList = this->m_ObjectList[i];
			int	l_iCount2 = l_pList->Count();
			for( int j=0;j<l_iCount2;++j )
			{
				if( (*l_pList)[j]->GetUniqueID() == e_uiID )
				{
					return (*l_pList)[j];
				}
			}
		}
		return 0;
	}
};

//=================
//the object T must has update and render
//=================
template <class T> class    cBehaviorObjectList:public cNamedTypedObjectVector<T>
{
public:
	cBehaviorObjectList(){}
	cBehaviorObjectList(cBehaviorObjectList*e_pBehaviorObjectList):cNamedTypedObjectVector<T>(e_pBehaviorObjectList){}
	virtual ~cBehaviorObjectList(){}
    virtual void    Init()
    {
        int l_iCount = this->Count();
        for(int i=0;i<l_iCount;++i)
        {
            this->m_ObjectList[i]->Init();
        }
    }
    virtual void    Update(float e_fElpaseTime)
    {
        int l_iCount = this->Count();
        for(int i=0;i<l_iCount;++i)
        {
            this->m_ObjectList[i]->Update(e_fElpaseTime);
        }
    }
    inline	virtual void    Render()
    {
        int l_iCount = this->Count();
        for(int i=0;i<l_iCount;++i)
        {
            this->m_ObjectList[i]->Render();
        }    
    }
    //inline	virtual void    Start(float e_fElpaseTime = EPSIONAL)
    //{
    //    int l_iCount = this->Count();
    //    for(int i=0;i<l_iCount;++i)
    //    {
    //        this->m_ObjectList[i]->Start(e_fElpaseTime);
    //    }    
    //}
    inline	virtual void    Destroy()
    {
        int l_iCount = this->Count();
        for(int i=0;i<l_iCount;++i)
        {
            this->m_ObjectList[i]->Destroy();
        }
    }
};
//for level editor cMapLayer
template <class T> class cNamedTypedObjectVectorWithData:public cNamedTypedObjectVector<T>
{
	GET_SET_DEC(char*,m_pData,GetData,SetData);
public:
	cNamedTypedObjectVectorWithData():cNamedTypedObjectVector<T>()
	{
		m_pData = 0;
	}
	virtual ~cNamedTypedObjectVectorWithData()
	{
		SAFE_DELETE(m_pData);
	}

	cNamedTypedObjectVectorWithData(cNamedTypedObjectVectorWithData*e_pObjectListByNameWithData):cNamedTypedObjectVector<T>(e_pObjectListByNameWithData)
	{
		m_pData = 0;
	}

	virtual	NamedTypedObject*	Clone(){ return new cNamedTypedObjectVectorWithData<T>(this); }
};

//T must inhirent NamedTypedObject
template<class T>class	cObjectAndName:public NamedTypedObject
{
	T	*m_pObject;
public:
	//must set up object
	cObjectAndName(const wchar_t*e_strName){SetName(e_strName);}
	cObjectAndName(const wchar_t*e_strName,T*e_pObject)
	{
		SetName(e_strName);
		m_pObject = dynamic_cast<T*>(e_pObject->Clone());
	}
	cObjectAndName(cObjectAndName*e_pObjectAndName)
	{
		SetName(e_pObjectAndName->GetName());
		m_pObject = dynamic_cast<T*>(e_pObjectAndName->m_pObject->Clone());
	}
	CLONE_MYSELF(cObjectAndName);
	virtual ~cObjectAndName()
	{
		SAFE_DELETE(m_pObject);
	}
	//virtual	void	Start(float e_fElpaseTime = 0.f){ m_pObject->Start(e_fElpaseTime); }
	virtual	void	Init(){ m_pObject->Init(); }
	virtual	void	Destroy(){ m_pObject->Destroy(); }
	virtual	void	Update(float e_fElpaseTime){ m_pObject->Update(e_fElpaseTime); }
	virtual	void	Render(){ m_pObject->Render(); }
	T*				GetObject(){ return m_pObject; }
	//ensure memory control
	void			SetObject(T*e_pData){ m_pObject = e_pData; }
};
//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/1008289#1008289
#define SINGLETON_BASIC_FUNCTION(TYPE)	TYPE(TYPE const&) = delete;										\
									TYPE(TYPE*) = delete;											\
									static void DestroyInstance(){	SAFE_DELETE(m_spInstance);	}	\
									static TYPE* GetInstance() { if (!m_spInstance) { m_spInstance = new TYPE(); }return m_spInstance; }
template< class TYPE >
class cSingltonTemplate
{
protected:
	static TYPE*	m_spInstance;
public:
	static bool		m_sbEnableSinglton;
	cSingltonTemplate()
	{
		assert(m_spInstance == nullptr && "singlton only can have one instance");
		FMLog::LogWithFlag(ComposeMsgByFormat(L"cSingltonTemplate constructor called:%ls", TYPE::TypeID), CORE_LOG_FLAG);
	}
	virtual ~cSingltonTemplate()
	{
		FMLog::LogWithFlag(ComposeMsgByFormat(L"cSingltonTemplate destructor called:%ls", TYPE::TypeID), CORE_LOG_FLAG);
	}
};

template<class TYPE>TYPE*	cSingltonTemplate<TYPE>::m_spInstance = nullptr;
template<class TYPE>bool	cSingltonTemplate<TYPE>::m_sbEnableSinglton = true;


//do not support pointer data
template <class TYPE>inline	void	InvertVectorDateOrder(std::vector<TYPE>*e_pVectorData)
{
	int l_iLastIndex = (int)e_pVectorData->size()-1;
	//only 1 or 0 data is not necessery to invert
	if( l_iLastIndex <1 )
		return;
	size_t l_iSize = (size_t)e_pVectorData->size();
	std::vector<TYPE> l_OriginalPointList = *e_pVectorData;
	for(size_t i=0;i<l_iSize;++i )
	{
		l_OriginalPointList[l_iLastIndex-i] = (*e_pVectorData)[i];
	}
	*e_pVectorData = l_OriginalPointList;
}
//template <class Type>  inline	void	EraseVectorMemberByList(std::vector<Type>*e_pTypeVector,std::list<int> *e_pEraseIndexList)
//{
//	e_pEraseIndexList->sort();
//	int	l_iGoAhead = 0;
//	for( UINT i=0;i<e_pEraseIndexList->size();++i )
//	{
//		e_pTypeVector->erase(e_pTypeVector->begin()+e_pEraseIndexList[i]-l_iGoAhead);
//		l_iGoAhead++;
//	}
//};
//it dones't support pointer
template <class Type1,class Type2> map<Type1,Type2>	SwapMapData(Type1 e_iIndex1,Type1 e_iIndex2,map<Type1,Type2> e_pMap)
{
	Type2	Object1 = e_pMap[e_iIndex1];
	Type2	Object2 = e_pMap[e_iIndex2];
	e_pMap.erase(e_iIndex1);
	e_pMap.erase(e_iIndex2);
	e_pMap.insert(make_pair(e_iIndex1,Object2));
	e_pMap.insert(make_pair(e_iIndex2,Object1));
	return e_pMap;
}

//it dones't support pointer
template <class Type1,class Type2> map<Type1,Type2>	ReplaceMapData(Type1 e_iIndex,Type2 e_Data,map<Type1,Type2> e_pMap)
{
	e_pMap.erase(e_iIndex);
	e_pMap.insert(make_pair(e_iIndex,e_Data));
	return e_pMap;
}
template<class T>	inline	std::vector<int>	IndexListOfVector(T e_Object,std::vector<T>*e_pSrc)
{
	std::vector<int>l_List;
	size_t	l_uiSize = e_pSrc->size();
	for(size_t i=0;i<l_uiSize;++i  )
	{
		if((*e_pSrc)[i] == e_Object)
			l_List.push_back(i);
	}
	return l_List;
}
template<class T>	inline	int		IndexOfVector(T e_Object,std::vector<T>*e_pSrc)
{
	size_t	l_uiSize = e_pSrc->size();
	for(size_t i=0;i<l_uiSize;++i  )
	{
		if((*e_pSrc)[i] == e_Object)
			return (int)i;
	}
	return -1;
}
template<class T>	inline	bool	IsVectorContain(T e_Object,std::vector<T>*e_pSrc)
{
	size_t	l_uiSize = e_pSrc->size();
	for(size_t i=0;i<l_uiSize;++i  )
	{
		if((*e_pSrc)[i] == e_Object)
			return true;
	}
	return false;
}
template<class T>	inline	int	NumVectorContain(T e_Object,std::vector<T>*e_pSrc)
{
	size_t	l_uiSize = e_pSrc->size();
	int	l_iNum = 0;
	for(size_t i=0;i<l_uiSize;++i  )
	{
		if((*e_pSrc)[i] == e_Object)
			++l_iNum;
	}
	return l_iNum;
}


#define	cStatusObjectChanger(T)	cFMWorkingObjectChanger<cObjectAndName<T > >
//typedef cFMWorkingObjectChanger<cObjectAndName<T>>	cStatusObjectChanger;

//https://thispointer.com/how-to-search-by-value-in-a-map-c/
//std::map<std::string, int> wordMap = {{ "is", 6 },{ "the", 5 },{ "hat", 9 },{ "at", 6 }};
//// Create a vector of string
//std::vector<std::string> vec;
//int value = 6;
//// Find all the keys with value 6
//bool result = findByValue(vec, wordMap, value);
//
//
//std::map<int,int>					m_UserIDAndDeliverNodeIDMap = {{1,1,},{2,2}};
//int l_iUserID = -1;
//FindKeyByValueInMap<int, int>(m_UserIDAndDeliverNodeIDMap, 1, l_iUserID);
//
template<typename K, typename V>
bool FindKeyByValueInMap(std::vector<K>& vec, std::map<K, V> mapOfElemen, V value)
{
	bool bResult = false;
	auto it = mapOfElemen.begin();
	// Iterate through the map
	while (it != mapOfElemen.end())
	{
		// Check if value of this entry matches with given value
		if (it->second == value)
		{
			// Yes found
			bResult = true;
			// Push the key in given map
			vec.push_back(it->first);
		}
		// Go to next entry in map
		it++;
	}
	return bResult;
}

template<typename K, typename V>
bool FindKeyByValueInMap(std::map<K, V>& e_MapOfElemen, V value, K& e_Key)
{
	for (auto l_Iterator = e_MapOfElemen.begin(); l_Iterator != e_MapOfElemen.end(); l_Iterator++)
	{
		if (l_Iterator->second == value)
		{
			e_Key = l_Iterator->first;
			return true;
		}
	}
	return false;
}

#endif
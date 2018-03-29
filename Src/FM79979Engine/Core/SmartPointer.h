/*! 
@file cSmartObject.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		original implementation is from <http://github.com/juhgiyo/eplibrary>
@date April 17, 2009
@brief Smart Object Interface
@version 2.0

An Interface for the Smart Object.

*/
#ifndef __SMART_OBJECT_H__
#define __SMART_OBJECT_H__

#include "Synchronization/AllFUThreadInclude.h"
#include "NamedTypedObject.h"
#include "TemplateClass.h"
/*! 
@class cSmartObject cSmartObject.h
@brief This is a base class for Smart Object Classes  

Implements the System Functions.
*/
//====================
//smart pointer regesitor machine?like DirectDevice?
//any object inhiretance cSmartObject,make sure u are not calling delete but release
//or u have to know u have control resource very well
//====================
class cSmartObject
{
private:

	/// Reference Counter
	int m_refCount;
	NamedTypedObject*m_pRecource;
#ifdef DEBUG
	cNamedTypedObjectVector<NamedTypedObject>	m_ReferenceList;
#endif
protected:
	cSmartObject(NamedTypedObject*e_pResource);

	virtual ~cSmartObject();
public:
	/*!
	Returns the current reference count.
	@return the current reference count.
	*/
	int GetReferenceCount();

	/*!
	Increment this object's reference count
	*/
	int AddRef(NamedTypedObject*e_pNamedTypedObject);

	/*!
	Decrement this object's reference count
	if the reference count is 0 then delete this object.
	*/
	int Release(NamedTypedObject*e_pNamedTypedObject);

	const wchar_t*GetSmartObjectName();
};
//sample
//class	aa:public cSmartObject,public NamedTypedObject
//{
//	int*a;
//public:
//	aa():cSmartObject(this){ a = new int[50]; }
//	~aa(){ delete a; }
//};

#endif
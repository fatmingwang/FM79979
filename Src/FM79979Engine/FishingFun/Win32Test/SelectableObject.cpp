#include "stdafx.h"
#include "SelectableObject.h"

cSelectableObject::cSelectableObject()
{

}

cSelectableObject::~cSelectableObject()
{

}

NamedTypedObject*	cSelectableObject::Clone()
{
	return NamedTypedObject::Clone();
}

void	cSelectableObject::InternalMouseMove(int e_iPosX,int e_iPosY)
{

}

void	cSelectableObject::InternalMouseDown(int e_iPosX,int e_iPosY)
{

}

void    cSelectableObject::InternalMouseUp(int e_iPosX,int e_iPosY)
{

}

void    cSelectableObject::Update(float e_fElpaseTime)
{
	cClickMouseBehavior::Update(e_fElpaseTime);
}

void    cSelectableObject::Render()
{

}
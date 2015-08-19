#ifndef	_SELECTABLE_OBJECT_H_
#define	_SELECTABLE_OBJECT_H_


class	cSelectableObject:public cTabPage
{
    virtual void				InternalMouseMove(int e_iPosX,int e_iPosY);
    virtual void				InternalMouseDown(int e_iPosX,int e_iPosY);
    virtual void				InternalMouseUp(int e_iPosX,int e_iPosY);
public:
	cSelectableObject();
	~cSelectableObject();
	virtual	NamedTypedObject*	Clone();
    virtual void    			Update(float e_fElpaseTime);
    virtual void    			Render();
};

#endif
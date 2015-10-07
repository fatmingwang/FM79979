#ifndef	_MAGIC_TOWER_CHARACTER_BASE_H
#define	_MAGIC_TOWER_CHARACTER_BASE_H

class	cCharacterBase:public cFAIAnimationCharacter
{
public:
	cCharacterBase(TiXmlElement*e_pTiXmlElement);
	~cCharacterBase();
	virtual	void	RenderStatusInfo();
};

#endif
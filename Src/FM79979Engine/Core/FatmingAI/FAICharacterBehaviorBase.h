#ifndef	_FAI_CHARACTER_BEHAVIOR_BASE_H_
#define	_FAI_CHARACTER_BEHAVIOR_BASE_H_

#include "FAIBaseBehavior.h"
#include "FAIMachine.h"

namespace FATMING_AI
{
	//<Root>
	//	<cFAICharacterStatus Name="HammerheadFish" HP="100" Strength="1" Defence="100" Agile="100" Dexterous="100" Lucky="100" />
	//	<cFAICharacterInterface Name="HammerheadFish" Type="cFAIAnimationCharacter" cFAICharacterStatus="HammerheadFish" Speed="22" RunAwaySpeed="51" >
	//		<cFAIAnimationCharacter >
	//			<Status Name="" MPDIList="" MPDI="" Loop="" />
	//			<Status Name="" MPDIList="" MPDI="" Loop="" />
	//			<Status Name="" MPDIList="" MPDI="" Loop="" />
	//			<Status Name="" MPDIList="" MPDI="" Loop="" />
	//		</cFAIAnimationCharacter>
	//	</cFAICharacterInterface>
	//	<cFAIMachine Name="HammerheadFish" StartBehaviorName="Path" cFAICharacterInterface="HammerheadFish" StopWhileNoAI="1" >
	//		<cFAIFiledPathBehavior Name="Path"  Probability="100" Wall="0,0,1280,720" XDistanceRestrict="2,5" YDistanceRestrict="2,5" TimeRestrict="35,40" PointRestrict="35,40" DistanceChangePoint="3,7" DirectionOffsetX="-0.2,0.2" DirectionOffsetY="-0.2,0.2" LOD="5" />
	//		<cFAILeaveFiledBehavior Name="Leave" Probability="0" Wall="0,0,1280,720" Speed="250" Radius="650" />
	//		<cFAIPathBehavior Name="FixedPath" Probability="0" PathFile="Fish/MonsterData/Path/HammerheadFish.path" />
	//	</cFAIMachine>
	//</Root>

	//position and collide update,
	//class	cFAICharacterBehaviorBase:public cAIMachine
	//{
	//public:
	//	cFAICharacterBehaviorBase(TiXmlElement*e_pTiXmlElement);
	//	cFAICharacterBehaviorBase(cCharacterBase*e_pCharacterBase);
	//	CLONE_MYSELF(cFAICharacterBehaviorBase);
	//	virtual ~cFAICharacterBehaviorBase();
	//	virtual	void	Render();
	//	virtual	void	Update(float e_fElpaseTime);
	//};

}

#endif
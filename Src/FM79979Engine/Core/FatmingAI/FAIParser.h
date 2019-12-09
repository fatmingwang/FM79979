#ifndef _FAI_PARSER_H_
#define _FAI_PARSER_H_

#include "FAICharacterInterface.h"
#include "FAIMachine.h"
namespace FATMING_AI
{
    //===============
    //
    //<!-- ensure one AI file only contain a AI,because AIBehavior might need target character's data,if it has over more than one...the data might be incorrect! -->
    //<AI Name="Test" StartIndex="0" >
    //    <cFAIWalkToDestinationAIBehavior Name="0" Probability="10" KillAfterDone="false" />
    //    <!-- DestPos coudl set at the game -->
    //    <cFAIWalkToDestinationAIBehavior Name="1" DestPos="0,0,0" NextAIName="0" Speed="100" />
    //    <!-- if TargetAIBehavior is done MutiAIBehaivor condition is fileed! -->
    //    <cFAIMultiAITaskBehavior Name="2" Probability="20" TargetAIBehavior="x1" >
    //        <x1 />
    //        <x2 />
    //    </cFAIMultiAITaskBehavior>
    //    <cFAIMPDIAnimationChangeAIBehavior AnimationName="Walking" />
    //    <cFAIMPDIAnimationChangeAIBehavior />
    //    <cFAIIdleAIBehavior Name="3" Time="2" Probability="10"/>
    //</AI>
    //
    //class   cFAIMaintainMachine:public cNamedTypedObjectVector<cFAIBaseBehave>

    //call ParseWithMyParse(char*e_strFileName);
    //exten sion name is fat(Fatming AI)
    //ensure one AI file only contain a AI,because AIBehavior might need target character's data
    //if it has over more than one...the data might be incorrect!
    
    
	//for each project might has different AI,so implement this one u could get extract AIBehavior
	//typedef cFAIBaseBehave* (*ExtractAIBehavior)(TiXmlElement*e_pRoot);
	//class	cFAIMPDIAnimationMovingAIBehavior;
	//class	cFAIWalkToDestinationAIBehavior;
	//class	cFAIIdleAIBehavior;
	//class	cFAIMultiAITaskBehavior;
	//class	cFAIMPDIAnimationChangeAIBehavior;
	//<Root>
	//	<cFAICharacterStatus Name="TestFishStatus" HP="100" Strength="1" Defence="100" Agile="100" Dexterous="100" Lucky="100" />
	//	<cFAICharacterInterface cFAICharacterStatus="TestFishStatus" >
	//		<cFAIAnimationCharacter Name="">
	//		</cFAIAnimationCharacter>
	//	</cFAICharacterInterface>
	//	<!-- ensure one AI file only contain a AI,because AIBehavior might need target character's data,if it has over more than one...the data might be incorrect! -->
	//	<cFAIMachine Name="NormalZombie" StartBehaviorName="Walk" cFAICharacterInterface="" >
	//	    <!-- DestPos coudl set at the game -->
	//	    <cFAIMultiAITaskBehavior Name="Walk" Probability="10" TargetAIBehavior="1" NextAIName="Idle">
	//	    	<cFAIWalkToDestinationAIBehavior Name="1" DestPos="0,200,0" Speed="100" />
	//	    	<cFAIMPDIAnimationChangeAIBehavior AnimationName="Walking" />
	//	    </cFAIMultiAITaskBehavior>
	//	    <cFAIMultiAITaskBehavior Name="Idle" Probability="10" TargetAIBehavior="Idle" >
	//		<cFAIIdleAIBehavior Name="Idle" Time="2" />
	//	    	<cFAIMPDIAnimationChangeAIBehavior Name="AnimationIdle" AnimationName="Idle" />
	//	    </cFAIMultiAITaskBehavior>
	//	    <cFAIMultiAITaskBehavior Name="Waddle" Probability="50" TargetAIBehavior="Waddle" NextAIName="Idle" >
	//	    	<cFAIWalkToDestinationAIBehavior Name="Waddle" DestPos="0,200,0" Speed="100" />
	//	    	<cFAIMPDIAnimationChangeAIBehavior Name="AnimationIdle" AnimationName="Idle" />
	//	    </cFAIMultiAITaskBehavior>
	//	</cFAIMachine>
	//</Root>
    class   cFAIBehaviorParser:public cNodeISAX
    {
		//<AIMachine>
		void	ProcessFAIMachine(TiXmlElement*e_pTiXmlElement);
		//<AICharacterInterface>
		void	ProcessFAICharacterInterface(TiXmlElement*e_pTiXmlElement);
		//
		void	ProcessFAICharacterStatus(TiXmlElement*e_pTiXmlElement);
        //
        virtual	bool	MyParse(TiXmlElement*e_pRoot);
    public:
		cFAIBehaviorParser();
		virtual ~cFAIBehaviorParser();
		cNamedTypedObjectVector<cFAICharacterStatus>		m_FAICharacterStatusList;
		cNamedTypedObjectVector<cFAICharacterInterface>	m_FAICharacterInterfaceList;
		cNamedTypedObjectVector<cFAIMachine>				m_FAIMachineList;
		cFAIMachine*								GetFAIMachineByFileName(const WCHAR*e_strFileName);
		static cFAIBaseBehave*						ProcessAIBehavior(TiXmlElement*e_pTiXmlElement);
    };
	//while MyParse is called assign this to it to provide AI data
	extern cFAIBehaviorParser*g_pFAIBehaviorParser;
}

#endif
#include "stdafx.h"
#include "DiceTable.h"
#include "BluffingGameProbability.h"
//<DiceRule Min="1" Max="7" PercentForSpecialNumeral1="0.1" NumDice="5">
sDiceRule::sDiceRule(TiXmlElement*e_pelement)
{
	iNumDice = 5;
	iMin = 1;
	iMax = 6;
	fPercentForSpecialNumeral1 = 0.1f;
	iAllDiceCount = 10;
	PARSE_ELEMENT_START(e_pelement)
		COMPARE_NAME("Min")
		{
			iMin = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("Max")
		{
			iMax = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("PercentForSpecialNumeral1")
		{
			fPercentForSpecialNumeral1 = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("NumDice")
		{
			iNumDice = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("AllDiceCount")
		{
			iAllDiceCount = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
}

//<DiceTable PI="Image/DiceImage.pi" Dice1="" Dice2="" Dice3="" Dice4="" Dice5="" Dice6="" BlockDice=""/>

cDiceTable::cDiceTable(TiXmlElement*e_pElement,sDiceRule*e_pDiceRule,eBehaviorType e_eBehaviorType)
{
	m_eBehaviorType = e_eBehaviorType;
	m_pShowMPDI = 0;
	m_vMyDiceImagePos = Vector3::Zero;
	m_fMyDiceImageGap = 50.f;
	m_iCurrentSelectDiceCount = -1;;
	m_iCurrentSelectDicePoint = -1;
	m_pDiceRule = e_pDiceRule;
	m_pBlockImage = 0;
	m_pPI = 0;
	m_pSelectedNumeralImage = 0;
	m_pSelectedDiceImage = 0;
	m_bReveal = false;
	ProcessImageData(e_pElement);
	m_NumeralImageButton.SetReclickable(true);
	m_DiceImageButtonContainer.SetReclickable(true);
}

cDiceTable::~cDiceTable()
{
	SAFE_DELETE(m_pShowMPDI);
	m_NumeralImageButton.Destroy();
	m_DiceImageButtonContainer.Destroy();
}

cDiceTable::sCallBehavior::sCallBehavior()
{
	pBGSubMPDI = 0;
	pNumberSubMPDI = 0;
	pDiceSubMPDI = 0;
}
cDiceTable::sCallBehavior::~sCallBehavior()
{
	SAFE_DELETE(pBGSubMPDI);
	SAFE_DELETE(pNumberSubMPDI);
	SAFE_DELETE(pDiceSubMPDI);
}
void	cDiceTable::sCallBehavior::Init()
{
	if( pBGSubMPDI )
	{
		pBGSubMPDI->Init();
		pBGSubMPDI->Update(0.016f);
	}
	if( pNumberSubMPDI )
	{
		pNumberSubMPDI->Init();
		pNumberSubMPDI->Update(0.016f);
	}
	if( pDiceSubMPDI )
	{
		pDiceSubMPDI->Init();
		pDiceSubMPDI->Update(0.016f);
	}
}

void	cDiceTable::sCallBehavior::ChangeData(int e_iDiceCount,int e_iDicePoint)
{
	if( pNumberSubMPDI )
	{
		char	l_strPrefixTemp[MAX_PATH];
		sprintf(l_strPrefixTemp,"%s%.2d",UT::WcharToChar(strNumberPrefix).c_str(),e_iDiceCount+1);
		std::wstring	l_str = UT::CharToWchar(l_strPrefixTemp);
		int	l_iIndex = pNumberSubMPDI->GetPointData(0)->pPI->GetObjectIndexByName(l_str.c_str());
		if( l_iIndex == -1 )
		{
			int a=0;
		}
		assert(l_iIndex != -1 );
		pNumberSubMPDI->ChangeAllImageByImageIndex(l_iIndex);
		pNumberSubMPDI->GetPointData(0)->vColor.a = 1.f;
	}
	if( pDiceSubMPDI )
	{
		std::wstring	l_str = this->strDicePrefix.c_str();
		l_str += ValueToStringW(e_iDicePoint+1);
		int	l_iIndex = pDiceSubMPDI->GetPointData(0)->pPI->GetObjectIndexByName(l_str.c_str());
		if( l_iIndex == -1 )
		{
			int a=0;
		}
		assert(l_iIndex != -1 );
		pDiceSubMPDI->ChangeAllImageByImageIndex(l_iIndex);
		pDiceSubMPDI->GetPointData(0)->vColor.a = 1.f;
	}
	this->Init();
}

void	cDiceTable::sCallBehavior::Disable()
{
	if( pBGSubMPDI )
	{
		pBGSubMPDI->SetAnimationDone(true);
	}
	if( pNumberSubMPDI )
	{
		pNumberSubMPDI->SetAnimationDone(true);
	}
	if( pDiceSubMPDI )
	{
		pDiceSubMPDI->SetAnimationDone(true);
	}
}

void	cDiceTable::sCallBehavior::Render()
{
	if( pBGSubMPDI )
	{
		pBGSubMPDI->Render();
	}
	if( pNumberSubMPDI )
	{
		pNumberSubMPDI->Render();
	}
	if( pDiceSubMPDI )
	{
		pDiceSubMPDI->Render();
	}
}

//<DiceTable PI="Image/DiceImage.pi" DiceCount="6" Dice1="" Dice2="" Dice3="" Dice4="" Dice5="" Dice6="" BlockDice="" NumeralImage="Num0,Num9"/>
//	<Data Name="AI" 	DicePos="900,100" DiceImageGap="30" NumeralPos="" NumeralImageGap="" />
//</DiceTable>
void		cDiceTable::ProcessImageData(TiXmlElement*e_pElement)
{
	Vector4	l_vPressedColor(1,0,0,1);
	cPuzzleImage*l_pNumberImagePI = 0;
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("NumberImagePI")
		{
			l_pNumberImagePI = cGameApp::GetPuzzleImageByFileName(l_strValue);
			if( !l_pNumberImagePI )
			{
				UT::ErrorMsg(l_strValue,L"not exists!");
			}
		}
		else
		COMPARE_NAME("PI")
		{
			assert(m_pPI == 0);
			this->m_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
			if( !m_pPI )
			{
				UT::ErrorMsg(l_strValue,L"not exists!");
			}
		}
		else
		COMPARE_NAME("DicePressedColor")
		{
			l_vPressedColor = VALUE_TO_VECTOR4;
		}
		else
		COMPARE_NAME("BlockDice")
		{
			if( m_pPI )
				this->m_pBlockImage = m_pPI->GetObject(l_strValue);
		}
		else
		COMPARE_NAME("DiceCount")
		{
			int	l_iCount = VALUE_TO_INT;
			for( int i=0;i<l_iCount;++i )
				m_DiceImageButtonContainer.AddObjectNeglectExist(0);
		}
		else
		COMPARE_NAME("NumeralImage")
		{
			if(!l_pNumberImagePI)
				l_pNumberImagePI = m_pPI;
			for( int i=2;i<m_pDiceRule->iAllDiceCount+1;++i )
			{
				std::wstring	l_strNumerialImage = l_strValue;
				cAssembleImageButton*l_pAssembleImageButton = new cAssembleImageButton();
				cAssembleImageButton*l_pFinishButton = new cAssembleImageButton();
				char	l_strPrefixTemp[MAX_PATH];
				sprintf(l_strPrefixTemp,"%s%.3d",UT::WcharToChar(l_strValue).c_str(),i);
				l_strNumerialImage = ValueToStringW(l_strPrefixTemp);
				cPuzzleImageUnit*l_pPIUnit = l_pNumberImagePI->GetObject(l_strNumerialImage.c_str());
				cPuzzleImageUnit*l_pClone = dynamic_cast<cPuzzleImageUnit*>(l_pPIUnit->Clone());
				cPuzzleImageUnit*l_pClone2 = dynamic_cast<cPuzzleImageUnit*>(l_pPIUnit->Clone());
				l_pAssembleImageButton->AddObjectNeglectExist(l_pClone);
				l_pFinishButton->AddObjectNeglectExist(l_pClone2);
				l_pFinishButton->SetColor(l_vPressedColor);
				l_pFinishButton->SetName(l_strNumerialImage.c_str());
				l_pAssembleImageButton->SetFinishImage(l_pFinishButton,true);
				l_pAssembleImageButton->Init();
				SAFE_DELETE(l_pFinishButton);
				m_NumeralImageButton.AddObjectNeglectExist(l_pAssembleImageButton);
				//cImageButton*l_pImageButton = new cImageButton(l_pPIUnit,Vector3::Zero);
				//l_pPIUnit->SetColor(Vector4::HalfBright);
				//l_pImageButton->SetFinishImage(l_pPIUnit);
				//l_pPIUnit->SetColor(Vector4::One);
				//m_NumeralImageButton.AddObjectNeglectExist(l_pImageButton);
			}
		}
		else
		{
			if( l_strName[0] == 'D' &&
				l_strName[1] == 'i' &&
				l_strName[2] == 'c' &&
				l_strName[3] == 'e')
			{
				const WCHAR*l_strDiceValue = &l_strName[4];
				int	l_iDiceNumeral = GetInt(l_strDiceValue);
				if( m_pPI )
				{
					cPuzzleImageUnit*l_pDiceImage = m_pPI->GetObject(l_strValue);
					if( l_pDiceImage )
					{
						cImageButton*l_pImageButton = new cImageButton(l_pDiceImage,Vector3::Zero);
						m_DiceImageButtonContainer.ReplaceObject(l_iDiceNumeral-1,l_pImageButton);
						l_pDiceImage->SetColor(l_vPressedColor);
						l_pImageButton->SetFinishImage(l_pDiceImage,true);
						l_pDiceImage->SetColor(Vector4::One);
					}
				}
			}
		}
	TO_NEXT_VALUE
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE("Data")
		{
			ProcessPosData(e_pElement);
		}
		else
		COMPARE_VALUE("ShowMPDI")
		{
			if( e_pElement->FirstChildElement() )
			{
				m_pShowMPDI = cMPDI::GetMe(e_pElement->FirstChildElement());
				if( m_pShowMPDI )
				{
					m_pShowMPDI->Init();
					m_pShowMPDI->SetAnimationLoop(false);
				}
			}
		}
		else
		COMPARE_VALUE("CallHint")
		{
			ProcessCallHintData(e_pElement);
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}
//<Data DicePos="100,800" DiceImageGap="59" NumeralPos="100,840" NumeralImageGap="39" MyDiceImagePrefixName="Probability board_point" MyDiceImagePos="100,900,0" MyDiceImageGap="50" />
void		cDiceTable::ProcessPosData(TiXmlElement*e_pElement)
{
	if( !e_pElement )
		return;
	Vector3	l_vDicePos = Vector3::Zero;
	float	l_fDiceGap = 0.f;
	Vector3	l_vNumeralPos = Vector3::Zero;
	float	l_fNimeralGap = 0.f;
	PARSE_ELEMENT_START(e_pElement)
		//COMPARE_NAME("Name")
		//{
		//}
		//else
		COMPARE_NAME("DicePos")
		{
			l_vDicePos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("DiceImageGap")
		{
			l_fDiceGap = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("NumeralPos")
		{
			l_vNumeralPos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("NumeralImageGap")
		{
			l_fNimeralGap = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("MyDiceImagePrefixName")
		{
			m_strMyDiceImagePrefixName = l_strValue;
		}
		else
		COMPARE_NAME("DiceImageButtonPrefixName")
		{
			m_strDiceImageButtonPrefixName = l_strValue;
		}
		else
		COMPARE_NAME("MyDiceImagePos")
		{
			m_vMyDiceImagePos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("MyDiceImageGap")
		{
			m_fMyDiceImageGap = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
	if( m_DiceImageButtonContainer.GetObject(0) )
	{
		TiXmlElement*l_pElement = GetXmlElementByNameFromElement(L"DicePosSubMPDI",e_pElement);
		bool	l_bSetPos = true;
		if( l_pElement )
		{
			//<DicePosSubMPDI PrefixName="dice_off_00">
			//	<cMPDI cMPDIList="Image/gamplay_mpdi.mpdi" cMPDI="gameplay"/>
			//</DicePosSubMPDI>
			l_bSetPos = false;
			std::wstring	l_strPrefixName = l_pElement->Attribute(L"PrefixName");
			cMPDI*l_pMPDI = cMPDI::GetMe(l_pElement->FirstChildElement(),false);
			Vector4	l_vNumeralCollideRange;
			for( int i=0;i<m_DiceImageButtonContainer.Count();++i )
			{
				std::wstring	l_strNumberPosSubMPDI = l_strPrefixName;
				l_strNumberPosSubMPDI += ValueToStringW(i+1);
				cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(l_strNumberPosSubMPDI.c_str());
				if( l_pSubMPDI )
				{
					Vector3	l_vNumerialPos = l_pSubMPDI->GetPointList()[0];
					m_DiceImageButtonContainer[i]->SetPos(l_vNumerialPos);
					if( i == 0 )
					{
						l_vNumeralCollideRange.x = l_vNumerialPos.x;
						l_vNumeralCollideRange.y = l_vNumerialPos.y;
					}
					if( i == m_DiceImageButtonContainer.Count()-1 )
					{
						l_vNumeralCollideRange.z = l_vNumerialPos.x+l_pSubMPDI->GetPointData(0)->Size.x;
						l_vNumeralCollideRange.w = l_vNumerialPos.y+l_pSubMPDI->GetPointData(0)->Size.y;						
					}
				}
			}
			m_DiceImageButtonContainer.SetCollisionRange(l_vNumeralCollideRange);	
		}
		if( l_bSetPos  )
		{
			int	l_iDiceImageWidth = m_DiceImageButtonContainer.GetObject(0)->GetWidth();
			Vector4	l_vDiceCollideRange(l_vDicePos.x,l_vDicePos.y,l_vDicePos.x,l_vDicePos.y+m_DiceImageButtonContainer.GetObject(0)->GetHeight());
			for( int i=0;i<m_DiceImageButtonContainer.Count();++i )
			{
				m_DiceImageButtonContainer.GetObject(i)->SetPos(l_vDicePos);
				l_vDicePos.x += l_fDiceGap;
			}
			l_vDiceCollideRange.z = l_vDicePos.x+l_iDiceImageWidth;
			m_DiceImageButtonContainer.SetCollisionRange(l_vDiceCollideRange);
		}
	}
	if( m_NumeralImageButton.Count() )
	{
		bool	l_bSetPos  =true;
		TiXmlElement*l_pElement = GetXmlElementByNameFromElement(L"NumerialPosSubMPDI",e_pElement);
		if( l_pElement )
		{
			//<NumerialPosSubMPDI PrefixName="dice_iff_00">
			//	<cMPDI cMPDIList="Image/gamplay_mpdi.mpdi" cMPDI="gameplay"/>
			//</NumerialPosSubMPDI>
			l_bSetPos = false;
			std::wstring	l_strPrefixName = l_pElement->Attribute(L"PrefixName");
			cMPDI*l_pMPDI = cMPDI::GetMe(l_pElement->FirstChildElement(),false);
			Vector4	l_vNumeralCollideRange;
			for( int i=0;i<m_NumeralImageButton.Count();++i )
			{
				char	l_strPrefixTemp[MAX_PATH];
				sprintf(l_strPrefixTemp,"%s%.3d",UT::WcharToChar(l_strPrefixName).c_str(),i+2);
				std::wstring	l_strNumberPosSubMPDI = UT::CharToWchar(l_strPrefixTemp);
				//l_strNumberPosSubMPDI += ValueToStringW(i+1);
				cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(l_strNumberPosSubMPDI.c_str());
				if( l_pSubMPDI )
				{
					Vector3	l_vNumerialPos = l_pSubMPDI->GetPointList()[0];
					m_NumeralImageButton[i]->SetPos(l_vNumerialPos);
					if( i == 0 )
					{
						l_vNumeralCollideRange.x = l_vNumerialPos.x;
						l_vNumeralCollideRange.y = l_vNumerialPos.y;
					}
					if( i == m_NumeralImageButton.Count()-1 )
					{
						l_vNumeralCollideRange.z = l_vNumerialPos.x+l_pSubMPDI->GetPointData(0)->Size.x;
						l_vNumeralCollideRange.w = l_vNumerialPos.y+l_pSubMPDI->GetPointData(0)->Size.y;						
					}
				}
			}
			m_NumeralImageButton.SetCollisionRange(l_vNumeralCollideRange);
		}
		if( l_bSetPos  )
		{
			int	l_iNumeralImageWidth = m_NumeralImageButton.GetObject(0)->GetWidth();
			Vector4	l_vNumeralCollideRange(l_vNumeralPos.x,l_vNumeralPos.y,l_vNumeralPos.x,l_vNumeralPos.y+m_NumeralImageButton.GetObject(0)->GetHeight());
			for( int i=0;i<m_NumeralImageButton.Count();++i )
			{
				m_NumeralImageButton.GetObject(i)->SetPos(l_vNumeralPos);
				l_vNumeralPos.x += l_fNimeralGap;
				if( i>=10 )
					l_vNumeralPos.x += l_fNimeralGap;
				if( i>=9 && (i+1)%10 == 0 )
				{
					l_vNumeralCollideRange.z = l_vNumeralPos.x;
					l_vNumeralPos.x -= 9*l_fNimeralGap;
					l_vNumeralPos.y += l_fNimeralGap;
					l_vNumeralCollideRange.w = l_vNumeralPos.y+l_fNimeralGap;
				}
			}
			if( m_NumeralImageButton.Count() <=10 )			
				l_vNumeralCollideRange.z = l_vNumeralPos.x+l_iNumeralImageWidth;
			m_NumeralImageButton.SetCollisionRange(l_vNumeralCollideRange);
		}
	}
}
//<CallHint>
//	<BG>
//		<cSubMPDI cMPDIList="Image/gamplay_mpdi.mpdi" cMPDI="PlayerCallHit" cSubMPDI="BG"/>
//	</BG>
//	<Number Prefix="color_0">
//		<cSubMPDI cMPDIList="Image/gamplay_mpdi.mpdi" cMPDI="PlayerCallHit" cSubMPDI="Number"/>
//	</Number>
//	<Dice Prefix="dice_color_00">
//		<cSubMPDI cMPDIList="Image/gamplay_mpdi.mpdi" cMPDI="PlayerCallHit" cSubMPDI="Dice"/>
//	</Dice>
//</CallHint>
void		cDiceTable::ProcessCallHintData(TiXmlElement*e_pElement)
{
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE("BG")
		{
			m_CallBehavior.pBGSubMPDI = cSubMPDI::GetMe(e_pElement->FirstChildElement());
			if( m_CallBehavior.pBGSubMPDI )
			{
				m_CallBehavior.pBGSubMPDI->GetPointData(0)->vColor.a = 1.f;
				m_CallBehavior.pBGSubMPDI->SetAnimationLoop(false);
			}
		}
		else
		COMPARE_VALUE("Number")
		{
			m_CallBehavior.pNumberSubMPDI = cSubMPDI::GetMe(e_pElement->FirstChildElement());
			m_CallBehavior.strNumberPrefix = e_pElement->Attribute(L"Prefix");
		}
		else
		COMPARE_VALUE("Dice")
		{
			m_CallBehavior.pDiceSubMPDI = cSubMPDI::GetMe(e_pElement->FirstChildElement());
			m_CallBehavior.strDicePrefix = e_pElement->Attribute(L"Prefix");
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}

void		cDiceTable::Init()
{
	m_iCurrentSelectDiceCount = -1;;
	m_iCurrentSelectDicePoint = -1;
	m_DiceImageButtonContainer.Init();
	m_NumeralImageButton.Init();
	m_MyDiceTable.clear();
	m_MyDiceTable = GenerateDiceTable(m_pDiceRule->iNumDice,m_pDiceRule->iMax,m_pDiceRule->fPercentForSpecialNumeral1);
	GenerateDiceTableImages();
	this->m_CallBehavior.Disable();
}

void		cDiceTable::Update(float e_fElpaseTime)
{
	if( m_pShowMPDI&&!m_pShowMPDI->IsAnimationDone() )
	{
		m_pShowMPDI->Update(e_fElpaseTime);
		return;
	}

	m_DiceImageButtonContainer.Update(e_fElpaseTime);
	m_NumeralImageButton.Update(e_fElpaseTime);
	if( m_DiceImageButtonContainer.IsSatisfiedCondition() )
	{
		m_iCurrentSelectDicePoint = m_DiceImageButtonContainer.GetCurrentWorkingObjectIndex();
	}
	if( m_NumeralImageButton.IsSatisfiedCondition() )
	{
		m_iCurrentSelectDiceCount = m_NumeralImageButton.GetCurrentWorkingObjectIndex()+1;
		//if( m_NumeralImageButton.GetCurrentWorkingObject() )
		//{
		//	cAssembleImageButton*l_pImageButton = dynamic_cast<cAssembleImageButton*>(m_NumeralImageButton.GetCurrentWorkingObject());
		//	cAssembleImageButton*l_pFinishButton = (cAssembleImageButton*)l_pImageButton->GetFinishImage();
		//	if(l_pFinishButton)
		//	{
		//		l_pFinishButton->SetColor(Vector4::HalfBright);
		//	}
		//}
	}
	//if( !IsCallConditionOk(e_iCurrentDicePoint,e_iCurrentDiceCount,m_iCurrentSelectDicePoint,m_iCurrentSelectDiceCount,e_iTotalPlayer) )
	{
	//	m_iCurrentSelectDicePoint = -1;
	//	m_iCurrentSelectDiceCount = -1;
	}
}

void		cDiceTable::Render()
{
	if( m_pShowMPDI&&!m_pShowMPDI->IsAnimationDone() )
	{
		m_pShowMPDI->Render();
		return;
	}
	if( m_bReveal )
	{
		if( m_pShowMPDI )
			m_pShowMPDI->RenderLastFrameImage();
		//m_MyDiceTableImage.Render();
		m_DiceImageButtonContainer.Render();
	}
	else
	if( m_pBlockImage )
	{
		int	l_iCount = m_MyDiceTableImage.Count();
		for(int i=0;i<l_iCount;++i)		
		{
			m_pBlockImage->SetPos(m_MyDiceTableImage[i]->GetPos());
			m_pBlockImage->Render();
		}
	}
	m_NumeralImageButton.Render();
	m_CallBehavior.Render();
}

void		cDiceTable::ClickButtonRender()
{
	m_DiceImageButtonContainer.Render();
	m_NumeralImageButton.Render();
}

void		cDiceTable::DebugRender()
{
	m_DiceImageButtonContainer.DebugRender();
	m_NumeralImageButton.DebugRender();
}

void		cDiceTable::MouseDown(int e_iX,int e_iY)
{
	if( m_pShowMPDI&&!m_pShowMPDI->IsAnimationDone() )
		return;
	if( !m_bReveal )
		return;
	if( m_NumeralImageButton.Collide(e_iX,e_iY) )
		m_NumeralImageButton.MouseDown(e_iX,e_iY);
	if( m_DiceImageButtonContainer.Collide(e_iX,e_iY) )
		m_DiceImageButtonContainer.MouseDown(e_iX,e_iY);
}

void		cDiceTable::MouseMove(int e_iX,int e_iY)
{
	if( m_pShowMPDI&&!m_pShowMPDI->IsAnimationDone() )
		return;
	if( !m_bReveal )
		return;
	if( m_NumeralImageButton.Collide(e_iX,e_iY) )
		m_NumeralImageButton.MouseMove(e_iX,e_iY);
	if( m_DiceImageButtonContainer.Collide(e_iX,e_iY) )
		m_DiceImageButtonContainer.MouseMove(e_iX,e_iY);
}

void		cDiceTable::MouseUp(int e_iX,int e_iY)
{
	if( m_pShowMPDI&&!m_pShowMPDI->IsAnimationDone() )
		return;
	if( !m_bReveal )
		return;
	if( m_NumeralImageButton.Collide(e_iX,e_iY) )
		m_NumeralImageButton.MouseUp(e_iX,e_iY);
	if( m_DiceImageButtonContainer.Collide(e_iX,e_iY) )
		m_DiceImageButtonContainer.MouseUp(e_iX,e_iY);
}

int			cDiceTable::GetCurrentSelectNumeral()
{
	return m_iCurrentSelectDiceCount;
}

int			cDiceTable::GetCurrentSelectDice()
{
	return m_iCurrentSelectDicePoint;
}

bool		cDiceTable::IsCallConditionOk(int e_iCurrentDicePoint,int e_iCurrentDiceCount,int e_iNewDicePoint,int e_iNewDiceCount,int e_iTotalPlayer)
{
	if( e_iNewDiceCount > e_iCurrentDiceCount && e_iNewDiceCount <= m_pDiceRule->iNumDice*e_iTotalPlayer )
	{
		return true;
	}
	else
	if( e_iNewDiceCount == e_iCurrentDiceCount )
	{
		if( e_iNewDicePoint > e_iCurrentDicePoint  )
			return true;
	}
	//count is samller is not allowable
	return false;

}

bool		cDiceTable::IsConditionOk(int e_iCurrentDicePoint,int e_iCurrentDiceCount,int e_iTotalPlayer)
{
	return IsCallConditionOk(e_iCurrentDicePoint,e_iCurrentDiceCount,m_iCurrentSelectDicePoint,m_iCurrentSelectDiceCount,e_iTotalPlayer);
}

void		cDiceTable::GenerateDiceTableImages()
{
	m_MyDiceTableImage.Destroy();
	while(m_MyDiceTableImage.Count())
		m_MyDiceTableImage.RemoveObject(0);
	Vector3	l_vStartPos = m_vMyDiceImagePos;
	for( size_t i=0;i<m_MyDiceTable.size();++i )
	{
		std::wstring	l_strImageName = m_strMyDiceImagePrefixName;
		l_strImageName += ValueToStringW(m_MyDiceTable[i]+1);
		cPuzzleImageUnit*l_pPIUnit = this->m_pPI->GetObject(l_strImageName.c_str());
		if( l_pPIUnit )
		{
			cBaseImage*l_pImage = new cBaseImage(l_pPIUnit);
			l_pImage->SetPos(l_vStartPos);
			m_MyDiceTableImage.AddObjectNeglectExist(l_pImage);
		}
		l_vStartPos.x += m_fMyDiceImageGap;
	}
	if( m_pShowMPDI )
	{
		const WCHAR*l_strDicePrefix1 = L"dice_item_00";
		int	l_iCount = m_MyDiceTableImage.Count();
		//const WCHAR*l_strDicePrefix2 = L"CenterDice";
		for( int i=0;i<l_iCount;++i )
		{
			std::wstring	l_str = l_strDicePrefix1;
			l_str += ValueToStringW(i+1);
			cSubMPDI*l_pSubMPDI = m_pShowMPDI->GetObject(l_str.c_str());
			std::wstring	l_strDceImageName = m_strMyDiceImagePrefixName;
			l_strDceImageName += ValueToStringW(m_MyDiceTable[i]+1);
			int	l_iImageIndex = m_pPI->GetObjectIndexByName(l_strDceImageName.c_str());
			if( l_pSubMPDI )
			{
				l_pSubMPDI->ChangeAllImageByImageIndex(l_iImageIndex);			
				Vector3	l_vPos = l_pSubMPDI->GetPointList()[l_pSubMPDI->GetPointDataList()->size()-1];
				Vector2	l_vSize = l_pSubMPDI->GetPointData(l_pSubMPDI->GetPointDataList()->size()-1)->Size;
				//Vector2	l_vScale((int)(l_vSize.x*(*m_MyDiceTableImage[i]->GetWidth())),(int)(l_vSize.y*(*m_MyDiceTableImage[i]->GetHeight())));
				//POINT	l_Offset = {m_MyDiceTableImage[i]->GetOffsetPos()->x*l_vScale.x,m_MyDiceTableImage[i]->GetOffsetPos()->y*l_vScale.y};
				//m_MyDiceTableImage[i]->SetOffsetPos(l_Offset);
				m_MyDiceTableImage[i]->SetPos(l_vPos);
				m_MyDiceTableImage[i]->SetWidth((int)l_vSize.x);
				m_MyDiceTableImage[i]->SetHeight((int)l_vSize.y);
			}
			else
			{
				continue;
			}
			//l_str = l_strDicePrefix2;		l_str += ValueToStringW(i);
			//l_pSubMPDI = m_pShowMPDI->GetObject(l_str.c_str());
			//if( l_pSubMPDI )
			//	l_pSubMPDI->ChangeAllImageByImageIndex(l_iImageIndex);
		}
		m_pShowMPDI->Init();
	}
}

void		cDiceTable::SetDiceTable(std::vector<int> e_DiceContent)
{
	m_MyDiceTable = e_DiceContent;
	GenerateDiceTableImages();
}

void		cDiceTable::ChangeCallData(int e_iDiceCount,int e_iDicePoint)
{
	m_CallBehavior.ChangeData(e_iDiceCount,e_iDicePoint);
}
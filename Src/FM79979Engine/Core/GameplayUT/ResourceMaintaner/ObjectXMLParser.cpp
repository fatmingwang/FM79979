#include "ObjectXMLParser.h"
#include "../Render/UI/Mouse_Touch.h"
#include "../Render/UI/ImageButton.h"
#include "../Render/UI/AssembleImageButton.h"
#include "../Render/UI/FontFrame.h"
#include "../Render/UI/Scroller.h"
#include "../Render/UI/3DScroller.h"
#include "../Render/MPDI/MPDI.h"
#include "../Render/PuzzleImage/PuzzleImage.h"
#include "../Render/CommonRender/NumeralImage.h"
namespace FATMING_CORE
{
	cRenderObject*GetRenderObjectBehavior(TiXmlElement*e_pElement)
	{
		const wchar_t*l_strValue = e_pElement->Value();
		COMPARE_VALUE_WITH_DEFINE( cMPDI::TypeID)
		{
			return cMPDI::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cSubMPDI::TypeID)
		{
			return cSubMPDI::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cImageButton::TypeID)
		{
			return cImageButton::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cPuzzleImageUnit::TypeID )
		{
			return cPuzzleImageUnit::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cNumeralImage::TypeID )
		{
			return cNumeralImage::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cAssembleImageButton::TypeID )
		{
			return cAssembleImageButton::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cTextButton::TypeID )
		{
			return cTextButton::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cFontWithFrame::TypeID )
		{
			return cFontWithFrame::GetMe(e_pElement);
		}
		else
		{
			UT::ErrorMsg(L"GetRenderObjectBehavior not support this type",l_strValue);
		}
		return nullptr;
	}
//	<cClickMouseBehaviorVector Name="FailedMenu">
//		<cAssembleImageButton Name="GoToStore">
//			<cMPDI cMPDIList="Image/Dice/Dice.mpdi" cMPDI="GoToStore"/>
//		</cAssembleImageButton>
//		<cAssembleImageButton Name="BackToSelectStage">
//			<cMPDI cMPDIList="Image/Dice/Dice.mpdi" cMPDI="BackToSelectStage"/>
//		</cAssembleImageButton>
//		<cAssembleImageButton Name="Restart">
//			<cMPDI cMPDIList="Image/Dice/Dice.mpdi" cMPDI="Restart"/>
//		</cAssembleImageButton>
//	</cClickMouseBehaviorVector>
	cClickMouseBehaviorVector<cImageButton>*	GetClickMouseBehaviorList(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,L"cClickMouseBehaviorVector");
		cPuzzleImage*l_pPI = 0;
		cMPDIList*l_pMPDIList = 0;
		cClickMouseBehaviorVector<cImageButton>*l_pButtonList = new cClickMouseBehaviorVector<cImageButton>();
		const wchar_t*l_strListName = e_pElement->Attribute(L"Name");
		l_pButtonList->SetName(l_strListName);
		e_pElement = e_pElement->FirstChildElement();
		while( e_pElement )
		{
			const wchar_t*l_strValue = e_pElement->Value();
			COMPARE_VALUE_WITH_DEFINE( cImageButton::TypeID)
			{
				cImageButton*l_pImageButton = cImageButton::GetMe(e_pElement);
				if( l_pImageButton )
				{
#ifdef DEBUG
					if( l_pButtonList->GetObject(l_pImageButton->GetName()) )
					{
						UT::ErrorMsg(l_pImageButton->GetName(),L"thuis button name exists!");
					}
#endif
					l_pButtonList->AddObjectNeglectExist(l_pImageButton);
				}
			
			}
			else
			COMPARE_VALUE_WITH_DEFINE(cAssembleImageButton::TypeID)
			{
				cAssembleImageButton*l_pAssembleImageButton = cAssembleImageButton::GetMe(e_pElement);
				l_pButtonList->AddObjectNeglectExist(l_pAssembleImageButton);
			}
			e_pElement = e_pElement->NextSiblingElement();
		}
		return l_pButtonList;
	}

	cClickMouseBehavior*	GetClickMouseBehavior(TiXmlElement*e_pElement)
	{
		const wchar_t*l_strValue = e_pElement->Value();
		COMPARE_VALUE_WITH_DEFINE( cImageButton::TypeID)
		{
			return cImageButton::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cTextButton::TypeID)
		{
			return cTextButton::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cScroller::TypeID )
		{
			return cTabPage::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( c3DScroller::TypeID )
		{
			return cTabPage::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cTabPage::TypeID )
		{
			return cTabPage::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cAssembleImageButton::TypeID )
		{
			return cAssembleImageButton::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cButtonDialog::TypeID )
		{
			return cButtonDialog::GetMe(e_pElement);
		}
		else
		{
			UT::ErrorMsg(L"GetRenderObjectBehavior not support this type",l_strValue);
		}
		return nullptr;
	}
}
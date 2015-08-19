// stdafx.h : 可在此標頭檔中包含標準的系統 Include 檔，
// 或是經常使用卻很少變更的
// 專案專用 Include 檔案
#pragma once
#include "../../Core/AllCoreInclude.h"
#include "../../DotNetCommon/DotNetCommonTools.h"
//if u have add component into toolbox do not include the header that same resource
//#include "../../DotNetComponent/XmlNodeEditComponent.h"
#include "../../OpenglForm/OpenglFormControl.h"
#include "../../include/IL/il.h"
#include "../MapCellEditor/AllMapCellEditorInclude.h"

inline	void	CameraUpdate(System::Windows::Forms::MouseEventArgs^  e,System::Windows::Forms::Panel^e_pPanel,
							 OrthogonalCamera	*e_pOrthogonalCamer)
{
	 sMouseState	l_sMouseState;
	 memset(&l_sMouseState,0,sizeof(sMouseState));
	 l_sMouseState.iMoveX = e->X - e_pOrthogonalCamer->GetMouseBehave()->GetMouseState().iCurrentX;
	 l_sMouseState.iMoveY = e->Y - e_pOrthogonalCamer->GetMouseBehave()->GetMouseState().iCurrentY;
	 l_sMouseState.iCurrentX = e->X;
	 l_sMouseState.iCurrentY = e->Y;
	 if( e->Delta != 0 )
	 //if(e->Button == System::Windows::Forms::MouseButtons::Middle)
	 {
		 l_sMouseState.iMoveZ = e->Delta/120;
		 l_sMouseState.bMidButton = true;
	 }
	 else
	 if ( e->Button == System::Windows::Forms::MouseButtons::Left )
		 l_sMouseState.bLefButton = true;
	 else
		 if(e->Button == System::Windows::Forms::MouseButtons::Right)
	 //if (e->Button == System::Windows::Forms::MouseButtons::Right)
		 l_sMouseState.bRigButton = true;
	 e_pOrthogonalCamer->GetMouseBehave()->Update(l_sMouseState);
	 POINT l_ViewportSize = { e_pPanel->Width,e_pPanel->Height };
	 e_pOrthogonalCamer->Update(l_ViewportSize);
	 l_sMouseState.bLefButton = false;
	 l_sMouseState.bMidButton = false;;
	 l_sMouseState.bRigButton = false;;
	 e_pOrthogonalCamer->GetMouseBehave()->Update(l_sMouseState);
}
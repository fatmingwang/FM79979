// stdafx.h : 可在此標頭檔中包含標準的系統 Include 檔，
// 或是經常使用卻很少變更的
// 專案專用 Include 檔案
#pragma once

// TODO: 在此參考您的程式所需要的其他標頭

#include "../FMBook/AllFatmingBookInclude.h"
#include "../DotNetCommon/DotNetCommonTools.h"
#include "FatmingUtilitiesBase.h"

#include "../FMBook/AllFatmingBookInclude.h"

//#using "../../Media/CoreWPF.dll"
//#using "../../Media/FMBookEditorUI.dll"

typedef bool    (*FileIsUsing)(void*e_pContainer);

template<class T>inline	void DumpObjectListIntoListBox(System::Windows::Controls::ListBox^e_pListbox,T e_pList,bool e_bClearListBoxData = false,bool e_bAddIndex = true)
{
	if( e_bClearListBoxData )
		e_pListbox->Items->Clear();
	for( int i=0;i<e_pList->Count();++i )
	{
	    if(e_bAddIndex)
		    e_pListbox->Items->Add(i.ToString()+":"+DNCT::WcharToGcstring((*e_pList)[i]->GetName()));
		else
		    e_pListbox->Items->Add(DNCT::WcharToGcstring((*e_pList)[i]->GetName()));
		
	}
}

#define	WPF	System::Windows::Controls
extern cEventObject*	g_pEditorEventObject;
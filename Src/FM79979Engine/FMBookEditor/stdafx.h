// stdafx.h : �i�b�����Y�ɤ��]�t�зǪ��t�� Include �ɡA
// �άO�g�`�ϥΫo�ܤ��ܧ�
// �M�ױM�� Include �ɮ�
#pragma once

// TODO: �b���Ѧұz���{���һݭn����L���Y

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
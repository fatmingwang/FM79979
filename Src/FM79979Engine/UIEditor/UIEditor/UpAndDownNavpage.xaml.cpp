﻿//
// UpAndDownNavpage.xaml.cpp
// Implementation of the UpAndDownNavpage class
//

#include "pch.h"
#include "UpAndDownNavpage.xaml.h"
#include "UITreeView.xaml.h"
using namespace UIEditor;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

UpAndDownNavpage::UpAndDownNavpage()
{
	InitializeComponent();
}


void UIEditor::UpAndDownNavpage::m_NavigationView_SelectionChanged(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args)
{
	auto l_strSelectedItemName = m_NavigationView->SelectedItem->ToString();
	int a = 0;
}


void UIEditor::UpAndDownNavpage::NavigationViewItem_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	auto l_strSelectedItemName = m_NavigationView->SelectedItem;
	auto l_strName = sender->ToString();
	Windows::UI::Xaml::Controls::NavigationViewItem^ l_pNavigationViewItem = (Windows::UI::Xaml::Controls::NavigationViewItem^)sender;
	auto l_strTag = l_pNavigationViewItem->Tag;
}


void UIEditor::UpAndDownNavpage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!m_pUIData)
	{
		m_pUIData = ref new UIData();
		//m_OpenGLGrid->Children->Append(l_pUIData);
	}
	Windows::UI::Xaml::Controls::Button^l_pButton = (Windows::UI::Xaml::Controls::Button^)sender;
	//Button^ l_pNewButton = ref new Button();
	//l_pNewButton->Content = l_pButton->Content;
	//l_pButton->Content = "Selected";
	//this->m_NavigationView->Content = l_pNewButton;
	if (l_pButton->Content->ToString()->Equals("Page1"))
	{
		this->m_NavigationView->Content = m_pUIData;
	}
	else
	{
		this->m_NavigationView->Content = nullptr;
	}
	
}


void UIEditor::UpAndDownNavpage::Grid_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!m_OpenGLUserControl)
	{
		//UITreeView^ l_pUITreeView = ref new UITreeView();
		//m_OpenGLGrid->Children->Append(l_pUITreeView);
		m_OpenGLUserControl = ref new OpenGLUserControl();
		m_OpenGLGrid->Children->Append(m_OpenGLUserControl);

	}
}

﻿//
// NavPage.xaml.cpp
// Implementation of the NavPage class
//

#include "pch.h"
#include "NavPage.xaml.h"
#include "UpAndDownNavpage.xaml.h"
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

NavPage::NavPage()
{
	InitializeComponent();
	//UpAndDownNavpage^ l_pUpAndDownNavpage = ref new UpAndDownNavpage();
	//this->Content = l_pUpAndDownNavpage;
}


void UIEditor::NavPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}

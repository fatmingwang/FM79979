//
// TopNavPage.xaml.cpp
// Implementation of the TopNavPage class
//

#include "pch.h"
#include "TopNavPage.xaml.h"

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

TopNavPage::TopNavPage()
{
	InitializeComponent();
}
void UIEditor::TopNavPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//Windows::UI::Xaml::Controls::Button^ l_pButton = (Windows::UI::Xaml::Controls::Button^)sender;
	//Button^ l_pNewButton = ref new Button();
	//l_pNewButton->Content = l_pButton->Content;
	//l_pButton->Content = "Selected";
	//this->m_NavigationView->Content = l_pNewButton;
}

void UIEditor::TopNavPage::m_NavigationView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!m_pHorizontalNavPage)
	{
		m_pHorizontalNavPage = ref new HorizontalNavPage();
		this->m_NavigationView->Content = m_pHorizontalNavPage;
	}
}

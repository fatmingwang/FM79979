//
// HorizontalNavPage.xaml.cpp
// Implementation of the HorizontalNavPage class
//

#include "pch.h"
#include "HorizontalNavPage.xaml.h"

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

HorizontalNavPage::HorizontalNavPage()
{
	InitializeComponent();
	CoreWindow::GetForCurrentThread()->KeyUp += ref new Windows::Foundation::TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &HorizontalNavPage::NavView_KeyUp);
}


//public MyPage()
//{
//	CoreWindow.GetForCurrentThread().KeyDown += MyPage_KeyDown;
//}
//
//void MyPage_KeyDown(CoreWindow sender, KeyEventArgs args)
//{
//	Debug.WriteLine(args.VirtualKey.ToString());
//}

void UIEditor::HorizontalNavPage::NavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!m_OpenGLUserControl)
	{
		m_OpenGLUserControl = ref new OpenGLUserControl();
		NavView->Content = m_OpenGLUserControl;
	}
}


void UIEditor::HorizontalNavPage::NavView_KeyUp(CoreWindow^ sender, KeyEventArgs^ e)
{
	auto l_Key = e->VirtualKey;
	if (l_Key == Windows::System::VirtualKey::Q)
	{
		this->NavView->OpenPaneLength = 0;
		//NavView->PaneDisplayMode = Microsoft::UI::Xaml::Controls::NavigationViewDisplayMode::Expanded;
	}
	else
	if (l_Key == Windows::System::VirtualKey::W)
	{
		this->NavView->OpenPaneLength = 300;
		//NavView->PaneDisplayMode = Microsoft::UI::Xaml::Controls::NavigationViewDisplayMode::Expanded;
	}
	else
	if (l_Key == Windows::System::VirtualKey::E)
	{
		//NavView->PaneDisplayMode = Microsoft::UI::Xaml::Controls::NavigationViewDisplayMode::Minimal;
	}
}

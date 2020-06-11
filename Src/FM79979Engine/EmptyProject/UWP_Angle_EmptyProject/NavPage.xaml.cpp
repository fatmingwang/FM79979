//
// NavPage.xaml.cpp
// Implementation of the NavPage class
//

#include "pch.h"
#include "NavPage.xaml.h"

using namespace UWP_Angle_EmptyProject;

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
	m_OpenGLUserControl = ref new OpenGLUserControl();
	this->NavView->Content = m_OpenGLUserControl;
	//NavigationView^ l_NavigationView = (NavigationView^)this->MyUI->FindName("NavView");
	//Button^ l_pButton = ref new Button();
	//l_NavigationView->Content = l_pButton;
}

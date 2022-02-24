//
// UpAndDownNavpage.xaml.cpp
// Implementation of the UpAndDownNavpage class
//

#include "pch.h"
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

UpAndDownNavpage::UpAndDownNavpage()
{
	InitializeComponent();
	m_OpenGLUserControl = ref new OpenGLUserControl();
	this->m_NavigationView->Content = m_OpenGLUserControl;
}

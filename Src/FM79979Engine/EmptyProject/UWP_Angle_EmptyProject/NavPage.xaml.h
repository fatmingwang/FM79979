//
// NavPage.xaml.h
// Declaration of the NavPage class
//

#pragma once

#include "NavPage.g.h"
#include "OpenGLUserControl.xaml.h"
namespace UWP_Angle_EmptyProject
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class NavPage sealed
	{
		OpenGLUserControl^ m_OpenGLUserControl;
	public:
		NavPage();
	};
}

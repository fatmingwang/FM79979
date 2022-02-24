//
// UpAndDownNavpage.xaml.h
// Declaration of the UpAndDownNavpage class
//

#pragma once

#include "UpAndDownNavpage.g.h"
#include "OpenGLUserControl.xaml.h"
namespace UIEditor
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class UpAndDownNavpage sealed
	{
		OpenGLUserControl^ m_OpenGLUserControl;
	public:
		UpAndDownNavpage();
	};
}

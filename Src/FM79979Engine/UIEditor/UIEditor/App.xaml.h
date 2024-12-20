﻿//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"
#include "NavPage.xaml.h"
#include "TopNavPage.xaml.h"

namespace UIEditor
{
		/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed
	{
		bool m_bUseHorizontal;
	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnResuming(Platform::Object ^sender, Platform::Object ^args);
		void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);
		NavPage^ m_pNavPage;
		TopNavPage^ m_pTopNavPage;
	};
}

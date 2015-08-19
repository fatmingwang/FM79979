// ProbabilityStatics.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"


#pragma comment(lib, "../../lib/Devil.lib")
#pragma comment(lib, "../../lib/OpenAL32.lib")
#pragma comment(lib, "../../lib/alut.lib")
#ifdef _OGG_
#pragma comment(lib, "../../lib/ogg.lib")
#endif
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "../../lib/glew32.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Urlmon.lib")
#ifdef _DEBUG
#ifdef OPENGLES_2_X
//#pragma comment(lib, "../../../lib/Debug/Core.lib")
//#pragma comment(lib, "../../../lib/Debug/Particle.lib")
#else
//#pragma comment(lib, "../../../lib/DebugFixedPipeline/Core.lib")
//#pragma comment(lib, "../../../lib/DebugFixedPipeline/Particle.lib")
#endif
#pragma comment(lib, "../../lib/Debug/BulletCollision.lib")
#pragma comment(lib, "../../lib/Debug/BulletDynamics.lib")
#pragma comment(lib, "../../lib/Debug/LinearMath.lib")
#else
//#pragma comment(lib, "../../../lib/Release/FCollada.lib")
//#pragma comment(lib, "../../../lib/Release/FViewer.lib")
//#pragma comment(lib, "../../../lib/Release/FeelingLib.lib")
#ifdef OPENGLES_2_X
//#pragma comment(lib, "../../../lib/Release/Core.lib")
//#pragma comment(lib, "../../../lib/Release/Particle.lib")
#else
//#pragma comment(lib, "../../../lib/ReleaseFixedPipeline/Core.lib")
//#pragma comment(lib, "../../../lib/ReleaseFixedPipeline/Particle.lib")
#endif
#pragma comment(lib, "../../lib/Release/BulletCollision.lib")
#pragma comment(lib, "../../lib/Release/BulletDynamics.lib")
#pragma comment(lib, "../../lib/Release/LinearMath.lib")
#endif






using namespace ProbabilityStatics;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}

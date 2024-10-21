#include "stdafx.h"
#include "GameApp.h"
//#include "TestPhase.h"
#include "NetworkSample.h"
#include "Proto/addressbook.pb.h"
#include "Proto/MessageTest.pb.h"
#include "../../imgui/ImGuiRender.h"
#include "../../imgui/imgui.h"
#ifdef WASM
#include <emscripten/html5.h>
#endif
cNetworkSample* g_pNetworkSample = nullptr;

extern void	LoadSample();
extern void	DestorySampleObject();
extern void	SampleUpdate(float e_fElpaseTime);
extern void	SampleRender();
extern void	SampleMouseDown(int e_iPosX,int e_iPosY);
extern void	SampleMouseMove(int e_iPosX,int e_iPosY);
extern void	SampleMouseUp(int e_iPosX,int e_iPosY);
extern void	SampleKeyup(char e_cKey);
bool g_bUseIMGUI = true;

#if defined(ANDROID)
cEngineTestApp::cEngineTestApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#elif defined(WIN32)
cEngineTestApp::cEngineTestApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cEngineTestApp::cEngineTestApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
#ifdef WASM
	//FMLOG("new cNetworkSample");
	//g_pNetworkSample = new cNetworkSample();
#endif
	this->m_sbDebugFunctionWorking = true;
	this->m_sbSpeedControl = true;
	m_bLeave = false;
	m_pPhaseManager = new cPhaseManager();	
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
	if (g_bUseIMGUI)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
	}
}

cEngineTestApp::~cEngineTestApp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	SAFE_DELETE(m_pPhaseManager);
	SAFE_DELETE(g_pNetworkSample);
	DestorySampleObject();
	Destroy();
}

void	cEngineTestApp::Init()
{
	cGameApp::Init();
	if (g_pNetworkSample)
	{
		g_pNetworkSample->Init();
	}
	//LoadSample();

	//cGamePhase*l_pGamePhase = new cGamePhase();
	//l_pGamePhase->SetName(L"Test");
	//bool	l_b = this->m_pPhaseManager->AddObject(l_pGamePhase);
	//if( !l_b )
	{
		//assert(0&&"this phase has been added!");
	}
	//let first update is not too big
	this->m_sTimeAndFPS.Update();
}

void	cEngineTestApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	//SampleUpdate(e_fElpaseTime);
	this->m_pPhaseManager->Update(e_fElpaseTime);
	if (g_pNetworkSample)
	{
		g_pNetworkSample->Update(e_fElpaseTime);
	}
}


// Variables to control the clipping region
ImVec2 g_clipMin(0, 0);
ImVec2 g_clipMax(500, 500);

void MyCustomClippingWindow()
{
	//ImVec2 l_vStartPoint(cGameApp::m_spOpenGLRender->m_vViewPortSize.x, cGameApp::m_spOpenGLRender->m_vViewPortSize.y);
	ImVec2 l_vStartPoint(300,300);
	//ImVec2 l_clipMin(g_clipMin.x, g_clipMin.y);
	//ImVec2 l_clipMax(g_clipMax.x, g_clipMax.y);
	//ImVec2 windowPos(1920/2, 1080/2); // Set desired X and Y position
	//ImGui::SetNextWindowPos(windowPos);
	ImGui::Begin("Clipping Example");

	// Make sure l_clipMax is always greater than l_clipMin
	if (g_clipMax.x < g_clipMin.x) g_clipMax.x = g_clipMin.x + 1;
	if (g_clipMax.y < g_clipMin.y) g_clipMax.y = g_clipMin.y + 1;

	// Set up the clipping rectangle based on the current values of l_clipMin and l_clipMax
	auto l_vStartPos = ImVec2(g_clipMin.x + l_vStartPoint.x, g_clipMin.y + l_vStartPoint.y);
	auto l_vEndPos = ImVec2(g_clipMax.x + l_vStartPoint.x, g_clipMax.y + l_vStartPoint.y);
	ImGui::GetWindowDrawList()->PushClipRect(l_vStartPos, l_vEndPos, true); // true to intersect with current clip rect

	ImGui::DragFloat2("Clip Min", (float*)&g_clipMin, 1.0f, 0.0f, 1500.0f, "%.0f");
	ImGui::DragFloat2("Clip Max", (float*)&g_clipMax, 1.0f, 0.0f, 1500.0f, "%.0f");



	// Draw some content that will be clipped
	ImGui::Text("This text may be clipped based on the clip region.");
	ImGui::Text("Only part of this text will be visible.");

	// Draw some shapes for visualization
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRect(l_vStartPoint, l_vStartPos, IM_COL32(255, 0, 0, 255)); // Red rectangle outside clipping
	drawList->AddRect(l_vStartPoint, l_vEndPos, IM_COL32(0, 255, 0, 255)); // Green rectangle showing the clip region

	 //Restore the clipping
	ImGui::GetWindowDrawList()->PopClipRect();

	ImGui::End();
}

void	cEngineTestApp::Render()
{
	this->m_spOpenGLRender->m_vGameResolution = Vector2(1280, 720);
	this->m_spOpenGLRender->m_vBGColor = Vector4(0.5, 0.5, 0.5, 1);
	//glEnable(GL_ALPHA_TEST);
	MyGLEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	cGameApp::Render();
	GLRender::RenderFilledRectangle(Vector2(0, 0), 1920, 1080, Vector4::One, 0);
	int fb_width = (int)(ImGui::GetDrawData()->DisplaySize.x * ImGui::GetDrawData()->FramebufferScale.x);
	int fb_height = (int)(ImGui::GetDrawData()->DisplaySize.y * ImGui::GetDrawData()->FramebufferScale.y);
	int	l_iOpenGLViewportX = cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
	int	l_iOpenGLViewportY = cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();
#ifdef WASM
	int l_iBrowserWidth = EMSDK::EMSDK_GetBrowserWidth();
	int l_iBrowserHeight = EMSDK::EMSDK_GetBrowserHeight();
	int	l_iViewportWidth = EMSDK::EMSDK_GetViewportWidth();
	int	l_iViewportHeight = EMSDK::EMSDK_GetViewportHeight();
	int	l_iCanvasPosX = EMSDK::EMSDK_GetCanvasPosX();
	int	l_iCanvasPosY = EMSDK::EMSDK_GetCanvasPosY();
	int windowWidth, windowHeight;
	emscripten_get_canvas_element_size("#canvas", &windowWidth, &windowHeight);

	cGameApp::RenderFont(0, 300, UT::ComposeMsgByFormat(L"FBSize:%d,:%d\nBrowserSize:%d,%d\nCavansSize:%d,%d\nCanvasPos:%d,%d\nOpenGLViewport:%d,%d\nEMSCanvans:%d,%d",
		fb_width, fb_height,
		l_iBrowserWidth, l_iBrowserHeight,
		l_iViewportWidth, l_iViewportHeight,
		l_iCanvasPosX, l_iCanvasPosY,
		l_iOpenGLViewportX, l_iOpenGLViewportY,
		windowWidth, windowHeight).c_str());
#else
#endif
	//this->m_pPhaseManager->DebugRender();
		// Start the Dear ImGui frame
	if (g_bUseIMGUI)
	{
		auto io = ImGui::GetIO();
		bool show_demo_window = true;
		bool show_another_window = false;

		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#ifdef WIN32
		ImGui_ImplWin32_NewFrame();
#elif defined(WASM)
		ImGui_ImplSDL2_NewFrame();
#endif
		ImGui::NewFrame();
		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		 //2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}
		//MyCustomClippingWindow();// 
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	cGameApp::RenderFont(0, 300, UT::ComposeMsgByFormat(L"FBSize:%d,:%d\nOpenGLViewport:%d,%d",
		fb_width, fb_height,
		l_iOpenGLViewportX, l_iOpenGLViewportY).c_str());
	cGameApp::ShowInfo();
#ifdef WIN32
	SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
}

void	cEngineTestApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	SampleMouseDown(m_sMousePosition.x, m_sMousePosition.y);
}

void	cEngineTestApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	SampleMouseMove(m_sMousePosition.x, m_sMousePosition.y);
}

void	cEngineTestApp::MouseUp(int e_iPosX,int e_iPosY)
{
	//FMLOG("send data");
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	SampleMouseUp(m_sMousePosition.x, m_sMousePosition.y);
	//if (g_pNetworkSample && g_pNetworkSample->GetNetWorkStatus() == eNetWorkStatus::eNWS_CONNECTED)
	//{
	//	tutorial::Person l_Person;
	//	auto l_pPhone = l_Person.add_phones();
	//	l_pPhone->set_number("79979");
	//	//https://protobuf.narkive.com/Mv4sIIhp/sending-from-a-c-program-to-a-java-program-over-socket
	//	tutorial::AddressBook l_AddressBook;
	//	int size = l_AddressBook.ByteSizeLong();
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		auto l_pPeople1 = l_AddressBook.add_people();
	//		l_pPeople1->set_id(i);
	//		l_pPeople1->set_name("2266");
	//	}
	//	auto l_peopleSize = l_AddressBook.people_size();
	//	size = l_AddressBook.ByteSizeLong();
	//	//network buffer 
	//	char* buffer = (char*)alloca(size);
	//	l_AddressBook.SerializeToArray(buffer, size);
	//	g_pNetworkSample->SendDataToServer(buffer, size);
	//	FMLOG("send data");
	//}
}

void	cEngineTestApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cEngineTestApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	SampleKeyup(e_char);
}
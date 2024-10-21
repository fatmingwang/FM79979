#include "stdafx.h"
#include "GameApp.h"
//#include "TestPhase.h"
#include "NetworkSample.h"
#include "Proto/addressbook.pb.h"
#include "Proto/MessageTest.pb.h"
#include "../../imgui/ImGuiRender.h"
#include "../../imgui/imgui.h"
#include "MyImGui.h"
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
		cMyImGuiMenu::Init();
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

void	cEngineTestApp::Render()
{
	this->m_spOpenGLRender->m_vGameResolution = Vector2(1280, 720);
	this->m_spOpenGLRender->m_vBGColor = Vector4(0.5, 0.5, 0.5, 1);
	//glEnable(GL_ALPHA_TEST);
	MyGLEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	cGameApp::Render();
	if (g_bUseIMGUI)
	{
		MyImGuiTest();
	}
	//cGameApp::ShowInfo();
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
#include <emscripten.h>

//#define USE_SDL2
//add to linker -sUSE_SDL=2 
#ifdef USE_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#endif


#include "stdafx.h"


#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <assert.h>

#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>


#include "GameApp.h"

#include "Proto/addressbook.pb.h"
#include "Proto/MessageTest.pb.h"
#include "../../imgui/imgui.h"
#include "../../imgui/ImGuiRender.h"
#ifdef USE_SDL2
SDL_Window* g_pSDL2Window = nullptr;
SDL_GLContext g_glContext = nullptr;
#endif
extern bool g_bUseIMGUI;

//
//EMSCRIPTEN_WEBSOCKET_T g_WebSocket;
//
//EM_BOOL onopen(int eventType, const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData) 
//{
//	puts("onopen");
//	//EMSCRIPTEN_RESULT result;
//	//result = emscripten_websocket_send_utf8_text(websocketEvent->socket, "hoge");
//	//if (result) 
//	//{
//	//	printf("Failed to emscripten_websocket_send_utf8_text(): %d\n", result);
//	//}
//	{
//		tutorial::AddressBook l_AddressBook;
//		int size = l_AddressBook.ByteSizeLong();
//		for (int i = 0; i < 3; ++i)
//		{
//			auto l_pPeople1 = l_AddressBook.add_people();
//			l_pPeople1->set_id(i);
//			l_pPeople1->set_name("2266");
//		}
//		auto l_peopleSize = l_AddressBook.people_size();
//		size = l_AddressBook.ByteSizeLong();
//		//network buffer 
//		char* buffer = (char*)alloca(size);
//		l_AddressBook.SerializeToArray(buffer, size);
//		EMSCRIPTEN_RESULT l_Result = emscripten_websocket_send_binary(websocketEvent->socket, buffer, size);
//		FMLOG("send size:%d", size);
//	}
//	return EM_TRUE;
//}
//EM_BOOL onerror(int eventType, const EmscriptenWebSocketErrorEvent *websocketEvent, void *userData) {
//
//	printf("onerror:%d", eventType);
//
//	return EM_TRUE;
//}
//EM_BOOL onclose(int eventType, const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData) {
//	printf("onclose:%d", eventType);
//
//	return EM_TRUE;
//}
//EM_BOOL onmessage(int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData) {
//	puts("onmessage");
//	if (websocketEvent->isText) {
//		// For only ascii chars.
//		printf("message: %s\n", websocketEvent->data);
//	}
//
//	EMSCRIPTEN_RESULT result;
//	result = emscripten_websocket_close(websocketEvent->socket, 1000, "no reason");
//	if (result) {
//		printf("Failed to emscripten_websocket_close(): %d\n", result);
//	}
//	return EM_TRUE;
//}


cGameApp*g_pGameApp = 0;
cPreLoadFromInternet*g_pPreLoadFromInternet = nullptr;


//https://stackoverflow.com/questions/51343425/not-able-to-bind-function-in-emscripten
//https://segmentfault.com/a/1190000011229465
#include <emscripten/bind.h>
//using namespace emscripten;
//class cWASMBindingTest*g_pWASMBindingTest = nullptr;
//class cWASMBindingTest
//{
//public:
//	cWASMBindingTest(std::string e_strText)
//	{
//		g_pWASMBindingTest = this;
//		y = e_strText;
//		printf("-----------------------cWASMBindingTest------------------call ed");
//	}
//	cWASMBindingTest(int x, std::string y)
//		: x(x)
//		, y(y)
//	{}
//
//	void incrementX() {
//		++x;
//	}
//
//	int getX() const { return x; }
//	void setX(int x_) { x = x_; }
//	std::string getY() const { return y; }
//	void setY(std::string e_str) { y = e_str; }
//
//	static std::string getStringFromInstance(const cWASMBindingTest& instance) 
//	{
//		return instance.y;
//	}
//	void	Render()
//	{
//		cGameApp::RenderFont(Vector2(600.f, 600.f), ValueToStringW(y).c_str());
//	}
//private:
//	int x;
//	std::string y;
//};
//
//void JSBinding(std::string e_str)
//{
//	if (!g_pWASMBindingTest)
//	{
//		g_pWASMBindingTest = new cWASMBindingTest(e_str.c_str());
//	}
//}
//
//extern int g_iMPDIIndex;
//void	MPDIIndex(int e_iIndex)
//{
//	g_iMPDIIndex = e_iIndex;
//}
//

//EMSCRIPTEN_BINDINGS(my_module)
//{
//	class_<cWASMBindingTest>("cWASMBindingTest")
//		.constructor<int, std::string>()
//		.constructor<std::string>()
//		.function("incrementX", &cWASMBindingTest::incrementX)
//		.property("x", &cWASMBindingTest::getX, &cWASMBindingTest::setX)
//		.property("y", &cWASMBindingTest::getY, &cWASMBindingTest::setY)
//		.class_function("getStringFromInstance", &cWASMBindingTest::getStringFromInstance)
//		;
//	emscripten::function("JSBinding", &JSBinding);
//	emscripten::function("MPDIIndex", &MPDIIndex);
//}
//EMSCRIPTEN_BINDINGS(my_class_example) 
//{
//class_<cWASMBindingTest>("cWASMBindingTest")
//	.constructor<int, std::string>()
//	.constructor<const char*>()
//	.function("incrementX", &cWASMBindingTest::incrementX)
//	.property("x", &cWASMBindingTest::getX, &cWASMBindingTest::setX)
//	.class_function("getStringFromInstance", &cWASMBindingTest::getStringFromInstance)
//;
//}

//<script>
//    console.log("-----------------------");
//    //Module.JSBinding();
//    Module['onRuntimeInitialized'] = () =>
//    {
//        Module.JSBinding();
//    }
//    console.log("========================");
//Module.ready().then
//    (
//var l_Instance = new Module.cWASMBindingTest("Hi");
//l_Instance.y = "hello world";
//   )
//console.log("========================");
//</script>

void handle_key_up(SDL_Keysym* keysym)
{
	switch (keysym->sym)
	{
	case SDLK_ESCAPE:
		break;
	case SDLK_SPACE:

		break;
	default:
		break;
	}
}

void handle_key_down(SDL_Keysym* keysym)
{
	switch (keysym->sym) 
	{
		case SDLK_ESCAPE:
			break;
		case SDLK_SPACE:
			
			break;
		default:
			break;
	}
}

void process_events(void)
{
	//try {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (g_bUseIMGUI)
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
			}
			if (g_pGameApp)
			{
				switch (event.type)
				{
				case SDL_KEYDOWN:
					handle_key_down(&event.key.keysym);
					break;
				case SDL_KEYUP:
					handle_key_up(&event.key.keysym);
					break;
				case SDL_QUIT:
					emscripten_cancel_main_loop();
					break;
				case SDL_MOUSEMOTION:
					g_pGameApp->MouseMove(event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEBUTTONDOWN:
					g_pGameApp->MouseDown(event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEBUTTONUP:
					g_pGameApp->MouseUp(event.motion.x, event.motion.y);
					break;
				}
			}
		}
	//}
	//catch (const std::exception& e) {
	//	printf("Caught exception: %s", e.what());
	//}

}

void Loop()
{
	//if (!g_pGameApp)
	//{
	//	return;
	//}
	int windowWidth = EMSDK::EMSDK_GetBrowserWidth();
	int windowHeight = EMSDK::EMSDK_GetBrowserHeight();
	//try {
#ifdef USE_SDL2
		SDL_GetWindowSize(g_pSDL2Window, &windowWidth, &windowHeight);
#endif
		if (g_bUseIMGUI)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);  // Set the correct display size
		}
		g_pGameApp->Run();
		if (g_pPreLoadFromInternet)
		{
			g_pPreLoadFromInternet->Run();
			if (g_pPreLoadFromInternet->GetProgress() >= 1.f)
			{
				SAFE_DELETE(g_pPreLoadFromInternet);
				cGameApp::OutputDebugInfoString("finish pre-download files");
				if (g_pGameApp)
				{
					g_pGameApp->Init();
					g_pGameApp->m_spOpenGLRender->m_vBGColor = Vector4::Red;
				}
			}
		}
		else
		{
			process_events();
		}
		//if (g_pWASMBindingTest)
		//{
		//	g_pWASMBindingTest->Render();
		//}
#ifdef USE_SDL2
		SDL_GL_SwapWindow(g_pSDL2Window);
#else
		SDL_GL_SwapBuffers();
#endif
		
	//}
	//catch (const std::exception& e) {
	//	printf("Caught exception: %s", e.what());
	//}

}

int sockfd = -1;
void finish(int result) {
	close(sockfd);
#ifdef __EMSCRIPTEN__
	//REPORT_RESULT(result);
	emscripten_force_exit(result);
#else
	exit(result);
#endif
}


int main()
{
	//c+/c++
	//--profiling -pthread -s PROXY_POSIX_SOCKETS=1 -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1
	//no websocket --profiling -pthread 
	//linker
	//-s ERROR_ON_UNDEFINED_SYMBOLS=1 -s FETCH=1 -s FULL_ES3=1 -s ASSERTIONS=0 --bind -pthread  -s WEBSOCKET_SUBPROTOCOL=null -lwebsocket.js  -s USE_PTHREADS=1 -s SOCKET_DEBUG=1 -s PROXY_POSIX_SOCKETS=1 -s PROXY_TO_PTHREAD=1 -s WEBSOCKET_URL=192.168.31.242 
	// no tcp websocket.
	//-s ERROR_ON_UNDEFINED_SYMBOLS=1 -s FETCH=1 -s FULL_ES3=1 -s ASSERTIONS=0 --bind -pthread -lwebsocket.js  -s USE_PTHREADS=1 -s SOCKET_DEBUG=1 
	//if (1)
	//{
	//	printf("try to create websocket\n");
	//	//dire server
	//	//ws://sitapi.diresoft.net/api/agent
	//	//wss://funplayapi.pypc.net/api/agent
	//	//my
	//	//wss://localhost:9992
	//	EMSCRIPTEN_WEBSOCKET_T l_WebSocketFD = -1;
	//	bool l_bDo_WSS = true;
	//	if (l_bDo_WSS)
	//	{
	//		EmscriptenWebSocketCreateAttributes l_Att = { "wss://funplayapi.pypc.net/api/agent",0,EM_TRUE };
	//		printf("connect to wss://echo.websocket.events/.ws");
	//		l_WebSocketFD = emscripten_websocket_new(&l_Att);
	//	}
	//	else
	//	{
	//		EmscriptenWebSocketCreateAttributes l_Att = {"ws://sitapi.diresoft.net/api/agent",0,EM_TRUE};
	//		l_WebSocketFD = emscripten_websocket_new(&l_Att);
	//	}
	//	emscripten_websocket_set_onopen_callback(l_WebSocketFD, 0, onopen);
	//	emscripten_websocket_set_onerror_callback(l_WebSocketFD, 0, onerror);
	//	emscripten_websocket_set_onclose_callback(l_WebSocketFD, 0, onclose);
	//	emscripten_websocket_set_onmessage_callback(l_WebSocketFD, 0, onmessage);
	//}
	
	//struct sockaddr_in addr;
	//int res;
	//sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if (sockfd == -1) 
	//{
	//	printf("cannot create socket\n");
	//	exit(EXIT_FAILURE);
	//}
	//printf("socket created\n");
	//fcntl(sockfd, F_SETFL, O_NONBLOCK);

	//memset(&addr, 0, sizeof(addr));
	//addr.sin_family = AF_INET;
	//addr.sin_port = 2978;
	//if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) != 1) 
	//{
	//	printf("inet_pton failed\n");
	//	finish(EXIT_FAILURE);
	//}
	//printf("inet_pton ok\n");
	//res = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	//if (res == -1 && errno != EINPROGRESS) 
	//{
	//	printf("connect failed");
	//	finish(EXIT_FAILURE);
	//}
	//printf("connected\n");
	//linker command line for openglES2
	//building without any flags gives ability to target WebGL 1.0 support via GLES2 headers.No emulation.This is practically core GLES2.
	//building with - s LEGACY_GL_EMULATION = 1 gives(a very limited set of) desktop GL emulation
	//building with - s FULL_ES2 = 1 gives WebGL 1 via core GLES2 headers + emulation to get some missing GLES 2 features, most notably client side rendering support.The flags - s LEGACY_GL_EMULATION = 1 and -s FULL_ES2 = 1 are mutually orthogonal since it's not sensible to target both desktop GL and GLES at the same time.
	//building with - s USE_WEBGL2 = 1 enables targeting core WebGL 2.0 via GLES 3 headers.No emulation of GLES3 features that don't map directly. Having this flag does not yet mean that one will initialize a WebGL 2 context, or that WebGL 2 context will be required by the browser to run the generated build, but the application is capable of then initializing either aWebGL1 or WebGL 2 (GLES2 or GLES3) context. Once browsers ship stable versions of WebGL2, we'll likely make this flag default to 1 instead of current 0.
	//building with - s USE_WEBGL2 = 1 - s FULL_ES3 = 1 enables creating WebGL 2 contexts, and also adds emulation for some GLES3 features that we can emulate(mainly buffer mapping).
	//https://github.com/kripken/emscripten/issues/462
	//--preload-files
	//http://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html
	//embed-file
	//https://kripken.github.io/emscripten-site/docs/porting/files/packaging_files.html
	//exten max memory
	//http://www.cnblogs.com/ppgeneve/p/5085274.html
	FMLog::Init();
	int CANVANS_WIDTH = EMSDK::EMSDK_GetBrowserWidth();
	int CANVANS_HEIGHT = EMSDK::EMSDK_GetBrowserHeight();
	cGameApp::CreateDefaultOpenGLRender();
	cGameApp::m_spOpenGLRender->m_vViewPortSize.x = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = 0;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.y = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.y = 0;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.z = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.z = CANVANS_WIDTH;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.w = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.w = CANVANS_HEIGHT;
	cGameApp::m_sbDebugFunctionWorking = true;
	printf("do SDL_Init(SDL_INIT_EVERYTHING2)\n");
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) 
	{
		printf("device no touch\n");
#ifdef USE_SDL2
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) == -1)
#endif
		{
			std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
			return -1;
		}

	}
	printf("SDL_Init okay\n");
	//if (!emscripten_websocket_is_supported()) 
	//{
	//	printf("WebSockets are not supported in this browser");
	//}
	//https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html
	//http://lazyfoo.net/SDL_tutorials/lesson04/index.php
	//SDL_Surface*l_pSurf_Display = nullptr;
	printf("do SDL_SetVideoMode\n");
	bool l_bSDLInitOk = true;
#ifdef USE_SDL2
	// Create SDL window with OpenGL ES
	g_pSDL2Window = SDL_CreateWindow("SDL2 OpenGL ES Example",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		CANVANS_WIDTH, CANVANS_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	//if ((l_pSurf_Display = SDL_CreateWindow(CANVANS_WIDTH, CANVANS_HEIGHT, 32, SDL_OPENGL)) == NULL)
	if (!g_pSDL2Window)
	{
		printf("do SDL_SetVideoMode failed\n");
		return -1;
	}
	// Create OpenGL ES context
	g_glContext = SDL_GL_CreateContext(g_pSDL2Window);
	if (!g_glContext)
	{
		printf("Failed to create OpenGL ES context:");
		printf(SDL_GetError());
		SDL_DestroyWindow(g_pSDL2Window);
		SDL_Quit();
		return -1;
	}
#else
	SDL_Surface* l_pSurf_Display = nullptr;
	FMLog::Log("SDL_SetVideoMode \n", false);
	//if ((l_pSurf_Display = SDL_SetVideoMode(CANVANS_WIDTH, CANVANS_HEIGHT, 32, SDL_OPENGL| SDL_RESIZABLE)) == NULL)
	if ((l_pSurf_Display = SDL_SetVideoMode(CANVANS_WIDTH, CANVANS_HEIGHT, 32, SDL_OPENGL)) == NULL)
	{
		return -1;
	}
#endif
	printf("do SDL_SetVideoMode okay\n");
	//cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(800,600, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	//g_pPreLoadFromInternet = new cPreLoadFromInternet();
	//bool	l_bDurningPreload = g_pPreLoadFromInternet->Init("assets/PreloadResource.xml");

	if (l_bSDLInitOk)
	{
		g_pGameApp = new cEngineTestApp(cGameApp::m_spOpenGLRender->m_vGameResolution, Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height()));
		if (g_bUseIMGUI)
		{
#ifdef USE_SDL2
			ImGui_ImplSDL2_Init(g_pSDL2Window);
#else
			ImGui_ImplSDL2_Init();
#endif
		}
		ImGui_ImplOpenGL3_Init();
		g_pGameApp->Init();
		cGameApp::m_spOpenGLRender->m_vGameResolution.x = 1280;
		cGameApp::m_spOpenGLRender->m_vGameResolution.y = 720;
		cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(CANVANS_HEIGHT, CANVANS_WIDTH, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		//if (emscripten_websocket_is_supported()) 
		//{
		//	printf("emscripten_websocket_is_supported okay\n");
		//	EmscriptenWebSocketCreateAttributes ws_attrs = 
		//	{
		//		"127.0.0.1:2978",
		//		NULL,
		//		EM_TRUE
		//	};
		//	ws = emscripten_websocket_new(&ws_attrs);
		//	emscripten_websocket_set_onopen_callback(ws, NULL, onopen);
		//	emscripten_websocket_set_onerror_callback(ws, NULL, onerror);
		//	emscripten_websocket_set_onclose_callback(ws, NULL, onclose);
		//	emscripten_websocket_set_onmessage_callback(ws, NULL, onmessage);
		//}
		//else
		//{
		//	printf("emscripten_websocket_is_supported failed\n");
		//}
		//FMLog::Init();
		printf("try to call ImGui_ImplSDL2_Init");
		emscripten_set_main_loop(&Loop, 0 ,1);
	}
	return 0;
}
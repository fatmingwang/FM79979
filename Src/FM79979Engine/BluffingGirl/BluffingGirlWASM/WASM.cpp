#include "stdafx.h"
#include <emscripten.h>
#include <SDL/SDL.h>
#include "../../Core/AllCoreInclude.h"
#include "BluffingGirlApp.h"
#include "JSBindCode.h"
//#include "../../Core/Network/SDL_net.h"
cGameApp*g_pGameApp = 0;
cPreLoadFromInternet*g_pPreLoadFromInternet = nullptr;
//#define TEST_RUN
cMPDI*g_pMPDITest = nullptr;
cBasicSound*g_pSound = nullptr;


void handle_key_up(SDL_keysym* keysym)
{
	FMLOG("key up%d", (char)keysym->sym);
	g_pGameApp->KeyUp((char)keysym->sym);
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

void handle_key_down(SDL_keysym* keysym)
{
	FMLOG("key down%d", (char)keysym->sym);
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
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
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
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED || 
					event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event  == SDL_WINDOWEVENT_MAXIMIZED ||
					event.window.event == SDL_WINDOWEVENT_RESTORED)
				{
					if (cGameApp::m_spOpenGLRender)
					{
						printf("size change:%d,%d\n", event.window.data1, event.window.data2);
						cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(event.window.data2, event.window.data1,
							(int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
					}
				}
				break;
			}
		}
	}
}

void Loop()
{
	if (g_pGameApp)
	{
		g_pGameApp->Run();
	}
	if (g_pMPDITest)
	{
		g_pMPDITest->Update(0.016f);
		g_pMPDITest->Render();
	}
	if (g_pSound)
		g_pSound->Update(0.016f);
#ifndef TEST_RUN
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
			}
		}
	}
	else
#endif
	{
		process_events();
	}
	SDL_GL_SwapBuffers();
}

void FileTest()
{
	printf("try to write file\n");
	const char* l_strFileName = "/offline/666.txt";
	int l_iNumLength = 0;
	auto l_strFileContent = UT::GetFileContent(l_strFileName, l_iNumLength);
	if (l_strFileContent)
	{
		printf("has file\n");
		printf(l_strFileContent);
		printf("\n");
	}
	else
	{
		printf("no file\n");
		if (UT::SaveTxtToFile(l_strFileName, "3939889", 7))
		{
			printf("file saved\n");
		}
		else
		{
			printf("file not saved\n");
		}
	}
}

int main()
{
	////http://www.alternativegames.net/blog/porting-to-emscripten/
	//EM_ASM
	//(
	//	//FS.createFolder('/', 'user_data', true, true)
	//	//FS.mount(IDBFS, {}, '/user_data');
	//	// Make a directory other than '/'
	//	FS.mkdir('/offline');
	//	// Then mount with IDBFS type
	//	FS.mount(IDBFS, {}, '/offline');
	//	Module.print("start file sync");
	//	//flag to check when data are synchronized
	//	Module.syncdone = 0;
	//	// Then sync
	//	FS.syncfs(true, function(err)
	//	{
	//		// Error
	//		if (err) console.log('ERROR!', err);
	//		Module.syncdone = 1;
	//		console.log('finished syncing..');
	//	});
	//);
// Don't forget to sync to make sure you store it to IndexedDB
 //   EM_ASM
	//(
	//	FS.syncfs(function(error) 
	//{
	//	if (error) 
	//	{
	//		console.log("Error while syncing", error);
	//	}
	//});
 //   );
	//Sleep(300);
	//return 0;
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
	//with pthread memory is not allow enlarge instead enlarge to use -s TOTAL_MEMORY=33554432
	//if you got any problem with IIS cannt download file,please check /wwwroot/web.config
	//please add allow extension file in IIS setup
	//< ? xml version = "1.0" encoding = "UTF-8" ? >
	//	<configuration>
	//	<system.webServer>
	//	<httpProtocol>
	//	<customHeaders>
	//	<add name = "Access-Control-Allow-Origin" value = "*" / >
	//	<add name = "Access-Control-Allow-Methods" value = "OPTIONS,POST,GET,PUT" / >
	//	<add name = "Access-Control-Allow-Headers" value = "x-requested-with" / >
	//	<add name = "Access-Control-Allow-Credentials" value = "true" / >
	//	< / customHeaders>
	//	< / httpProtocol>
	//	<security>
	//	<requestFiltering>
	//	<fileExtensions>
	//	<remove fileExtension = ".asa" / >
	//	<add fileExtension = ".mem" allowed = "true" / >
	//	<add fileExtension = ".data" allowed = "true" / >
	//	<add fileExtension = ".xml" allowed = "true" / >
	//	<add fileExtension = ".pi" allowed = "true" / >
	//	<add fileExtension = ".ogg" allowed = "true" / >
	//	<add fileExtension = ".mpdi" allowed = "true" / >
	//	<add fileExtension = ".png" allowed = "true" / >
	//	<add fileExtension = ".wav" allowed = "true" / >
	//	<add fileExtension = ".html" allowed = "true" / >
	//	<add fileExtension = ".FontInfo" allowed = "true" / >
	//	< / fileExtensions>
	//	< / requestFiltering>
	//	< / security>
	//	<staticContent>
	//	<mimeMap fileExtension = ".data" mimeType = "text/plain" / >
	//	<mimeMap fileExtension = ".mem" mimeType = "text/plain" / >
	//	<mimeMap fileExtension = ".mpdi" mimeType = "text/plain" / >
	//	<mimeMap fileExtension = ".pi" mimeType = "text/plain" / >
	//	<mimeMap fileExtension = ".FontInfo" mimeType = "text/plain" / >
	//	< / staticContent>
	//	< / system.webServer>
	//	< / configuration>
	//http://kb.dynamsoft.com/questions/924/Error+"XMLHttpRequest+cannot+load+%2A%2A%2A.+No+%27Access-Control-Allow-Origin%27+header+is+present+on+the+requested+resource."
	JSInit();
	FMLog::Init();
	printf("start\n");
//#define	CANVANS_WIDTH	1280//*0.7
//#define	CANVANS_HEIGHT	720//*0.7
	int CANVANS_WIDTH = GetBrowserWidth();
	int CANVANS_HEIGHT = GetBrowserHeight();
	FMLOG("BrowserW:%d,BrowserH:%d", CANVANS_WIDTH, CANVANS_HEIGHT);
	//char cwd[PATH_MAX];
	//if (getcwd(cwd, sizeof(cwd)) != NULL)
	//{
	//	printf("Directory:%s\n",cwd);
	//}
	cGameApp::CreateDefaultOpenGLRender();
	cGameApp::m_spOpenGLRender->m_vViewPortSize.x = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = 0;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.y = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.y = 0;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.z = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.z = CANVANS_WIDTH;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.w = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.w = CANVANS_HEIGHT;
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return -1;
	}
	//https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html
	//http://lazyfoo.net/SDL_tutorials/lesson04/index.php
	SDL_Surface*l_pSurf_Display = nullptr;
	FMLog::Log("SDL_SetVideoMode \n", false);
	//if ((l_pSurf_Display = SDL_SetVideoMode(CANVANS_WIDTH, CANVANS_HEIGHT, 32, SDL_OPENGL| SDL_RESIZABLE)) == NULL)
	if ((l_pSurf_Display = SDL_SetVideoMode(CANVANS_WIDTH, CANVANS_HEIGHT, 32, SDL_OPENGL )) == NULL)
	{
		return -1;
	}
	FMLog::Log("new cPreLoadFromInternet\n", false);
	g_pGameApp = new cBluffingGirlApp(cGameApp::m_spOpenGLRender->m_vGameResolution, Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height()));
	//please copy Media/BluffingGirl folder into your server repository
	g_pPreLoadFromInternet = new cPreLoadFromInternet();
	bool	l_bDurningPreload = g_pPreLoadFromInternet->Init("PreloadFile.xml");
	if (l_pSurf_Display)
	{
		FMLog::Log("SDL surface exists start create our game\n", false);
		//cGameApp::m_sbDebugFunctionWorking = true;
		cGameApp::m_spOpenGLRender->m_vGameResolution.x = 720;
		cGameApp::m_spOpenGLRender->m_vGameResolution.y = 1280;
		cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(CANVANS_HEIGHT, CANVANS_WIDTH, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		FMLog::Log("start to emscripten_set_main_loop Mar 16 2022\n", false);
#ifdef TEST_RUN
		cMPDIList*l_pMPDILIst = cGameApp::GetMPDIListByFileName(L"BluffingGirl/Image/Main_Massage.mpdi");
		if(l_pMPDILIst)
		{ 
			g_pMPDITest = l_pMPDILIst->GetObject(L"Main_Bg_Animation");
		}
		g_pSound = new cOpanalOgg(nullptr, "BluffingGirl/Sound/MainBG.ogg", true);
		if (g_pSound)
		{
			g_pSound->Play(true);
		}
#endif
		emscripten_set_main_loop(&Loop, 0, 1);

	}
	return 0;
}
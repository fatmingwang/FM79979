#include "stdafx.h"
#include "../../AllLibInclude.h"
#include "BluffingGirlApp.h"
#include <emscripten.h>
#include <SDL/SDL.h>
cGameApp*g_pGameApp = 0;
cPreLoadFromInternet*g_pPreLoadFromInternet = nullptr;
void handle_key_up(SDL_keysym* keysym)
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

void handle_key_down(SDL_keysym* keysym)
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
			}
		}
	}
}

void Loop()
{
	if (g_pGameApp)
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
			}
		}
	}
	else
	{
		process_events();
	}
	SDL_GL_SwapBuffers();
}

int main()
{
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
#define	CANVANS_WIDTH	1024
#define	CANVANS_HEIGHT	768
	cGameApp::m_svViewPortSize.x = cGameApp::m_svDeviceViewPortSize.x = 0;
	cGameApp::m_svViewPortSize.y = cGameApp::m_svDeviceViewPortSize.y = 0;
	cGameApp::m_svViewPortSize.z = cGameApp::m_svDeviceViewPortSize.z = CANVANS_WIDTH;
	cGameApp::m_svViewPortSize.w = cGameApp::m_svDeviceViewPortSize.w = CANVANS_HEIGHT;
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return -1;
	}
	//https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html
	//http://lazyfoo.net/SDL_tutorials/lesson04/index.php
	SDL_Surface*l_pSurf_Display = nullptr;
	if ((l_pSurf_Display = SDL_SetVideoMode(CANVANS_WIDTH, CANVANS_HEIGHT, 32, SDL_OPENGL)) == NULL)
	{
		return -1;
	}
	//cGameApp::SetAcceptRationWithGameresolution(800,600, (int)cGameApp::m_svGameResolution.x, (int)cGameApp::m_svGameResolution.y);
	g_pPreLoadFromInternet = new cPreLoadFromInternet();
	bool	l_bDurningPreload = g_pPreLoadFromInternet->Init("assets/PreloadFile.xml");
	if (l_pSurf_Display)
	{
		//cGameApp::m_sbDebugFunctionWorking = true;
		cGameApp::m_svGameResolution.x = 1280;
		cGameApp::m_svGameResolution.y = 720;
		g_pGameApp = new cBluffingGirlApp(cGameApp::m_svGameResolution, Vector2(cGameApp::m_svViewPortSize.Width(), cGameApp::m_svViewPortSize.Height()));
		emscripten_set_main_loop(&Loop, 0, 1);
	}
	return 0;
}
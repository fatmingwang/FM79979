#include "stdafx.h"
//#include "esUtil.h"
#include "test.h"
#include "GameApp.h"

#include "../../AllLibInclude.h"
#include <emscripten.h>
#include <SDL/SDL.h>
cGameApp*g_pGameApp = 0;

//void Draw(ESContext *esContext)
//{
//	g_pGameApp->Run();
//}

static void handle_key_down(SDL_keysym* keysym)
{
	/*
	* We're only interested if 'Esc' has
	* been presssed.
	*
	* EXERCISE:
	* Handle the arrow keys and have that change the
	* viewing position/angle.
	*/
	switch (keysym->sym) {
	case SDLK_ESCAPE:
		break;
	case SDLK_SPACE:
		break;
	default:
		break;
	}
}

static void process_events(void)
{
	/* Our SDL event placeholder. */
	SDL_Event event;

	/* Grab all the events off the queue. */
	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_KEYDOWN:
			/* Handle key presses. */
			handle_key_down(&event.key.keysym);
			break;
		case SDL_QUIT:
			/* Handle quit requests (like Ctrl-c). */
			break;
		}

	}

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
	cGameApp::m_svDeviceViewPortSize.x = cGameApp::m_svGameResolution.x = 640;
	cGameApp::m_svDeviceViewPortSize.y = cGameApp::m_svGameResolution.y = 480;
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) 
	{
		return -1;
	}
	//https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html
	//http://lazyfoo.net/SDL_tutorials/lesson04/index.php
	SDL_Surface*l_pSurf_Display = nullptr;
	if ((l_pSurf_Display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) 
	{
		return -1;
	}

	//Set up the screen
	SDL_Surface *screen = SDL_SetVideoMode(cGameApp::m_svDeviceViewPortSize.x, cGameApp::m_svDeviceViewPortSize.y, 32, SDL_OPENGL);

	//ESContext esContext;
	//esInitContext(&esContext);
	//GLboolean l_b = esCreateWindow(&esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB);
	if (screen)
	{
		cGameApp::m_sbDebugFunctionWorking = true;
		g_pGameApp = new cEngineTestApp(cGameApp::m_svGameResolution, Vector2(cGameApp::m_svViewPortSize.Width(), cGameApp::m_svViewPortSize.Height()));
		g_pGameApp->Init();
		g_pGameApp->m_svBGColor = Vector4::Red;
		//cGameApp::SetAcceptRationWithGameresolution(800,600, (int)cGameApp::m_svGameResolution.x, (int)cGameApp::m_svGameResolution.y);
		//esRegisterDrawFunc(&esContext, Draw);
		//esMainLoop(&esContext);
		//emscripten_set_main_loop()
		while (1)
		{
			g_pGameApp->Run();
			//esMainLoop(&esContext);
			Sleep(1);
			SDL_GL_SwapBuffers();
		}
		//SAFE_DELETE(g_pGameApp);
	}
	else
	{
		cGameApp::OutputDebugInfoString("esCreateWindow failed!");
	}
	return 0;
}
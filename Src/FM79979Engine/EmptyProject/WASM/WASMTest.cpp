#include "stdafx.h"
#include "esUtil.h"
#include "test.h"
#include "GameApp.h"

#include "../../AllLibInclude.h"

cGameApp*g_pGameApp = 0;

void Draw(ESContext *esContext)
{
	g_pGameApp->Run();
}

int main()
{
	//--preload-files
	//http://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html
	//embed-file
	//https://kripken.github.io/emscripten-site/docs/porting/files/packaging_files.html
	FILE *file = fopen("assets/test.txt", "rb");
	if (!file) {
		printf("cannot open file\n");
		return 1;
	}
	printf("file opend\n");
	while (!feof(file)) {
		char c = fgetc(file);
		if (c != EOF) {
			putchar(c);
		}
	}
	fclose(file);
	ESContext esContext;
	esInitContext(&esContext);
	esCreateWindow(&esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB);
	cGameApp::m_sbDebugFunctionWorking = true;
	g_pGameApp = new cEngineTestApp(cGameApp::m_svGameResolution, Vector2(cGameApp::m_svViewPortSize.Width(), cGameApp::m_svViewPortSize.Height()));
	g_pGameApp->Init();
	//cGameApp::SetAcceptRationWithGameresolution(800,600, (int)cGameApp::m_svGameResolution.x, (int)cGameApp::m_svGameResolution.y);
	esRegisterDrawFunc(&esContext, Draw);

	esMainLoop(&esContext);

	//while (1)
	//{
	//	g_pGameApp->Run();
	//}

	SAFE_DELETE(g_pGameApp);
	return 0;
}
#include "stdafx.h"
#include "GameApp.h"
#include "../../Core/GameplayUT/OpenGL/GLSL/ToneMapping.h"
#include "../../Core/GameplayUT/OpenGL/GLSL/TunnelEffect.h"
#include "../../Core/Network/happyhttp.h"
#include "TestShader.h"

#ifdef WASM
//https://stackoverflow.com/questions/51343425/not-able-to-bind-function-in-emscripten
//please add --bind in linker command line!.
#include <emscripten/bind.h>
using namespace emscripten;
class cWASMBindingTest*g_pWASMBindingTest = nullptr;
class cWASMBindingTest
{
public:
	cWASMBindingTest(std::string e_strText)
	{
		g_pWASMBindingTest = this;
		y = e_strText;
		printf("-----------------------cWASMBindingTest------------------call ed");
	}
	cWASMBindingTest(int x, std::string y)
		: x(x)
		, y(y)
	{}

	void incrementX() {
		++x;
	}

	int getX() const { return x; }
	void setX(int x_) { x = x_; }
	std::string getY() const { return y; }
	void setY(std::string e_str) { y = e_str; }

	static std::string getStringFromInstance(const cWASMBindingTest& instance) {
		return instance.y;
	}
	void	Render()
	{
		cGameApp::RenderFont(Vector2(600.f, 600.f), ValueToStringW(y).c_str());
	}
private:
	int x;
	std::string y;
};

void JSBinding(std::string e_str)
{
	if (!g_pWASMBindingTest)
	{
		g_pWASMBindingTest = new cWASMBindingTest(e_str.c_str());
	}
}

void JSBindingRender()
{
	if (g_pWASMBindingTest)
	{
		g_pWASMBindingTest->Render();
	}
}

int g_iWASMMPDIIndex = 0;
void	MPDIIndex(int e_iIndex)
{
	g_iWASMMPDIIndex = e_iIndex;
}
//

EMSCRIPTEN_BINDINGS(my_module)
{
	class_<cWASMBindingTest>("cWASMBindingTest")
		.constructor<int, std::string>()
		.constructor<std::string>()
		.function("incrementX", &cWASMBindingTest::incrementX)
		.property("x", &cWASMBindingTest::getX, &cWASMBindingTest::setX)
		.property("y", &cWASMBindingTest::getY, &cWASMBindingTest::setY)
		.class_function("getStringFromInstance", &cWASMBindingTest::getStringFromInstance)
		;
	emscripten::function("JSBinding", &JSBinding);
	emscripten::function("MPDIIndex", &MPDIIndex);
}
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
//	Module.ready().then
//    (
//		var l_Instance = new Module.cWASMBindingTest("Hi");
//		l_Instance.y = "hello world";
//   )
//	console.log("========================");
//</script>

#endif
#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#define USE_SDL2
//add to linker -sUSE_SDL=2 
#ifdef USE_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#else
//#include <SDL/SDL.h>
//#include <SDL/SDL_events.h>								
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
			//if (g_bUseIMGUI)
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
			}
			if (g_pGameApp)
			{
				switch (event.type)
				{
					case SDL_KEYDOWN:
						g_pGameApp->KeyDown('R');
					break;
					case SDL_KEYUP:
						g_pGameApp->KeyUp('R');
					break;
					case SDL_QUIT:
						emscripten_cancel_main_loop();
					break;
					case SDL_MOUSEMOTION:
						g_pGameApp->MouseMove(event.motion.x, event.motion.y);
					break;
					case SDL_MOUSEBUTTONDOWN:
						g_pGameApp->MouseDown(event.motion.x, event.motion.y);
					switch (event.button.button)
					{
						case SDL_BUTTON_LEFT:
						cGameApp::m_sbMouseClickStatus[0] = true;
						break;
						case SDL_BUTTON_RIGHT:
						cGameApp::m_sbMouseClickStatus[1] = true;
						break;
						case SDL_BUTTON_MIDDLE:
						//printf("Middle button pressed at (%d, %d)\n", event.button.x, event.button.y);
						break;
					}
					break;
					case SDL_MOUSEBUTTONUP:
						g_pGameApp->MouseUp(event.motion.x, event.motion.y);
						switch (event.button.button)
						{
							case SDL_BUTTON_LEFT:
							cGameApp::m_sbMouseClickStatus[0] = false;
							break;
							case SDL_BUTTON_RIGHT:
							cGameApp::m_sbMouseClickStatus[1] = false;
							break;
							case SDL_BUTTON_MIDDLE:
							//printf("Middle button pressed at (%d, %d)\n", event.button.x, event.button.y);
							break;
						}
					break;
					case SDL_MOUSEWHEEL:
						//if (event.wheel.y != 0)
						{
							cGameApp::m_sMouseWhellDelta = event.wheel.y;
							//printf("Mouse wheel vertical: %d (y=%s)\n", event.wheel.y, event.wheel.y > 0 ? "up" : "down");
						}
						if (event.wheel.x != 0)
						{
							//printf("Mouse wheel horizontal: %d (x=%s)\n",event.wheel.x, event.wheel.x > 0 ? "right" : "left");
						}
					break;
				}
			}//end if(g_pGameApp)
		}//end while(SDL_PollEvent(&event));
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
	//try {
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
std::string g_strTestShaderCodeContent;
std::string GenerateVertexShaderWithFVF2(int64 e_i64FVFFlags, int e_iNumMorphTarget)
{
	if (e_i64FVFFlags & FVF_INSTANCING_FLAG && e_iNumMorphTarget > 0)
	{
		//assert(0&&"fuck....instance and morph exists at asame time....I am lazy to do this now fuck");
		printf("fuck....instance and morph exists at asame time....I am lazy to do this now fuck");
	}
	std::string l_strDefine;
	if (e_i64FVFFlags & FVF_POS_FLAG)
		l_strDefine += "#define USE_POSITION\n";
	if (e_i64FVFFlags & FVF_DIFFUSE_FLAG)
		l_strDefine += "#define USE_DIFFUSE\n";
	if (e_i64FVFFlags & FVF_NORMAL_FLAG)
		l_strDefine += "#define USE_NORMAL\n";
	if (e_i64FVFFlags & FVF_TANGENT_FLAG)
		l_strDefine += "#define USE_TANGENT\n";
	if (e_i64FVFFlags & FVF_BINORMAL_FLAG)
		l_strDefine += "#define USE_BINORMAL\n";
	if (e_i64FVFFlags & FVF_SKINNING_WEIGHT_FLAG)
		l_strDefine += "#define USE_WEIGHTS\n";
	if (e_i64FVFFlags & FVF_SKINNING_BONE_INDEX_FLAG)
		l_strDefine += "#define USE_JOINTS\n";
	if (e_i64FVFFlags & FVF_TEX0_FLAG)
		l_strDefine += "#define USE_TEXCOORD\n";
	if (e_i64FVFFlags & FVF_INSTANCING_FLAG)
		l_strDefine += "#define USE_INSTANCING\n";
	if (e_i64FVFFlags & FVF_ANIMATION_TEXTURE_FLAG)
		l_strDefine += "#define FVF_ANIMATION_TEXTURE_FLAG\n";

	std::string shaderCode = R"(
//this is Vertex shader
#version 330 core
#ifdef GL_ES
precision mediump float;
#endif
)" + l_strDefine + R"(
#ifdef USE_POSITION
layout(location = 0) in vec3 aPosition;
#endif
#ifdef USE_NORMAL
layout(location = 1) in vec3 aNormal;
#endif
#ifdef USE_DIFFUSE
layout(location = 2) in vec3 aVertexColor;
#endif
#ifdef USE_TANGENT
layout(location = 3) in vec3 aTangent;
#endif
#ifdef USE_BINORMAL
layout(location = 4) in vec3 aBinormal;
#endif
#ifdef USE_WEIGHTS
layout(location = 5) in vec4 aWeights;
#endif
#ifdef USE_JOINTS
layout(location = 6) in ivec4 aJoints;
#endif
#ifdef USE_TEXCOORD
layout(location = 7) in vec2 aTexCoord;
#endif
#ifdef USE_INSTANCING
//layout is not allow to assign mat4 so split it to vec4
// Per-instance matrix: locations 10,11,12,13
layout(location = 9) in vec4 aInstanceMatrix0;
layout(location = 10) in vec4 aInstanceMatrix1;
layout(location = 11) in vec4 aInstanceMatrix2;
layout(location = 12) in vec4 aInstanceMatrix3;
#endif
)";
#ifdef DEBUG
	shaderCode += R"(flat out int toFSInstanceID;)";
#endif

	if (e_iNumMorphTarget > 0)
	{
		for (int i = 0; i < e_iNumMorphTarget; i++)
		{
			shaderCode += "\nlayout(location = " + std::to_string(i + FVF_MORPHING_TARGET_POS1) + ") in vec3 aMorphTarget" + std::to_string(i) + ";";
		}
		shaderCode += "\nuniform float uMorphWeights[" + std::to_string(e_iNumMorphTarget) + "];";
	}
	shaderCode += R"(
uniform mat4 inMat4Model;
uniform mat4 inMat4View;
uniform mat4 inMat4Projection;

#ifdef USE_WEIGHTS
const int MAX_BONES = 100;
uniform mat4 uBoneTransforms[MAX_BONES];
#endif

out vec3 toFSVec3FragPos;
#ifdef USE_NORMAL
out vec3 toFSVec3Normal;
#endif
#ifdef USE_TEXCOORD
out vec2 toFSVec2TexCoord;
#endif
#ifdef USE_TANGENT
out vec3 toFSVec3Tangent;
#endif
#ifdef USE_BINORMAL
out vec3 toFSVec3Binormal;
#endif

//for animation texture

const int MAX_INSTANCES = )";
	//shaderCode += ValueToString((int)MAX_INSTANCES);
	shaderCode += R"(;
uniform sampler2D uAnimTexture;
uniform ivec2 uCurrentAndNextFrameIndex[MAX_INSTANCES];
uniform float uAnimationLerpTime[MAX_INSTANCES];
uniform int uNumAnimationModel;
uniform int uNumBones;
uniform int uTextureSize;

mat4 LerpMat4(mat4 a, mat4 b, float t)
{
    mat4 result;
    for (int i = 0; i < 4; ++i)
    {
        result[i] = mix(a[i], b[i], t); // mix each row (vec4)
    }
    return result;
}

mat4 GetAnimationPoseUseTightTexture(int joint, int instance)
{
    int numBones = uNumBones;
    int textureSize = uTextureSize;
    int numRowsPerMatrix = 4;
    int frame0 = uCurrentAndNextFrameIndex[instance].x;
    int frame1 = uCurrentAndNextFrameIndex[instance].y;
    float t = uAnimationLerpTime[instance];

    // Compute base texel index for each frame
    int linearIndex0 = (frame0 * numBones + joint) * numRowsPerMatrix;
    int linearIndex1 = (frame1 * numBones + joint) * numRowsPerMatrix;


    // Fetch 4 rows for each frame
    mat4 mat0, mat1;
    for (int row = 0; row < 4; ++row)
    {
        int idx0 = linearIndex0 + row;
        int x0 = idx0 % textureSize;
        int y0 = idx0 / textureSize;
        mat0[row] = texelFetch(uAnimTexture, ivec2(x0, y0), 0);

        int idx1 = linearIndex1 + row;
        int x1 = idx1 % textureSize;
        int y1 = idx1 / textureSize;
        mat1[row] = texelFetch(uAnimTexture, ivec2(x1, y1), 0);
    }
    
    // Linear interpolate each matrix element
    mat4 pose = LerpMat4(mat0, mat1, t);
    return pose;
}

void main()
{
    vec3 position = aPosition;
)";
	printf(shaderCode.c_str());
	if (e_iNumMorphTarget > 0)
	{
		for (int i = 0; i < e_iNumMorphTarget; i++)
		{
			shaderCode += "    position += uMorphWeights[" + std::to_string(i) + "] * aMorphTarget" + std::to_string(i) + ";\n";
		}
	}

	shaderCode += R"(
    vec4 worldPos = vec4(position, 1.0);

#ifdef USE_WEIGHTS
#ifdef USE_JOINTS
    mat4 skinMatrix;
    if(uNumAnimationModel != 0)
    {
        int instanceIndex = gl_InstanceID % uNumAnimationModel;
	    mat4 pose0 = GetAnimationPoseUseTightTexture(aJoints.x, instanceIndex);
	    mat4 pose1 = GetAnimationPoseUseTightTexture(aJoints.y, instanceIndex);
	    mat4 pose2 = GetAnimationPoseUseTightTexture(aJoints.z, instanceIndex);
	    mat4 pose3 = GetAnimationPoseUseTightTexture(aJoints.w, instanceIndex);
	    //mat4 pose0 = GetAnimationPose(aJoints.x, instanceIndex);
	    //mat4 pose1 = GetAnimationPose(aJoints.y, instanceIndex);
	    //mat4 pose2 = GetAnimationPose(aJoints.z, instanceIndex);
	    //mat4 pose3 = GetAnimationPose(aJoints.w, instanceIndex);
        skinMatrix = aWeights.x*10 * pose0 +
                          aWeights.y *10* pose1 +
                          aWeights.z *10* pose2 +
                          aWeights.w*10 * pose3;
    }
    else
    {
        skinMatrix = aWeights.x * uBoneTransforms[aJoints.x] +
                          aWeights.y * uBoneTransforms[aJoints.y] +
                          aWeights.z * uBoneTransforms[aJoints.z] +
                          aWeights.w * uBoneTransforms[aJoints.w];
    }   
    worldPos = skinMatrix * worldPos;
#endif
#endif
#ifdef USE_INSTANCING
    mat4 instanceMatrix = mat4(aInstanceMatrix0, aInstanceMatrix1, aInstanceMatrix2, aInstanceMatrix3);
    mat3 instanceNormalMatrix = mat3(instanceMatrix);
    worldPos = instanceMatrix * worldPos;)";
#ifdef DEBUG
	shaderCode += R"(
    toFSInstanceID = gl_InstanceID;)";
#endif
	shaderCode += R"(
#endif    
    worldPos = inMat4Model * worldPos;
    toFSVec3FragPos = worldPos.xyz;
    gl_Position = inMat4Projection * inMat4View * worldPos;

#ifdef USE_NORMAL
#ifdef USE_INSTANCING
    toFSVec3Normal = mat3(transpose(inverse(inMat4Model))) * (instanceNormalMatrix * aNormal);
#else
    toFSVec3Normal = mat3(transpose(inverse(inMat4Model))) * aNormal;
#endif
#endif

#ifdef USE_TANGENT
#ifdef USE_INSTANCING
    toFSVec3Tangent = mat3(transpose(inverse(inMat4Model))) * (instanceNormalMatrix * aTangent);
#else
    toFSVec3Tangent = mat3(transpose(inverse(inMat4Model))) * aTangent;
#endif
#endif

#ifdef USE_BINORMAL
#ifdef USE_INSTANCING
    toFSVec3Binormal = mat3(transpose(inverse(inMat4Model))) * (instanceNormalMatrix * aBinormal);
#else
    toFSVec3Binormal = mat3(transpose(inverse(inMat4Model))) * aBinormal;
#endif
#endif

#ifdef USE_TEXCOORD
    toFSVec2TexCoord = aTexCoord;
#endif
}
)";

	//#if defined(DEBUG) || defined(WASM)
	printf(shaderCode.c_str());
	//#endif
	return shaderCode;
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
	int CANVANS_WIDTH = 1280;// EMSDK::EMSDK_GetBrowserWidth();
	int CANVANS_HEIGHT= 720;// EMSDK::EMSDK_GetBrowserHeight();
	cGameApp::CreateDefaultOpenGLRender();
	cGameApp::m_spOpenGLRender->m_vViewPortSize.x = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = 0;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.y = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.y = 0;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.z = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.z = CANVANS_WIDTH;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.w = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.w = CANVANS_HEIGHT;
	cGameApp::m_sbDebugFunctionWorking = true;
	printf("do SDL_Init(SDL_INIT_EVERYTHING2)\n");
	//if (SDL_Init(SDL_INIT_EVERYTHING) == -1) 
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		printf("device no touch\n");
#ifdef USE_SDL2
		if (SDL_Init(SDL_INIT_VIDEO) == -1)
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
	//https://github.com/emscripten-ports/SDL2/issues/59
	//LINK_FLAGS "-Os -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s WASM=1 --preload-file ${CMAKE_SOURCE_DIR}/Assets@Assets --bind"

	// WebGL 2.0 == OpenGL ES 3.0
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
#ifdef USE_SDL2
	// Create SDL window with OpenGL ES
	g_pSDL2Window = SDL_CreateWindow("SDL2 OpenGL ES Example",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		CANVANS_WIDTH, CANVANS_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	cGameApp::m_spOpenGLRender->m_Handle = g_pSDL2Window;
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
	SDL_GL_MakeCurrent(g_pSDL2Window, g_glContext);
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
		g_strTestShaderCodeContent = GenerateVertexShaderWithFVF2(532707, 0);
		std::cout << g_strTestShaderCodeContent;
		cGameApp::m_spOpenGLRender->m_vGameResolution.x = 1920;
		cGameApp::m_spOpenGLRender->m_vGameResolution.y = 1080;
		g_pGameApp = new cEngineTestApp(cGameApp::m_spOpenGLRender->m_vGameResolution, Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height()));
		g_pGameApp->Init();
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
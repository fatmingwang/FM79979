#include "stdafx.h"
#include "GameApp.h"
#include "../../Core/GameplayUT/OpenGL/GLSL/ToneMapping.h"
#include "../../Core/GameplayUT/OpenGL/GLSL/TunnelEffect.h"
#include "../../Core/Network/happyhttp.h"
#include "NetworkSample.h"
//#include "TestShader.h"
#include "WebsocketServer.h"
#include "Proto/addressbook.pb.h"
#include "Proto/MessageTest.pb.h"
//multithread
//https://stackoverflow.com/questions/59550963/multithreading-in-webassembly
//#include "../../Core/Bluetooth/Bluetooth.h"

//cNetworkSample*				g_pNetworkSample = nullptr;
cFMMorphingAnimationVector*	g_pFMMorphingAnimationVector = nullptr;
cCameraZoomFunction*		g_pCameraZoomFunction = nullptr;
cFrameCamera*				g_pFrameCamera = 0;
cOrthogonalCamera*			g_pOrthogonalCamera = 0;
cCameraBehaveByMouseBehave*	g_pCameraBehaveByMouseBehave = 0;
cParticleEmitterGroup*		g_pParticleEmitterGroup = 0;
cPrtEmitter*				g_pPrtEmitter = nullptr;
cCurveManager*				g_p2DCurveLoader = 0;
cCurveWithTime*				g_p2DCurvesWithTime = 0;
cMPDIList*					g_pMPDIList = 0;
cMultiPathDynamicImage*		g_pMultiPathDynamicImage = 0;
cMultiPathDynamicImage*		g_pMultiPathDynamicImageClone = 0;
cCurveWithTime*				g_pTestCurveWithTime = nullptr;;
c2DImageCollisionData*		g_pCollisionData = nullptr;
//cAnimationMesh*				g_pAnimationMesh = 0;
cCurveWithTime*				g_pCurve = 0;
cMPDINode*					g_pMPDINode = 0;
cToneMappingShader*			g_pToneMappingShader = nullptr;
cBaseImage*					g_pBGImage = nullptr;
cTunnelEffect*				g_pTunnelEffect = nullptr;
//cTestShader*				g_pTestShader = nullptr;
//cMSAAFrameBuffer*			g_pMSAAFrameBuffer = nullptr;
cFrameBuffer*				g_pFrameBuffer = nullptr;
cFreetypeGlyphRender*		g_pFreetypeGlyphRender = nullptr;
cFreetypeGlyphRender*		g_pFreetypeGlyphRender2 = nullptr;
cPathChaser*				g_pPathChaser = nullptr;
cCurveWithTime*				g_pCurveWithTime = nullptr;
cCurveWithTime*				g_pCurveWithTime2 = nullptr;


void						LoadSample();
void						DestoryObject();
void						SampleUpdate(float e_fElpaseTime);
void						KeyboardDataRender();
void						SampleRender();
void						SampleMouseDown(int e_iPosX,int e_iPosY);
void						SampleMouseMove(int e_iPosX,int e_iPosY);
void						SampleMouseUp(int e_iPosX,int e_iPosY);
void						SampleKeyup(char e_cKey);

tutorial::AddressBook g_AddressBook;

#ifdef WASM
extern int g_iWASMMPDIIndex;
extern void JSBindingRender();
#endif

cBasicSound*g_pBG = nullptr;

void	ProtoBufTest()
{
	////TopLevelMessage;
	//GOOGLE_PROTOBUF_VERIFY_VERSION;
	//{
	//	//https://developers.google.com/protocol-buffers/docs/proto#options
	//	//import "google/protobuf/descriptor.proto";
	//	//extend google.protobuf.MessageOptions {
	//	//  optional string my_option = 51234;
	//	//}
	//	//message MyMessage {
	//	//  option (my_option) = "Hello world!";
	//	//}
	//	string value = MessageTest::MyMessage::descriptor()->options().GetExtension(MessageTest::my_option);
	//	int a = 0;
	//}
	//{
	//	//https://stackoverflow.com/questions/40450556/how-to-know-which-protobuf-message-the-byte-array-is/50291651#50291651
	//	//google::protobuf::Any l_Any = google::protobuf::Any::ParseFrom(person);
	//	//tutorial::Person person = google::protobuf::Any::pack(person);

	//	//out.write(any.toByteArray());
	//}
	//{
	//	tutorial::Person l_Person;
	//	l_Person.add_phones();
	//	g_AddressBook.ByteSizeLong();
	//	//https://protobuf.narkive.com/Mv4sIIhp/sending-from-a-c-program-to-a-java-program-over-socket
	//	int size = g_AddressBook.ByteSizeLong();
	//	//network buffer 
	//	char* buffer = (char*)alloca(size);
	//	g_AddressBook.SerializeToArray(buffer, size);
	//	int aa = 0;
	//}
	//{
	//	MessageTest::sNetworkData_eMI_C2S_LOGIN_REQUEST l_Data;
	//	l_Data.set_strauth("132");
	//	l_Data.set_strgameid("79979");
	//	//https://stackoverflow.com/questions/9496101/protocol-buffer-over-socket-in-c
	//}

}

void	SoundBGThread()
{

	google::protobuf::ShutdownProtobufLibrary();
//	g_AddressBook.add
	FMLog::Log("SoundBGThread start", false);
	if (g_pBG)
	{
		g_pBG->SetLoop(true);
		g_pBG->Play(true);
	}
	while (g_pBG)
	{
		if (g_pBG)
		{
			g_pBG->Update(0.016f);
			Sleep(16);
			//FMLog::Log("SoundBGThread 2", false);
		}
	}
	FMLog::Log("SoundBGThread end", false);
}



cBaseShader*g_pMSAAShader = nullptr;
//#ifdef WIN32
extern void ComputerShaderInit();
//#endif

void   ThreadLoadTest()
{
#ifdef WASM
	//cOpanalOgg*l_pSound = nullptr;//new cOpanalOgg(g_pPathChaser,"Media/MainBG.ogg",false);
	cBasicSound* l_pSound = cGameApp::m_spSoundParser->AddSound("Sound/MainBG.ogg",true);
	g_pBG = l_pSound;
	bool l_bMultiThreadTest = false;
	if (l_bMultiThreadTest)
	{
		FMLog::Log("try to do thread detach", false);
		std::thread l_Thread = std::thread([=] {SoundBGThread(); });
		l_Thread.detach();
		FMLog::Log("thread detached", false);
	}
	//if (0 && !g_pNetworkSample)
	//{
	//	g_pNetworkSample = new cNetworkSample();
	//	g_pNetworkSample->Init();

	//}
#endif
}

void	LoadSample()
{
	ProtoBufTest();
	FMLog::Log("LoadSample start", false);
#ifdef WIN32
	WSADATA wsaData;
	int error;
	if ((error = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0)
	{
		printf("Error %d in WSAStartup, result will fail\n", error);
	}
	//WebSocketInit();
#else
	//ComputerShaderInit();
#endif
	//TestGET();
	//Test2();
	//TestGET3();
	if (0)
	{
		g_pFMMorphingAnimationVector = cGameApp::GetObjectByFileName<cFMMorphingAnimationVector>("Morphing/777.mx", eGBT_2D_MORPHING_ANIMATION);
	}
	int l_iYBase = 500;
	//g_pCurveWithTime = new cCurveWithTime();
	//g_pCurveWithTime2 = new cCurveWithTime();
	if (g_pCurveWithTime)
	{
		g_pCurveWithTime->AddPoint(Vector3(100, 100, 0), 0);
		g_pCurveWithTime->AddPoint(Vector3(100.f, (float)l_iYBase, 0), 0);


		g_pCurveWithTime->AddPoint(Vector3(200, (float)l_iYBase, 0), 0);
		g_pCurveWithTime->AddPoint(Vector3(300, (float)l_iYBase, 0), 0);
		g_pCurveWithTime->AddPoint(Vector3(400, (float)l_iYBase, 0), 0);
		g_pCurveWithTime->AddPoint(Vector3(500, (float)l_iYBase, 0), 0);
		g_pCurveWithTime->AddPoint(Vector3(600, (float)l_iYBase, 0), 0);
		g_pCurveWithTime->AddPoint(Vector3(700, (float)l_iYBase, 0), 0);
		g_pCurveWithTime->AddPoint(Vector3(800, (float)l_iYBase, 0), 0);
		g_pCurveWithTime->AddPoint(Vector3(900, (float)l_iYBase, 0), 0);
	}
	if (g_pCurveWithTime2)
	{
		g_pCurveWithTime2->AddPoint(Vector3(100, (float)l_iYBase + 100, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(200, (float)l_iYBase + 200, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(300, (float)l_iYBase + 100, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(400, (float)l_iYBase + 200, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(500, (float)l_iYBase + 100, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(600, (float)l_iYBase + 200, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(700, (float)l_iYBase + 100, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(800, (float)l_iYBase + 200, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(900, (float)l_iYBase + 100, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(1000, (float)l_iYBase+ 200, 0), 0);
		g_pCurveWithTime2->AddPoint(Vector3(1100, (float)l_iYBase+ 100, 0), 0);
		g_pCurveWithTime2->SetLOD(5);
	}

	//g_pPathChaser = new cPathChaser(100.f, 100.f);
	if (g_pPathChaser)
	{
		cCurveWithTime l_cCurveWithTime;
		l_cCurveWithTime.AddPoint(Vector3(400, 500, 0),0);
		l_cCurveWithTime.AddPoint(Vector3(800, 500, 0),1);
		l_cCurveWithTime.AddPoint(Vector3(600, 500, 0),2);
		l_cCurveWithTime.AddPoint(Vector3(400, 900, 0),3);
		l_cCurveWithTime.AddPoint(Vector3(600, 800, 0),4);
		l_cCurveWithTime.AddPoint(Vector3(800, 1000, 0),5);
		//l_cCurveWithTime.SetLOD(3);
		g_pPathChaser->AssignPathData(&l_cCurveWithTime);
	}
	//g_pFreetypeGlyphRender = new cFreetypeGlyphRender("kaiu.ttf", 124);
	//g_pFreetypeGlyphRender2 = new cFreetypeGlyphRender(g_pFreetypeGlyphRender);
	//cBluetoothSinglton::GetInstance()->Init();
	//cBluetoothSinglton::GetInstance()->CreateAsServer(L"FMWin10");
	//cBluetoothSinglton::GetInstance()->CreateAsClient(L"FMWin7");
//	g_pMSAAShader = CreateShader(g_bCommonVSClientState, g_strGL3CommonVS, g_strGL3MSAA_FS,L"MSAA");
	//here should do mu;ti thread but I am lazy.
	//g_pToneMappingShader = cToneMappingShader::CreateShader(
	//	"shader/ToneMapping.vs","shader/ToneMapping.ps",
	//	"shader/DownSample.vs","shader/DownSample.ps",
	//	"shader/Blur.vs","shader/BlurH.ps",
	//	"shader/Blur.vs","shader/BlurV.ps",
	//	l_Size);
	//g_pTunnelEffect = cTunnelEffect::CreateShader("shader/TunnelEffect.vs","shader/TunnelEffect.ps",L"MyTunnelEffecr");
	//g_pTestShader = cTestShader::CreateShader("shader/test.vs", "shader/test.ps", L"MyTest");;
	//g_pBGImage = new cBaseImage("DownloadFont.png");
	//g_pBGImage = new cBaseImage("MyFMBook/iPhone_Monster/MPDI/Coveer_2.png");
	//g_pBGImage->SetWidth((int)cGameApp::m_spOpenGLRender->m_vGameResolution.x);
	//g_pBGImage->SetHeight((int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	//g_pCameraZoomFunction = new cCameraZoomFunction("Lee-Younh-Aes-twins.png");
	//g_pCameraZoomFunction = new cCameraZoomFunction();
	//g_pMPDINode = new cMPDINode();
	if( g_pMPDINode )
	{
		g_pMPDINode->SetAnimationLoop(true);
		cLinerDataProcessor<Vector3>*l_pPosData = g_pMPDINode->GetPosData();
		if( l_pPosData )
		{
			l_pPosData->SetUpdateWithLiner(false);
			l_pPosData->AddData(Vector3(649,285,0),0);
			l_pPosData->AddData(Vector3(500,0,0),2.5f);
			l_pPosData->AddData(Vector3(100,200,0),5.f);
			l_pPosData->AddData(Vector3(500,600,0),7.5f);
			l_pPosData->AddData(Vector3(649,285,0),10.f);
			l_pPosData->AddData(Vector3(1440, 900, 0), 20.f);
			l_pPosData->SetLOD(5);
			l_pPosData->Rearrange(5.f);
		}
		cLinerDataProcessor<Vector4>*l_pColorData = g_pMPDINode->GetColorData();
		cLinerDataProcessor<Vector2>*l_SozeData = g_pMPDINode->GetSizeData();
		g_pMPDINode->Init();
	}
	if (1)
	{
		FMLog::Log("parse MPDI/bgrounda01.mpdi",false);
		g_pMPDIList = cGameApp::GetObjectByFileName<cMPDIList>("MPDI/bgrounda01.mpdi", eGBT_MPDILIST);
	}
	//g_pMPDIList = cGameApp::GetMPDIListByFileName(L"MyFMBook/AnimationDemo/MPDI/startscreena01.mpdi");

	std::wstring l_strPrefixName;// = L"C:/Users/fatming/Desktop/Work/Resource/trunk/CN005/Fish-神龍九龍珠/Fish/Image/Fish/BlackFish_0001/BlackFish_0001";
#ifdef WASM
	std::wstring l_strMPDIResultFileName = L"MPDI/bgrounda01.mpdi";
#else
	
	std::wstring l_strMPDIResultFileName = L"BluffingGirl/Image/GamePlay.mpdi";
#endif
	g_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIResultFileName.c_str());
	if( g_pMPDIList )
	{
		FMLog::Log("g_pMPDIList exists", false);
		//g_pMultiPathDynamicImage = g_pMPDIList->GetObject(L"PlayerNormalBody");
#ifdef WASM
		g_pMultiPathDynamicImage = g_pMPDIList->GetObject(g_iWASMMPDIIndex);
#else
		g_pMultiPathDynamicImage = g_pMPDIList->GetObject(0);
#endif
		//ensure u have call start
		g_pMultiPathDynamicImage->Init();
		//g_pMultiPathDynamicImage->SetPos(Vector3(350, 500, 0));
		g_pMultiPathDynamicImage->SetAnimationLoop(true);
		if( 0 )
		{
			//g_pMultiPathDynamicImageClone = new cMultiPathDynamicImage(g_pMPDIList->GetObject(L"DiedShow2"));
			g_pMultiPathDynamicImageClone = new cMultiPathDynamicImage(g_pMPDIList->GetObject(0));
			g_pMultiPathDynamicImageClone->Init();
			g_pMultiPathDynamicImageClone->SetAnimationLoop(true);
			//g_pMultiPathDynamicImage->SetStayAtLastFrame(true);
		}
	}
	//g_pCollisionData = new c2DImageCollisionData();
	//if (g_pCollisionData && g_pCollisionData->Parse(l_strCollisionResultFileName.c_str()))
	{

	}
	//g_pParticleEmitterGroup = cGameApp::GetPRTG("ParticleData/Stage1.prtg",L"Fire");
	if( g_pParticleEmitterGroup )
	{
		g_pParticleEmitterGroup->Init();
		g_pParticleEmitterGroup->SetAnimationLoop(true);
	}
	//g_pPrtEmitter = cGameApp::GetPrtEmitter("ParticleData/Glyph.prt",L"Glyph");
	if( g_pPrtEmitter )
	{
		g_pPrtEmitter->Emit(Vector3(200,200,0));
		g_pPrtEmitter->SetLoop(true);
		g_pPrtEmitter->Init();
		g_pPrtEmitter->SetPos(Vector3(1000,540,0));
	}

	g_pOrthogonalCamera = new cOrthogonalCamera(cGameApp::m_spOpenGLRender->m_vGameResolution);
	//
	//g_pTestCurveWithTime = new cCurveWithTime();
	if (g_pTestCurveWithTime)
	{
		g_pTestCurveWithTime->SetCalAngle(true);
		g_pTestCurveWithTime->AddPoint(Vector3(300, 300, 0), 0);
		g_pTestCurveWithTime->AddPoint(Vector3(300, 500, 0), 5);
		g_pTestCurveWithTime->AddPoint(Vector3(700, 300, 0), 10);
		g_pTestCurveWithTime->AddPoint(Vector3(1000, 680, 0), 15);
		g_pTestCurveWithTime->AddPoint(Vector3(1440, 900, 0), 20);
		g_pTestCurveWithTime->SetLOD(3);
		g_pTestCurveWithTime->Init();
	}
	//ThreadLoadTest();
	//g_pMSAAFrameBuffer = new cMSAAFrameBuffer(1920/2,1080/2);
	//g_pFrameBuffer = new cFrameBuffer(1920 / 2, 1080 / 2);
	cGameApp::OutputDebugInfoString("LoadSample() finish2");
}

void	DestorySampleObject()
{
#ifdef WIN32
	WSACleanup();
#endif
	SAFE_DELETE(g_pCurveWithTime);
	SAFE_DELETE(g_pCurveWithTime2);
	SAFE_DELETE(g_pPathChaser);
	SAFE_DELETE(g_pFreetypeGlyphRender);
	SAFE_DELETE(g_pFreetypeGlyphRender2);
	//SAFE_DELETE(g_pMSAAFrameBuffer);
	SAFE_DELETE(g_pFrameBuffer);
	//do not delete g_pMultiPathDynamicImage,it come from g_pMPDIList
	//g_pMultiPathDynamicImageClone is a clone object so delete it.
	SAFE_DELETE(g_pCollisionData);
	SAFE_DELETE(g_pMultiPathDynamicImageClone);
	SAFE_DELETE(g_pCameraZoomFunction);
	SAFE_DELETE(g_pTestCurveWithTime);
	//not necessary,it will auto release while m_pAnimationParser is delete
	//cGameApp::m_pAnimationParser->RemoveObject(L"fmbook_mpdi");
	SAFE_DELETE(g_pFrameCamera);
	SAFE_DELETE(g_pBGImage);
	SAFE_DELETE(g_pOrthogonalCamera);	
	SAFE_DELETE(g_pCurve);
	SAFE_DELETE(g_pMPDINode);
	SAFE_DELETE(g_pToneMappingShader);
	SAFE_DELETE(g_pTunnelEffect);
	//SAFE_DELETE(g_pTestShader);
	//cBluetoothSinglton::GetInstance()->DestroyInstance();
	//cBluetoothSinglton::DestroyInstance();
}

void	SampleUpdate(float e_fElpaseTime)
{
#if defined(WIN32)
	WebSocketUpdate(e_fElpaseTime);
#endif


#ifdef WASM
	//if (g_pNetworkSample)
	//{
	//	g_pNetworkSample->Update(0.016f);
	//}
#endif
	if (g_pPathChaser)
		g_pPathChaser->Update(e_fElpaseTime/4.f);
	//cBluetoothSinglton::GetInstance()->Update(e_fElpaseTime);
	//std::vector<sBluetoothPacket*> l_DataVector;
	//cBluetoothSinglton::GetInstance()->GetReceivedData(&l_DataVector);
	//if (l_DataVector.size())
	//{
	//	for (size_t i = 0; i < l_DataVector.size(); i++)
	//	{
	//		auto l_Data = l_DataVector[i];
	//		delete l_Data;
	//	}
	//}
	MyGlErrorTest("SampleUpdate start");
	if (cGameApp::m_spSoundParser)
		cGameApp::m_spSoundParser->Update(0.016f);
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->Update(e_fElpaseTime);
	}
	if( g_pMPDINode )
	{
		g_pMPDINode->Update(e_fElpaseTime);
	}
	MyGlErrorTest("before g_pMultiPathDynamicImage");
	if (g_pMultiPathDynamicImage)
	{
		Vector2 l_vDrawSize = g_pMultiPathDynamicImage->GetDrawSize() / 2;
		if (g_pTestCurveWithTime)
		{
			g_pTestCurveWithTime->Update(e_fElpaseTime);
			//g_pMultiPathDynamicImage->SetWorldTransform(cMatrix44::TranslationMatrix(g_pTestCurveWithTime->GetCurrentPosition() - Vector3(l_vDrawSize.x, l_vDrawSize.y, 0))*cMatrix44::ZAxisRotationMatrix(g_pTestCurveWithTime->GetCurrentPosToNextPointAngle() - 90));
		}
		static float l_sfAngle = e_fElpaseTime;
		l_sfAngle += 1 * e_fElpaseTime;
		//g_pMultiPathDynamicImage->SetRotationAnglePosOffsetWithDrawSize(true);
		g_pMultiPathDynamicImage->Update(e_fElpaseTime);
		if (g_pMultiPathDynamicImageClone)
		{			
			if (g_pTestCurveWithTime)
				g_pMultiPathDynamicImageClone->SetWorldTransform(cMatrix44::TranslationMatrix(g_pTestCurveWithTime->GetCurrentPosition())*cMatrix44::ZAxisRotationMatrix(g_pTestCurveWithTime->GetCurrentPosToNextPointAngle() - 90));
			g_pMultiPathDynamicImageClone->Update(e_fElpaseTime);
			g_pMultiPathDynamicImageClone->SetColor(Vector4::Red);
		}
		//int l_iIndex = (*g_pMultiPathDynamicImage)[0]->GetCurrentPointData()->iImageIndex;
		//if (g_pTestCurveWithTime)
		//{
		//	Vector3 l_vPos = g_pTestCurveWithTime->GetCurrentPosition();
		//	{
		//		cMatrix44 l_mat = cMatrix44::TranslationMatrix(Vector3(l_vDrawSize.x, l_vDrawSize.y, 0))*g_pMultiPathDynamicImage->GetWorldTransform();
		//		if (g_pCollisionData)
		//		{
		//			(*g_pCollisionData)[l_iIndex]->SetTransform(l_mat);
		//		}
		//	}
		//}
	}
	MyGlErrorTest("after g_pMultiPathDynamicImage");
	if( g_pPrtEmitter )
		g_pPrtEmitter->Update(e_fElpaseTime);
	if( g_pParticleEmitterGroup )
		g_pParticleEmitterGroup->Update(e_fElpaseTime);
	Vector2	l_vViewPort(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(),cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
	//if(g_pOrthogonalCamera)
		//g_pOrthogonalCamera->CameraUpdateByMouse(cGameApp::m_sbMouseClickStatus[0],cGameApp::m_sbMouseClickStatus[1],cGameApp::m_sMouseWhellDelta,cGameApp::m_sScreenMousePosition.x,cGameApp::m_sScreenMousePosition.y,l_vViewPort);
	MyGlErrorTest("SampleUpdate end");
}

void	KeyboardDataRender()
{
	int l_iStartPosX = 100;
	int l_iStartPosY = 100;
	static bool l_bAllKeyHasBeenPressed[260];
	for (int i = 0; i < MAX_PATH; ++i)
	{
		bool l_b = cGameApp::m_sucKeyData[i];
		wchar_t l_str[3] = {(wchar_t)i,L':',0};
		std::wstring l_strInfo = ValueToStringW(i);
		l_strInfo += L":";
		l_strInfo += l_str;
		if(l_b)
			l_bAllKeyHasBeenPressed[i] = l_b;
		if (l_bAllKeyHasBeenPressed[i])
		{
			cGameApp::m_spGlyphFontRender->SetColor(Vector4::Red);
		}
		else
		{
			cGameApp::m_spGlyphFontRender->SetColor(Vector4::One);
		}
		cGameApp::RenderFont(Vector2(l_iStartPosX, l_iStartPosY), l_strInfo.c_str());
		cGameApp::RenderFont(Vector2(l_iStartPosX+110, l_iStartPosY), l_b?L"1":L"0");
		l_iStartPosY += 30;
		if (l_iStartPosY >= 1000)
		{
			l_iStartPosX += 200;
			l_iStartPosY = 100;
		}
	}
}
bool	g_bWSMCall = false;

void	WASMBindingFunctionTest(bool e_bWSMCall)
{
	g_bWSMCall = e_bWSMCall;
}

void	SampleRender()
{
	if (g_pFrameBuffer)
	{
		g_pFrameBuffer->StartDraw();
	}
	//glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	//KeyboardDataRender();
	if( g_pTunnelEffect )
	{
		g_pTunnelEffect->Use();
		//g_pTunnelEffect->Update(0.016f);
	}
	//if (g_pTestShader)
	//{
	//	g_pTestShader->Use();
	//	g_pTestShader->Update(0.016f);
	//}	
	if( g_pToneMappingShader )
		g_pToneMappingShader->StartDraw();
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	if( g_pBGImage && g_pTunnelEffect )
	{
		g_pBGImage->SetPos(Vector3(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2*0.1f,0.f,0.f));
		g_pBGImage->SetWidth((int)(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2*0.8));
		g_pBGImage->SetHeight((int)(cGameApp::m_spOpenGLRender->m_vGameResolution.y/2*0.8));
	//	g_pBGImage->Render();
		g_pBGImage->RenderWithShader(g_pTunnelEffect->GetName());
		g_pBGImage->SetPos(Vector3(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2+cGameApp::m_spOpenGLRender->m_vGameResolution.x/2*0.1f,0,0));
		g_pTunnelEffect->SetTime(-1);
		g_pBGImage->RenderWithShader(g_pTunnelEffect->GetName());
		//g_pBGImage->Render(cMatrix44::Identity);
	}
	if (g_pBGImage)
	{
		g_pBGImage->SetPos(Vector3(cGameApp::m_spOpenGLRender->m_vGameResolution.x / 2 * 0.1f, 0.f, 0.f));
		g_pBGImage->SetWidth((int)(cGameApp::m_spOpenGLRender->m_vGameResolution.x / 2 * 0.8));
		g_pBGImage->SetHeight((int)(cGameApp::m_spOpenGLRender->m_vGameResolution.y / 2 * 0.8));
		//	g_pBGImage->Render();
		//g_pBGImage->RenderWithShader(g_pTestShader->GetName());
		g_pBGImage->SetPos(Vector3(cGameApp::m_spOpenGLRender->m_vGameResolution.x / 2 + cGameApp::m_spOpenGLRender->m_vGameResolution.x / 2 * 0.1f, 0, 0));
		//g_pBGImage->RenderWithShader(g_pTestShader->GetName());
	}
	if (g_pBGImage)
	{
		static float l_fAngle = 0.f;
		l_fAngle += 1.f;
		UseShaderProgram();
		glEnable2D(1920,1080);
		g_pBGImage->SetLocalTransform(cMatrix44::YAxisRotationMatrix(l_fAngle));
		g_pBGImage->Render();
	}
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->Render();
	}
	if(g_pOrthogonalCamera)
	{
		g_pOrthogonalCamera->Render();
		g_pOrthogonalCamera->DrawGrid();
	}
	//GLRender::RenderRectangle(1440, 900, cMatrix44::Identity, Vector4::One);
	if (g_pMultiPathDynamicImage)
	{
		static bool l_btTest = false;
		if (!l_btTest)
		{
			l_btTest = true;
			FMLog::Log("g_pMultiPathDynamicImage rendered!",false);
		}
		g_pMultiPathDynamicImage->Render();
		if (g_pMultiPathDynamicImageClone)
			g_pMultiPathDynamicImageClone->Render();
		if (g_pTestCurveWithTime)
		{
			g_pTestCurveWithTime->Render();
			//Vector3 l_vPos = g_pTestCurveWithTime->GetCurrentPosition();
			//int l_iIndex = (*g_pMultiPathDynamicImage)[0]->GetCurrentPointData()->iImageIndex;
			//if (g_pCollisionData)
			//	//for (int i = 0; i < g_CollisionData.Count();++i)
			//{
			//	(*g_pCollisionData)[l_iIndex]->DebugRender();
			//}
		}
		//GLRender::RenderPoint(Vector3(500, 500, 0), 30);
	}
	if( g_pPrtEmitter )
		g_pPrtEmitter->Render();
	if( g_pParticleEmitterGroup )
		g_pParticleEmitterGroup->Render();

	if (g_pFMMorphingAnimationVector)
	{
		auto l_pMAObject = g_pFMMorphingAnimationVector->GetObject(0);
		if (l_pMAObject)
		{
			static float l_sfTime = 0.f;
			l_sfTime += 0.005f;
			float l_fEndTimeTime = l_pMAObject->GetEndTime();
			if (l_sfTime >= 1.f)
			{
				l_sfTime = 0.f;
			}
			l_pMAObject->RenderByGlobalTime(l_sfTime);
			l_pMAObject->SetWorldPosition(Vector3(600,000,0));
		}
	}
	//RenderFilledRectangle(Vector2(0, 0), 1920, 1080, Vector4(0.f, 0.3f, 0.3f, 0.8f), 0);
	if( g_pToneMappingShader )
		g_pToneMappingShader->EndDraw();
	if (g_pFrameBuffer)
	{
		g_pFrameBuffer->EndDraw();
		POINT l_Pos = { 0,0 };
		POINT l_SizePos = { (long)cGameApp::m_spOpenGLRender->m_vGameResolution.x,(long)cGameApp::m_spOpenGLRender->m_vGameResolution.y };
		g_pFrameBuffer->DrawBuffer(l_Pos, l_SizePos);
	}

	//cBluetoothSinglton::GetInstance()->Render(400, 400);
	if (g_pFreetypeGlyphRender)
	{
		wchar_t l_str[4] = { 24185,20320,23064,0 };
		g_pFreetypeGlyphRender->RenderFont(Vector2(100, 100), l_str);
		//g_pFreetypeGlyphRender->RenderFont(200, 200, L"中");
		//if (g_pFreetypeGlyphRender2)
		//{
		//	g_pFreetypeGlyphRender2->SetColor(Vector4(0.5, 0.5, 1, 1));
		//	std::wstring l_str = L"中口以嗎\n部口已\nQQ";
		//	g_pFreetypeGlyphRender2->RenderFont(200, 400, l_str.c_str());
		//}
	}
	if (g_pPathChaser)
	{
		g_pPathChaser->RenderCurve(Vector4(1,1,0,1));
	}
	if (g_pCurveWithTime2)
	{
		g_pCurveWithTime2->RenderCurve(Vector4::One,cMatrix44::Identity,2.f,false);
		static float l_fStart = 0.1f;
		static float l_fIncrease = l_fStart;
		bool l_bDoPositive = true;
		static bool l_bDoUpdate = true;
		static int l_iNextChangeCount = 5;
		static float l_fRandSpeed = 1.f;
		--l_iNextChangeCount;
		if (l_iNextChangeCount == 0)
		{
			l_fRandSpeed = frand(0.01f, 3.f);
			if (l_fRandSpeed > 2.f)
			{
				l_iNextChangeCount = rand() % 5 + 2;
			}
			else
			{
				l_iNextChangeCount = rand() % 120 + 20;
			}
		}
		if (cGameApp::m_sucKeyData['P'])
		{
			l_bDoUpdate = false;
		}
		else
		if (cGameApp::m_sucKeyData['S'])
		{
			l_bDoUpdate = true;
		}
		bool l_bFinish = false;

		if (l_bDoUpdate)
		{
			if (l_bDoPositive)
			{
				l_fIncrease += (0.005f* l_fRandSpeed);
				if (l_fIncrease >= 1.f)
				{
					l_fStart = frand(0, 0.5f);
					//l_fStart = 0.1f;
					l_fIncrease = l_fStart;
					l_bFinish = true;
				}
			}
			else
			{
				l_fIncrease -= (0.005f * l_fRandSpeed);
				if (l_fIncrease <= 0.f)
				{
					l_fStart = frand(0.5f, 1.f);
					//l_fStart = 0.9f;
					l_fIncrease = l_fStart;
					l_bFinish = true;
				}
			}
		}
		//auto l_mat = cMatrix44::TranslationMatrix(100, 100, 0);
		auto l_mat = cMatrix44::Identity;
		if (0)
		{
			for (int i = 0; i < 10; ++i)
			{
				//g_pCurveWithTime2->RenderByRange(l_fStart/(i+1), l_fIncrease / (i + 1), Vector4::Red, l_mat,(i+1)/10.f, false);
			}
		}
		else
		{
			g_pCurveWithTime2->RenderByRange(l_fStart , l_fIncrease , Vector4::Red, l_mat);
		}
		if (l_bFinish)
		{
			g_pCurveWithTime2->Destroy();
			g_pCurveWithTime2->SetLOD(1);
			int l_iCount = rand() % 20 + 2;
			auto l_vLastPos = Vector3(frand(100, 1910), frand(100, 1070), 0);
			int l_iRandX = 600;
			int l_iRandY = 300;
			for (int i = 0; i < l_iCount; ++i)
			{
				g_pCurveWithTime2->AddPoint(l_vLastPos, 0);
				l_vLastPos = Vector3(l_vLastPos.x+frand(-l_iRandX, l_iRandX), l_vLastPos.y + frand(-l_iRandY, l_iRandY), l_vLastPos.z);
				if(l_vLastPos.x <= 0.f) l_vLastPos.x = 0.f;
				if(l_vLastPos.x >= 1920.f) l_vLastPos.x = 1920.f;
				if(l_vLastPos.y <= 0.f) l_vLastPos.y = 0.f;
				if(l_vLastPos.y >= 1080.f) l_vLastPos.y = 1080.f;
			}
			g_pCurveWithTime2->SetLOD(rand()%5+1);
		}
	}
	if (g_bWSMCall)
	{
		if (g_pCurveWithTime2)
		{
			g_pCurveWithTime2->RenderCurve();
			g_pCurveWithTime2->RenderCurve();
			auto l_List = g_pCurveWithTime2->GetPointList();
			int l_iValueData[] = {
				3,
				8,
				19,
				26,
				32,
				34,
				36,
				38 };

			for (int i = 0; i < l_List.size(); ++i)
			{
				if (i > 0)
				{
					l_List[i].y -= 50;
					l_List[i].x -= 20;
					cGameApp::RenderFont(l_List[i].x, l_List[i].y, ValueToStringW(l_iValueData[i - 1]));
					cGameApp::RenderFont(l_List[i].x, 950.f, ValueToStringW(i));
				}
			}
			cGameApp::RenderFont(20, 100, L"80");
		}
	}
#ifdef WASM
	JSBindingRender();
#endif
	static bool l_bTest = false;
	if (!l_bTest)
	{
		auto l_iWidth = cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
		auto l_iHeight = cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();
		//SaveCurrentBufferToImage("99.jpg", l_iWidth, l_iHeight);
		l_bTest = true;
	}
}

void	SampleMouseDown(int e_iPosX,int e_iPosY)
{
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->MouseDown(e_iPosX,e_iPosY);
	}
	if (g_pPathChaser)
	{
		g_pPathChaser->MouseDown(e_iPosX, e_iPosY);
	}
}

void	SampleMouseMove(int e_iPosX,int e_iPosY)
{
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->MouseMove(e_iPosX,e_iPosY);
	}
	if (g_pPathChaser)
	{
		g_pPathChaser->MouseMove(e_iPosX, e_iPosY);
	}
	if (g_pPrtEmitter)
	{
		g_pPrtEmitter->SetPos(Vector3(e_iPosX,e_iPosY,0));
	}
}

void	SampleMouseUp(int e_iPosX,int e_iPosY)
{
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->MouseUp(e_iPosX,e_iPosY);
	}
	//char l_Data[5] = {5,4,3,2,1};
	//cBluetoothSinglton::GetInstance()->SendDataToServer(5, l_Data);
	if (g_pPathChaser)
	{
		g_pPathChaser->MouseUp(e_iPosX, e_iPosY);
	}
}

void	SampleKeyup(char e_cKey)
{
	//char l_cData[5] = {0,1,2,3,4};
	//cBluetoothSinglton::GetInstance()->SendDataToServer(5, l_cData);
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->KeyUp(e_cKey);
	}
	if( e_cKey == 'R' )
	{
		if (g_pTunnelEffect)
		{
			SAFE_DELETE(g_pTunnelEffect);
			g_pTunnelEffect = cTunnelEffect::CreateShader("shader/TunnelEffect.vs", "shader/TunnelEffect.ps", L"MyTunnelEffecr");
		}
//		if (g_pTestShader)
//		{
//			SAFE_DELETE(g_pTestShader);
////			g_pTestShader = cTestShader::CreateShader("shader/TunnelEffect.vs", "shader/TunnelEffect.ps", L"MyTunnelEffecr");
//		}
		

		if (g_pTestCurveWithTime)
			g_pTestCurveWithTime->Init();


	}
}
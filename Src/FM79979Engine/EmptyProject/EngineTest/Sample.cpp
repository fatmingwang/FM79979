#include "stdafx.h"
#include "GameApp.h"
#include "../../Core/GLSL/ToneMapping.h"
#include "../../Core/GLSL/TunnelEffect.h"

cCameraZoomFunction*g_pCameraZoomFunction = nullptr;

cFrameCamera	*g_pFrameCamera = 0;
cOrthogonalCamera*g_pOrthogonalCamera = 0;
cCameraBehaveByMouseBehave*g_pCameraBehaveByMouseBehave = 0;


cParticleEmitterGroup*g_pParticleEmitterGroup = 0;
cPrtEmitter*g_pPrtEmitter;


cPrtEmitter*g_pPrtEmiter = 0;
cCurveManager*g_p2DCurveLoader = 0;
cCurveWithTime*g_p2DCurvesWithTime = 0;

cMPDIList*g_pMPDIList = 0;
cMultiPathDynamicImage*g_pMultiPathDynamicImage = 0;
cMultiPathDynamicImage*g_pMultiPathDynamicImageClone = 0;

cColladaParser*g_pColladaParser = 0;
//cAnimationMesh*g_pAnimationMesh = 0;
cCurveWithTime*	g_pCurve = 0;

cMPDINode*g_pMPDINode = 0;


cToneMappingShader*g_pToneMappingShader = nullptr;

cBaseImage*g_pBGImage = nullptr;

cTunnelEffect*g_pTunnelEffect = nullptr;

void	LoadSample();
void	DestoryObject();
void	SampleUpdate(float e_fElpaseTime);
void	SampleRender();

void	SampleMouseDown(int e_iPosX,int e_iPosY);
void	SampleMouseMove(int e_iPosX,int e_iPosY);
void	SampleMouseUp(int e_iPosX,int e_iPosY);
void	SampleKeyup(char e_cKey);
void	LoadSample()
{
	POINT l_Size = { (int)cGameApp::m_svGameResolution.x/8,(int)cGameApp::m_svGameResolution.y/8 };
	//g_pToneMappingShader = cToneMappingShader::CreateShader(
	//	"shader/ToneMapping.vs","shader/ToneMapping.ps",
	//	"shader/DownSample.vs","shader/DownSample.ps",
	//	"shader/Blur.vs","shader/BlurH.ps",
	//	"shader/Blur.vs","shader/BlurV.ps",
	//	l_Size);
	g_pTunnelEffect = cTunnelEffect::CreateShader("shader/TunnelEffect.vs","shader/TunnelEffect.ps",L"MyTunnelEffecr");
	g_pBGImage = new cBaseImage("DownloadFont.png");
	g_pBGImage->SetWidth((int)cGameApp::m_svGameResolution.x);
	g_pBGImage->SetHeight((int)cGameApp::m_svGameResolution.y);
	//g_pBGImage = new cBaseImage("MyFMBook/iPhone_Monster/MPDI/Coveer_2.png");
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
			l_pPosData->SetLOD(5);
			l_pPosData->Rearrange(5.f);
		}
		cLinerDataProcessor<Vector4>*l_pColorData = g_pMPDINode->GetColorData();
		cLinerDataProcessor<Vector2>*l_SozeData = g_pMPDINode->GetSizeData();
		g_pMPDINode->Init();
	}

	//g_pMPDIList = cGameApp::GetMPDIListByFileName(L"MyFMBook/AnimationDemo/MPDI/startscreena01.mpdi");
	if( g_pMPDIList )
	{
		g_pMultiPathDynamicImage = g_pMPDIList->GetObject(0);
		//ensure u have call start
		g_pMultiPathDynamicImage->Init();
		g_pMultiPathDynamicImage->SetAnimationLoop(true);
		if( 0 )
		{
			g_pMultiPathDynamicImageClone = new cMultiPathDynamicImage(g_pMultiPathDynamicImage);
			//g_pMultiPathDynamicImage->SetLoop(true);
			g_pMultiPathDynamicImage->SetStayAtLastFrame(true);
		}
	}
	//g_pParticleEmitterGroup = cGameApp::GetPRTG("ParticleData/Stage1.prtg",L"Fire");
	if( g_pParticleEmitterGroup )
	{
		g_pParticleEmitterGroup->Init();
		g_pParticleEmitterGroup->SetAnimationLoop(true);
	}
	//g_pPrtEmitter = cGameApp::GetPrtEmitter("ParticleData/Smoke.prt",L"Fire");
	if( g_pPrtEmitter )
	{
		g_pPrtEmitter->Emit(Vector3(200,200,0));
		g_pPrtEmitter->SetLoop(true);
	}

	//g_pColladaParser = new cColladaParser();
	if( g_pColladaParser )
	{
		if(g_pColladaParser->ParseDataIntoXMLNode("3DFish_test/test.DAE"))
		{
			FATMING_CORE::CreateShader(g_bMyMeshVSClientState,g_strMyMeshVS,g_strMyMeshFS,STATIC_MESH_SHADER);
			FATMING_CORE::CreateShader(g_bMySkinningMeshVSClientState,g_strMySkinningMeshVS,g_strMySkinningMeshFS,SKINNING_MESH_SHADER);
			g_pCurve = new cCurveWithTime();
			g_pCurve->AddPoint(Vector3(0,300,0),0);
			g_pCurve->AddPoint(Vector3(300,300,0),2.f);
			g_pCurve->AddPoint(Vector3(720,300,0),4.5);
			g_pCurve->AddPoint(Vector3(0,300,0),9);
			g_pCurve->SetCurveLoop(true);
			g_pCurve->Init();
		}
	}
	g_pOrthogonalCamera = new cOrthogonalCamera(cGameApp::m_svGameResolution);
	//
}

void	DestorySampleObject()
{
	//do not delete g_pMultiPathDynamicImage,it come from g_pMPDIList
	//g_pMultiPathDynamicImageClone is a clone object so delete it.
	SAFE_DELETE(g_pMultiPathDynamicImageClone);
	SAFE_DELETE(g_pCameraZoomFunction);
	//not necessary,it will auto release while m_pAnimationParser is delete
	//cGameApp::m_pAnimationParser->RemoveObject(L"fmbook_mpdi");
	SAFE_DELETE(g_pFrameCamera);
	SAFE_DELETE(g_pBGImage);
	SAFE_DELETE(g_pOrthogonalCamera);
	SAFE_DELETE(g_pColladaParser);
	SAFE_DELETE(g_pCurve);
	SAFE_DELETE(g_pMPDINode);
	SAFE_DELETE(g_pToneMappingShader);
	SAFE_DELETE(g_pTunnelEffect);
}

void	SampleUpdate(float e_fElpaseTime)
{
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->Update(e_fElpaseTime);
	}
	if( g_pMPDINode )
	{
		g_pMPDINode->Update(e_fElpaseTime);
	}
	if( g_pMultiPathDynamicImage )
		g_pMultiPathDynamicImage->Update(e_fElpaseTime);
	if( g_pPrtEmitter )
		g_pPrtEmitter->Update(e_fElpaseTime);
	if( g_pParticleEmitterGroup )
		g_pParticleEmitterGroup->Update(e_fElpaseTime);
	Vector2	l_vViewPort(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height());
	if(g_pOrthogonalCamera)
		g_pOrthogonalCamera->CameraUpdateByMouse(cGameApp::m_sbMouseClickStatus[0],cGameApp::m_sbMouseClickStatus[1],cGameApp::m_sMouseWhellDelta,cGameApp::m_sScreenMousePosition.x,cGameApp::m_sScreenMousePosition.y,l_vViewPort);
	if( g_pColladaParser && g_pColladaParser->m_pAllAnimationMesh )
	{
		if( g_pCurve )
			g_pCurve->Update(e_fElpaseTime);
		int	l_iCount = g_pColladaParser->m_pAllAnimationMesh->Count();
		for( int i=0;i<l_iCount;++i )
		{
			g_pColladaParser->m_pAllAnimationMesh->GetObject(i)->Update(e_fElpaseTime);
			if( g_pMPDINode && i==0 )
			{
				g_pColladaParser->m_pAllAnimationMesh->GetObject(i)->SetLocalTransform(g_pMPDINode->GetWorldTransform());
			}
		}
	}
}

void	SampleRender()
{
	if( g_pTunnelEffect )
	{
		g_pTunnelEffect->Use();
		//g_pTunnelEffect->Update(0.016f);
	}
	if( g_pToneMappingShader )
		g_pToneMappingShader->StartDraw();

	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	//glEnable2D(1280,720);
	if( g_pBGImage && g_pTunnelEffect )
	{
		g_pBGImage->SetPos(Vector3(cGameApp::m_svGameResolution.x/2*0.1,0,0));
		g_pBGImage->SetWidth((int)(cGameApp::m_svGameResolution.x/2*0.8));
		g_pBGImage->SetHeight((int)(cGameApp::m_svGameResolution.y/2*0.8));
	//	g_pBGImage->Render();
		g_pBGImage->RenderWithShader(g_pTunnelEffect->GetName());
		g_pBGImage->SetPos(Vector3(cGameApp::m_svGameResolution.x/2+cGameApp::m_svGameResolution.x/2*0.1,0,0));
		g_pTunnelEffect->SetTime(-1);
		g_pBGImage->RenderWithShader(g_pTunnelEffect->GetName());
		//g_pBGImage->Render(cMatrix44::Identity);
	}

	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->Render();
	}

	if(g_pOrthogonalCamera)
	{
		g_pOrthogonalCamera->Render();
		//g_pOrthogonalCamera->DrawGrid();
	}
	if( g_pMultiPathDynamicImage )
		g_pMultiPathDynamicImage->Render();
	if( g_pPrtEmitter )
		g_pPrtEmitter->Render();
	if( g_pParticleEmitterGroup )
		g_pParticleEmitterGroup->Render();
	if( g_pColladaParser && g_pColladaParser->m_pAllAnimationMesh )
	{
		FATMING_CORE::UseShaderProgram(SKINNING_MESH_SHADER);
		if(g_pOrthogonalCamera)
			g_pOrthogonalCamera->Render();
		int	l_iCount = g_pColladaParser->m_pAllAnimationMesh->Count();
		for( int i=0;i<l_iCount;++i )
		{
			float	l_fAngle = -90;
			if( g_pCurve && g_pCurve->GetCurrentTime() > 4.5 )
			{
				l_fAngle = 90;
				g_pColladaParser->m_pAllAnimationMesh->GetObject(i)->SetLocalTransform(cMatrix44::TranslationMatrix(g_pCurve->GetCurrentPosition())*cMatrix44::RotationMatrix(Vector3(0,l_fAngle,0)));
			}
			if( g_pMPDINode )
			{
				g_pColladaParser->m_pAllAnimationMesh->GetObject(i)->SetLocalPosition(g_pMPDINode->GetWorldTransform().GetTranslation());
			}
			g_pColladaParser->m_pAllAnimationMesh->GetObject(i)->SetLocalPosition(g_pCurve->GetCurrentPosition());
			g_pColladaParser->m_pAllAnimationMesh->GetObject(i)->Render();
		}
	}
	if( g_pToneMappingShader )
		g_pToneMappingShader->EndDraw();
}

void	SampleMouseDown(int e_iPosX,int e_iPosY)
{
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->MouseDown(e_iPosX,e_iPosY);
	}
}

void	SampleMouseMove(int e_iPosX,int e_iPosY)
{
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->MouseMove(e_iPosX,e_iPosY);
	}

}

void	SampleMouseUp(int e_iPosX,int e_iPosY)
{
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->MouseUp(e_iPosX,e_iPosY);
	}
}

void	SampleKeyup(char e_cKey)
{
	if( g_pCameraZoomFunction )
	{
		g_pCameraZoomFunction->KeyUp(e_cKey);
	}
	if( e_cKey == 'R' )
	{
		SAFE_DELETE(g_pTunnelEffect);
		g_pTunnelEffect = cTunnelEffect::CreateShader("shader/TunnelEffect.vs","shader/TunnelEffect.ps",L"MyTunnelEffecr");
	}
}
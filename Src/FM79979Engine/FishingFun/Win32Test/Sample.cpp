#include "stdafx.h"
#include "GameApp.h"

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

void	LoadSample();
void	DestoryObject();
void	SampleUpdate(float e_fElpaseTime);
void	SampleRender();

void	SampleMouseDown(int e_iPosX,int e_iPosY,int e_iZoneID);
void	SampleMouseMove(int e_iPosX,int e_iPosY,int e_iZoneID);
void	SampleMouseUp(int e_iPosX,int e_iPosY,int e_iZoneID);
void	SampleKeyup(char e_cKey);

void	LoadSample()
{
	g_pMPDIList = cGameApp::GetMPDIListByFileName(L"../MyFMBook/AnimationDemo/MPDI/startscreena01.mpdi");
	if( g_pMPDIList )
	{
		g_pMultiPathDynamicImage = g_pMPDIList->GetObject(0);
		g_pMultiPathDynamicImage->SetWorldTransform(cMatrix44::ScaleMatrix(Vector3(20,20,20)));
		g_pMultiPathDynamicImage->SetColor(Vector4(1,1,2,1));
		//ensure u have call start
		g_pMultiPathDynamicImage->Init();
		if( 0 )
		{
			g_pMultiPathDynamicImageClone = new cMultiPathDynamicImage(g_pMultiPathDynamicImage);
			//g_pMultiPathDynamicImage->SetLoop(true);
			g_pMultiPathDynamicImage->SetStayAtLastFrame(true);
		}
	}
	g_pParticleEmitterGroup = cGameApp::GetPRTG("../ParticleData/Stage1.prtg",L"Fire");
	if( g_pParticleEmitterGroup )
	{
		g_pParticleEmitterGroup->Init();
		g_pParticleEmitterGroup->SetAnimationLoop(true);
	}
	g_pPrtEmitter = cGameApp::GetPrtEmitter("../ParticleData/Smoke.prt",L"Fire");
	if( g_pPrtEmitter )
	{
		g_pPrtEmitter->Emit(Vector3(200,200,0));
		g_pPrtEmitter->SetLoop(true);
	}

	//g_pColladaParser = new cColladaParser();
	if( g_pColladaParser )
	{
		//if(g_pColladaParser->ParseDataIntoXMLNode("C:/Users/Fatming/Desktop/123.dae"))
		{
			//g_pColladaParser->
		}
	}
	g_pOrthogonalCamera = new cOrthogonalCamera(Vector2(720,1280));
}

void	DestorySampleObject()
{
	//do not delete g_pMultiPathDynamicImage,it come from g_pMPDIList
	//g_pMultiPathDynamicImageClone is a clone object so delete it.
	SAFE_DELETE(g_pMultiPathDynamicImageClone);
	//not necessary,it will auto release while m_pAnimationParser is delete
	//cGameApp::m_pAnimationParser->RemoveObject(L"fmbook_mpdi");
	SAFE_DELETE(g_pFrameCamera);
	SAFE_DELETE(g_pOrthogonalCamera);
}

void	SampleUpdate(float e_fElpaseTime)
{
	if( g_pMultiPathDynamicImage )
		g_pMultiPathDynamicImage->Update(e_fElpaseTime);
	if( g_pPrtEmitter )
		g_pPrtEmitter->Update(e_fElpaseTime);
	if( g_pParticleEmitterGroup )
		g_pParticleEmitterGroup->Update(e_fElpaseTime);
	Vector2	l_vViewPort(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height());
	if(g_pOrthogonalCamera)
		g_pOrthogonalCamera->CameraUpdateByMouse(cGameApp::m_sbMouseClickStatus[0],cGameApp::m_sbMouseClickStatus[1],cGameApp::m_sMouseWhellDelta,cGameApp::m_sScreenMousePosition.x,cGameApp::m_sScreenMousePosition.y,l_vViewPort);
}

void	SampleRender()
{
	//if(g_pOrthogonalCamera)
	//	g_pOrthogonalCamera->Render();
	if( g_pMultiPathDynamicImage )
		g_pMultiPathDynamicImage->Render();
	if( g_pPrtEmitter )
		g_pPrtEmitter->Render();
	if( g_pParticleEmitterGroup )
		g_pParticleEmitterGroup->Render();
}

void	SampleMouseDown(int e_iPosX,int e_iPosY,int e_iZoneID)
{

}

void	SampleMouseMove(int e_iPosX,int e_iPosY,int e_iZoneID)
{


}

void	SampleMouseUp(int e_iPosX,int e_iPosY,int e_iZoneID)
{

}

void	SampleKeyup(char e_cKey)
{

}
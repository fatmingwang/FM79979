#include "stdafx.h"
#include "QRCodeDecode.h"
Vector2 g_vQRCodeSize(1440/4,2560/4);

cChangeQRCodePhotoToDecode*g_pChangeQRCodePhotoToDecode = nullptr;
bool	g_bWaitRCodeResult = false;

void	QRCodeResult(JNIEnv *env, jobject obj, jstring e_strResult)
{
	std::wstring l_strResult = JavaToWchar(env, e_strResult);
	if (g_pChangeQRCodePhotoToDecode)
	{
		if (l_strResult.length())
		{
			g_pChangeQRCodePhotoToDecode->m_bSucess = true;
			g_pChangeQRCodePhotoToDecode->m_strResult = ValueToString(l_strResult);
		}
		else
		{
			g_pChangeQRCodePhotoToDecode->m_bSucess = false;
		}
	}
	g_bWaitRCodeResult = false;
}

#ifdef __cplusplus
extern "C" {
#endif


void Java_util_QRCode_GoogleMobileVisionQRCode_QRCodeResult(JNIEnv* env, jobject obj, jstring e_strResult)
{
	QRCodeResult(env, obj, e_strResult);
}

void Java_util_QRCode_ZxingQRCode_QRCodeResult(JNIEnv* env, jobject obj, jstring e_strResult)
{
	QRCodeResult(env, obj, e_strResult);
}

#ifdef __cplusplus
}
#endif

cChangeQRCodePhotoToDecode::cChangeQRCodePhotoToDecode(const char*e_strFileName)
{
	m_TimeToScan.SetTargetTime(0.2f);
	m_TimeToScan.Start();
	g_pChangeQRCodePhotoToDecode = this;
	m_pCaptureImage = nullptr;
	m_bSucess = false;
	Vector2 l_vQRCodeSize = g_vQRCodeSize;
	l_vQRCodeSize /= 2;
	m_pCameraZoomFunction = new cCameraZoomFunction(e_strFileName);
	cZoomableCamera*l_pZoomableCamera = m_pCameraZoomFunction->GetZoomableCamera();
	l_pZoomableCamera->SetViewData(Vector2(100, 100), Vector4(0,0,4096, 4096), Vector4(0, 0, 720, 1280));
	m_vDetectQRCodeWall = Vector4(cGameApp::m_svGameResolution.x / 2 - l_vQRCodeSize.x, cGameApp::m_svGameResolution.y - l_vQRCodeSize.y, cGameApp::m_svGameResolution.x / 2 + l_vQRCodeSize.x, cGameApp::m_svGameResolution.y + l_vQRCodeSize.y);
	cGameApp::m_sTimeAndFPS.Update();
}

cChangeQRCodePhotoToDecode::~cChangeQRCodePhotoToDecode()
{
	SAFE_DELETE(m_pCameraZoomFunction);
	SAFE_DELETE(m_pCaptureImage);
}

bool	cChangeQRCodePhotoToDecode::DoDecodeByPixelreadQRCode()
{
	if (g_bWaitRCodeResult)
		return false;
	g_bWaitRCodeResult = true;
	float	l_fWidthScale = cGameApp::m_svDeviceViewPortSize.Width() / cGameApp::m_svGameResolution.x;
	float	l_fHeightScale = cGameApp::m_svDeviceViewPortSize.Height() / cGameApp::m_svGameResolution.y;
	int		l_iCaptureWidth = (int)(l_fWidthScale*g_vQRCodeSize.x);
	int		l_iCaptureHeight = (int)(l_fHeightScale*g_vQRCodeSize.y);
	int l_iCutPosX = cGameApp::m_svDeviceViewPortSize.z / 2 - l_iCaptureWidth / 2;
	int l_iCutPosY = cGameApp::m_svDeviceViewPortSize.w / 2 - l_iCaptureHeight / 2;
	int l_iPixelSize = l_iCaptureWidth*l_iCaptureHeight * 4;
	GLubyte*l_pBuffer = (GLubyte *)malloc(l_iPixelSize);
	glReadPixels(l_iCutPosX, l_iCutPosY, l_iCaptureWidth, l_iCaptureHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_pBuffer);
	unsigned char *l_pPixelData = l_pBuffer;
	bool l_bDoUVChange = false;
	if (l_bDoUVChange)
	{
		unsigned char *l_pPixelData2 = new unsigned char[l_iCaptureWidth * 4];
		int l_iHalfHeight = l_iCaptureHeight / 2;
		for (int i = 0; i < l_iHalfHeight; ++i)
		{
			int l_iIndex1 = 4 * l_iCaptureWidth*(l_iCaptureHeight - i - 1);
			int l_iIndex2 = 4 * l_iCaptureWidth*i;
			memcpy(l_pPixelData2, &l_pPixelData[l_iIndex1], sizeof(unsigned char) * 4 * l_iCaptureWidth);
			memcpy(&l_pPixelData[l_iIndex1], &l_pPixelData[l_iIndex2], sizeof(unsigned char) * 4 * l_iCaptureWidth);
			memcpy(&l_pPixelData[l_iIndex2], l_pPixelData2, sizeof(unsigned char) * 4 * l_iCaptureWidth);
		}
		delete l_pPixelData2;
	}
	int l_iNumInt = l_iPixelSize/4;
	jintArray l_pIntArray = cGameApp::m_spThreadEnv->NewIntArray(l_iNumInt);
	cGameApp::m_spThreadEnv->SetIntArrayRegion(l_pIntArray, 0, l_iNumInt, reinterpret_cast<jint*>(l_pBuffer));
	bool l_bUseZxing = false;
	char*l_strClassName = nullptr;
	char*l_strMethodID = nullptr;
	if (l_bUseZxing)
	{
		l_strClassName = "util/QRCode/ZxingQRCode";
		l_strMethodID = "ZxingDecodeByPixelreadQRCode";
	}
	else
	{
		l_strClassName = "util/QRCode/GoogleMobileVisionQRCode";
		l_strMethodID = "DecodeQRCodePixelsWithGoogleVisionWithThread";
	}
	bool l_bCallResult = CallStaticVoidMethod4(l_strClassName, l_strMethodID, "([III)V", l_pIntArray, (void*)l_iCaptureWidth, (void*)l_iCaptureHeight,g_pMainThreadJNIUtilData,true);
	SAFE_DELETE(m_pCaptureImage);
	m_pCaptureImage = new cUIImage(L"", (char*)l_pBuffer, l_iCaptureWidth, l_iCaptureHeight, GL_RGBA);
	m_pCaptureImage->SetWidth(100);
	m_pCaptureImage->SetHeight(100* g_vQRCodeSize.y / g_vQRCodeSize.x);
	//float l_fUV[4] = { 0,1,1,0 };
	//m_pCaptureImage->SetUV(l_fUV);
	cGameApp::m_spThreadEnv->DeleteLocalRef(l_pIntArray);
	//free(l_pBuffer);
	return true;
}

void cChangeQRCodePhotoToDecode::Update(float e_fElpaseTime)
{
	if (m_pCameraZoomFunction)
	{
		m_pCameraZoomFunction->Update(e_fElpaseTime);
		static float l_fAngleX = 0.f;
		static float l_fAngleY = 0.f;
		static float l_fAngleZ = 0.f;
		l_fAngleX += frand(0.01, 0.1);
		l_fAngleY += frand(0.01, 0.1);
		l_fAngleZ += frand(0.01, 0.1);
		if (l_fAngleX >= 10.f)
			l_fAngleX = -10.f;
		if (l_fAngleY >= 10.f)
			l_fAngleY = -10.f;
		if (l_fAngleZ >= 5.f)
			l_fAngleZ = -5.f;
		auto l_pImage = m_pCameraZoomFunction->GetBaseImage();
		if(l_pImage)
				l_pImage->SetLocalTransform(cMatrix44::RotationMatrix(Vector3(l_fAngleX, l_fAngleY, l_fAngleZ)));
	}
	if (!m_bSucess)
	{
		m_TimeToScan.Update(e_fElpaseTime);
		if (m_TimeToScan.bTragetTimrReached)
		{
			m_TimeToScan.Start();
			DoDecodeByPixelreadQRCode();
		}
	}
}

void cChangeQRCodePhotoToDecode::Render()
{
	if (m_pCameraZoomFunction)
	{
		m_pCameraZoomFunction->Render();
	}
	glEnable2D(cGameApp::m_svGameResolution.x, cGameApp::m_svGameResolution.y);
	GLRender::RenderRectangle(g_vQRCodeSize.x, g_vQRCodeSize.y, cMatrix44::TranslationMatrix(Vector3(cGameApp::m_svGameResolution.x / 2- g_vQRCodeSize.x/2, cGameApp::m_svGameResolution.y / 2 - g_vQRCodeSize.y / 2, 0)), Vector4::Green);
	if (m_bSucess)
	{

	}
	if (m_pCaptureImage)
	{
		m_pCaptureImage->Render();
	}
}

void cChangeQRCodePhotoToDecode::MouseDown(int e_iPosX, int e_iPosY)
{
	m_TimeToScan.Start();
	if (e_iPosX > 600 && e_iPosY > 1000)
	{
		m_bSucess = true;
	}
}
#include "stdafx.h"
#include "CameraShake.h"

//template<class TYPE = int> class  cTtt
//{
//public:
//	TYPE aa;
//};
//
//class  cTtt0 :public cTtt<>{};
//class  cTtt2:public cTtt<double>{};
//class  cTtt3 :public cTtt<float>{};
//void Testwe87f4f56d()
//{
//	cTtt0 l_;
//	cTtt2 l_1;
//	cTtt3 l_2;
//}

TYPDE_DEFINE_MARCO(sCameraShakeParameter);
TYPDE_DEFINE_MARCO(cCameraShake);

sCameraShakeParameter::sCameraShakeParameter(TiXmlElement*e_pTiXmlElement)
{

}

sCameraShakeParameter::sCameraShakeParameter(float e_fStrength, float e_fDuration, bool e_bDoRotation)
{
	fStrength = e_fStrength;
	fDuration = e_fDuration;
	bDoRotation = e_bDoRotation;
}

cCameraShake::cCameraShake()
{	
	m_bShakeCamera = false;
	//UT::sTimeCounter	RestTimeTC;
	//Vector3				m_vRotation;
	//Vector3				m_vPos;
}
cCameraShake::~cCameraShake()
{

}

bool	cCameraShake::ProcessCameraShakeParameter(TiXmlElement*e_pRoot)
{
	return true;
}

bool	cCameraShake::MyParse(TiXmlElement*e_pRoot)
{
	return true;
}

void	cCameraShake::StartCameraShake(void*e_pData)
{

}

//
void	cCameraShake::EndCameraShake(void*e_pData)
{

}

void	cCameraShake::StartWithParameter(float e_fStrength, float e_fDuration, bool e_bDoRotation)
{

}


void	cCameraShake::Update()
{

}

void	cCameraShake::Render()
{

}

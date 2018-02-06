#pragma once

class cBaseScene;
struct sCameraShakeParameter:public NamedTypedObject
{
	float	fStrength;
	float	fDuration;
	bool	bDoRotation;
	sCameraShakeParameter(TiXmlElement*e_pTiXmlElement);
	sCameraShakeParameter(float e_fStrength, float e_fDuration, bool e_bDoRotation);
	virtual ~sCameraShakeParameter(){}
	DEFINE_TYPE_INFO();
};
//shake
class cCameraShake :public cMessageSender,public cNamedTypedObjectVector<sCameraShakeParameter>,public cNodeISAX
{
	bool				ProcessCameraShakeParameter(TiXmlElement*e_pRoot);
	virtual	bool		MyParse(TiXmlElement*e_pRoot)override ;
	//
	bool				m_bShakeCamera;
	UT::sTimeCounter	RestTimeTC;
	Vector3				m_vRotation;
	Vector3				m_vPos;
	void				StartCameraShake(void*e_pData);
	//
	void				EndCameraShake(void*e_pData);
	void				StartWithParameter(float e_fStrength, float e_fDuration, bool e_bDoRotation);
public:
	cCameraShake();
	virtual ~cCameraShake();
	DEFINE_TYPE_INFO();
	void	Update();
	void	Render();
};
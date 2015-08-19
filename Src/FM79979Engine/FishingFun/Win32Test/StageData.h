#ifndef	_STAGE_DATA_H_
#define	_STAGE_DATA_H_

class	cFishingRodCamera;
class	cFishBehaviorManager;
class	cFishThread;
class	cFishRodData;
class	cHook;
class	cBrakeImage;
class	cFishShowProbability;
class	cFishBehavior;

class	cStageData:public ISAXCallback
{
	cUVAnimationImage*		m_pUVAnimationImage;
	//cMPDI*					m_pBGMPDI;
	std::vector<wstring>	m_strSelectableFishThreadNameVector;
	std::string				m_strStageFileName;
	bool					m_bQuickLeave;
	//
	cImageButton*			m_pQuickLeaveButton;
	float					m_fQuickLeaveSpeed;
	cBrakeImage*			m_pBrakeImage;
	cImageButton*			m_pFishDataButton;
	bool					m_bActiveRenderInfo;
	//

	//
	cHook*					m_pHook;
	cFishRodData*			m_pFishRodData;
	cFishThread*			m_pFishThread;
	cFishBehaviorManager*	m_pFishBehaviorManager;
	cFishingRodCamera*		m_pFishingRodCamera;
	cFishShowProbability*	m_pFishShowProbability;
	void					DoCollideUpdate();
	virtual	void			HandleElementData(TiXmlElement*e_pTiXmlElement);
public:
	//input from select fish tools phase
	cStageData(cFishRodData*e_pFishRodData,cFishThread*e_pFishThread);
	~cStageData();
	void	Init(const char*e_strStageFileName);
	void	Update(float e_fElpaseTime);
	void	Render();
	void	DebugRender();
	void	Destroy();
	void	MouseDown(int e_iPosX,int e_iPosY);
	void	MouseMove(int e_iPosX,int e_iPosY);
	void	MouseUp(int e_iPosX,int e_iPosY);
	bool	IsStageClear();
	//
	cNamedTypedObjectVector<cFishBehavior>*GetBeenHookedFishes();
};


class	cHook
{
	float						m_fAxisMinValue;
	float						m_fNextLevelStep;
	float						m_fRiseSpeed;
	float						m_fMovingSpeed;
	friend	class				cStageData;
	cbtShapeCollision*			m_pbtShapeCollision;
	cRenderObject*		m_pHookImage;
	Vector3						m_vMovingPos;
public:
	cHook(TiXmlElement*e_pElement);
	~cHook();
	void	Update(float e_fElpaseTime,cOrthogonalCamera*e_pOrthogonalCamera);
	void	Render();
	void	DebugRender();
	Frame*	GetFrame();
};
//<BrakeData Time="" SlowSpeed="" >
//	<cImageButton cMPDIList="" cMPDI="" cSubMPDI=""/>
//</BrakeData>
class	cBrakeImage
{
	cImageButton*		m_pButton;
	UT::sTimeCounter	m_PowerTimeTC;
	float				m_fBrakeSpeed;
	//
	bool				m_bWorking;
public:
	cBrakeImage(TiXmlElement*e_pImageButtonXML,float e_fPowerTime,float e_fBrakeSpeed);
	~cBrakeImage();
	void				Update(float e_fElpaseTime);
	void				Render();
	bool				IsWorking(){return m_bWorking;}
	float				GetBrakeSpeed(){return m_fBrakeSpeed;}
};

#endif
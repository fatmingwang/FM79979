#pragma once

#include "../../../Core/AllCoreInclude.h"
#include "../../../Core/Bluetooth/Bluetooth.h"
#include "../../../FishGame/Game/UseBluetoothDoCodeReport/UseBluetoothDoCodeReportParameter.h"
#include "../../../FishGame/FishGameParameterize/FishGameParameterize.h"
#include "../../../FishGame/FishGameParameterize/FishGameParameterObject.h"

using namespace FISG_GAME_PARAMETERIZE;
using namespace FATMING_CORE;

class FATMING_CORE::cBluetoothSinglton;
class cChangeQRCodePhotoToDecode;
class cCodeReport :public cRenderObject
{
	std::string							m_strQRCode;
	cChangeQRCodePhotoToDecode*			m_pChangeQRCodePhotoToDecode;
	//
	cMPDI*								m_pBG;
	cRenderObject						m_UINode;
	bool								m_bWaitServerRequestReportResult;
	FATMING_CORE::cBluetoothSinglton*	m_pBluetoothSinglton;
	//
	sFiGameReporterParameterManager		m_FiGameReporterParameterManager;
	sReportInfo							m_ReportInfo;
	bool								m_bReportResult;
	sReportInfo							m_ReportSuccessInfo;
	cClickBehaviorDispatcher			m_ClickBehaviorDispatcher;
	void								ProcessMessage(sBaseMessageInfo*e_pBaseMessageInfo,int e_iPacketSize);
public:
	cCodeReport();
	~cCodeReport();
	virtual void	Init()override;
	virtual void	Update(float e_fElpaseTime)override;
	virtual void	Render()override;
	void			MouseDown(int e_iPosX, int e_iPosY);
	void			MouseMove(int e_iPosX, int e_iPosY);
	void			MouseUp(int e_iPosX, int e_iPosY);
	void			CreateChangeQRCodePhotoToDecode(const char*e_strFileName);
};
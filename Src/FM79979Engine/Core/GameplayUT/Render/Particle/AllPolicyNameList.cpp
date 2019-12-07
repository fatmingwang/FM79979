#include "AllPolicyNameList.h"
#include "ParticleBase.h"
#include "PrtColor.h"
#include "PrtLife.h"
#include "PrtRotate.h"
#include "PrtSize.h"
#include "PrtStartPosition.h"
#include "PrtVelocity.h"
#include "PrtTexture.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
namespace FATMING_CORE
{
	//all policy name
	const char*	cPrtAllClassName::m_strAllParticlePolicy[] = 
	{
		"Color",
		"Life",
		"Rotate",
		"Size",
		"StartPosition",
		"Velocity",
		"Texture",
		0
	};
	//color
	const wchar_t*	cPrtAllClassName::m_sstrColorInitNameList[] = {cPrtColorInitrSetColor::TypeID,cPrtColorInitrSetRandomColor::TypeID,0};
	const wchar_t*	cPrtAllClassName::m_sstrColorActNameList[] = {cPrtColorActBlending::TypeID,cPrtColorActBlendingByLife::TypeID,cPrtColorActBlendingBy2Color::TypeID,0};
	//life
	const wchar_t*	cPrtAllClassName::m_sstrLifeInitNameList[] = {cPrtLifeInitrSetLife::TypeID,0};
	const wchar_t*	cPrtAllClassName::m_sstrLifeActNameList[] = {cPrtLifeActDyingByGameTime::TypeID,0};
	//rotate
	const wchar_t*	cPrtAllClassName::m_sstrRotateInitNameList[] = {cPrtRotateInitRotate::TypeID,0};
	const wchar_t*	cPrtAllClassName::m_sstrRotateActNameList[] = {cPrtRotateActRotate::TypeID,0};
	//size
	const wchar_t*	cPrtAllClassName::m_sstrSizeInitNameList[] = {cPrtSizeInitSetSize::TypeID,0};
	const wchar_t*	cPrtAllClassName::m_sstrSizeActNameList[] = {cPrtSizeActBlending::TypeID,0};
	//start position
	const wchar_t*	cPrtAllClassName::m_sstrStartPositionInitNameList[] = {cPrtStartPositionInitBySquareRange::TypeID,cPrtStartPositionInitByFrame::TypeID,0};
	const wchar_t*	cPrtAllClassName::m_sstrStartPositionActNameList[] = {0};
	//velocity
	const wchar_t*	cPrtAllClassName::m_sstrVelocityInitNameList[] = { /*cPrtVelocityInitRandomFly::TypeID,cPrtVelocityInitSetVelocity::TypeID,*/0};
	const wchar_t*	cPrtAllClassName::m_sstrVelocityActNameList[] = {cPrtVelocityActBySatelliteAction::TypeID,cPrtVelocityActAcceleration::TypeID,cPrtVelocityActDircctionChange::TypeID,0};
	//texture
	const wchar_t*	cPrtAllClassName::m_sstrTextureInitNameList[] = {0};
	const wchar_t*	cPrtAllClassName::m_sstrTextureActNameList[] = {cPrtTextureActDynamicTexture::TypeID,0};
	//all
	const wchar_t**	cPrtAllClassName::m_sstrAllInitNameList[]=
	{
		cPrtAllClassName::m_sstrColorInitNameList,
		cPrtAllClassName::m_sstrLifeInitNameList,
		cPrtAllClassName::m_sstrRotateInitNameList,
		cPrtAllClassName::m_sstrSizeInitNameList,
		cPrtAllClassName::m_sstrStartPositionInitNameList,
		cPrtAllClassName::m_sstrVelocityInitNameList,
		cPrtAllClassName::m_sstrTextureInitNameList,
		0
	};
	const wchar_t**	cPrtAllClassName::m_sstrAllActNameList[]=
	{
		cPrtAllClassName::m_sstrColorActNameList,
		cPrtAllClassName::m_sstrLifeActNameList,
		cPrtAllClassName::m_sstrRotateActNameList,
		cPrtAllClassName::m_sstrSizeActNameList,
		cPrtAllClassName::m_sstrStartPositionActNameList,
		cPrtAllClassName::m_sstrVelocityActNameList,
		cPrtAllClassName::m_sstrTextureActNameList,
		0
	};
}
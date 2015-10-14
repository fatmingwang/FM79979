#include "StdAfx.h"
#include "MoveObject.h"

//<MovingObject Range="0,0,100,100" Speed="" Gesture="" DelayToGo="">
//	<MovingObjectImage MPDIList="" MPDI="" />
//</MovingObject>

sMovingObject::sMovingObject(TiXmlElement*e_pTiXmlElement)
{
	vMovingOrientation = Vector3::One;
	vRestrict = Vector4(100,100,600,200);
	Data.fSpeed = 100.f;
	Data.fDelayTimeToGo = 1.f;
	assert(wcscmp(e_pTiXmlElement->Value(),L"MovingObject") == 0 );
	pMovingObjectImage = 0;

	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Range")
		{
			vRestrict = VALUE_TO_VECTOR4;
		}
		else
		COMPARE_NAME("Speed")
		{
			Data.fSpeed = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("Gesture")
		{
			MovingOrientation = GetGesture(l_strValue);
		}
		else
		COMPARE_NAME("DelayToGo")
		{
			Data.fDelayTimeToGo = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("CloseToMinusSpeed")
		{
			Data.fCloseToMinusSpeed = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
	TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	if( l_pTiXmlElement )
	{
		const WCHAR*l_strMPDIList = l_pTiXmlElement->Attribute(L"MPDIList");
		if( l_strMPDIList )
		{
			cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIList);
			const WCHAR*l_strMPDI = l_pTiXmlElement->Attribute(L"MPDI");
			if( l_strMPDI )
			{
				assert(l_pMPDIList->GetObject(l_strMPDI)&&"sMovingObject l_pMPDIList->GetObject(l_strMPDI) failed");
				pMovingObjectImage = new cMPDI(l_pMPDIList->GetObject(l_strMPDI));
			}
		}
	}
}

sMovingObject::~sMovingObject()
{
	SAFE_DELETE(pMovingObjectImage);
}

void	sMovingObject::Init(sData e_Data)
{
	assert(e_Data.DistanceRandPos.Min>=0.f && e_Data.DistanceRandPos.Max<=1.f);
	Data = e_Data;
	Vector3	l_vShowPos;
	l_vShowPos.z = 0.f;
	float	l_fWidth = vRestrict.Width();
	float	l_fHeight = vRestrict.Height();
	vMovingOrientation = Vector2::Zero;
	if( pMovingObjectImage )
	{
		pMovingObjectImage->Init();
		float	l_fRandValue = Data.DistanceRandPos.Rand();
		l_fWidth	= l_fRandValue*l_fWidth;
		l_fHeight	= l_fRandValue*l_fHeight;
		switch( MovingOrientation  )
		{
			case eGestureEnum::eGE_SLIDE_LEFT:
				vMovingOrientation.x = -Data.fSpeed;
				l_vShowPos.x = vRestrict.z-l_fWidth;
				l_vShowPos.y = vRestrict.y;
				break;
			case eGestureEnum::eGE_SLIDE_UP:
				vMovingOrientation.y = -Data.fSpeed;
				l_vShowPos.x = vRestrict.x;
				l_vShowPos.y = vRestrict.w-l_fHeight;
				break;
			case eGestureEnum::eGE_SLIDE_RIGHT:
				vMovingOrientation.x = Data.fSpeed;
				l_vShowPos.x = vRestrict.x+l_fWidth;
				l_vShowPos.y = vRestrict.y;
				break;
			case eGestureEnum::eGE_SLIDE_DOWN:
				vMovingOrientation.y = Data.fSpeed;
				l_vShowPos.x = vRestrict.x;
				l_vShowPos.y = vRestrict.y+l_fHeight;
				break;
			default:
				assert(0 && "MovingOrientation is not legal" );
				break;
		}
		pMovingObjectImage->SetPos(l_vShowPos);
	}
}

void	sMovingObject::Update(float e_fElpaseTime)
{
	if( pMovingObjectImage )
	{
		Vector3	l_vCurrentPos = pMovingObjectImage->GetPos();
		l_vCurrentPos += vMovingOrientation*e_fElpaseTime;
		switch( MovingOrientation  )
		{
			case eGestureEnum::eGE_SLIDE_LEFT:
				if( l_vCurrentPos.x <= this->vRestrict.x )
				{
					float	l_fOffset = this->vRestrict.x-l_vCurrentPos.x;
					l_vCurrentPos.x = this->vRestrict.x+l_fOffset;
					MovingOrientation = eGestureEnum::eGE_SLIDE_RIGHT;
					vMovingOrientation.x *= -1;
				}
				break;
			case eGestureEnum::eGE_SLIDE_UP:
				if( l_vCurrentPos.y <= this->vRestrict.y )
				{
					float	l_fOffset = this->vRestrict.y-l_vCurrentPos.y;
					l_vCurrentPos.y = this->vRestrict.y+l_fOffset;
					MovingOrientation = eGestureEnum::eGE_SLIDE_DOWN;
					vMovingOrientation.y *= -1;
				}
				break;
			case eGestureEnum::eGE_SLIDE_RIGHT:
				if( l_vCurrentPos.x >= this->vRestrict.z )
				{
					float	l_fOffset = l_vCurrentPos.x-this->vRestrict.z;
					l_vCurrentPos.x = this->vRestrict.z-l_fOffset;
					MovingOrientation = eGestureEnum::eGE_SLIDE_LEFT;
					vMovingOrientation.x *= -1;
				}
				break;
			case eGestureEnum::eGE_SLIDE_DOWN:
				if( l_vCurrentPos.y >= this->vRestrict.w )
				{
					float	l_fOffset = l_vCurrentPos.y-this->vRestrict.w;
					l_vCurrentPos.x = this->vRestrict.w-l_fOffset;
					MovingOrientation = eGestureEnum::eGE_SLIDE_UP;
					vMovingOrientation.y *= -1;
				}
				break;
			default:
				assert(0 && "MovingOrientation is not legal" );
				break;
		}
		pMovingObjectImage->SetPos(l_vCurrentPos);
	}
}

void	sMovingObject::Render()
{
	if(pMovingObjectImage)
	{
		pMovingObjectImage->Render();
	}
}

//<TwoMovingObjectController FrameMPDIList="" FrameMPDI="">
//	<MovingObject Range="0,0,100,100" Speed="" Gesture="" DelayToGo="">
//		<MovingObjectImage MPDIList="" MPDI="" />
//	</MovingObject>
//	<MovingObject Range="0,0,100,100" Speed="" Gesture="" DelayToGo="">
//		<MovingObjectImage MPDIList="" MPDI="" />
//	</MovingObject>
//	<LVData LV="1" Speed="30" DelayToGo="" CloseToMinusSpeed="10" />
//</TwoMovingObjectController>
sTwoMovingObjectController::sTwoMovingObjectController(TiXmlElement*e_pTiXmlElement)
{
	m_pHorverSound = 0;
	pFrameImage = 0;
	pMovingObject[0] = pMovingObject[1] = 0;

	cMPDIList*l_pMPDILIST = 0;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("FrameMPDIList")
		{
			l_pMPDILIST = cGameApp::GetMPDIListByFileName(l_strValue);
		}
		else
		COMPARE_NAME("FrameMPDI")
		{
			if( l_pMPDILIST )
			{
				cMPDI*l_pMPDI = l_pMPDILIST->GetObject(l_strValue);
				pFrameImage = new cMPDI(l_pMPDI);
			}
		}
	PARSE_NAME_VALUE_END

	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	int	l_iIndex = 0;
	while( e_pTiXmlElement )
	{
		if( !wcscmp( e_pTiXmlElement->Value(),L"MovingObject") )
		{
			assert( l_iIndex<2 );
			pMovingObject[l_iIndex] = new sMovingObject(e_pTiXmlElement);
			++l_iIndex;
		}
		else
		if( !wcscmp( e_pTiXmlElement->Value(),L"LVData") )
		{
			sLVData	l_LVData = GetLVData(e_pTiXmlElement);
			LVDataList.push_back(l_LVData);
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}

sTwoMovingObjectController::~sTwoMovingObjectController()
{

	SAFE_DELETE(pMovingObject[0]);
	SAFE_DELETE(pMovingObject[1]);
	SAFE_DELETE(pFrameImage);
}
//<LVData LV="1">
//	<Object Speed="200" DelayToGo="1" CloseToMinusSpeed="10" />
//	<Object Speed="200" DelayToGo="1" CloseToMinusSpeed="10" />
//</LVData>
sTwoMovingObjectController::sLVData	sTwoMovingObjectController::GetLVData(TiXmlElement*e_pElement)
{
	sLVData	l_LVData;
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("LV")
		{
			int	l_iLV = VALUE_TO_INT;
		}
		COMPARE_NAME("CollectEnergyCount")
		{
			l_LVData.iCollectEnergy = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
	e_pElement = e_pElement->FirstChildElement();
	int	l_iIndex = 0;
	while(e_pElement)
	{
		sMovingObject::sData	l_Data;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("Speed")
			{
				l_Data.fSpeed = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("DelayToGo")
			{
				l_Data.fDelayTimeToGo = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("CloseToMinusSpeed")
			{
				l_Data.fCloseToMinusSpeed = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("DistanceRandPos")
			{
				l_Data.DistanceRandPos = VALUE_TO_VECTOR2;
			}
		PARSE_NAME_VALUE_END
		l_LVData.Data[l_iIndex] = l_Data;
		e_pElement = e_pElement->NextSiblingElement();
		++l_iIndex;
	}
	return l_LVData;
}

void	sTwoMovingObjectController::Init(int e_iLVIndex)
{
	assert( (int)LVDataList.size() > e_iLVIndex );
	assert( e_iLVIndex >-1 );
	sLVData	l_LVData = this->LVDataList[e_iLVIndex];
	sMovingObject::sData l_Data1 = l_LVData.Data[0];
	sMovingObject::sData l_Data2 = l_LVData.Data[1];
	if( pFrameImage )
		pFrameImage->Init();
	if( pMovingObject[0] )
		pMovingObject[0]->Init(l_Data1);
	if( pMovingObject[1] )
		pMovingObject[1]->Init(l_Data2);
	fTwoObjectCloseValue = 0.f;
	bIsDone = false;
	bTouchInVaildArea = false;
	KeyToCollide.Init();
}
void	sTwoMovingObjectController::Update(float e_fElpaseTime)
{
	if(pFrameImage)
		pFrameImage->Update(e_fElpaseTime);
	if( pMovingObject[0] )
		pMovingObject[0]->Update(e_fElpaseTime);
	if( pMovingObject[1] )
		pMovingObject[1]->Update(e_fElpaseTime);
	KeyToCollide.SingnalProcess();
	if( KeyToCollide.IsSatisfiedCondition() )
	{
		MouseUp((int)pFrameImage->GetPos().x,(int)pFrameImage->GetPos().y);
	}
}

void	sTwoMovingObjectController::Render()
{
	if(pFrameImage)
		pFrameImage->Render();
	if(pMovingObject[0])
		pMovingObject[0]->Render();
	if(pMovingObject[1])
		pMovingObject[1]->Render();
}

bool	sTwoMovingObjectController::IsCollide(int e_iPosX,int e_iPosY)
{
	if(this->pFrameImage->Collide(e_iPosX,e_iPosY))
	{
		return pMovingObject[0]->pMovingObjectImage->Collide(pMovingObject[1]->pMovingObjectImage);
	}
	return false;
}

bool	sTwoMovingObjectController::IsCollide()
{
	return pMovingObject[0]->pMovingObjectImage->Collide(pMovingObject[0]->pMovingObjectImage);
}

void	sTwoMovingObjectController::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pHorverSound )
		m_pHorverSound->Play(true);
	if(this->pFrameImage->Collide(e_iPosX,e_iPosY))
	{
		bTouchInVaildArea = true;
	}
	else
		bTouchInVaildArea = false;
}

void	sTwoMovingObjectController::MouseMove(int e_iPosX,int e_iPosY)
{

}

void	sTwoMovingObjectController::MouseUp(int e_iPosX,int e_iPosY)
{
	if( !bTouchInVaildArea )
		return;
	if( IsCollide(e_iPosX,e_iPosY) )
	{
		//find out how close they were and give power value
		Vector3	l_vDis = pMovingObject[0]->pMovingObjectImage->GetPos()-pMovingObject[1]->pMovingObjectImage->GetPos();
		float	l_fLength = l_vDis.Length();
		Vector2 l_vSize = pMovingObject[0]->pMovingObjectImage->GetDrawSize()+pMovingObject[1]->pMovingObjectImage->GetDrawSize();
		l_vSize /= 2.f;
		float	l_fObjectMaxLength = l_vSize.Length();
		l_fLength = l_fObjectMaxLength - l_fLength;
		fTwoObjectCloseValue = l_fLength/l_fObjectMaxLength;
	}
	else
	{
		fTwoObjectCloseValue = 0.01f;
	}
	bIsDone = true;
	if( m_pHorverSound )
		m_pHorverSound->Play(false);
}

void	sTwoMovingObjectController::MouseLeave(int e_iPosX,int e_iPosY)
{
	if( m_pHorverSound )
		m_pHorverSound->Play(false);
}

void	sTwoMovingObjectController::DebugRender()
{
	pMovingObject[0]->pMovingObjectImage->RenderCollide();
	pMovingObject[1]->pMovingObjectImage->RenderCollide();
	this->pFrameImage->RenderCollide();
}
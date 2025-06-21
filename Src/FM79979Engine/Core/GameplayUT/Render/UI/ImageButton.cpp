#include "ImageButton.h"
#include "../../GameApp/GameApp.h"
#include "../../ResourceMaintaner/ObjectXMLParser.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cImageButton);
	TYPDE_DEFINE_MARCO(cButtonDialog);
	Vector4	g_vImageButtonClickedColor(0.5f,0.5f,0.5f,1);
	Vector4	g_vDisableColor(0.5f,0.5f,0.5f,0.3f);
	cImageButton::cImageButton():cBaseImage((char*)0)
	{
		m_vOriginalColor = Vector4::One;
		m_vPressedColor = g_vImageButtonClickedColor;
		m_pPressedImage = 0;
		m_pFinishImage = 0;	
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
	}

	cImageButton::cImageButton(cPuzzleImageUnit*e_pImage,Vector3 e_vPos):cBaseImage(e_pImage)
	{
		m_vOriginalColor = *e_pImage->GetColor();
		m_vPressedColor = g_vImageButtonClickedColor;
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		Frame::SetLocalPosition(e_vPos);
	}

	cImageButton::cImageButton(cBaseImage*e_pImage,Vector3 e_vPos):cBaseImage(e_pImage)
	{
		m_vOriginalColor = *e_pImage->GetColor();
		m_vPressedColor = g_vImageButtonClickedColor;
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		Frame::SetLocalPosition(e_vPos);
		//Vector2	l_vLeftPos(this->m_OffsetPos.x+e_vPos.x,this->m_OffsetPos.y+e_vPos.y);
		//m_vCollisionRange = Vector4(l_vLeftPos.x,l_vLeftPos.y,(float)m_iWidth+l_vLeftPos.x,(float)m_iHeight+l_vLeftPos.y);
	}

	cImageButton::cImageButton(cImageButton*e_pImageButton):cClickMouseBehavior(e_pImageButton),cBaseImage(e_pImageButton)
	{
		m_vOriginalColor = e_pImageButton->m_vOriginalColor;
		m_vPressedColor = e_pImageButton->m_vPressedColor;
		m_strButtonClickSoundName = e_pImageButton->m_strButtonClickSoundName;
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		if( e_pImageButton->m_pPressedImage )
		{
			cRenderObject*l_pRenderObject = dynamic_cast<cRenderObject*>(e_pImageButton->m_pPressedImage->Clone());
			m_pPressedImage = l_pRenderObject;
		}
		if( e_pImageButton->m_pFinishImage )
		{
			cRenderObject*l_pRenderObject = dynamic_cast<cRenderObject*>(e_pImageButton->m_pFinishImage->Clone());
			m_pFinishImage = l_pRenderObject;
		}
		m_TC.SetTargetTime(e_pImageButton->m_TC.fTargetTime);
		Frame::SetLocalPosition(e_pImageButton->GetPos());
		//this->SetParent(e_pImageButton->GetParent());
	}

	void	SubMPDIAssignData(cSubMPDI*e_pSubMPDI,cImageButton*e_pImageButton)
	{
		e_pSubMPDI->Init();
		e_pSubMPDI->Update(0.000001f);
		Vector3	l_vPos = e_pSubMPDI->GetOriginalPointList()[0];
		Vector4	l_vCollisionRange;
		float l_fTempBufferForRenderVertices[18];
		if(e_pSubMPDI->GetTransformedVerticesByIndex(l_fTempBufferForRenderVertices,nullptr, nullptr,0))
		{
			bool l_bImageZRotate90ToSaveMemory = false;
			Vector3	l_vAngle = e_pSubMPDI->GetPointData(0)->vAngle;
			e_pImageButton->SetAngle(l_vAngle);
			//left up
			l_vCollisionRange.x = l_fTempBufferForRenderVertices[6];
			l_vCollisionRange.y = l_fTempBufferForRenderVertices[7];
			//right down
			l_vCollisionRange.z = l_fTempBufferForRenderVertices[12];
			l_vCollisionRange.w = l_fTempBufferForRenderVertices[13];
			if( l_vCollisionRange.x > l_vCollisionRange.z )
			{//set x as left,the axis has been rotated
				float	l_fPosX = l_vCollisionRange.z;
				l_vCollisionRange.z = l_vCollisionRange.x;
				l_vCollisionRange.x = l_fPosX;
			}
			if( l_vCollisionRange.x > l_vCollisionRange.z )
			{//set y as top,the axis has been rotated
				float	l_fPosY = l_vCollisionRange.w;
				l_vCollisionRange.w = l_vCollisionRange.y;
				l_vCollisionRange.y = l_fPosY;
			}
			//why I wrote this?
			//l_vPos = e_pSubMPDI->GetPointList()[0];
			l_vPos.x = l_vCollisionRange.x;
			l_vPos.y = l_vCollisionRange.y;
			e_pImageButton->SetPos(l_vPos);
		}
		else
		{
			POINT l_OriginalSize = e_pImageButton->GetOriginalSize();
			l_vPos -= e_pSubMPDI->GetPointData(0)->Size/2.f;
			e_pImageButton->SetPos(l_vPos);
			l_vCollisionRange = Vector4(l_vPos.x,l_vPos.y,l_vPos.x+l_OriginalSize.x,l_vPos.y+l_OriginalSize.y);
		}
		POINT	l_OffsetPos = {0,0};
		Vector2	l_vSize = e_pSubMPDI->GetPointData(0)->Size;
		POINT	l_OriginalSize = {(int)l_vSize.x,(int)l_vSize.y};
		e_pImageButton->SetOffsetPos(l_OffsetPos);
		e_pImageButton->SetWidth(l_OriginalSize.x);
		e_pImageButton->SetHeight(l_OriginalSize.y);
		e_pImageButton->SetOriginalSize(l_OriginalSize);
		e_pImageButton->SetCollisionRange(l_vCollisionRange);
	}

	cImageButton::cImageButton(cSubMPDI*e_pSubMPDI):cBaseImage(e_pSubMPDI->GetPointData(0)->GetPuzzleImageUnit())
	{
		this->SetName(e_pSubMPDI->GetName());
		m_vOriginalColor = Vector4::One;
		m_vPressedColor = g_vImageButtonClickedColor;
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		SubMPDIAssignData(e_pSubMPDI,this);
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
	}

	cImageButton::cImageButton(cMPDI*e_pMPDI):cBaseImage(e_pMPDI->GetObject(0)->GetPointData(0)->GetPuzzleImageUnit())
	{
		this->SetName(e_pMPDI->GetName());
		m_vOriginalColor = Vector4::One;
		m_vPressedColor = g_vImageButtonClickedColor;
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		SubMPDIAssignData(e_pMPDI->GetObject(0),this);
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
	}
	bool	cImageButton::ProcessFinishAndPressedImage(TiXmlElement*e_pTiXmlelement)
	{
		const wchar_t*l_strElementValue = e_pTiXmlelement->Value();
		cRenderObject*l_pImage = 0;
		bool	l_bLinkParent = true;
		const wchar_t*l_strLinkParent = e_pTiXmlelement->Attribute(L"LinkParent");
		if( l_strLinkParent )
		{
			l_bLinkParent = GetInt(l_strLinkParent)?true:false;
		}
		if( !wcscmp(l_strElementValue,L"FinishImage") )
		{
			l_pImage = GetRenderObjectBehavior(e_pTiXmlelement->FirstChildElement());
			if( l_pImage )
				this->SetFinishImage(l_pImage,l_bLinkParent);
		}
		else
		if( !wcscmp(l_strElementValue,L"PressedImage") )
		{
			l_pImage = GetRenderObjectBehavior(e_pTiXmlelement->FirstChildElement());
			if( l_pImage )
				this->SetPressedImage(l_pImage,l_bLinkParent);
		}
		if( l_pImage )
		{
			SAFE_DELETE(l_pImage);
			return true;
		}
		return false;
	}
	//<cImageButton Name="TestButton" PI="Image/Dice/Dice.pi" Image="Call" Pos="" />
	//<cImageButton Name="TestButton" cMPDIList="Image/Dice/Dice.mpdi" cMPDI="aabb" />
	//<cImageButton Name="TestButton" cMPDIList="Image/Dice/Dice.mpdi" cMPDI="aabb" cSubMPDI="" />
	//<cImageButton Name="TestButton" PI="Image/Dice/Dice.pi" Image="Call" Pos="">
	//	<FinishImage>
	//		<MPDI>
	//	</FinishImage>
	//	<PressedImage>
	//		<MPDI>
	//	</PressedImage>
	//</cImageButton>
	cImageButton*		cImageButton::GetMe(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cImageButton::TypeID);
		cImageButton*l_pImageButton = 0;
		std::wstring	l_strExternalFunctionName;
		const wchar_t*l_strPI = e_pElement->Attribute(L"PI");
		if( l_strPI )
		{
			cPuzzleImage*l_pPI = cGameApp::GetPuzzleImageByFileName(l_strPI);
			const wchar_t*l_strPIUnit = e_pElement->Attribute(L"Image");
			const wchar_t*l_strPos = e_pElement->Attribute(L"Pos");
			if( l_pPI && l_strPIUnit && l_strPos )
			{
				Vector3	l_vPos = GetVector3(l_strPos);
				cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(l_strPIUnit);
				l_pImageButton = new cImageButton(l_pPIUnit,l_vPos);
			}
		}
		else
		{
			const wchar_t*l_strMPDIList = e_pElement->Attribute(cMPDIList::TypeID);
			if( l_strMPDIList )
			{
				cMPDIList*l_pMPDILIst = cGameApp::GetMPDIListByFileName(l_strMPDIList);
				const wchar_t*l_strMPDI = e_pElement->Attribute(cMPDI::TypeID);
				const wchar_t*l_strSubMPDI = e_pElement->Attribute(cSubMPDI::TypeID);
				if( l_pMPDILIst && l_strMPDI )
				{
					cMPDI*l_pMPDI = l_pMPDILIst->GetObject(l_strMPDI);
					if( l_pMPDI )
					{
						if( !l_strSubMPDI )
							l_pImageButton = new cImageButton(l_pMPDI);
						else
						{
							cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(l_strSubMPDI);
							if( l_pSubMPDI )
								l_pImageButton = new cImageButton(l_pSubMPDI);
							else
							{
								UT::ErrorMsg(l_strSubMPDI,L"not exists!");
							}
						}
					}
					else
					{
						UT::ErrorMsg(l_strMPDI,L"not exists!");
					}
				}
			}
		}
		if( !l_pImageButton )
		{
			UT::ErrorMsg(L"cImageButton::GetMe",L"xml data error");
		}
		else
		{
			const wchar_t*l_strExternalFunction = e_pElement->Attribute(L"ExternalFunction");
			if( l_strExternalFunction )
			{
				l_strExternalFunctionName = l_strExternalFunction;
				l_pImageButton->SetExternalFunctionName(l_strExternalFunctionName);
			}
			const wchar_t*l_strSound = e_pElement->Attribute(L"Sound");
			if( l_strSound )
			{
				l_pImageButton->m_strButtonClickSoundName = l_strSound;
			}
			const wchar_t*l_strOriginalColor = e_pElement->Attribute(L"OriginalColor");
			if( l_strOriginalColor )
			{
				l_pImageButton->m_vOriginalColor = GetVector4(l_strOriginalColor);
			}
			const wchar_t*l_strPressedColor = e_pElement->Attribute(L"PressedColor");
			if( l_strPressedColor )
			{
				l_pImageButton->m_vPressedColor = GetVector4(l_strPressedColor);
			}
			PARSE_ELEMENT_START(e_pElement)
				IMAGE_COMMON_PARAMETERS(l_pImageButton)
			TO_NEXT_VALUE
			e_pElement = e_pElement->FirstChildElement();
			while( e_pElement )
			{
				if(!l_pImageButton->ProcessFinishAndPressedImage(e_pElement))
				{
					UT::ErrorMsg(L"cImageButton::GetMe not finis or pressed image element",e_pElement->Value());
				}
				e_pElement = e_pElement->NextSiblingElement();
			}
		}
		return l_pImageButton;
	}

	cImageButton::~cImageButton()
	{
		SAFE_DELETE(m_pPressedImage);
		SAFE_DELETE(m_pFinishImage);
	}

	void	cImageButton::SetEnable(bool e_b)
	{
		cClickMouseBehavior::SetEnable(e_b);
		if( e_b )
			this->SetColor(m_vOriginalColor);
		else
			this->m_vColor = g_vDisableColor;
	}

	void	cImageButton::SetColor(Vector4 e_vColor)
	{
		this->m_vColor = e_vColor;
		this->m_vOriginalColor = e_vColor;
	}

	void	cImageButton::SetTransformInternalData()
	{
		cRenderObject::SetTransformInternalData();
		Vector3	l_vPos = this->GetWorldPosition();
		POINT	l_OriginalSize = GetOriginalSize();
		m_vCollisionRange = Vector4(l_vPos.x,l_vPos.y,l_vPos.x+l_OriginalSize.x,l_vPos.y+l_OriginalSize.y);
		//if(this->m_pFinishImage)
			//m_pFinishImage->SetPos(this->GetPos());
		//if(this->m_pPressedImage)
			//m_pPressedImage->SetPos(this->GetPos());
	}
	//bool    cImageButton::InternalCollide(int e_iPosX,int e_iPosY)
	//{
	//	e_iPosX -= (int)m_vPos.x;
	//	e_iPosY	-= (int)m_vPos.y;
	//	return this->m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY);
	//	//return this->CollideTextureWithTransform(e_iPosX,e_iPosY,false);
	//}

    void    cImageButton::InternalMouseDown(int e_iPosX,int e_iPosY)
	{
		this->m_vColor = m_vPressedColor;
		if( m_strButtonClickSoundName.length() )
		{
			cGameApp::SoundPlay(m_strButtonClickSoundName.c_str(),true);
		}
	}

    void    cImageButton::InternalMouseUp(int e_iPosX,int e_iPosY)
	{
		if( m_TC.fRestTime >0.00001f )
			m_TC.Start();
	}

    void    cImageButton::InternalMouseLeave(int e_iPosX,int e_iPosY)
	{
		if( this->m_bEnable )
			SetColor(m_vOriginalColor);
	}

	void    cImageButton::InitRepeat()
	{

	}

	bool	cImageButton::InternalCollide(int e_iPosX,int e_iPosY)
	{
		return this->m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY);
	}

	void    cImageButton::Init()
	{
		bool	l_bCollided = this->m_bCollided;
		bool	l_bKeyDown = this->m_bKeyDown;
		eObjectMouseBehavior	l_eObjectMouseBehavior = m_eObjectMouseBehavior;
		m_TC.Start();
		cClickMouseBehavior::Init();
		//set color here?seems not good,but I am lazy to fix it
		if( this->m_bEnable )
			this->m_vColor = m_vOriginalColor;
		if( m_pFinishImage )
			m_pFinishImage->Init();
		if( m_pPressedImage )
			m_pPressedImage->Init();
		//make sure repeat works
		if( l_eObjectMouseBehavior == eOMB_HORVER )
		{
			m_eObjectMouseBehavior = eOMB_HORVER;
			this->m_bCollided = l_bCollided;
			this->m_bKeyDown = l_bKeyDown;
		}
	}

	void	cImageButton::SetFinishImage(cRenderObject*e_pImage,bool e_bLinkParent)
	{
		SAFE_DELETE(m_pFinishImage);
		m_pFinishImage = dynamic_cast<cRenderObject*>(e_pImage->Clone());
		//m_pFinishImage->Init();
		//m_pFinishImage->Update(EPSIONAL);
		if( e_bLinkParent )
			m_pFinishImage->SetParent(this,false);
	}

	void	cImageButton::SetPressedImage(cRenderObject*e_pImage,bool e_bLinkParent)
	{
		SAFE_DELETE(m_pPressedImage);
		m_pPressedImage = dynamic_cast<cRenderObject*>(e_pImage->Clone());
		//m_pPressedImage->Init();
		//m_pPressedImage->Update(EPSIONAL);
		if( e_bLinkParent )
			m_pPressedImage->SetParent(this);
	}


	bool    cImageButton::IsSatisfiedCondition()
	{
		if( this->m_bPressedRepeatMouseUp && this->m_eObjectMouseBehavior == eOMB_HORVER )
		{
			return this->m_TCForRepeat.bTragetTimrReached;
		}
		if( cClickMouseBehavior::IsSatisfiedCondition() )
		{
			return this->m_TC.bTragetTimrReached;
		}
		return false;
	}

	void    cImageButton::Update(float e_fElpaseTime)
	{
		cClickMouseBehavior::Update(e_fElpaseTime);
		if( cClickMouseBehavior::IsSatisfiedCondition() )
		{
			if( !m_TC.bTragetTimrReached )
			{
				m_TC.Update(e_fElpaseTime);
				if( m_TC.bTragetTimrReached )
				{
					this->m_vColor = m_vOriginalColor;
					CallExternalFunction(this);
				}
			}
		}
		if( this->GetFirstChild() )
		{
			cRenderObject*l_pRenderObjectBehavior = dynamic_cast<cRenderObject*>(this->GetFirstChild());
			if( l_pRenderObjectBehavior &&l_pRenderObjectBehavior != m_pFinishImage &&  l_pRenderObjectBehavior != m_pPressedImage )
			{
				if( l_pRenderObjectBehavior )
					l_pRenderObjectBehavior->Update(e_fElpaseTime);
			}
		}
	}

	void	cImageButton::Render()
	{
		//if( cClickMouseBehavior::IsSatisfiedCondition() && m_pPressedImage )
		if( m_pPressedImage && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ) )
		{
			//m_pPressedImage->SetPos(m_vPos);
			this->m_pPressedImage->Render();
		}
		else
		{
			if( this->IsSatisfiedCondition() && m_pFinishImage )
				m_pFinishImage->Render();
			else
				//cBaseImage::RenderWithOffset();
				cBaseImage::Render((int)this->m_pvPos->x,(int)this->m_pvPos->y);
		}
		if( this->GetFirstChild() )
		{
			cRenderObject*l_pRenderObjectBehavior = dynamic_cast<cRenderObject*>(this->GetFirstChild());
			if( l_pRenderObjectBehavior &&l_pRenderObjectBehavior != m_pFinishImage &&  l_pRenderObjectBehavior != m_pPressedImage )
				l_pRenderObjectBehavior->Render();
		}
	}
//================================================
//
//================================================

	cButtonDialog::cButtonDialog(cBaseImage*e_pYesImageButton,cBaseImage*e_pNoImageButton,cRenderObject*e_pBK,wchar_t*e_strDescription,Vector3 e_vYesPos,Vector3 e_vNoPos,Vector3 e_vBKPos,Vector3 e_vFontPos)
	{
		m_pFont = 0;
		m_pYesImageButton = 0;
		m_pNoImageButton = 0;
		m_pBG = 0;
		this->m_FontPos.SetLocalPosition(e_vFontPos);
		this->AddChild(&m_FontPos);
		//m_vPos = Vector3::Zero;
		if( e_pYesImageButton )
		{
			m_pYesImageButton = new cImageButton(e_pYesImageButton,e_vYesPos);
			AddObject(m_pYesImageButton);
		}
		if( e_pNoImageButton )
		{
			m_pNoImageButton = new cImageButton(e_pNoImageButton,e_vNoPos);
			AddObject(m_pNoImageButton);
		}
		if( e_pBK )
		{
			m_pBG = dynamic_cast<cRenderObject*>(e_pBK->Clone());
			m_pBG->SetPos(e_vBKPos);
			AddChild(m_pBG);
		}
		m_strDescription = e_strDescription;
		m_eYesNoDialogResult = eYNDR_NONE;
	}
	#define		YES_NO_DIALOG_YES_BUTTON	L"Yes"
	#define		YES_NO_DIALOG_NO_BUTTON		L"No"
	#define		YES_NO_DIALOG_BG			L"BG"
	#define		YES_NO_DIALOG_FONT			L"Font"

	cButtonDialog::cButtonDialog()
	{
		m_pYesImageButton = 0;
		m_pNoImageButton = 0;
		m_pFont = 0;
		m_pBG = 0;	
	}

	cButtonDialog::cButtonDialog(cMPDI*e_pMPDI,const wchar_t*e_strDescription,const wchar_t*e_strYesButton,const wchar_t*e_strNoButton,const wchar_t*e_strBG,bool e_bStripButtonForBG)
	{
		m_pFont = 0;
		m_pYesImageButton = 0;
		m_pNoImageButton = 0;
		m_pBG = 0;
		ProcessData(e_pMPDI,e_strDescription,e_strYesButton,e_strNoButton,e_strBG,e_bStripButtonForBG);
	}

	cButtonDialog::cButtonDialog(cButtonDialog*e_pYesNoDialog):cClickMouseBehaviorVector<cClickMouseBehavior>(e_pYesNoDialog)
	{
		this->SetName(e_pYesNoDialog->GetName());
		m_pFont = 0;
		m_pYesImageButton = 0;
		m_pNoImageButton = 0;
		m_pBG = 0;
		this->m_FontPos.SetLocalTransform(e_pYesNoDialog->m_FontPos.GetLocalTransform());
		//m_vPos = e_pYesNoDialog->m_vPos;
		if( e_pYesNoDialog->m_pYesImageButton )
		{
			m_pYesImageButton = this->GetObject(e_pYesNoDialog->m_pYesImageButton->GetName());
		}
		if( e_pYesNoDialog->m_pYesImageButton )
		{
			m_pYesImageButton = this->GetObject(e_pYesNoDialog->m_pYesImageButton->GetName());
		}
		if(e_pYesNoDialog->m_pFont)
		{
			m_pFont = dynamic_cast<cRenderObject*>(e_pYesNoDialog->m_pFont->Clone());
			AddChild(m_pFont);
		}
		if( e_pYesNoDialog->m_pBG )
		{
			m_pBG = dynamic_cast<cRenderObject*>(e_pYesNoDialog->m_pBG->Clone());
			AddChild(m_pBG);
		}
		m_strDescription = e_pYesNoDialog->m_strDescription;
		m_eYesNoDialogResult = eYNDR_NONE;
		AddChild(&m_FontPos);
	}

	cButtonDialog::~cButtonDialog()
	{
		//SAFE_DELETE(m_pYesImageButton);
		//SAFE_DELETE(m_pNoImageButton);
		SAFE_DELETE(m_pBG);
		SAFE_DELETE(m_pFont);
	}

	void	cButtonDialog::BGStripButtonByName(const wchar_t*e_strName)
	{
		if( m_pBG->Type() == cMPDI::TypeID )
		{
			cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(m_pBG);
			l_pMPDI->RemoveObject(e_strName);
		}	
	}

	void	cButtonDialog::ProcessData(cMPDI*e_pMPDI,const wchar_t*e_strDescription,const wchar_t*e_strYesButton,const wchar_t*e_strNoButton,const wchar_t*e_strBG,bool e_bStripButtonForBG)
	{
		const wchar_t*l_strYesButtonName = YES_NO_DIALOG_YES_BUTTON;
		const wchar_t*l_strNoButtonName = YES_NO_DIALOG_NO_BUTTON;
		const wchar_t*l_strBG= YES_NO_DIALOG_BG;		
		bool	l_bStripYesNo = e_bStripButtonForBG;
		if( e_strYesButton )
			l_strYesButtonName = e_strYesButton;
		if( e_strNoButton )
			l_strNoButtonName = e_strNoButton;
		if( e_strBG )
			l_strBG = e_strBG;
		cSubMPDI*l_pYesSubMPDI = e_pMPDI->GetObject(l_strYesButtonName);
		cSubMPDI*l_pNoSubMPDI = e_pMPDI->GetObject(l_strNoButtonName);
		cSubMPDI*l_pFontSubMPDI = e_pMPDI->GetObject(YES_NO_DIALOG_FONT);
		cSubMPDI*l_pBGSubMPDI = 0;
		if( !l_bStripYesNo )
			l_pBGSubMPDI = e_pMPDI->GetObject(l_strBG);
		if( l_pYesSubMPDI )
			m_pYesImageButton = new cImageButton(l_pYesSubMPDI);
		if( l_pNoSubMPDI )
			m_pNoImageButton = new cImageButton(l_pNoSubMPDI);
		if( l_bStripYesNo && e_pMPDI )
		{
			cMPDI*l_pClone = dynamic_cast<cMPDI*>(e_pMPDI->Clone());
			m_pBG = l_pClone;
			l_pClone->SetAnimationLoop(true);
			l_pClone->RemoveObject(l_strYesButtonName);
			l_pClone->RemoveObject(e_strNoButton);
			l_pClone->RemoveObject(YES_NO_DIALOG_FONT);
		}
		else
		if( l_pBGSubMPDI )
		{
			m_pBG = dynamic_cast<cRenderObject*>(l_pBGSubMPDI->Clone());
		}
		//m_vPos = Vector3::Zero;
		if( e_strDescription )
			m_strDescription = e_strDescription;
		if( l_pFontSubMPDI&&l_pFontSubMPDI->GetOriginalPointList().size() )
			m_FontPos.SetLocalPosition(l_pFontSubMPDI->GetOriginalPointList()[0]);
		this->AddChild(&m_FontPos);
		if( !e_strDescription && l_pFontSubMPDI )
			m_pFont = dynamic_cast<cRenderObject*>(l_pFontSubMPDI->Clone());
		m_eYesNoDialogResult = eYNDR_NONE;
		if( m_pYesImageButton )
			AddObject(m_pYesImageButton);
		if( m_pNoImageButton )
			AddObject(m_pNoImageButton);
		if( m_pBG )
			AddChild(m_pBG);	
	}

	bool	cButtonDialog::ProcessData(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cButtonDialog::TypeID);
		bool	l_bProcessData = false;
		const wchar_t*l_strDescription = e_pElement->Attribute(L"Description");
		const wchar_t*l_strYes = e_pElement->Attribute(L"YesButton");
		const wchar_t*l_strNo = e_pElement->Attribute(L"NoButton");
		const wchar_t*l_strBG = e_pElement->Attribute(L"BG");
		const wchar_t*l_strStripButtonForBG = e_pElement->Attribute(L"StripButtonForBG");
		const wchar_t*l_strAllElementIsButton = e_pElement->Attribute(L"AllElementIsButton");
		bool	l_bStripButtonByName = false;
		if( l_strStripButtonForBG )
			l_bStripButtonByName = GetBoolean(l_strStripButtonForBG);
		TiXmlElement*l_pElement = e_pElement->FirstChildElement();
		bool	l_bExtractButton = false;
		std::vector<std::wstring>	l_ExtractButtonVector;
		cMPDI*l_pMPDI = 0;
		while( l_pElement )
		{
			const wchar_t*l_strValue = l_pElement->Value();
			if( !wcscmp(l_strValue,cMPDI::TypeID) )
			{
				l_pMPDI = cMPDI::GetMe(l_pElement,false);
				if( l_pMPDI )
				{
					ProcessData(l_pMPDI,l_strDescription,l_strYes,l_strNo,l_strBG,l_strStripButtonForBG?GetBoolean(l_strStripButtonForBG):false);
					l_bProcessData = true;
				}
			}
			else
			if( !wcscmp(l_strValue,cImageButton::TypeID) && l_bProcessData )
			{
				l_bExtractButton = true;
				cImageButton*l_pNewButton = cImageButton::GetMe(l_pElement);
				AddObject(l_pNewButton);
				const wchar_t*	l_strSubMPDIName = l_pElement->Attribute(cSubMPDI::TypeID);
				if( l_strSubMPDIName )
				{
					std::wstring	l_strrr = l_strSubMPDIName;
					l_ExtractButtonVector.push_back(l_strrr);
				}
			}
			l_pElement = l_pElement->NextSiblingElement();
		}
		if( !l_bProcessData )
		{
			UT::ErrorMsg(L"cButtonDialog::GetMe format Error",L"please check cMPDI data");
			return false;
		}
		if( l_strAllElementIsButton )
		{
			int	l_iCount = l_pMPDI->Count();
			for( int i=0;i<l_iCount;++i )
			{
				cImageButton*l_pImageButton = new cImageButton(l_pMPDI->GetObject(i));
				AddObjectNeglectExist(l_pImageButton);
			}
		}
		else
		if( l_bExtractButton && l_bStripButtonByName && l_bProcessData )
		{
			for( size_t i=0;i<l_ExtractButtonVector.size();++i )
				BGStripButtonByName(l_ExtractButtonVector[0].c_str());
		}
		return true;
	}

	void    cButtonDialog::Init()
	{
		//if( m_pYesImageButton )m_pYesImageButton->Init();
		//if( m_pNoImageButton )m_pNoImageButton->Init();
		cClickMouseBehaviorVector<cClickMouseBehavior>::Init();
		if( m_pBG )
			m_pBG->Init();
		m_eYesNoDialogResult = eYNDR_NONE;
	}
	void    cButtonDialog::Update(float e_fElpaseTime)
	{
		if( m_pBG )
			m_pBG->Update(e_fElpaseTime);
		cClickMouseBehaviorVector<cClickMouseBehavior>::Update(e_fElpaseTime);
		//if( m_pYesImageButton )
		//	m_pYesImageButton->Update(e_fElpaseTime);
		//if( m_pNoImageButton )
		//	m_pNoImageButton->Update(e_fElpaseTime);
		if(IsSatisfiedCondition())
		{
			if( m_pYesImageButton && m_pYesImageButton->IsSatisfiedCondition() )
			{
				m_eYesNoDialogResult = eYNDR_YES;
			}
			else
			if( m_pNoImageButton && m_pNoImageButton->IsSatisfiedCondition() )
			{
				m_eYesNoDialogResult = eYNDR_NO;
			}
			else
			{
				m_eYesNoDialogResult = eYNDR_ORTHER;
			}
		}
	}

	void	cButtonDialog::Render()
	{
		if( m_pBG )
			m_pBG->Render();
		//if( m_pYesImageButton )
		//	m_pYesImageButton->Render();
		//if( m_pNoImageButton )
		//	m_pNoImageButton->Render();
		cClickMouseBehaviorVector<cClickMouseBehavior>::Render();
		if( m_strDescription.length() )
		{
			Vector3	l_vPos = m_FontPos.GetWorldPosition();
			cGameApp::RenderFont(l_vPos.x,l_vPos.y,(wchar_t*)m_strDescription.c_str());
		}
		else
		if( this->m_pFont )
		{
			m_pFont->Render();
		}
	}

	void	cButtonDialog::Destroy()
	{
	
	}

	//void    cButtonDialog::MouseMove(int e_iPosX,int e_iPosY)
	//{
	//	//if( m_pYesImageButton )
	//	//	m_pYesImageButton->MouseMove(e_iPosX,e_iPosY);
	//	//if( m_pNoImageButton )
	//	//	m_pNoImageButton->MouseMove(e_iPosX,e_iPosY);
	//	m_pAllButton->MouseDown(e_iPosX,e_iPosY);
	//}

	////first time into
	//void    cButtonDialog::MouseDown(int e_iPosX,int e_iPosY)
	//{
	//	//if( m_pYesImageButton )
	//	//	m_pYesImageButton->MouseDown(e_iPosX,e_iPosY);
	//	//if( m_pNoImageButton )
	//	//	m_pNoImageButton->MouseDown(e_iPosX,e_iPosY);
	//	m_pAllButton->MouseDown(e_iPosX,e_iPosY);
	//}

	////
	//void    cButtonDialog::MouseUp(int e_iPosX,int e_iPosY)
	//{
	//	//if( m_pYesImageButton )
	//	//	m_pYesImageButton->MouseUp(e_iPosX,e_iPosY);
	//	//if( m_pNoImageButton )
	//	//	m_pNoImageButton->MouseUp(e_iPosX,e_iPosY);
	//	m_pAllButton->MouseUp(e_iPosX,e_iPosY);
	//}
	cButtonDialog*	cButtonDialog::GetMe(const char*e_strFileName)
	{
		cNodeISAX	l_NodeISAX;
		if(l_NodeISAX.ParseDataIntoXMLNode(e_strFileName))
		{
			TiXmlElement*l_pRoot = l_NodeISAX.GetRootElement();
			if(l_pRoot && !wcscmp(l_pRoot->Value(),cButtonDialog::TypeID))
			{
				return cButtonDialog::GetMe(l_pRoot);
			}
		}
		return 0;
	}
	//<cButtonDialog  AllElementIsButton="true"/>

	//<cButtonDialog Description="">
	//	<cMPDI MPDIList="" MPDI=""/>
	//</cButtonDialog>
	//<cButtonDialog Description="" YesButton="IAmYes" NoButton="IAmNo" BG="BGG" StripButtonForBG="1">
	//	<cMPDI MPDIList="" MPDI=""/>
	//	<cImageButton ....>
	//</cButtonDialog>
	cButtonDialog*		cButtonDialog::GetMe(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cButtonDialog::TypeID);
		cButtonDialog*l_pYesNoDialog = new cButtonDialog();;
		if(l_pYesNoDialog->ProcessData(e_pElement))
			return l_pYesNoDialog;
		SAFE_DELETE(l_pYesNoDialog);
		return 0;
	}
//end namespace namespace FATMING_CORE
}
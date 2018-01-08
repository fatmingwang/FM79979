#include "stdafx.h"
#include "Form1.h"
cPaticleManager*g_pPaticleManager = 0;
cPrtEmitter*g_pPrtEmitter = 0;
UT::sTimeAndFPS g_sTimeAndFPS;
cOrthogonalCamera*g_pOrthogonalCamera = 0;
GLenum g_sfactor = GL_SRC_ALPHA;
GLenum g_dfactor = GL_ONE_MINUS_SRC_ALPHA;
float	DRAW_PANEL_RESOLUTION_WIDTH = 1920.f;
float	DRAW_PANEL_RESOLUTION_HEIGHT = 1080.f;
bool	g_bCameraResetClick = false;
cBaseImage*g_pBKImage = 0;
eEditState	g_eEditState = eES_MOUSE;
Vector3	g_vEmiterPos = Vector3(500,500,0);

cPrtTextureActDynamicTexture*g_pPrtTextureActDynamicTexture = 0;

char*	GetUsageExplanaing(const WCHAR*e_str)
{
	if( !wcscmp(e_str,cPrtColorInitrSetColor::TypeID ))
	{
		return cPrtColorInitrSetColor::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtColorInitrSetRandomColor::TypeID ))
	{
		return cPrtColorInitrSetRandomColor::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtColorActBlending::TypeID ))
	{
		return cPrtColorActBlending::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtColorActBlendingByLife::TypeID ))
	{
		return cPrtColorActBlendingByLife::UsageExplanation;
	}		
	else
	if( !wcscmp(e_str,cPrtColorActBlendingBy2Color::TypeID ))
	{
		return cPrtColorActBlendingBy2Color::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtRotateActRotate::TypeID ))
	{
		return cPrtRotateActRotate::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtSizeInitSetSize::TypeID ))
	{
		return cPrtSizeInitSetSize::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtSizeActBlending::TypeID ))
	{
		return cPrtSizeActBlending::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtStartPositionInitBySquareRange::TypeID ))
	{
		return cPrtStartPositionInitBySquareRange::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtStartPositionInitByFrame::TypeID ))
	{
		return cPrtStartPositionInitByFrame::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtVelocityInitSetVelocity::TypeID ))
	{
		return cPrtVelocityInitSetVelocity::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtVelocityActAcceleration::TypeID ))
	{
		return cPrtVelocityActAcceleration::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtVelocityActBySatelliteAction::TypeID ))
	{
		return cPrtVelocityActBySatelliteAction::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtLifeInitrSetLife::TypeID ))
	{
		return cPrtLifeInitrSetLife::UsageExplanation;
	}
	else
	if( !wcscmp(e_str,cPrtLifeActDyingByGameTime::TypeID ))
	{
		return cPrtLifeActDyingByGameTime::UsageExplanation;
	}
	return "????? call fatming";
}

namespace ParticalEditor
{
//===================
//fill all policy into tabpages
//===================
	AutoCollpaseUnit^	Form1::GetPolicyFormByParticleBase(cParticleBase*e_pParticleBase)
	{
		AutoCollpaseUnit^l_pAutoCollpaseUnit;
		System::Windows::Forms::UserControl^l_pUserControl;
		const WCHAR*l_strTypeName = e_pParticleBase->Type();
		if( l_strTypeName == cPrtColorInitrSetColor::TypeID )
		{
			cPrtColorInitrSetColor*l_pPrtColorInitrSetColor = (cPrtColorInitrSetColor*)e_pParticleBase;
			l_pUserControl = gcnew PrtColorInitrSetColor(&l_pPrtColorInitrSetColor->GetColor()->r,
														 &l_pPrtColorInitrSetColor->GetColor()->g,
														 &l_pPrtColorInitrSetColor->GetColor()->b,
														 &l_pPrtColorInitrSetColor->GetColor()->a);
		}
		else
		if( l_strTypeName == cPrtColorInitrSetRandomColor::TypeID )
		{
			cPrtColorInitrSetRandomColor*l_pPrtColorInitrSetRandomColor = (cPrtColorInitrSetRandomColor*)e_pParticleBase;
			l_pUserControl = gcnew PrtColorInitrSetRandomColor(&l_pPrtColorInitrSetRandomColor->GetColor()->r,
				&l_pPrtColorInitrSetRandomColor->GetColor()->g,
				&l_pPrtColorInitrSetRandomColor->GetColor()->b,
				&l_pPrtColorInitrSetRandomColor->GetColor()->a);
		}
		else
		if( l_strTypeName == cPrtColorActBlending::TypeID )
		{
			cPrtColorActBlending*l_pPrtColorActBlending = (cPrtColorActBlending*)e_pParticleBase;
			l_pUserControl = gcnew PrtColorActBlending(&l_pPrtColorActBlending->GetColor()->r,
				&l_pPrtColorActBlending->GetColor()->g,
				&l_pPrtColorActBlending->GetColor()->b,
				&l_pPrtColorActBlending->GetColor()->a,l_pPrtColorActBlending->IsFade());
		}
		else
		if( l_strTypeName == cPrtColorActBlendingByLife::TypeID )
		{
			cPrtColorActBlendingByLife*l_pPrtColorActBlendingByLife = (cPrtColorActBlendingByLife*)e_pParticleBase;
			l_pUserControl = gcnew PrtColorActBlendingByLife(&l_pPrtColorActBlendingByLife->GetColor()->r,
				&l_pPrtColorActBlendingByLife->GetColor()->g,
				&l_pPrtColorActBlendingByLife->GetColor()->b,
				&l_pPrtColorActBlendingByLife->GetColor()->a);
		}		
		else
		if( l_strTypeName == cPrtColorActBlendingBy2Color::TypeID )
		{
			cPrtColorActBlendingBy2Color*l_pPrtColorActBlendingBy2Color = (cPrtColorActBlendingBy2Color*)e_pParticleBase;
			l_pUserControl = gcnew PrtColorActBlendingBy2Color(&l_pPrtColorActBlendingBy2Color->GetColor1()->r,
				&l_pPrtColorActBlendingBy2Color->GetColor1()->g,
				&l_pPrtColorActBlendingBy2Color->GetColor1()->b,
				&l_pPrtColorActBlendingBy2Color->GetColor1()->a,
				&l_pPrtColorActBlendingBy2Color->GetColor2()->r,
				&l_pPrtColorActBlendingBy2Color->GetColor2()->g,
				&l_pPrtColorActBlendingBy2Color->GetColor2()->b,
				&l_pPrtColorActBlendingBy2Color->GetColor2()->a);
		}
		else
		if( l_strTypeName == cPrtRotateActRotate::TypeID )
		{
			cPrtRotateActRotate*l_pPrtRotateActRotate = (cPrtRotateActRotate*)e_pParticleBase;
			l_pUserControl = gcnew RotateActRotate(&l_pPrtRotateActRotate->GetAngle()->x,
				&l_pPrtRotateActRotate->GetAngle()->y,
				&l_pPrtRotateActRotate->GetAngle()->z,(int*)l_pPrtRotateActRotate->GetPRM(),
				l_pPrtRotateActRotate->IsRandom(),l_pPrtRotateActRotate->GetRandomOffset(),
				l_pPrtRotateActRotate->IsNegative());
		}
		else
		if( l_strTypeName == cPrtSizeInitSetSize::TypeID )
		{
			cPrtSizeInitSetSize*l_pPrtSizeInitSetSize = (cPrtSizeInitSetSize*)e_pParticleBase;
			l_pUserControl = gcnew PrtSizeInitSetSize(&l_pPrtSizeInitSetSize->x,&l_pPrtSizeInitSetSize->y,l_pPrtSizeInitSetSize->GetRandomValue());
		}
		else
		if( l_strTypeName == cPrtSizeActBlending::TypeID )
		{
			cPrtSizeActBlending*l_pPrtSizeActBlending = (cPrtSizeActBlending*)e_pParticleBase;
			l_pUserControl = gcnew PrtSizeActBlending(&l_pPrtSizeActBlending->x,&l_pPrtSizeActBlending->y,l_pPrtSizeActBlending->IsAddOrSubtract());
		}
		else
		if( l_strTypeName == cPrtStartPositionInitBySquareRange::TypeID )
		{
			cPrtStartPositionInitBySquareRange*l_pPrtStartPositionInitBySquareRange = (cPrtStartPositionInitBySquareRange*)e_pParticleBase;
			l_pUserControl = gcnew StartPositionInitBySquareRange(l_pPrtStartPositionInitBySquareRange->GetWidth(),
				l_pPrtStartPositionInitBySquareRange->GetHeight(),
				l_pPrtStartPositionInitBySquareRange->GetDeepth());
		}
		else
		if( l_strTypeName == cPrtStartPositionInitByFrame::TypeID )
		{
			//cPrtStartPositionInitByFrame*l_pPrtStartPositionInitByFrame = (cPrtStartPositionInitByFrame*)e_pParticleBase;
			WARNING_MSG("not support for now");
			return l_pAutoCollpaseUnit;
			//l_pUserControl = gcnew StartPositionInitByFrame();
		}
		//else
		//if( l_strTypeName == cPrtVelocityInitRandomFly::TypeID )
		//{
		//	cPrtVelocityInitRandomFly*l_pPrtVelocityInitRandomFly = (cPrtVelocityInitRandomFly*)e_pParticleBase;
		//	l_pUserControl = gcnew PrtVelocityInitRandomFly(&l_pPrtVelocityInitRandomFly->GetDir()->x,
		//		&l_pPrtVelocityInitRandomFly->GetDir()->y,
		//		&l_pPrtVelocityInitRandomFly->GetDir()->z,l_pPrtVelocityInitRandomFly->GetDistance(),l_pPrtVelocityInitRandomFly->IsRandom());
		//}
		else
		if( l_strTypeName == cPrtVelocityInitSetVelocity::TypeID )
		{
			//cPrtVelocityInitSetVelocity*l_pPrtVelocityInitSetVelocity = (cPrtVelocityInitSetVelocity*)e_pParticleBase;
			//l_pUserControl = gcnew PrtVelocityInitSetVelocity(&l_pPrtVelocityInitSetVelocity->GetVeolcity()->x,
			//	&l_pPrtVelocityInitSetVelocity->GetVeolcity()->y,
			//	&l_pPrtVelocityInitSetVelocity->GetVeolcity()->z,l_pPrtVelocityInitSetVelocity->IsRandom(),l_pPrtVelocityInitSetVelocity->GetRandomOffSet());
			WARNING_MSG("only can have one default");
			return l_pAutoCollpaseUnit;
		}
		else
		if( l_strTypeName == cPrtVelocityActAcceleration::TypeID )
		{
			cPrtVelocityActAcceleration*l_pPrtVelocityActAcceleration = (cPrtVelocityActAcceleration*)e_pParticleBase;
			l_pUserControl = gcnew PrtVelocityActAcceleration(l_pPrtVelocityActAcceleration->GetAccelerationSpeed());
		}
		else
		if( l_strTypeName == cPrtVelocityActBySatelliteAction::TypeID )
		{
			WARNING_MSG("not support for now");
			return l_pAutoCollpaseUnit;
		}
		else
		if( l_strTypeName == cPrtLifeInitrSetLife::TypeID )
		{
			cPrtLifeInitrSetLife*l_pPrtColorActBlendingByLife = (cPrtLifeInitrSetLife*)e_pParticleBase;
			l_pUserControl = gcnew PrtLifeInitrSetLife(l_pPrtColorActBlendingByLife->GetLifeMin(),l_pPrtColorActBlendingByLife->GetLifeRange(),l_pPrtColorActBlendingByLife->IsRandom());
		}
		else
		if( l_strTypeName == cPrtLifeActDyingByGameTime::TypeID )
		{
			l_pUserControl = gcnew System::Windows::Forms::UserControl;
			l_pUserControl->Size = System::Drawing::Size(200,0);
		}
		else
		if( l_strTypeName == cPrtRotateInitRotate::TypeID )
		{
			cPrtRotateInitRotate*l_pPrtRotateInitRotate = (cPrtRotateInitRotate*)e_pParticleBase;
			l_pUserControl = gcnew RotateInitRotate(&l_pPrtRotateInitRotate->GetAngle()->x,
				&l_pPrtRotateInitRotate->GetAngle()->y,
				&l_pPrtRotateInitRotate->GetAngle()->z);
		}
		else
		if( l_strTypeName == cPrtTextureActDynamicTexture::TypeID )
		{
			cPrtTextureActDynamicTexture*l_pPrtTextureActDynamicTexture = (cPrtTextureActDynamicTexture*)e_pParticleBase;
			l_pUserControl = gcnew DynamicTexture();
			g_pPrtTextureActDynamicTexture = (cPrtTextureActDynamicTexture*)e_pParticleBase;
			((DynamicTexture^)l_pUserControl)->RefreshPI();
			m_pCurrentDynamicTexture = (DynamicTexture^)l_pUserControl;
		}
		else
		if( l_strTypeName == cPrtVelocityActDircctionChange::TypeID )
		{
			cPrtVelocityActDircctionChange*l_pPrtVelocityActDircctionChange = (cPrtVelocityActDircctionChange*)e_pParticleBase;
			l_pUserControl = gcnew PrtVelocityActDircctionChange(l_pPrtVelocityActDircctionChange->GetStopTime(),l_pPrtVelocityActDircctionChange->GetNewSpeed(),l_pPrtVelocityActDircctionChange->IsAccesleration());
		}
		else
		{
			assert(0&&"do not support such type");
		}
		e_pParticleBase->SetOwner(g_pPrtEmitter);
		l_pAutoCollpaseUnit = gcnew AutoCollpaseUnit(DNCT::WcharToGcstring((WCHAR*)l_strTypeName),l_pUserControl->Size,l_pUserControl);
		return l_pAutoCollpaseUnit;
	}
	void	Form1::TextureRefresh()
	{
	    Texture_treeView->Nodes->Clear();
		EmiteListr_listBox->Items->Clear();
		Texture_treeView->Nodes->Clear();
		for( int i=0;i<g_pPaticleManager->Count();++i )
		{
			cPrtEmitter*l_pPrtEmitter = (*g_pPaticleManager)[i];
			EmiteListr_listBox->Items->Add(DNCT::WcharToGcstring(l_pPrtEmitter->GetName()));
		}
		ImagerParserAddIntiTreeView(g_pPaticleManager->GetImageParser(),Texture_treeView,true);
	}

	System::Void	Form1::MyInitPolicyToListBox(const WCHAR***e_strDataList,GCFORM::TabControl^e_pTabControl,System::Collections::Hashtable^e_pHashTable)
	{
		const WCHAR***l_strInit = e_strDataList;
		const char*l_strAllPolicyName = cPrtAllClassName::m_strAllParticlePolicy[0];
		int	l_i = 0;
		while(l_strAllPolicyName)
		{
			GCFORM::TabPage^l_pTabPage = gcnew GCFORM::TabPage;
			l_pTabPage->Name = String(l_strAllPolicyName).ToString();
			l_pTabPage->Text = String(l_strAllPolicyName).ToString();
			e_pTabControl->TabPages->Add(l_pTabPage);
			l_pTabPage->BackColor = System::Drawing::Color::FromKnownColor(System::Drawing::KnownColor::ControlDarkDark);
			GCFORM::ListBox^l_pListBox = gcnew GCFORM::ListBox;
			l_pListBox->Margin = GCFORM::Padding(0);
			l_pListBox->BackColor = System::Drawing::Color::FromKnownColor(System::Drawing::KnownColor::ControlDark);
			l_pListBox->Dock = System::Windows::Forms::DockStyle::Fill;
			l_pListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::Policy_listBox_SelectedIndexChanged);
			l_pListBox->ForeColor = SrcBlending_listBox->ForeColor;
			l_pTabPage->Controls->Add(l_pListBox);
			e_pHashTable[l_pTabPage] = l_pListBox;
			const WCHAR**l_strCurrentData = l_strInit[l_i];
			const WCHAR*l_strName = l_strCurrentData[0];
			while(l_strName)
			{
				l_pListBox->Items->Add(DNCT::WcharToGcstring((WCHAR*)l_strName));
				l_strCurrentData++;
				l_strName = l_strCurrentData[0];
			}
			l_i++;
			l_strAllPolicyName = cPrtAllClassName::m_strAllParticlePolicy[l_i];
		}	
	}

	System::Void	Form1::InitPolicyToListBox()
	{
		MyInitPolicyToListBox(cPrtAllClassName::m_sstrAllInitNameList,InitPolicy_tabControl,m_pInitTabpageWithListBox);
		MyInitPolicyToListBox(cPrtAllClassName::m_sstrAllActNameList,ActPolicy_tabControl,m_pActTabpageWithListBox);
	}
//===================
//
//===================
	System::String^	MyAddPolicyIntoListBox(System::Collections::Hashtable^e_pHashTable,
		GCFORM::TabControl^e_pTabControl,GCFORM::ListBox^e_pListBox)
	{
		String^l_str;
		GCFORM::ListBox^l_pListBox = (GCFORM::ListBox^)e_pHashTable[e_pTabControl->SelectedTab];
		if(l_pListBox->SelectedIndex == -1)
			return l_str;
		if(e_pListBox->Items->Contains(l_pListBox->SelectedItem->ToString()))
		{
			if(::MessageBox(0
				,L"such policy is exists,would u like to add it?",
				DNCT::GcStringToWchar(l_pListBox->SelectedItem->ToString()).c_str(),MB_YESNO) != 6)
				return l_str;
		}
		e_pListBox->Items->Add(l_pListBox->SelectedItem->ToString());
		return l_pListBox->SelectedItem->ToString();
	}

	int	MyDelPolicyIntoListBox(GCFORM::ListBox^e_pListBox,bool e_bInitPolicy)
	{
		int	l_iSelectedIndex = e_pListBox->SelectedIndex;
		if( l_iSelectedIndex != -1 )
		{
			if( e_bInitPolicy )
			{
				g_pPrtEmitter->GetInitPolicyParticleList()->RemoveObject(e_pListBox->SelectedIndex);
			}
			else
			{
				g_pPrtEmitter->GetActPolicyParticleList()->RemoveObject(e_pListBox->SelectedIndex);
			}
			e_pListBox->Items->RemoveAt(e_pListBox->SelectedIndex);
		}
		return l_iSelectedIndex;
	}

	System::Void	Form1::AddInitPolicy()
	{
		String^l_str = MyAddPolicyIntoListBox(m_pInitTabpageWithListBox,InitPolicy_tabControl,AllInitPolicy_listBox);
		if( l_str )
		{
			cParticleBase*l_pParticleBase = NameGetParticleType(DNCT::GcStringToWchar(l_str).c_str());
			AutoCollpaseUnit^l_pAutoCollpaseUnit;
			if( l_pParticleBase )
			{
				l_pAutoCollpaseUnit = GetPolicyFormByParticleBase(l_pParticleBase);
				if( l_pAutoCollpaseUnit )
				{
					InitPolicy_flowLayoutPanel->Controls->Add(l_pAutoCollpaseUnit);
					m_pInitUserControlList->Add(l_pAutoCollpaseUnit);
				}
			}
			if( l_pParticleBase&&l_pAutoCollpaseUnit )
			{
				g_pPrtEmitter->AddInitPolicy(l_pParticleBase);
				g_pPrtEmitter->SetupPolicy();
			}
			if( !l_pParticleBase||!l_pAutoCollpaseUnit )
			{
				AllInitPolicy_listBox->Items->RemoveAt(AllInitPolicy_listBox->Items->Count-1);
			}
		}
	}

	System::Void	Form1::DelInitPolicy()
	{
		int	l_iIndex = MyDelPolicyIntoListBox(AllInitPolicy_listBox,true);
		if( l_iIndex!=-1 )
		{
			InitPolicy_flowLayoutPanel->Controls->Remove((GCFORM::Control^)m_pInitUserControlList[l_iIndex]);
			m_pInitUserControlList->RemoveAt(l_iIndex);
			g_pPrtEmitter->SetupPolicy();
		}
	}

	System::Void	Form1::AddActPolicy()
	{
		String^l_str = MyAddPolicyIntoListBox(m_pActTabpageWithListBox,ActPolicy_tabControl,AllActPolicy_listBox);
		if( l_str )
		{
			cParticleBase*l_pParticleBase = NameGetParticleType(DNCT::GcStringToWchar(l_str).c_str());
			l_pParticleBase->SetOwner(g_pPrtEmitter);
			AutoCollpaseUnit^l_pAutoCollpaseUnit;
			if( l_pParticleBase )
			{
				l_pAutoCollpaseUnit = GetPolicyFormByParticleBase(l_pParticleBase);
				if( l_pAutoCollpaseUnit )
				{
					ActPolicy_flowLayoutPanel->Controls->Add(l_pAutoCollpaseUnit);
					m_pActUserControlList->Add(l_pAutoCollpaseUnit);
				}
			}
			if( l_pParticleBase&&l_pAutoCollpaseUnit )
			{
				g_pPrtEmitter->AddActPolicy(l_pParticleBase);
				g_pPrtEmitter->SetupPolicy();
			}
			if( !l_pParticleBase||!l_pAutoCollpaseUnit )
			{
				AllActPolicy_listBox->Items->RemoveAt(AllActPolicy_listBox->Items->Count-1);
			}
		}
	}

	System::Void	Form1::DelActPolicy()
	{
		int	l_iIndex = MyDelPolicyIntoListBox(AllActPolicy_listBox,false);
		if( l_iIndex!=-1 )
		{
			ActPolicy_flowLayoutPanel->Controls->Remove((GCFORM::Control^)m_pActUserControlList[l_iIndex]);
			m_pActUserControlList->RemoveAt(l_iIndex);
			g_pPrtEmitter->SetupPolicy();
		}
	}

	void	Form1::AddPolicyIntoData(cParticleBase*e_pParticleBase,bool e_bInitPolicy)
	{
		if( !e_pParticleBase )
		{
			WARNING_MSG("not support now!!");
			return;
		}
		AutoCollpaseUnit^l_pAutoCollpaseUnit;
		l_pAutoCollpaseUnit = GetPolicyFormByParticleBase(e_pParticleBase);
		if( !l_pAutoCollpaseUnit )
		{
			WARNING_MSG("not support now!!");
			return;
		}
		if( e_bInitPolicy )
		{
			InitPolicy_flowLayoutPanel->Controls->Add(l_pAutoCollpaseUnit);
			m_pInitUserControlList->Add(l_pAutoCollpaseUnit);
			AllInitPolicy_listBox->Items->Add(DNCT::WcharToGcstring((WCHAR*)e_pParticleBase->Type()));
			g_pPrtEmitter->AddInitPolicy(e_pParticleBase);
		}
		else
		{
			ActPolicy_flowLayoutPanel->Controls->Add(l_pAutoCollpaseUnit);
			m_pActUserControlList->Add(l_pAutoCollpaseUnit);
			AllActPolicy_listBox->Items->Add(DNCT::WcharToGcstring((WCHAR*)e_pParticleBase->Type()));
			g_pPrtEmitter->AddActPolicy(e_pParticleBase);
		}
		g_pPrtEmitter->SetupPolicy();
	}

	System::Void	Form1::InitEmiter(bool e_bAssignDefultParticleData)
	{
		InitPolicy_flowLayoutPanel->Controls->Clear();
		ActPolicy_flowLayoutPanel->Controls->Clear();
		m_pInitUserControlList->Clear();
		m_pActUserControlList->Clear();
		AllInitPolicy_listBox->Items->Clear();
		AllActPolicy_listBox->Items->Clear();
		SAFE_DELETE(g_pPrtEmitter);
		g_pPrtEmitter = new cPrtEmitter(L"Test");
		g_pPrtEmitter->Init();
		if( m_pEmitterEditor&&this->m_pEmitterEditor->m_pParticleEmiterWithShowPosition )
			this->m_pEmitterEditor->m_pParticleEmiterWithShowPosition->SetPrtEmitter(g_pPrtEmitter);
		g_pPrtEmitter->SetPos(g_vEmiterPos);
		if( e_bAssignDefultParticleData )
		{
			cPrtVelocityInitSetVelocity*l_pPrtVelocityInitSetVelocity = (cPrtVelocityInitSetVelocity*)g_pPrtEmitter->GetVelocity();;
			Vector3*l_pvVelocity = l_pPrtVelocityInitSetVelocity->GetVeolcity();
			InitPolicy_flowLayoutPanel->Controls->Add(gcnew PrtVelocityInitSetVelocity(&l_pvVelocity->x,
					&l_pvVelocity->y,
					&l_pvVelocity->z,l_pPrtVelocityInitSetVelocity->IsRandom(),l_pPrtVelocityInitSetVelocity->GetRandomOffSet()));

			cPrtLifeInitrSetLife*l_pPrtLifeInitrSetLife = new cPrtLifeInitrSetLife();
			cPrtColorInitrSetColor*l_pPrtColorInitrSetColor = new cPrtColorInitrSetColor();
			cPrtSizeInitSetSize*l_pPrtSizeInitSetSize = new cPrtSizeInitSetSize();
			cPrtLifeActDyingByGameTime*l_pPrtLifeActDyingByGameTime = new cPrtLifeActDyingByGameTime;
			AddPolicyIntoData(l_pPrtLifeInitrSetLife,true);
			AddPolicyIntoData(l_pPrtColorInitrSetColor,true);
			AddPolicyIntoData(l_pPrtSizeInitSetSize,true);
			AddPolicyIntoData(l_pPrtLifeActDyingByGameTime,false);
		}
	}
}
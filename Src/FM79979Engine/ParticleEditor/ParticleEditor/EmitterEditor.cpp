
#include "StdAfx.h"
#include "EmitterEditor.h"

namespace ParticalEditor
{

	void	EmitterEditor::RefreshParticle(cPrtEmitter*e_pPrtEmitter)
	{
		if( m_pBehaviorPaticleManager )
		{
			int	l_iCount = this->m_pBehaviorPaticleManager->Count();
			for( int i=0;i<l_iCount;++i )
			{
				cParticleEmitterGroup*l_pParticleEmitterGroup = dynamic_cast<cParticleEmitterGroup*>((*m_pBehaviorPaticleManager)[i]);
				int	l_iCount2 = l_pParticleEmitterGroup->Count();
				for( int j=0;j<l_iCount2;++j )
				{
					if(l_pParticleEmitterGroup->GetObject(j)->GetPrtEmitter()->IsSameName(e_pPrtEmitter))
					{
						cPrtEmitter*l_pPrtEmitter = l_pParticleEmitterGroup->GetObject(j)->GetPrtEmitter();
						SAFE_DELETE(l_pPrtEmitter);
						l_pParticleEmitterGroup->GetObject(j)->SetPrtEmitter(new cPrtEmitter(e_pPrtEmitter));
						l_pParticleEmitterGroup->GetObject(j)->SetEmitDirection(*l_pParticleEmitterGroup->GetObject(j)->GetEmitDirection());
					}
				}
			}
			if( this->m_pParticleEmitterGroup )
			{
				int	l_iCount2 = this->m_pParticleEmitterGroup->Count();
				for( int j=0;j<l_iCount2;++j )
				{
					if(this->m_pParticleEmitterGroup->GetObject(j)->GetPrtEmitter()->IsSameName(e_pPrtEmitter))
					{
						cPrtEmitter*l_pPrtEmitter = this->m_pParticleEmitterGroup->GetObject(j)->GetPrtEmitter();
						SAFE_DELETE(l_pPrtEmitter);
						m_pParticleEmitterGroup->GetObject(j)->SetPrtEmitter(new cPrtEmitter(e_pPrtEmitter));
						m_pParticleEmitterGroup->GetObject(j)->SetEmitDirection(*this->m_pParticleEmitterGroup->GetObject(j)->GetEmitDirection());
					}
				}
			}
		}
	}

	void	EmitterEditor::ReassignParticleEmiterWithShowPositionData(bool e_bAssignEmitterAngle)
	{
 		if( !m_pParticleEmiterWithShowPosition )
			return;
		m_pParticleEmiterWithShowPosition->SetPrtEmitter(g_pPrtEmitter);
		ParticlePathAngle_xyzNumeriaclControl->m_bToRadius = true;
		ParticlePathAngle_xyzNumeriaclControl->SetValue(m_pParticleEmiterWithShowPosition->GetCurveRotationAngle());
		assert(m_pParticleEmiterWithShowPosition->GetPrtEmitter());
		cPrtVelocityInitSetVelocity*l_pPrtVelocityInitSetVelocity = (cPrtVelocityInitSetVelocity*)g_pPrtEmitter->GetVelocity();;
		if(e_bAssignEmitterAngle)
			g_pPrtEmitter->SetEmitDirection(*m_pParticleEmiterWithShowPosition->GetEmitDirection());
		else
			m_pParticleEmiterWithShowPosition->SetEmitDirection(g_pPrtEmitter->GetEmitDirection());
		EmitDirection_xyzNumeriaclControl->SetValue(l_pPrtVelocityInitSetVelocity->GetVeolcity());
		ParticlePathPos_xyzNumeriaclControl->SetValue(m_pParticleEmiterWithShowPosition->GetEmitterPos());
		ImmediateDired_checkBox->SetValue( m_pParticleEmiterWithShowPosition->IsImediatelyKilled() );
		StartTime_numericalControl->SetValue(m_pParticleEmiterWithShowPosition->GetStartTimePointer());
		EndTime_numericalControl->SetValue(&m_pParticleEmiterWithShowPosition->GetEndTimeCounter()->fTargetTime);
		Loop_checkBox->Checked = m_pParticleEmiterWithShowPosition->IsAnimationLoop();
	}

	bool	EmitterEditor::IsBehaviorEmitterUsingPrtByName(const WCHAR*e_strName)
	{
		int	l_iParticleEmitterGroupCount = m_pBehaviorPaticleManager->Count();
		for( int i=0;i<l_iParticleEmitterGroupCount;++i )
		{
			assert(m_pBehaviorPaticleManager->GetObject(i)->Type() == cParticleEmitterGroup::TypeID);
			cParticleEmitterGroup*l_pParticleEmitterGroup = dynamic_cast<cParticleEmitterGroup*>((*m_pBehaviorPaticleManager)[i]);
			int	l_iCount = l_pParticleEmitterGroup->Count();
			for( int j=0;j<l_iCount;++j )
			{
				if(!wcscmp((*l_pParticleEmitterGroup)[j]->GetPrtEmitter()->GetName(),e_strName))
					return true;
			}
		}
		return false;
	}
	 bool	EmitterEditor::OpenFile()
	 {
		String^l_str = DNCT::OpenFileAndGetName(ConbineFileDescribtionWithExtensionToFilter("ParticleGroup Files",m_pBehaviorPaticleManager->ExtensionName(),true));
		if( l_str )
		{
			m_pBehaviorPaticleManager->Destroy();
			bool	l_b = m_pBehaviorPaticleManager->Parse(DNCT::GcStringToChar(l_str));
			if( l_b&&m_pBehaviorPaticleManager->Count() )
			{
				WholeAnimationList_listBox->Items->Clear();
				Path_listBox->Items->Clear();
				Path_listBox->Enabled = false;
				PathType_comboBox->SelectedIndex = 0;
				ParticleBehavior_listBox->Items->Clear();
				m_pParticleEmiterWithShowPosition->SetPrtEmitter(0);
				SAFE_DELETE(m_pParticleEmiterWithShowPosition);
				SAFE_DELETE(m_pParticleEmitterGroup);
				SAFE_DELETE(g_pPaticleManager);
				m_pParticleEmitterGroup = new cParticleEmitterGroup();
				m_pParticleEmiterWithShowPosition = new cParticleEmiterWithShowPosition(L"test");
				ReassignParticleEmiterWithShowPositionData(false);
				g_pPaticleManager = m_pBehaviorPaticleManager->m_pPaticleManager;
				m_pBehaviorPaticleManager->m_pPaticleManager = 0;
				for(int i=0;i<m_pBehaviorPaticleManager->Count();++i)
				{
					WholeAnimationList_listBox->Items->Add(DNCT::WcharToGcstring(m_pBehaviorPaticleManager->GetObject(i)->GetName()));
				}
				for( int i=0;i<m_pBehaviorPaticleManager->m_CurveManager.Count();++i )
				{
					Path_listBox->Items->Add(DNCT::WcharToGcstring(m_pBehaviorPaticleManager->m_CurveManager[i]->GetName()));
				}
				return true;
			}
		}
		return false;
	 }

	 void	EmitterEditor::SaveFile()
	 {
		String^l_strFileName = DNCT::SaveFileAndGetName(ConvertExtensionToFilter((char*)m_pBehaviorPaticleManager->ExtensionName()));
		if( l_strFileName )
		{
			l_strFileName = ForceAddExtenName(l_strFileName,(char*)m_pBehaviorPaticleManager->ExtensionName());
			string	l_strUseerNameAndTime = DNCT::GcStringToChar(DNCT::GetUseerNameAndTime());
			//just take it for save data
			m_pBehaviorPaticleManager->m_pPaticleManager = g_pPaticleManager;
			this->m_pBehaviorPaticleManager->Export(DNCT::GcStringToChar(l_strFileName),(char*)l_strUseerNameAndTime.c_str());
			m_pBehaviorPaticleManager->m_pPaticleManager = 0;
		}	 
	 }

	void	EmitterEditor::AssignUIToParticleBehaviorData(bool e_bRefresh)
	{
	    if( !e_bRefresh )
	        return;
		 //assign path data
		 m_pParticleEmiterWithShowPosition->SetAnimationLoop(Loop_checkBox->Checked);
		 if( Path_listBox->SelectedIndex == -1 )
		 {
			 m_pParticleEmiterWithShowPosition->SetCurveWithTime(0);
			 m_pParticleEmiterWithShowPosition->SetPathType(cParticleEmiterWithShowPosition::ePT_NO_PATH);
		 }
		 else
		 {
			m_pParticleEmiterWithShowPosition->SetCurveWithTime(m_pBehaviorPaticleManager->m_CurveManager[Path_listBox->SelectedIndex]);
			m_pParticleEmiterWithShowPosition->Init();
		 }
		 m_pParticleEmiterWithShowPosition->SetEmitDirection(Vector3(*EmitDirection_xyzNumeriaclControl->m_pfX,*EmitDirection_xyzNumeriaclControl->m_pfY,*EmitDirection_xyzNumeriaclControl->m_pfZ));
		 //set proper angle and position for path 
		 m_pParticleEmiterWithShowPosition->RefreshAngleAndPosData();
		 //reset path end time1
		 if(!Loop_checkBox->Checked)
		 {
			if(m_pParticleEmiterWithShowPosition->GetEndTimeCounter()->fTargetTime <= 0.f)
			{
				WARNING_MSG("no end time and not loop");
				return;
			}
			m_pParticleEmiterWithShowPosition->SetEndTimeCounter(m_pParticleEmiterWithShowPosition->GetEndTimeCounter()->fTargetTime);
		 }
		 m_pParticleEmiterWithShowPosition->SetAnimationLoop(m_pParticleEmiterWithShowPosition->IsAnimationLoop());	
	}

	void	EmitterEditor::RefreshPathData()
	{
		if( Path_listBox->SelectedIndex != -1 )
		{
			m_pParticleEmiterWithShowPosition->SetCurveWithTime(m_pBehaviorPaticleManager->m_CurveManager[Path_listBox->SelectedIndex]);
			m_pParticleEmiterWithShowPosition->SetParticleRotateMode((eParticleRotateMode)RotateType_comboBox->SelectedIndex);
			m_pParticleEmiterWithShowPosition->RefreshAngleAndPosData();
			ParticlePathAngle_xyzNumeriaclControl->m_bValueChanged = false;
			ParticlePathPos_xyzNumeriaclControl->m_bValueChanged = false;
		}
	}

	void	EmitterEditor::RenderAssistance(float e_fElpaseTime)
	{
		static float	l_fColor = 1.f;
		l_fColor += e_fElpaseTime;
		if(  l_fColor>1.f )
			l_fColor = 0.f;
		FATMING_CORE::SetupShaderWorldMatrix(cMatrix44::Identity);
		Vector3	l_vPos((float)ParticlePathPos_xyzNumeriaclControl->X_numericUpDown->Value,
			(float)ParticlePathPos_xyzNumeriaclControl->Y_numericUpDown->Value,
			(float)ParticlePathPos_xyzNumeriaclControl->Z_numericUpDown->Value);
		cMatrix44	l_mat;
		if( AllPlay_checkBox->Checked )
		{
			for(int i=0;i<this->m_pParticleEmitterGroup->Count();++i)
			{
				cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition = (*m_pParticleEmitterGroup)[i];
				cCurveWithTime*l_pCurveWithTime = l_pParticleEmiterWithShowPosition->GetCurveWithTime();
				if( this->m_bShowLine )
				{
					if( l_pCurveWithTime )
						l_pCurveWithTime->Render(Vector4(1,l_fColor,l_fColor,1));
				}
				if( m_bShowEmitter )
				{
					Vector3 l_vPos = *l_pParticleEmiterWithShowPosition->GetEmitterPos();
					if(l_pParticleEmiterWithShowPosition->GetPrtEmitter())
					{
						GLRender::DrawCube(Vector3(1,1,1),cMatrix44::TranslationMatrix(l_vPos),Vector4(l_fColor,l_fColor*i,l_fColor*2,0.2f));
						GLRender::Render3DArrow(l_vPos,l_pParticleEmiterWithShowPosition->GetPrtEmitter()->GetEmitDirection(),Vector4(l_fColor,l_fColor*i,l_fColor*2,l_fColor),3);
					}
				}
			}
		}
		else
		if( Path_listBox->SelectedIndex !=-1 )
		{
			if( this->m_bShowLine )
			{
				if( m_pParticleEmiterWithShowPosition->GetCurveWithTime() )
					m_pParticleEmiterWithShowPosition->GetCurveWithTime()->Render(Vector4(1,l_fColor,l_fColor,1));
			}
		}
		if( m_bShowEmitter && m_pParticleEmiterWithShowPosition )
		{
			Vector3 l_vPos = *m_pParticleEmiterWithShowPosition->GetEmitterPos();
			if(m_pParticleEmiterWithShowPosition->GetPrtEmitter())
			{
				GLRender::Render3DArrow(l_vPos,l_vPos+m_pParticleEmiterWithShowPosition->GetPrtEmitter()->GetEmitDirection(),Vector4(1,l_fColor,l_fColor*2,l_fColor),3);
				GLRender::DrawCube(Vector3(1,1,1),cMatrix44::TranslationMatrix(l_vPos),Vector4(1,1,1,0.1f));
			}
		}
		if(ParticlePathPos_xyzNumeriaclControl->m_bValueChanged||ParticlePathAngle_xyzNumeriaclControl->m_bValueChanged)
		{
			RefreshPathData();
		}
	}
}
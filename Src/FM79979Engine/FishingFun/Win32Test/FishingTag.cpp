#include "stdafx.h"
#include "FishingTag.h"

cFishTag::cFishTag(TiXmlElement*e_pTemplateXml,cAnimationMesh*e_pFishModel,Vector3 e_vModelPosition):cGameItemInfoButton(e_pTemplateXml)
{
	m_pFishModel = 0;
	if( e_pFishModel )
	{
		m_pFishModel = new cAnimationMesh(e_pFishModel);
		m_pFishModel->SetLocalPosition(e_vModelPosition);
		this->AddChild(m_pFishModel,false);
		m_vModelPos = e_vModelPosition;
	}
}

cFishTag::cFishTag(cFishTag*e_pFishTag):cGameItemInfoButton(e_pFishTag)
{
	m_pFishModel = 0;
	if( e_pFishTag->m_pFishModel )
	{
		m_pFishModel = new cAnimationMesh(e_pFishTag->m_pFishModel);
		this->AddChild(m_pFishModel,false);
	}
}

cFishTag::~cFishTag()
{
	SAFE_DELETE(m_pFishModel);
}
void	cFishTag::Update(float e_fElpaseTime)
{
	if( m_pFishModel )
		m_pFishModel->Update(e_fElpaseTime);
	cGameItemInfoButton::Update(e_fElpaseTime);
}

void	cFishTag::Render()
{
	UseShaderProgram();
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	cGameItemInfoButton::Render();
	if( m_pFishModel )
	{
		UseShaderProgram(SKINNING_MESH_SHADER);
		glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		Vector4	l_vRect = m_pFishModel->Get2DCollisionRect();
		Vector3	l_vPos =Vector3::Zero;// m_vModelPos;
		l_vPos.x += l_vRect.Width()/2;
		l_vPos.y += l_vRect.Height()/2;
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::XAxisRotationMatrix(D3DXToRadian(-30))*cMatrix44::YAxisRotationMatrix(D3DXToRadian(-90));
		m_pFishModel->SetLocalTransform(l_mat);
		m_pFishModel->Render();
	}
}

void	cFishTag::SetModel(cAnimationMesh*e_pFishModel,Vector3 e_vPos)
{
	if( e_pFishModel )
	{
		SAFE_DELETE(m_pFishModel);
		m_pFishModel = new cAnimationMesh(e_pFishModel);
		m_pFishModel->SetLocalPosition(e_vPos);
		m_pFishModel->SetParent(this,false);
		m_vModelPos = e_vPos;
	}
}

void	cFishTag::Init()
{
	Vector4	l_v = this->m_vCollisionRange;
	cGameItemInfoButton::Init();
	Vector4	l_v2 = this->m_vCollisionRange;
	int a=0;
}

void	cFishTag::SetModel(const WCHAR*e_strModelName)
{
	if( !e_strModelName )
		return;
	std::wstring	l_strFileName = L"Image/Fish/";
	l_strFileName += e_strModelName;
	l_strFileName += L".DAE";
	cColladaParser*l_pColladaParser = (cColladaParser*)cGameApp::m_spColladaParserVector->GetObject(l_strFileName.c_str());
	if( !l_pColladaParser && e_strModelName != NULL)//for debug test
	{
		l_pColladaParser = (cColladaParser*)cGameApp::m_spColladaParserVector->GetObject(0);
	}
	if( l_pColladaParser )
	{
		if( l_pColladaParser->m_pAllAnimationMesh )
		{
			SAFE_DELETE(m_pFishModel);
			cAnimationMesh*l_pAnimationMesh = l_pColladaParser->m_pAllAnimationMesh->GetObject(0);
			m_pFishModel = new cAnimationMesh(l_pAnimationMesh);
		}
	}
}

//cFishTagManager::cFishTagManager()
//{
//
//}
//
//cFishTagManager::~cFishTagManager()
//{
//
//}
//
////set data to correct
//cFishTagManager::Init()
//{
//
//}
//
//cFishTagManager::Update(float e_fElpaseTime)
//{
//
//}
//
//cFishTagManager::Render()
//{
//
//}
//
//cFishTagManager::DebugRender()
//{
//
//}
//
////
//cFishTagManager::MouseMove(int e_iPosX,int e_iPosY)
//{
//
//}
//
//cFishTagManager::MouseDown(int e_iPosX,int e_iPosY)
//{
//
//}
//
//cFishTagManager::MouseUp(int e_iPosX,int e_iPosY)
//{
//
//}
//<cFishTagManager>
//	//<cGameItemInfoButton>
//	//	<cButtonDialog BG="Road_sub_pole">
//	//		<cMPDI cMPDIList="Image/In_Stage.mpdi" cMPDI="Stage_Select"/>
//	//	</cButtonDialog>
//	//	<cRenderObjectVector>
//	//		<cFontWithFrame  Text="Probability" >
//	//			<cSubMPDI cMPDIList="Image/In_Stage.mpdi" cMPDI="Stage_Select" cSubMPDI="Detial_No_000"/>
//	//		</cFontWithFrame>
//	//		<cFontWithFrame  Text="Cost" >
//	//			<cSubMPDI cMPDIList="Image/In_Stage.mpdi" cMPDI="Stage_Select" cSubMPDI="Detal_No_001"/>
//	//		</cFontWithFrame>
//	//		<cFontWithFrame  Text="BrakeTime" >
//	//			<cSubMPDI cMPDIList="Image/In_Stage.mpdi" cMPDI="Stage_Select" cSubMPDI="Detal_No_002"/>
//	//		</cFontWithFrame>
//	//	</cRenderObjectVector>
//	//</cGameItemInfoButton>
//</cFishTagManager>
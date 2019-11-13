#include "stdafx.h"
#include "FishBehavior.h"
#include "GameApp.h"
//<cFishBehavior ID="" EnableFishLeaving = "1">
//		<DaeModel File="" />
//		<Collide FileName="" Name="" />
//		<cFAICharacterInterface Speed="50" RunSpeed="100"/>
//</cFishBehavior>
cFishBehavior::cFishBehavior(const char*e_strFileName,cNamedTypedObjectVector<cColladaParser>*e_pCollada,cNamedTypedObjectVector<c2DImageCollisionData>*e_pCollision):cFAICharacterInterface()
{
	m_fLeaveProbability = 0.1f;
	m_HeightRange = Vector2(1,10);
	m_WidthRange = Vector2(1,10);
	m_GiveMoneyRange.x = 10;m_GiveMoneyRange.y = 100;
	m_GiveGold.x = m_GiveGold.y = 0;
	m_fStamina = 10.f;
	m_UnhookTC.SetTargetTime(-1.f);
	m_fCoseStamina = 1.f;
	m_pCollideShape = 0;
	m_pAnimationMesh = 0;
	m_bStunningable = true;
	m_eFishBehavior = eFB_NONE;
	this->SetName(UT::GetFileNameWithoutFullPath(e_strFileName).c_str());
	cNodeISAX	l_File;
	if(l_File.ParseDataIntoXMLNode(e_strFileName))
	{
		TiXmlElement*l_pElement = l_File.GetRootElement();
		ProcessXmlData(l_pElement,e_pCollada,e_pCollision);
		TiXmlElement*l_pFAICharacterInterfaceElement = GetXmlElementByNameFromElement(cFAICharacterInterface::TypeID,l_pElement);
		cFAICharacterInterface::ProcessXmlData(l_pFAICharacterInterfaceElement);
	}
}

//cFishBehavior::cFishBehavior(TiXmlElement*e_pElement,cNamedTypedObjectVector<cColladaParser>*e_pCollada,cNamedTypedObjectVector<c2DImageCollisionData>*e_pCollision):cFAICharacterInterface(e_pElement)
//{
//	m_fCoseStamina = 1.f;
//	m_pCollideShape = 0;
//	m_pAnimationMesh = 0;
//	m_bStunningable = false;
//	m_eFishBehavior = eFB_NONE;
//	ProcessData(e_pElement,e_pCollada,e_pCollision);
//
//}
cFishBehavior::cFishBehavior(cFishBehavior*e_pFishBehavior):cFAICharacterInterface(e_pFishBehavior)
{
	this->SetName(e_pFishBehavior->GetName());
	m_fLeaveProbability = e_pFishBehavior->m_fLeaveProbability;
	m_HeightRange = e_pFishBehavior->m_HeightRange;
	m_WidthRange = e_pFishBehavior->m_WidthRange;
	m_GiveMoneyRange = e_pFishBehavior->m_GiveMoneyRange;
	m_GiveGold = e_pFishBehavior->m_GiveGold;
	m_fStamina = e_pFishBehavior->m_fStamina;
	m_UnhookTC.SetTargetTime(-1.f);
	//m_matDummy = e_pFishBehavior->m_matDummy;
	m_fCoseStamina = e_pFishBehavior->m_fCoseStamina;
	m_pCollideShape = 0;
	m_pAnimationMesh = 0;
	if( e_pFishBehavior->m_pCollideShape )
		m_pCollideShape = dynamic_cast<cbtShapeCollision*>(e_pFishBehavior->m_pCollideShape->Clone());
	if( e_pFishBehavior->m_pAnimationMesh )
		m_pAnimationMesh = dynamic_cast<cAnimationMesh*>(e_pFishBehavior->m_pAnimationMesh->Clone());
	m_bStunningable = e_pFishBehavior->m_bStunningable;
	m_eFishBehavior = eFB_NONE;
}

cFishBehavior::~cFishBehavior()
{
	SAFE_DELETE(m_pCollideShape);
	SAFE_DELETE(m_pAnimationMesh);
}

void	cFishBehavior::ProcessDaeModelData(TiXmlElement*e_pElement)
{
	SAFE_DELETE(m_pAnimationMesh);
	cColladaParser*l_pColladaParser = cFishingFunApp::ProcessDaeFile(e_pElement);
	if( l_pColladaParser )
	{
		cLibraryVisualScenes*l_pLibraryVisualScenes = l_pColladaParser->GetLibraryVisualScenes();
		if( l_pColladaParser->m_pAllAnimationMesh != 0 )
		{
			int	l_iCount = l_pColladaParser->m_pAllAnimationMesh->Count();
			if( l_iCount )
			{
				//m_matDummy = l_pLibraryVisualScenes->m_VisualSceneData.AllNodesVector[4]->mat;
				m_pAnimationMesh = dynamic_cast<cAnimationMesh*>(l_pColladaParser->m_pAllAnimationMesh->GetObject(0)->Clone());
				cBone*l_pRoot = m_pAnimationMesh->GetMainRootBone();
				Vector4	l_vViewRect = m_pAnimationMesh->Get2DCollisionRect();
				Vector2	l_vEdge(l_vViewRect.Width(),l_vViewRect.Height());
				sSphere	l_Sphere;
				l_Sphere.fRadius = l_vEdge.Length()/2;
				l_Sphere.vCenter = Vector3(l_vViewRect.Width()/2+l_vViewRect.x,l_vViewRect.Height()/2+l_vViewRect.y,0);		
				m_pAnimationMesh->SetSphere(l_Sphere);
			}
		}
	}
}

void	cFishBehavior::ProcessCollisionData(TiXmlElement*e_pElement)
{
	const WCHAR*l_strType = e_pElement->Attribute(L"Type");
	if( !wcscmp(l_strType,L"Collada") )
	{
		cColladaParser*l_pColladaParser = cFishingFunApp::ProcessDaeFile(e_pElement);
		std::wstring	l_strName = e_pElement->Attribute(L"Name");
		if( l_pColladaParser && l_pColladaParser->m_pAllMesh )
		{
			int l_iSize = l_pColladaParser->m_pAllMesh->Count();
			cMesh*l_pMesh = l_pColladaParser->m_pAllMesh->GetObject(l_strName);
			if( l_pMesh )
			{
				sSphere	l_Sphere = l_pMesh->GetSphere();
				SAFE_DELETE(m_pCollideShape);
				m_pCollideShape = new cbtSphereShape(l_Sphere.fRadius,l_Sphere.vCenter);
			}
		}
	}
	else
	if( !wcscmp(l_strType,L"Bullet") )
	{
		UT::ErrorMsg(L"not support yet",L"not support yet");
	}
	else
	{
		UT::ErrorMsg(L"not support yet",L"not support yet");
	}
}

void	cFishBehavior::ProcessXmlData(TiXmlElement*e_pElement,cNamedTypedObjectVector<cColladaParser>*e_pCollada,cNamedTypedObjectVector<c2DImageCollisionData>*e_pCollision)
{
	const WCHAR*l_str = e_pElement->Attribute(L"EnableFishLeaving");
	const WCHAR*l_strStamina = e_pElement->Attribute(L"Stamina");
	const WCHAR*l_strPlace = e_pElement->Attribute(L"Place");
	const WCHAR*l_strHeight = e_pElement->Attribute(L"Height");
	const WCHAR*l_strWidth = e_pElement->Attribute(L"Width");
	const WCHAR*l_strMoney = e_pElement->Attribute(L"Money");
	const WCHAR*l_strGold = e_pElement->Attribute(L"Gold");

	if( l_strHeight )
		m_HeightRange = GetVector2(l_strHeight);
	if( l_strWidth )
		m_WidthRange = GetVector2(l_strWidth);
	if( l_strMoney )
		m_GiveMoneyRange = GetPoint(l_strMoney);
	if( l_strStamina )
		m_fStamina = GetFloat(l_strStamina);
	if( l_strPlace )
		m_strShowPlace = GetWStringListByCommaDivide(l_strPlace);
	if( l_strGold )
		m_GiveGold = GetPoint(l_strGold);

	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE("DaeModel")
		{
			if( e_pCollada )
				ProcessDaeModelData(e_pElement);
		}
		else
		COMPARE_VALUE("Collision")
		{
			if( e_pCollada )
				ProcessCollisionData(e_pElement);
		}
		else
		COMPARE_VALUE("Description")
		{
			const WCHAR*l_strChangeLineLength = e_pElement->Attribute(L"ChangeLineLength");
			const WCHAR*l_strText = e_pElement->Attribute(L"Text");
			if( l_strChangeLineLength )
				m_Description.iChangeLineLength = GetInt(l_strChangeLineLength);
			if( l_strText )
				m_Description.strDescription = l_strText;
			else
				m_Description.strDescription = L"No Data!";
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}

void	cFishBehavior::Init()
{
	m_Path.Init();
	if( m_pAnimationMesh )
		m_pAnimationMesh->SetParent(0);
	m_eFishBehavior = eFB_MOVING;
}

void		cFishBehavior::UpdateVisibleRect()
{
	if( m_pAnimationMesh )
	{
		Vector3	l_vPos = m_pAnimationMesh->GetWorldPosition();
		Vector4	l_vRect = m_pAnimationMesh->Get2DCollisionRect();
		l_vPos.x -= l_vRect.Width()/2;
		l_vPos.y -= l_vRect.Height()/2;
		m_vVisibleRect.x = l_vPos.x;
		m_vVisibleRect.y = l_vPos.y;
		m_vVisibleRect.z = l_vPos.x+l_vRect.Width();
		m_vVisibleRect.w = l_vPos.y+l_vRect.Height();
	}
}

void		cFishBehavior::Update(float e_fElpaseTime)
{
	if( m_eFishBehavior == eFB_HOOKED || m_eFishBehavior == eFB_UNHOOKED )
	{
		if(  e_fElpaseTime == 0 )
			e_fElpaseTime = 0.016f;
		m_pAnimationMesh->Update(e_fElpaseTime*frand(2,15));
		if( m_UnhookTC.fRestTime > 0.f )
		{
			m_UnhookTC.Update(e_fElpaseTime);
			if( m_UnhookTC.bTragetTimrReached )
			{
				this->m_pAnimationMesh->SetParent(0);
				m_eFishBehavior = eFB_UNHOOKED;
				UpdateVisibleRect();
			}
		}
		return;
	}
	//m_bCollided = false;
	if( this->m_eFishBehavior == eFB_STUNNING_TO_LEAVE )
		e_fElpaseTime *= (this->m_fRunSpeed/(this->m_fSpeed+1.f));
	m_Path.Update(e_fElpaseTime);
	if(  e_fElpaseTime == 0 )
		e_fElpaseTime = 0.016f;
	if( m_pAnimationMesh )
	{
		float	l_fAngle2 = -30.f;
		float	l_fAngle = 90.f;
		if( m_Path.GetCurrentWorkingFinalListIndex() != 1)
		{
			l_fAngle = -90.f;
			l_fAngle2 = 30;
		}
		m_pAnimationMesh->Update(e_fElpaseTime);
		Vector3	l_vPos = m_Path.GetCurrentPosition();
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::XAxisRotationMatrix(D3DXToRadian(l_fAngle2))*cMatrix44::YAxisRotationMatrix(D3DXToRadian(l_fAngle));
		m_pAnimationMesh->SetLocalTransform(l_mat);
		//Vector4	l_vRect = m_pAnimationMesh->Get2DCollisionRect();
		if( m_pCollideShape )
		{
			Vector4	l_vRect = m_pAnimationMesh->Get2DCollisionRect();
			l_mat = cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::XAxisRotationMatrix(D3DXToRadian(l_fAngle2))*cMatrix44::YAxisRotationMatrix(D3DXToRadian(l_fAngle))*cMatrix44::YupToYDown;
			m_pCollideShape->SetTransform(l_mat);
		}
		UpdateVisibleRect();
		//l_vPos.x -= l_vRect.Width()/2;
		//l_vPos.y -= l_vRect.Height()/2;
		//m_vVisibleRect.x = l_vPos.x;
		//m_vVisibleRect.y = l_vPos.y;
		//m_vVisibleRect.z = l_vPos.x+l_vRect.Width();
		//m_vVisibleRect.w = l_vPos.y+l_vRect.Height();
	}
}

void	cFishBehavior::Render()
{
	if( m_pAnimationMesh )
	{
		m_pAnimationMesh->Render();
	}
}

void	cFishBehavior::RenderData()
{
	if( m_pAnimationMesh )
	{
		std::wstring	l_strInfo = L"Stamina:";
		l_strInfo += ValueToStringW((int)m_fStamina);
		l_strInfo += L"\n";
		l_strInfo += L"HookedProbability:";
		l_strInfo += ValueToStringW((int)((1-m_fLeaveProbability)*100));
		Vector3	l_vPos = m_pAnimationMesh->GetWorldPosition();
		Vector4	l_vRect = m_pAnimationMesh->Get2DCollisionRect();
		//Vector3	l_vPos = m_Path.GetCurrentPosition();
		l_vPos.y -= l_vRect.Height()/2;

		cGameApp::RenderFont(l_vPos.x,l_vPos.y,l_strInfo);
	}
}

void		cFishBehavior::DebugRender()
{
	if( m_pAnimationMesh )
	{
		Vector3	l_vPos = m_pAnimationMesh->GetWorldPosition();
		Vector4	l_vRect = m_pAnimationMesh->Get2DCollisionRect();
		//Vector3	l_vPos = m_Path.GetCurrentPosition();
		l_vPos.x -= l_vRect.Width()/2;
		l_vPos.y -= l_vRect.Height()/2;
		//GLRender::RenderRectangle(l_vPos,l_vRect.Width(),l_vRect.Height(),Vector4(1,0,1,1));
		GLRender::RenderRectangle(l_vPos,m_vVisibleRect.Width(),m_vVisibleRect.Height(),m_eFishBehavior == eFB_HOOKED?Vector4(1,0,1,1):Vector4(1,1,1,1));
		//l_vPos.x += l_vRect.Width()/2;
		//l_vPos.y += l_vRect.Height()/2;
		//RenderCircle(Vector2(l_vPos.x,l_vPos.y),5);
		if( m_pCollideShape )
			m_pCollideShape->DebugRender();
		cGameApp::RenderFont(l_vPos.x,l_vPos.y,this->GetName());
	}
}

void		cFishBehavior::SetStatus(eFishBehavior e_eFishBehavior,Frame*e_pTarget,float e_fRestTimeToLeave)
{
	if( e_eFishBehavior == eFB_STUNNING_TO_LEAVE )
	{
		m_eFishBehavior = eFB_STUNNING_TO_LEAVE;
		if( m_pAnimationMesh )
		{
			//this->m_pAnimationMesh->SetParent(0);
			//Vector3	l_vPos = m_pAnimationMesh->GetWorldPosition();
			//float	l_fDistanceToLeave = 0.f;
			//m_Path.Destroy();
			//if( l_vPos.x >= cGameApp::m_svGameResolution.x / 2 )
			//{
			//	l_fDistanceToLeave = cGameApp::m_svGameResolution.x-l_vPos.x;
			//	m_Path.AddPoint(l_vPos,0);
			//	l_vPos.x += l_fDistanceToLeave;
			//	float	l_fTime = l_fDistanceToLeave/this->m_fRunSpeed;
			//	m_Path.AddPoint(l_vPos,l_fTime);
			//}
			//else
			//{
			//	l_fDistanceToLeave = l_vPos.x+this->m_vVisibleRect.Width()*2;
			//	m_Path.AddPoint(l_vPos,0);
			//	l_vPos.x -= l_fDistanceToLeave;
			//	float	l_fTime = l_fDistanceToLeave/this->m_fRunSpeed;
			//	m_Path.AddPoint(l_vPos,l_fTime);
			//}
		}
	}
	else
	if(e_eFishBehavior == eFB_HOOKED)
	{
		float	l_fFaceAngle = 90.f;
		float	l_fZangle = 0.f;
		cMatrix44	l_mat;
		cMatrix44	l_matOffset = cMatrix44::TranslationMatrix(20,770,0);
		Vector4	l_vRect = this->m_pAnimationMesh->Get2DCollisionRect();
		if( m_Path.GetCurrentWorkingFinalListIndex() == 1)
		{
			l_fFaceAngle = -90.f;
		}
		m_fHookedAngle = frand(-100,-150);
		l_mat = l_matOffset*cMatrix44::YAxisRotationMatrix(D3DXToRadian(l_fFaceAngle))*cMatrix44::XAxisRotationMatrix(D3DXToRadian(m_fHookedAngle))*cMatrix44::ZAxisRotationMatrix(D3DXToRadian(180))*cMatrix44::TranslationMatrix(0,0,l_vRect.Width()/2);
		m_pAnimationMesh->SetParent(e_pTarget);
		m_pAnimationMesh->SetLocalTransform(l_mat);
		m_UnhookTC.SetTargetTime(e_fRestTimeToLeave);
		m_eFishBehavior = eFB_HOOKED;
	}
	else
		m_eFishBehavior = e_eFishBehavior;
}

bool		cFishBehavior::IsCollide(cbtShapeCollision*e_pbtShapeCollision)
{
	if( m_pCollideShape && e_pbtShapeCollision )
		return m_pCollideShape->Collide(e_pbtShapeCollision);
	return false;
}

bool		cFishBehavior::IsCollide(Vector2 e_vPos)
{
	if( m_pAnimationMesh )
	{
		bool	l_b = m_vVisibleRect.CollidePoint(e_vPos.x,e_vPos.y);
		return l_b;
	}
	return false;
}

bool		cFishBehavior::IsVisible(Vector4 e_vCameraViewRect)
{
	if( m_pAnimationMesh )
	{
		//return true;
		//if( m_vVisibleRect.x >= e_vCameraViewRect.x &&
			if(m_vVisibleRect.y <= e_vCameraViewRect.w&&
			//m_vVisibleRect.z <= e_vCameraViewRect.z&&
			m_vVisibleRect.w >= e_vCameraViewRect.y)
			return true;
	}
	return false;
}

int		cFishBehavior::SellForGold()
{
	return 1;
}

int		cFishBehavior::SellForCash()
{
	return 100;
}

void	cFishBehavior::GenerateHorizontalPath(float e_fWidth,float e_fHeight)
{
	if( !m_pAnimationMesh )
		return;
	float	l_fRadius = m_pAnimationMesh->GetSphere().fRadius;
	float	l_fTime = (l_fRadius+e_fWidth)/this->m_fSpeed;
	m_Path.Destroy();
	m_Path.AddPoint(Vector3(-e_fWidth,e_fHeight,0),0);
	m_Path.AddPoint(Vector3(e_fWidth+l_fRadius,e_fHeight,0),l_fTime/2.f);
	m_Path.AddPoint(Vector3(-e_fWidth,e_fHeight,0),l_fTime);
	m_Path.SetCurveLoop(true);
	if(rand()%2)
		m_Path.InvertOrder();
	m_Path.Init();
	m_Path.Update(frand(0.01f,l_fTime));
	m_pAnimationMesh->Update(l_fTime);
	Vector3	l_vPos = m_Path.GetCurrentPosition();
	Vector4	l_vRect = m_pAnimationMesh->Get2DCollisionRect();
	l_vPos.x -= l_vRect.Width()/2;
	l_vPos.y -= l_vRect.Height()/2;
	m_vVisibleRect.x = l_vPos.x;
	m_vVisibleRect.y = l_vPos.y;
	m_vVisibleRect.z = l_vPos.x+l_vRect.Width();
	m_vVisibleRect.w = l_vPos.y+l_vRect.Height();
}

bool	cFishBehavior::IsAllowToHook()
{
	return m_eFishBehavior == eFB_MOVING;
}

float	cFishBehavior::GetRandomHeight()
{
	return m_HeightRange.Rand();
}

float	cFishBehavior::GetRandomWidth()
{
	return m_WidthRange.Rand();
}

float	cFishBehavior::GetStamina()
{
	return m_fStamina;
}

void	cFishBehavior::SetLeaveProbability(float e_fLeaveProbability)
{
	m_fLeaveProbability = e_fLeaveProbability;
}

int		cFishBehavior::GetGiveGold()
{
	return UT::rand(m_GiveGold.x,m_GiveGold.y);
}

int		cFishBehavior::GetGiveMoneyByWidth(float e_fWidth)
{
	float	l_fLength = m_WidthRange.Distance();
	if( e_fWidth < m_WidthRange.x )
		return m_GiveMoneyRange.x;
	if( e_fWidth > m_WidthRange.y )
		return m_GiveMoneyRange.y;
	float	l_fLerp = (e_fWidth-m_WidthRange.x)/l_fLength;
	int	l_iCost = (int)((m_GiveMoneyRange.y-m_GiveMoneyRange.x)*l_fLerp)+m_GiveMoneyRange.x;
	return l_iCost;
}

float	cFishBehavior::StruggleToLeaveUpdate()
{
	return frand(0,0.1f);
}

cFishBehaviorManager::cFishBehaviorManager()
{
	m_iCurrentVisibleFishIndex = 0;
	m_VisibleFishes.SetFromResource(true);
	m_BeenHookedFishes.SetFromResource(true);
	cMesh::m_smatAxisTransform = cMatrix44::YupToYDown;
}

cFishBehaviorManager::~cFishBehaviorManager()
{

}

void	cFishBehaviorManager::Init()
{
	m_GeneratedFishes.Destroy();
	m_VisibleFishes.Destroy();
	m_BeenHookedFishes.Destroy();
}

//<StageFishData>
//	<ShowFishes FileName=""/>
//	<ShowFishes FileName=""/>
//</StageFishData>

void	cFishBehaviorManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
{

}

void	cFishBehaviorManager::DoFishGameBehaviorUpdate(float e_fElpaseTime)
{

}

void	cFishBehaviorManager::Update(float e_fElpaseTime,Vector4 e_vCameraViewRect,bool e_bSinking)
{
	m_VisibleFishes.Destroy();
	int	l_iCount = m_GeneratedFishes.Count();
	int	l_iViewCount = 0;
	for( int i=0;i<l_iCount;i++)
	//for( int i=m_iCurrentVisibleFishIndex;e_bSinking?i<l_iCount:i>-1;e_bSinking?i++:i-- )
	{
		cFishBehavior*l_pFish = m_GeneratedFishes[i];
		if((l_pFish->IsAllowToHook() ||l_pFish->m_eFishBehavior == cFishBehavior::eFB_STUNNING_TO_LEAVE)&& l_pFish->IsVisible(e_vCameraViewRect) )
		{
			m_VisibleFishes.AddObjectNeglectExist(l_pFish);
			l_pFish->Update(e_fElpaseTime);
			++l_iViewCount;
		}
		else
		if( l_pFish->m_eFishBehavior == cFishBehavior::eFB_STUNNING_TO_LEAVE )
		{
			l_pFish->SetStatus(cFishBehavior::eFB_MOVING);
		}
	}
	//DoFishGameBehaviorUpdate(e_fElpaseTime);
	l_iCount = m_BeenHookedFishes.Count();
	for( int i=0;i<l_iCount;i++)
	{
		cFishBehavior*l_pFish = m_BeenHookedFishes[i];
		l_pFish->Update(e_fElpaseTime);
		if(l_pFish->m_eFishBehavior == cFishBehavior::eFB_UNHOOKED)
		{
			if(!l_pFish->IsVisible(e_vCameraViewRect))
			{
				m_BeenHookedFishes.RemoveObjectWithoutDelete(i);
				i--;
				l_iCount -= 1;
			}
		}
	}
}

void	cFishBehaviorManager::Render()
{
	int	l_iCount = m_VisibleFishes.Count();
	for( int i=0;i<l_iCount;i++)
	{
		m_VisibleFishes[i]->Render();
	}
	l_iCount = m_BeenHookedFishes.Count();
	for( int i=0;i<l_iCount;i++)
	{
		m_BeenHookedFishes[i]->Render();
	}
}

void	cFishBehaviorManager::RenderData()
{
	int	l_iCount = m_VisibleFishes.Count();
	for( int i=0;i<l_iCount;i++)
	{
		m_VisibleFishes[i]->RenderData();
	}
}

void	cFishBehaviorManager::DebugRender()
{
	int	l_iCount = m_VisibleFishes.Count();
	for( int i=0;i<l_iCount;i++)
	{
		m_VisibleFishes[i]->DebugRender();
	}
}

void	cFishBehaviorManager::RenderInfo()
{

}

void	cFishBehaviorManager::DoRodCollide(cbtShapeCollision*e_pbtShapeCollision,Frame*e_pHookFrame,cFishRodData*e_pFishRodData,float e_fRestTimeToTop,cOrthogonalCamera*e_pOrthogonalCamera,int e_iMoneyToLeave)
{
	int	l_iCount = m_VisibleFishes.Count();
	for( int i=0;i<l_iCount;i++)
	{
		cFishBehavior*l_pFish = m_VisibleFishes[i];
		if(l_pFish->IsAllowToHook() && l_pFish->IsCollide(e_pbtShapeCollision))
		{
			bool	l_bUnHooked = e_pFishRodData->IsFishLeaving(l_pFish->GetName());
			m_VisibleFishes[i]->SetStatus(cFishBehavior::eFB_HOOKED,e_pHookFrame,l_bUnHooked?frand(0.1f,e_fRestTimeToTop):-1.f);
			m_BeenHookedFishes.AddObjectNeglectExist(m_VisibleFishes[i]);
			continue;
		}
		if( l_pFish->m_eFishBehavior != cFishBehavior::eFB_STUNNING_TO_LEAVE )
		{
#ifdef	WIN32
			Vector2	l_vTouchPos = e_pOrthogonalCamera->ConvertMousePositionToWorldPosition(Vector2(cGameApp::m_sScreenMousePosition.x,cGameApp::m_sScreenMousePosition.y),Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
			l_vTouchPos += e_pOrthogonalCamera->GetCameraPos();
			if(cGameApp::m_sbTouched && l_pFish->IsCollide(l_vTouchPos))
			{
				if(cFishingFunApp::m_spPlayerData->MoneyMinus(e_iMoneyToLeave))
					l_pFish->SetStatus(cFishBehavior::eFB_STUNNING_TO_LEAVE);
			}
#else
			std::vector<int>l_TouchPointWorkingIndex = cGameApp::m_spMultiTouchPoints->TouchPointWorkingIndex;
			size_t	l_siSize = l_TouchPointWorkingIndex.size();
			for( size_t j=0;j<l_siSize;++j )
			{
				POINT	l_Pos = cGameApp::m_spMultiTouchPoints->TouchPointData[j].ScreenPos;
				Vector2	l_vTouchPos = e_pOrthogonalCamera->ConvertMousePositionToWorldPosition(Vector2(l_Pos.x,l_Pos.y),Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
				l_vTouchPos += e_pOrthogonalCamera->GetCameraPos();
				if(cGameApp::m_sbTouched && l_pFish->IsCollide(l_vTouchPos))
				{
					l_pFish->SetStatus(cFishBehavior::eFB_STUNNING_TO_LEAVE);
					break;
				}
			}
#endif
		}
	}
}

void	cFishBehaviorManager::AddFishData(char*e_strFileName)
{
	PARSE_FAILED_MESSAGE_BOX(this,e_strFileName);
}
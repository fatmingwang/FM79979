#include "../stdafx.h"
#include "Curve.h"
#include "../Image/GlyphFontRender.h"
#include "../OpenGL/GLSL/Shader.h"
#include "../Image/SimplePrimitive.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cCurve);
	TYPDE_DEFINE_MARCO(cCurveWithTime);
	cCurve::cCurve()
	{
		//m_bRotationObject = false;
		m_iLOD = 1;
	}

	cCurve::cCurve(cCurve*e_pCurve)
	{
		m_iLOD = e_pCurve->m_iLOD;
		m_FinallyPointList = e_pCurve->m_FinallyPointList;
		m_OriginalPointList = e_pCurve->m_OriginalPointList;
	}

	cCurve::~cCurve()
	{

	}

	void	cCurve::SetOriginalToFinal()
	{
		this->m_OriginalPointList = this->m_FinallyPointList;
		this->m_iLOD = 1;
	}

	void	cCurve::RenderPoint(float e_fSize,int e_iColorR,int e_iColorG,int e_iColorB)
	{
		int	l_iNumPoint = (int)m_OriginalPointList.size();
		if( l_iNumPoint )
		{
			//draw each point
			Vector4	l_vColor(e_iColorR/255.f,e_iColorG/255.f,e_iColorB/255.f,0.3f);
			RenderPoints(&m_OriginalPointList[0],l_iNumPoint,e_fSize,l_vColor);
		}
	}

	void	cCurve::RenderPointIndex()
	{
		if( !cGameApp::m_spGlyphFontRender )
			return;
		int	l_iNumPoint = (int)m_OriginalPointList.size();
		if( l_iNumPoint == -1 )
			return;
		wchar_t	l_str[TEMP_SIZE];
		for(int i=0;i<l_iNumPoint;++i)
		{
			_itow(i,l_str,10);
			cGameApp::m_spGlyphFontRender->RenderFont((int)m_OriginalPointList[i].x-10,(int)m_OriginalPointList[i].y,l_str);
		}
	}

	bool	cCurve::SetLOD(int e_iLOD)
	{
		assert(e_iLOD>=1&&"LOD can't smaller than 1");
		//if(this->m_iLOD == e_iLOD)
		//	return false;
		m_iLOD = e_iLOD;
		if( m_OriginalPointList.size() == 0 )
			return false;
		m_FinallyPointList = m_OriginalPointList;
		if( m_iLOD>=2 )
		for( int i=0;i<this->m_iLOD-1;++i )
			IncreaseLod();
		//assert(m_iLOD>=2?m_OriginalPointList.size()*(1<<(m_iLOD-1)) == m_FinallyPointList.size():true);
		return true;
	}
	//===============
	//Distance = (Want_Count-1);
	//step = (Original_Count)/Distance
	//for Want_Count-1
	//do Current step +Step
	//do data LERPassign( betwnne LeftPoint and RightPoint )
	//if CurrentStep is bigger than l_iRightPoint
	//l_iRightPoint = CurrentStep,l_iLeftPoint = l_iRightPoint-1
	//===============
	//void	cCurve::ReplacePointsByCount(int e_iNumPoint)
	//{
	//	if( !e_iNumPoint )
	//		return;
	//	this->m_OriginalPointList.clear();
	//	int	l_iNumPoint = this->m_FinallyPointList.size();
	//	//less than 2 points,could not to be lerp
	//	if( l_iNumPoint<2 )
	//		return;
	//	float	l_fStep = (float)l_iNumPoint/(e_iNumPoint-1);
	//	float	l_fCurrentStep = l_fStep;
	//	int	l_iLeftPoint = 0;
	//	int	l_iRightPoint = (int)l_fStep;
	//	//force float value to next point
	//	if( l_fStep-(int)l_fStep )
	//	{
	//		l_iRightPoint = (int)l_fStep+1;
	//	}
	//	m_OriginalPointList.push_back(m_FinallyPointList[0]);
	//	e_iNumPoint--;//first point has pushed
	//	for( int i=0;i<e_iNumPoint;++i )
	//	{
	//		//make sure the theory is correct
	//		assert(l_iLeftPoint != l_iRightPoint);
	//		//find distance
	//		Vector3	l_vPos = m_FinallyPointList[l_iRightPoint]-m_FinallyPointList[l_iLeftPoint];
	//		//do LERP,find step
	//		l_vPos*=(1-((l_iRightPoint-l_fCurrentStep)/l_iRightPoint));
	//		//add start pos
	//		l_vPos+=m_FinallyPointList[l_iLeftPoint];
	//		//push into data
	//		this->m_OriginalPointList.push_back(l_vPos);
	//		//step move
	//		l_fCurrentStep+=l_fStep;
	//		//
	//		if( (int)l_fCurrentStep>l_iRightPoint )
	//		{
	//			if( l_iRightPoint<m_FinallyPointList.size() )
	//			{
	//				l_iRightPoint = (int)l_fCurrentStep;
	//				l_iLeftPoint = l_iRightPoint-1;
	//			}
	//		}
	//	}
	//	SetLOD(1);
	//}

	void	cCurve::RenderCurve(Vector4 e_vColor,cMatrix44 e_mat)
	{
		if( m_FinallyPointList.size() < 2 )
		{
			if( m_FinallyPointList.size() == 1 )
				RenderPoints(&m_OriginalPointList[0],(int)m_OriginalPointList.size(),10,e_vColor,e_mat);
			return;
		}
		//draw line
		std::vector<Vector3>	l_CurvePointVector;
		int	l_iNumPoint = ((int)m_FinallyPointList.size()-1)*2;
		l_CurvePointVector.resize(l_iNumPoint);
		for( int i=0;i<l_iNumPoint/2;++i )
		{
			l_CurvePointVector[i*2] = m_FinallyPointList[i];
			l_CurvePointVector[i*2+1] = m_FinallyPointList[i+1];
		}
		RenderLine((float*)&l_CurvePointVector[0],l_iNumPoint,e_vColor,3,e_mat);
		RenderPoints(&m_OriginalPointList[0],(int)m_OriginalPointList.size(),10,e_vColor,e_mat);
	}

	//------------------------------------------------------------	IncreaseLod()
	// When we increase the LOD we will have to re-create the points
	// array inserting the new intermediate points into it.
	//
	//	Basically the subdivision works like this. each line,
	//
	//            A  *------------*  B
	//
	//	will be split into 2 new points, Q and R.
	//
	//                   Q    R
	//            A  *---|----|---*  B
	//
	//	Q and R are given by the equations :
	//
	// 		Q = 3/4*A + 1/4*B
	// 		R = 3/4*B + 1/4*A
	//
	void cCurve::IncreaseLod()
	{
		std::vector<Vector3> NewPoints;

		// keep the first point
		NewPoints.push_back(m_FinallyPointList[0]);
		for(unsigned int i=0;i<(m_FinallyPointList.size()-1);++i) {
		
			// get 2 original points
			const Vector3& p0 = m_FinallyPointList[i];
			const Vector3& p1 = m_FinallyPointList[i+1];
			Vector3 Q;
			Vector3 R;

			// average the 2 original points to create 2 new points. For each
			// CV, another 2 verts are created.
			Q.x = 0.75f*p0.x + 0.25f*p1.x;
			Q.y = 0.75f*p0.y + 0.25f*p1.y;
			Q.z = 0.75f*p0.z + 0.25f*p1.z;
			//Q.z = 0.75f*p0.z + 0.25f*p1.z;

			R.x = 0.25f*p0.x + 0.75f*p1.x;
			R.y = 0.25f*p0.y + 0.75f*p1.y;
			R.z = 0.25f*p0.z + 0.75f*p1.z;
			//R.z = 0.25f*p0.z + 0.75f*p1.z;

			NewPoints.push_back(Q);
			NewPoints.push_back(R);
		}
		// keep the last point
		NewPoints.push_back(m_FinallyPointList[m_FinallyPointList.size()-1]);

		// update the points array
		m_FinallyPointList = NewPoints;
	}
	//------------------------------------------------------------	DecreaseLod()
	// When we decrease the LOD, we can rather niftily get back
	// to exactly what we had before. Since the original subdivision
	// simply required a basic ratio of both points, we can simply
	// reverse the ratio's to get the previous point...
	//
	void cCurve::DecreaseLod()
	{
		// make sure we dont loose any points!!
		if (m_FinallyPointList.size()<=2) 
			return;

		std::vector<Vector3> NewPoints;

		// keep the first point
		NewPoints.push_back(m_FinallyPointList[0]);

		// step over every 2 points
		for(unsigned int i=1;i<(m_FinallyPointList.size()-1);i+=2) {

			// get last point found in reduced array
			const Vector3& pLast = m_FinallyPointList[m_FinallyPointList.size()-1];

			// get 2 original point
			const Vector3& p0 = m_FinallyPointList[i];
			Vector3 Q;

			// calculate the original point
			Q.x = p0.x - 0.75f*pLast.x;
			Q.y = p0.y - 0.75f*pLast.y;
			Q.z = p0.z - 0.75f*pLast.z;

			Q.x *= 4.0f;
			Q.y *= 4.0f;
			Q.z *= 4.0f;

			// add to new curve
			NewPoints.push_back(Q);
		}

		// copy over points
		m_FinallyPointList = NewPoints;
	}
	void	cCurve::DoLOD()
	{
		m_FinallyPointList = m_OriginalPointList;
		if( m_FinallyPointList.size() == 0 )
			return;
		if( m_iLOD>=2 )
		for( int i=0;i<this->m_iLOD-1;++i )
			IncreaseLod();
//		assert(m_iLOD>=2?m_OriginalPointList.size()*(1<<(m_iLOD-1)) == m_FinallyPointList.size():true);
	}
	//
	void	cCurve::AddPoint(Vector3 e_vPos)
	{
		m_OriginalPointList.push_back(e_vPos);
		DoLOD();
	}
	//
	void	cCurve::InsertPoint(Vector3 e_vPos,int e_iIndex)
	{
		if( e_iIndex >= (int)this->m_OriginalPointList.size() )
			this->AddPoint(e_vPos);
		else
		{
			this->m_OriginalPointList.insert(m_OriginalPointList.begin()+e_iIndex,e_vPos);
			this->DoLOD();
		}
	}
	//
	void	cCurve::DelPoint(int e_iIndex)
	{
		m_OriginalPointList.erase(m_OriginalPointList.begin()+e_iIndex);
		m_FinallyPointList = m_OriginalPointList;
		if( m_iLOD>=2 )
		for( int i=0;i<this->m_iLOD-1;++i )
			IncreaseLod();
	}

	void	cCurve::FixPoint(int e_iIndex,Vector3 e_vPos)
	{
		this->m_OriginalPointList.erase(m_OriginalPointList.begin()+e_iIndex);
		m_OriginalPointList.insert(m_OriginalPointList.begin()+e_iIndex,e_vPos);
		DoLOD();
	}

	int		cCurve::GetClosetPointIndex(int e_x,int e_y,int e_z)
	{
		Vector3	l_Target = Vector3((float)e_x,(float)e_y,(float )e_z);
		int	l_iNumPoint = (int)m_OriginalPointList.size();
		int	l_ismallestPointIndex = -1;
		float	l_fSmallestRange = 10000000;//asume this is maxima value
		for( int i=0;i<l_iNumPoint;++i )
		{
			Vector3	l_Vector3 = m_OriginalPointList[i];
			float	l_fLength = (l_Target-l_Vector3).Length();
			if(l_fSmallestRange > l_fLength )
			{
				l_fSmallestRange = l_fLength;
				l_ismallestPointIndex = i;
			}
		}
		return l_ismallestPointIndex;
	}

	void	cCurve::AddOffsetToAllPoint(Vector3 e_vOffset)
	{
		int	l_iNumPoint = (int)m_OriginalPointList.size();
		for( int i=0;i<l_iNumPoint;++i )
		{
			Vector3*l_pVector3 = &m_OriginalPointList[i];
			*l_pVector3+=e_vOffset;
		}
		this->DoLOD();
	}

	void	cCurve::Destroy()
	{
		m_OriginalPointList.clear();
		m_FinallyPointList.clear();
	}

	int		cCurve::GetClosestPointIndex(Vector3 e_vPos,float e_fOffsetDis)
	{
		return UT::GetClosestPointIndex(e_vPos,&m_OriginalPointList,e_fOffsetDis);
	}

	Vector3	cCurve::GetCurveCenter(float*e_pfRightDownToLeftUpperLength)
	{
		int	l_iNum = (int)m_OriginalPointList.size();
		if( l_iNum)
		{
			Vector3	l_veLeftUp = m_OriginalPointList[0];
			Vector3	l_veRightDown = l_veLeftUp;
			for( int i=1;i<l_iNum;++i )
			{
				Vector3	l_vPos = m_OriginalPointList[i];
				if( l_veLeftUp.x>l_vPos.x )
					l_veLeftUp.x = l_vPos.x;
				if( l_veLeftUp.y>l_vPos.y )//2d so y up is getting small and small
					l_veLeftUp.y = l_vPos.y;
				if( l_veLeftUp.z>l_vPos.z )
					l_veLeftUp.z = l_vPos.z;
				if( l_veRightDown.x<l_vPos.x )
					l_veRightDown.x = l_vPos.x;
				if( l_veRightDown.y<l_vPos.y )
					l_veRightDown.y = l_vPos.y;
				if( l_veRightDown.z<l_vPos.z )
					l_veRightDown.z = l_vPos.z;
			}
			if( e_pfRightDownToLeftUpperLength )
				*e_pfRightDownToLeftUpperLength = (l_veRightDown-l_veLeftUp).Length();
			return (l_veLeftUp+l_veRightDown)/2.f;
		}
		return Vector3::Zero;
	}
	//==========================================
	//first find the center point
	//get the length from left lower and right upper
	//get the final point foreach point's proper position
	//original position minus center point divide the length
	//then the final vector add center point,got it!
	//==========================================
	void	cCurve::Scale(float e_fScale)
	{
		int	l_iNum = (int)m_OriginalPointList.size();
		if( l_iNum)
		{
			float	l_fLength = 0.f;
			Vector3	l_vCenter = GetCurveCenter(&l_fLength);
			for( int i=0;i<l_iNum;++i )
			{
				//scale the point as vector length 1
				Vector3	*l_pvPos = &m_OriginalPointList[i];
				//point minus center
				Vector3	l_vTargetVector = *l_pvPos - l_vCenter;
				//set to proper position
				l_vTargetVector *= e_fScale;
				//final position we expect
				*l_pvPos = l_vTargetVector+l_vCenter;
			}
			this->DoLOD();
		}
	}

	void	cCurve::ScaleByDistance(float e_fDistance)
	{
		int	l_iNum = (int)m_OriginalPointList.size();
		if( l_iNum)
		{
			float	l_fLength = 0.f;
			Vector3	l_vCenter = GetCurveCenter(&l_fLength);
			for( int i=0;i<l_iNum;++i )
			{
				//scale the point as vector length 1
				Vector3	*l_pvPos = &m_OriginalPointList[i];
				//point minus center
				Vector3	l_vTargetVector = *l_pvPos - l_vCenter;
				//set to proper position
				l_vTargetVector += l_vTargetVector.Normalize()*e_fDistance;
				//final position we expect
				*l_pvPos = l_vTargetVector+l_vCenter;
			}
			this->DoLOD();
		}	
	}

	int	cCurve::FinalListIndexToOriginalIndex(int e_iFinalListIndex)
	{
		size_t	l_iOriginalSize = m_OriginalPointList.size();
		size_t	l_iFinalSize = m_FinallyPointList.size();
		if( l_iFinalSize == l_iOriginalSize )
		{
			return e_iFinalListIndex;
		}
		float	l_fPercentage = (float)l_iOriginalSize/(float)l_iFinalSize;
		return (int)(e_iFinalListIndex*l_fPercentage);
	}

	void	cCurve::TransformCurve(cMatrix44 e_mat)
	{
		size_t	l_uiSize = this->m_OriginalPointList.size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			Vector3	l_vNewPos = e_mat.TransformVector(m_OriginalPointList[i]);
			m_OriginalPointList[i] = l_vNewPos;
		}
		this->DoLOD();
	}

	void	cCurve::Translate(Vector3 e_vPos)
	{
		size_t	l_uiSize = this->m_OriginalPointList.size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			Vector3	l_vNewPos = m_OriginalPointList[i]+e_vPos;
			m_OriginalPointList[i] = l_vNewPos;
		}
		this->DoLOD();
	}
	//============================
	//1st find the center
	//2nd if center is not Vector3::zero all point minus this
	//3rd now the center is Vector3::Zero,transform it
	//4th add original canter to each point
	//============================
	void	cCurve::RotateCurveWithCurveCenter(cMatrix44 e_mat)
	{
		int	l_iNum = (int)m_OriginalPointList.size();
		if( l_iNum)
		{
			float	l_fLength = 0.f;
			Vector3	l_vCenter = GetCurveCenter(&l_fLength);
			if( l_vCenter != Vector3::Zero )
			{
				for( int i=0;i<l_iNum;++i )
				{
					Vector3	*l_pvPos = &m_OriginalPointList[i];
					*l_pvPos -= l_vCenter;
					*l_pvPos = e_mat.TransformVector(*l_pvPos);
					*l_pvPos += l_vCenter;
				}
			}
			this->DoLOD();
		}
	}


	void	cCurve::RotateCurveWithSpecificCenterPosition(Vector3 e_vCenterPos,Quaternion e_Quaternion)
	{
		size_t	l_uiSize = m_OriginalPointList.size();
		for( size_t i =0;i<l_uiSize;++i )
		{
			Vector3	l_vOffset = m_OriginalPointList[i]-e_vCenterPos;
			cMatrix44	l_mat = e_Quaternion.ToMatrix()*cMatrix44::TranslationMatrix(l_vOffset);
			m_OriginalPointList[i] = l_mat.GetAxis(MyMath::W)+e_vCenterPos;
		}
		this->DoLOD();
	}

	float	cCurve::GetTotalDistance()
	{
		float	l_fDis = 0;
		int	l_iSize = (int)this->m_OriginalPointList.size()-1;
		for( int i=0;i<l_iSize;++i )
		{
			Vector3	l_vDistance = m_OriginalPointList[i+1]-m_OriginalPointList[i];
			l_fDis += l_vDistance.Length();
		}
		return l_fDis;
	}

	Vector3	cCurve::GetLastPoint()
	{
		if( this->m_OriginalPointList.size()>0 )
			return m_OriginalPointList[m_OriginalPointList.size()-1];
		return Vector3::Zero;
	}

	Vector3	cCurve::GetInterplotValue(float e_fInterpolationValue)
	{
		size_t	l_iSize = this->m_FinallyPointList.size();
		if( l_iSize < 1 )
		{
			return Vector3::Zero;
		}
		size_t	l_iTargetIndex = (size_t)(e_fInterpolationValue*l_iSize);
		if( e_fInterpolationValue >= 1.f )
		{
			l_iTargetIndex = m_FinallyPointList.size()-1;
		}
		if( e_fInterpolationValue <= 0.f )
		{
			l_iTargetIndex = 0;
		}
		return m_FinallyPointList[l_iTargetIndex];
	}

	cCurve		cCurve::GetInterplotValue(Vector3 e_vStart,Vector3 e_vEnd,int e_iLOD,float e_fInterpolationValue,Vector3*e_vResult)
	{
		//A(start)
		//
		//
		//
		//C			B(end)
		//ABC is a equilateral triangle
		float	l_fXLength = e_vEnd.y-e_vStart.y;
		float	l_fYLength = e_vEnd.y-e_vStart.y;
		//this security is get from MPDI editor vision test.
		Vector3	l_vC(e_vStart.x+l_fXLength/4,e_vStart.y+(l_fYLength/4*3),e_vStart.z);
		cCurve	l_Curve;
		l_Curve.AddPoint(e_vStart);

		l_Curve.AddPoint(l_vC);

		l_Curve.AddPoint(e_vEnd);
		l_Curve.SetLOD(e_iLOD);
		if( e_vResult )
		{
			*e_vResult = l_Curve.GetInterplotValue(e_fInterpolationValue);
		}
		return l_Curve;
	}

	cCurveWithTime::cCurveWithTime()
	{
		m_vCurrentPosition = Vector3::Zero;
		m_fPreviousPosToNextPointAngle = m_fCurrentPosToNextPointAngle = 0.f;
		m_fCurrentTime = 0.f;
		m_iCurrentPointIndex = -1;
		m_iPerviousPointIndex = -1;
		//m_bCurveMoveDone = true;
		m_bCurveLoop = false;
		m_bCalculateAngle = false;
		m_fCurrentLERPTime = 0.f;
		m_vMovedCoordinate = m_vCurrentDirection = Vector3::Zero;
		m_bStayAtLastFrame = true;
	}

	cCurveWithTime::cCurveWithTime(cCurveWithTime*e_pCurveWithTime):cCurve(e_pCurveWithTime),cFMTimeLineAnimationRule(e_pCurveWithTime),Frame(e_pCurveWithTime)
	{
		m_vMovedCoordinate = Vector3::Zero;
		m_vCurrentDirection = e_pCurveWithTime->m_vCurrentDirection;
		m_OriginalTimeList = e_pCurveWithTime->m_OriginalTimeList;
		m_FinalTimeList = e_pCurveWithTime->m_FinalTimeList;
		m_vCurrentPosition = e_pCurveWithTime->m_vCurrentPosition;
		m_fPreviousPosToNextPointAngle = e_pCurveWithTime->m_fPreviousPosToNextPointAngle;
		m_fCurrentPosToNextPointAngle = e_pCurveWithTime->m_fCurrentPosToNextPointAngle;
		m_fCurrentTime = e_pCurveWithTime->m_fCurrentTime;
		m_iCurrentPointIndex = e_pCurveWithTime->m_iCurrentPointIndex;
		m_iPerviousPointIndex = e_pCurveWithTime->m_iPerviousPointIndex;
		//m_bCurveMoveDone = e_pCurveWithTime->m_bCurveMoveDone;
		m_bCurveLoop = e_pCurveWithTime->m_bCurveLoop;
		m_bCalculateAngle = e_pCurveWithTime->m_bCalculateAngle;
		m_fCurrentLERPTime = e_pCurveWithTime->m_fCurrentLERPTime;
	}

	cCurveWithTime::cCurveWithTime(TiXmlElement*e_pTiXmlElement)
	{
		assert(0&&"not support yet!");
	}

	cCurveWithTime::~cCurveWithTime()
	{

	}
	//haha hte data is not same with cCurveManager::ExportData,so ensure the data type is correct.
	//because I am lazy to fix this right now.
	//<cCurveWithTime Name="" Count="" LOD="">
	//	<Data Pos="0,0,0,1,0,0" Time="0,1"/>
	//</cCurveWithTime>
	TiXmlElement*		cCurveWithTime::ToTiXmlElement()
	{
		TiXmlElement*l_pTiXmlElement = new TiXmlElement(cCurveWithTime::TypeID);
		int	l_iCount = (int)m_OriginalTimeList.size();
		l_pTiXmlElement->SetAttribute(L"Name",this->GetName());
		l_pTiXmlElement->SetAttribute(L"Count",(int)m_OriginalPointList.size());
		l_pTiXmlElement->SetAttribute(L"LOD",this->m_iLOD);
		{
			TiXmlElement*l_pCurvePointElement = new TiXmlElement(L"Data");
			l_pTiXmlElement->LinkEndChild(l_pCurvePointElement);
			std::wstring l_CurvePoins;
			std::wstring l_CurveTimes;
			for( int j=0;j<l_iCount;++j )
			{
				Vector3	l_v = m_OriginalPointList[j];
				l_CurvePoins += ValueToStringW(l_v);
				float	l_fTime = m_OriginalTimeList[j];
				l_CurveTimes += ValueToStringW(l_v);
			}
			l_pCurvePointElement->SetAttribute(L"Pos",l_CurvePoins.c_str());
			l_pCurvePointElement->SetAttribute(L"Time",l_CurveTimes.c_str());
		}
		return l_pTiXmlElement;
	}
	//<cCurveWithTime Name="" Count="" LOD="">
	//	<Data Pos="0,0,0,1,0,0" Time="0,1"/>
	//</cCurveWithTime>
	bool cCurveWithTime::ProcessTiXmlElement(TiXmlElement * e_pTiXmlElement)
	{
		this->Destroy();
		bool l_bDataValid = true;
		auto l_strName = e_pTiXmlElement->Attribute(L"Name");
		auto l_strLOD = e_pTiXmlElement->Attribute(L"LOD");
		if (l_strName)
		{
			this->SetName(l_strName);
		}
		if (l_strLOD)
		{
			this->SetLOD(GetInt(l_strLOD));
		}
		auto l_pDataElement = e_pTiXmlElement->FirstChildElement();
		if (l_pDataElement)
		{
			auto l_strPos = l_pDataElement->Attribute(L"Pos");
			auto l_strTime = l_pDataElement->Attribute(L"Time");
			if (!l_strPos || !l_strTime)
			{
				l_bDataValid = false;
			}
			std::vector<Vector3>l_PosVector = StringToVector3Vector(l_strPos);
			std::vector<float>l_fTimeVector = GetValueListByCommaDivide<float>(l_strTime);
			this->SetOriginalPointList(l_PosVector);
			this->SetOriginalTimeList(l_fTimeVector);
			this->DoLOD();

		}
		return l_bDataValid;
	}

	void	cCurveWithTime::SetOriginalToFinal()
	{
		m_OriginalTimeList = m_FinalTimeList;
		this->m_OriginalPointList = this->m_FinallyPointList;
		this->m_iLOD = 1;
	}

	bool	cCurveWithTime::SetLOD(int e_iLOD)
	{
		if( e_iLOD <1 )
			return false;
		if(!cCurve::SetLOD(e_iLOD))
			return false;
		m_FinalTimeList = m_OriginalTimeList;
		if( m_iLOD>=2 )
		for( int i=0;i<this->m_iLOD-1;++i )
			IncreaseTimeLod();
		assert(m_FinalTimeList.size() == m_FinallyPointList.size()&& "final point position size is not equal time list size" );
		return true;
	}

	//correspond LOD for finalpointlist
	void	cCurveWithTime::IncreaseTimeLod()
	{
		if( m_FinalTimeList.size() == 0 )
			return;
		std::vector<float>	l_NewTimesList;
		l_NewTimesList.push_back(m_FinalTimeList[0]);
		
		for( UINT i=0;i<m_FinalTimeList.size()-1;++i )
		{
			float	l_Value = m_FinalTimeList[i];
			float	l_Value2 = m_FinalTimeList[i+1];
			float	l_fStep = (l_Value2-l_Value);

			float	l_fNewValue = l_Value+(l_fStep*0.25f);
			l_NewTimesList.push_back(l_fNewValue);

			l_fNewValue = l_Value+(l_fStep*0.75f);
			l_NewTimesList.push_back(l_fNewValue);
		}
		l_NewTimesList.push_back(m_FinalTimeList[m_FinalTimeList.size()-1]);
		m_FinalTimeList = l_NewTimesList;
	}
	//correspond LOD for finalpointlist
	void	cCurveWithTime::DecreaseTimeLod()
	{
		assert(0&&"lazy to implement");
	}

	void	cCurveWithTime::DoLOD()
	{
		cCurve::DoLOD();
		m_FinalTimeList = m_OriginalTimeList;
		if( m_FinalTimeList.size() == 0 )
			return;
		if( m_iLOD>=2 )
		for( int i=0;i<this->m_iLOD-1;++i )
			IncreaseTimeLod();
		assert(m_FinalTimeList.size() == m_FinallyPointList.size()&& "final point position size is not equal time list size" );
	}

	void	cCurveWithTime::AddPoint(Vector3 e_vPos,float e_fTime)
	{
		//if(m_FinalTimeList.size()==0)
		//	if(e_fTime != 0.f)
		//		assert(0&&"first point time must be zero");
		m_OriginalTimeList.push_back(e_fTime);
		cCurve::AddPoint(e_vPos);
		DoLOD();
	}
	//
	void	cCurveWithTime::DelPoint(int e_iIndex)
	{
		cCurve::DelPoint(e_iIndex);
		m_OriginalTimeList.erase(m_OriginalTimeList.begin()+e_iIndex);
		DoLOD();
	}
	//
	void	cCurveWithTime::InsertPoint(Vector3 e_vPos,float e_fTime,int e_iIndex)
	{
		if( e_iIndex >= (int)this->m_OriginalPointList.size() )
		{
			this->AddPoint(e_vPos,e_fTime);
		}
		else
		{
			this->m_OriginalPointList.insert(m_OriginalPointList.begin()+e_iIndex,e_vPos);
			this->m_OriginalTimeList.insert(m_OriginalTimeList.begin()+e_iIndex,e_fTime);
			this->DoLOD();
		}
	}
	//erase old point replace new one
	void	cCurveWithTime::FixPoint(int e_iIndex,Vector3 e_vPos,float e_fTime)
	{
		cCurve::FixPoint(e_iIndex,e_vPos);
		this->m_OriginalTimeList.erase(m_OriginalTimeList.begin()+e_iIndex);
		m_OriginalTimeList.insert(m_OriginalTimeList.begin()+e_iIndex,e_fTime);
		DoLOD();
	}

	void	cCurveWithTime::FixPoint(int e_iIndex,Vector3 e_vPos)
	{
		cCurve::FixPoint(e_iIndex,e_vPos);
	}

	float	cCurveWithTime::GetEndTime()
	{
		if( m_OriginalTimeList.size()>0 )
			return m_OriginalTimeList[m_OriginalTimeList.size()-1]+m_fStartTime;
		return m_fStartTime;
	}

	Vector3 cCurveWithTime::GetPositionByTime(float e_fTime)
	{
		if( e_fTime <= 0.f )
		{
			return m_OriginalPointList[0];
		}
		assert(e_fTime>=0.f&&"time is smaller than 0");
		assert(m_FinalTimeList.size()&&"there is no any time data");

		Vector3	l_Vector3;
		int	l_iNum = (int)m_FinallyPointList.size();
		//first time is 0,neglect it
		float	l_fPerviousTime = 0.f;
		Vector3	l_PerviousPos = this->m_OriginalPointList[0];
		//check time is over last time and it's loop,if it is,let time be the right between range
		float	l_fLastTime = this->m_FinalTimeList[l_iNum-1];
		if(e_fTime>m_FinalTimeList[l_iNum-1]&&this->m_bCurveLoop)
		{
			if( l_fLastTime != 0.f )
				e_fTime = UT::GetFloatModulus(e_fTime,l_fLastTime);
		}
		for( int i=1;i<l_iNum;++i )
		{
			float	l_fCurrentTime = m_FinalTimeList[i];
			Vector3	l_vCurrentPos = this->m_FinallyPointList[i];
			if( l_fCurrentTime>=e_fTime )
			{
				this->m_iCurrentPointIndex = i-1;
				float	l_fTimeDis = l_fCurrentTime-l_fPerviousTime;
				//the time  is though closet point,LERP
				float	l_fStepThough = 1-((l_fCurrentTime-e_fTime)/l_fTimeDis);
				m_fCurrentLERPTime = l_fStepThough;
				//end point minus step distance
				Vector3	l_vCurrentPos = Vector3Lerp(l_PerviousPos,m_FinallyPointList[i],l_fStepThough);
				if(this->m_bCalculateAngle)
				{
					this->m_fCurrentPosToNextPointAngle = GetAngleByFinalListIndex(m_iCurrentPointIndex);
				}
				return l_vCurrentPos;
			}
			l_PerviousPos = l_vCurrentPos;
			l_fPerviousTime = l_fCurrentTime;
		}
		this->m_iCurrentPointIndex = l_iNum-1;
		return m_FinallyPointList[l_iNum-1];
	}

	void	cCurveWithTime::AverageTimeAssign(float e_fTime)
	{
		int	l_iNum = (int)this->m_OriginalTimeList.size();
		if( l_iNum>1 )
		{
			m_OriginalTimeList[0] = 0.f;
			float	l_fStepTime = e_fTime/(l_iNum-1);
			for( int i=1;i<l_iNum-1;i++ )
			{
				m_OriginalTimeList[i] = i*l_fStepTime;
			}
			m_OriginalTimeList[m_OriginalTimeList.size()-1] = e_fTime;
			this->DoLOD();
		}
	}

	void	cCurveWithTime::SetCurrentTime(float e_fCurrentTime)
	{
		this->m_fCurrentTime = e_fCurrentTime;
		if(!this->m_FinalTimeList.size())
			return;
		int	l_i = 0;
		while(m_fCurrentTime>this->m_FinalTimeList[l_i])
		{
			this->m_iCurrentPointIndex = l_i++;
			//multex
			if( l_i>=(int)m_FinalTimeList.size() )
			{
				m_iCurrentPointIndex = l_i-1;
				return;
			}
		}
	}

	void	cCurveWithTime::InternalInit()
	{
		m_vCurrentPosition = m_vMovedCoordinate = Vector3::Zero;
		//assert(this->GetLastTime()>0.f&&"last time is zero is not allow");
		if( m_OriginalTimeList.size() == 0 || this->m_OriginalPointList.size() == 0 )
			return;
		//assert(this->m_OriginalTimeList.size()&&"time list has not any data");
		//assert(this->m_OriginalPointList.size()&&"m_OriginalPoint list has not any data");
		this->m_fCurrentTime = 0.f;
		m_iCurrentPointIndex = 0;
		m_iPerviousPointIndex = -1;
		//m_bCurveMoveDone = false;
		m_vPreviousPosition = this->m_vCurrentPosition = this->m_OriginalPointList[0];
		m_fCurrentPosToNextPointAngle = 0.f;
		m_fCurrentPosToNextPointAngle = 0.f;
		//if( e_fElpaseTime>EPSIONAL )
		//	this->Update(e_fElpaseTime);
	}

	void	cCurveWithTime::MovedDistanceCalculate(float e_fElpaseTime)
	{
		//if( m_bCurveMoveDone )
			//return;
		if( m_FinalTimeList.size() == 0 )
		{
			//m_bCurveMoveDone = true;
			return;
		}

		if( m_iCurrentPointIndex == -1 )//time has changed,it should't happen but for edit is fine
		{
			m_iCurrentPointIndex = 0;
			//return;
		}

		assert(m_iCurrentPointIndex != -1&&"forget to call start");
		m_fCurrentTime+=e_fElpaseTime;
		if( m_FinalTimeList.size() == 1 )
		{
			m_vCurrentPosition = this->m_FinallyPointList[0];
			return;
		}
		//reach last time
		if( m_fCurrentTime>=m_FinalTimeList[m_FinalTimeList.size()-1] )
		{
			if( m_bCurveLoop )
			{
				cCurveWithTime::Init();
				cCurveWithTime::Update(GetFloatModulus(m_fCurrentTime,m_FinalTimeList[m_FinalTimeList.size()-1]));
				//below Start is not cCurveWithTime::Start,it wil  call the target which inhirent....fuck
				//this->Start(GetFloatModulus(m_fCurrentTime,m_FinalTimeList[m_FinalTimeList.size()-1]));
			}
			else
			{
				m_fCurrentLERPTime = 1.f;
				m_iCurrentPointIndex = (int)m_FinallyPointList.size()-1;
				m_vCurrentPosition = this->m_FinallyPointList[m_iCurrentPointIndex];
				//m_bCurveMoveDone = true;
			}
			return;
		}
		while(m_fCurrentTime>this->m_FinalTimeList[m_iCurrentPointIndex])
		{
			m_iCurrentPointIndex++;
			//multex,should not happen...but I am lazy to test this.
			if( m_iCurrentPointIndex>(int)m_FinalTimeList.size() )
			{
				this->m_vCurrentPosition = this->m_FinallyPointList[m_FinallyPointList.size()-1];
				return;
			}
		}
		m_iCurrentPointIndex -= 1;

		if( m_iCurrentPointIndex == -1 )//time has changed,it's not legally,but edit is fine
			m_iCurrentPointIndex = 0;
		assert(m_iCurrentPointIndex<(int)m_FinalTimeList.size()&&"m_iCurrentPointIndex<(int)m_FinalTimeList.size()");
		//get time gap
		float	l_fTimeGap = m_FinalTimeList[m_iCurrentPointIndex+1]-m_FinalTimeList[m_iCurrentPointIndex];
		//assert(l_fTimeGap>0.f&&"2 points has same time!?");
		//find timestep
		float	l_fStepTime = m_fCurrentTime-m_FinalTimeList[m_iCurrentPointIndex];
		//find LERP
		float	l_fLERPTime = l_fStepTime/l_fTimeGap;
		m_fCurrentLERPTime = l_fLERPTime;
		//end point minus step distance
		Vector3	l_vStartPos = m_FinallyPointList[m_iCurrentPointIndex];
		Vector3	l_vEndPos = m_FinallyPointList[m_iCurrentPointIndex+1];
		m_vCurrentPosition = UT::LERP<Vector3>(l_vStartPos,l_vEndPos,l_fLERPTime);
		//m_vCurrentPosition = m_FinallyPointList[m_iCurrentPointIndex]+
		//	((this->m_FinallyPointList[m_iCurrentPointIndex+1]-this->m_FinallyPointList[m_iCurrentPointIndex])*l_fLERPTime);
		if( this->m_bCalculateAngle )
		{
			if( m_iCurrentPointIndex != m_iPerviousPointIndex )
			{
				if( m_iPerviousPointIndex >-1 )
				{
					m_fPreviousPosToNextPointAngle = GetAngleByFinalListIndex(m_iPerviousPointIndex);
				}
				else
				{
					m_fPreviousPosToNextPointAngle = GetAngleByFinalListIndex(0);
				}
			}
			float	l_fAngle = GetAngleByFinalListIndex(m_iCurrentPointIndex);
			float	l_fDiffAngle = CalculateDifferenceBetweenRadius(m_fPreviousPosToNextPointAngle,l_fAngle);
			m_fCurrentPosToNextPointAngle = l_fAngle-(l_fDiffAngle*(1-l_fLERPTime));
		}
		if( m_iPerviousPointIndex != m_iCurrentPointIndex )
			m_iPerviousPointIndex = this->m_iCurrentPointIndex;	
	}

	void	cCurveWithTime::InternalUpdate(float e_fElpaseTime)
	{
		m_vPreviousPosition = m_vCurrentPosition;
		MovedDistanceCalculate(e_fElpaseTime);
		m_vMovedCoordinate = m_vCurrentPosition-m_vPreviousPosition;
		//cMatrix44	l_mat = cMatrix44::TranslationMatrix(m_vMovedCoordinate);
		//if( this->m_bCalculateAngle )
		//{
		//	this->SetLocalRotation(Vector3(0,0,m_fCurrentPosToNextPointAngle));
		//	m_fCurrentPosToNextPointAngle;
		//}
	}

	float	cCurveWithTime::GetAngleByFinalListIndex(int e_iIndex)
	{
		Vector3	l_vNextPos = this->m_FinallyPointList[e_iIndex+1];
		Vector3	l_vCurrentPos = this->m_FinallyPointList[e_iIndex];
		Vector3	l_vDirection = l_vNextPos-l_vCurrentPos;
		m_vCurrentDirection = l_vDirection.Normalize();
		//l_vDirection.z = l_vDirection.y;l_vDirection.y = 0.f;
		//180 - for Z invert
		return D3DXToRadian(180-UT::GetAngleBy2Point(l_vCurrentPos,l_vNextPos));
	}

	float	cCurveWithTime::GetMovedDistance()
	{
		return (m_vCurrentPosition-this->m_vPreviousPosition).Length();
	}

	Vector3	cCurveWithTime::GetMovedCoordinate()
	{
		return m_vMovedCoordinate;
	}

	float	cCurveWithTime::GetSpeedBySecond(float e_fTime)
	{
		return GetMovedDistance()/e_fTime;
	}

	float	cCurveWithTime::GetSpeedByMinute(float e_fTime)
	{
		return GetMovedDistance()/e_fTime*60.f;
	}

	float	cCurveWithTime::GetSpeedByHour(float e_fTime)
	{
		return GetMovedDistance()/e_fTime*60.f*60.f;
	}

	//void	cCurveWithTime::ReplacePointsByCount(int e_iNumPoint)
	//{
	//	if( !e_iNumPoint )
	//		return;
	//	this->m_OriginalTimeList.clear();
	//	int	l_iNumPoint = this->m_FinalTimeList.size();
	//	//less than 2 points,could not to be lerp
	//	if( l_iNumPoint<2 )
	//		return;
	//	float	l_fStep = (float)l_iNumPoint/(e_iNumPoint-1);
	//	float	l_fCurrentStep = l_fStep;
	//	int	l_iLeftPoint = 0;
	//	int	l_iRightPoint = (int)l_fStep;
	//	//force float value to next point
	//	if( l_fStep-(int)l_fStep )
	//	{
	//		l_iRightPoint = (int)l_fStep+1;
	//	}
	//	this->m_OriginalTimeList.push_back(this->m_FinalTimeList[0]);
	//	e_iNumPoint--;//first point has pushed
	//	for( int i=0;i<e_iNumPoint;++i )
	//	{
	//		//make sure the theory is correct
	//		assert(l_iLeftPoint != l_iRightPoint);
	//		//find distance
	//		float	l_fTime = m_FinalTimeList[l_iRightPoint]-m_FinalTimeList[l_iLeftPoint];
	//		//do LERP,find step
	//		l_fTime*=(1-((l_iRightPoint-l_fCurrentStep)/l_iRightPoint));
	//		//add start pos
	//		l_fTime+=m_FinalTimeList[l_iLeftPoint];
	//		//push into data
	//		this->m_OriginalTimeList.push_back(l_fTime);
	//		//step move
	//		l_fCurrentStep+=l_fStep;
	//		//
	//		if( (int)l_fCurrentStep>l_iRightPoint )
	//		{
	//			if( l_iRightPoint<m_FinallyPointList.size() )
	//			{
	//				l_iRightPoint = (int)l_fCurrentStep;
	//				l_iLeftPoint = l_iRightPoint-1;
	//			}
	//		}
	//	}
	//	cCurve::ReplacePointsByCount(e_iNumPoint+1);
	//	this->SetLOD(1);
	//}


	void	cCurveWithTime::InternalRender()
	{
		Vector4 l_vColor = Vector4::One;
		cMatrix44 l_mat = cMatrix44::Identity;
		this->RenderCurve(l_vColor,l_mat);
	}

	void	cCurveWithTime::Render(Vector4 e_vColor,cMatrix44 e_mat)
	{
		this->RenderCurve(e_vColor,e_mat);
		GLRender::RenderPoint(this->m_vCurrentPosition);
	}

	void	cCurveWithTime::InvertOrder()
	{
		InvertVectorDateOrder(&m_OriginalPointList);
		cCurve::DoLOD();
	}


	void	cCurveWithTime::RearrangeTimeByPercent(float e_fPercenttage)
	{
		this->m_fStartTime *= e_fPercenttage;
		float	l_fPercentage = e_fPercenttage;
		int	l_iNum = (int)m_OriginalTimeList.size();
		for( int i=0;i<l_iNum;++i )
		{
			m_OriginalTimeList[i] = m_OriginalTimeList[i]*l_fPercentage;
		}
		this->DoLOD();
	}

	void	cCurveWithTime::UpdateByGlobalTime(float e_fGlobalTime)
	{
		GetPositionByTime(e_fGlobalTime);
	}

	void	cCurveWithTime::RenderByGlobalTime(float e_fTime)
	{
		GetPositionByTime(e_fTime);
		this->Render();
	}

	void	cCurveWithTime::RearrangeTime(float e_fNewMaxTime)
	{
		int	l_iNum = (int)m_OriginalTimeList.size();
		if( l_iNum>0 )
		{
			if( m_OriginalTimeList[l_iNum-1]>0.f )
			{
				float	l_fPercentage = e_fNewMaxTime/this->m_OriginalTimeList[l_iNum-1];
				RearrangeTimeByPercent(l_fPercentage);
			}
		}
	}

	void	cCurveWithTime::Destroy()
	{
		cCurve::Destroy();
		m_OriginalTimeList.clear();
		m_FinalTimeList.clear();
		m_fCurrentTime = this->m_fCurrentLERPTime = 0.f;
		m_fCurrentLERPTime = m_fCurrentPosToNextPointAngle = m_fPreviousPosToNextPointAngle = 0.f;
	}

	Vector3	cCurveWithTime::GetClosePoint()
	{
		assert(this->m_OriginalPointList.size()>0&&"no point!! how to get close point");
		if (m_iCurrentPointIndex == -1)
			return m_OriginalPointList[0];
		int	l_iIndex = (int)(((float)m_iCurrentPointIndex/(float)this->m_FinallyPointList.size())*m_iCurrentPointIndex);
		return m_OriginalPointList[l_iIndex];
	}

	int	cCurveWithTime::GetTimeRelativeIndex(float e_fTargetTime)
	{
		int	l_iCount = (int)this->m_OriginalTimeList.size();
		//at least must have 2 points
		if( l_iCount<2 )
			return -1;
		assert(m_OriginalTimeList[0] == 0.f&&"first point time must be 0");
		int i=1;
		for( ;i<l_iCount;++i )
		{
			if(e_fTargetTime<m_OriginalTimeList[i])
				return i;
		}
		return -1;
	}

	float	cCurveWithTime::GetVelocity(float e_fTargetTime)
	{
		int	l_iIndex = GetTimeRelativeIndex(e_fTargetTime);
		if( l_iIndex == -1 )
		{
			return 0.f;
		}
		//get 2 position then divide them by time
		Vector3	l_vLength = this->m_OriginalPointList[l_iIndex]-m_OriginalPointList[l_iIndex-1];
		float	l_fTime = this->m_OriginalTimeList[l_iIndex]-this->m_OriginalTimeList[l_iIndex-1];
		assert(l_fTime>0.f&&"time is small than 0!!");
		return l_vLength.Length()/l_fTime;
	}

	cMatrix44	cCurveWithTime::GetTransformWithOffsetPos(Vector3 e_vOffsetPos,float e_fLength)
	{
		Vector3	l_vTarget = m_vCurrentPosition+(this->m_vCurrentDirection*e_fLength);
		Vector3	l_vOffsetPos = e_vOffsetPos+m_vCurrentPosition-(e_vOffsetPos.Length()*m_vCurrentDirection);
		return cMatrix44::LookAtMatrix(l_vOffsetPos,l_vTarget,Vector3::ZAxis);
		//return cMatrix44::TranslationMatrix(l_vOffsetPos)*cMatrix44::ZAxisRotationMatrix(m_fCurrentPosToNextPointAngle);
	}

	int	cCurveWithTime::GetCurrentWorkingOriginalIndex()
	{
		if( m_iCurrentPointIndex != -1 )
		{
			return this->FinalListIndexToOriginalIndex(m_iCurrentPointIndex);
		}
		return 0;
	}

	float	cCurveWithTime::GetRestTime()
	{
		float	l_fRestTime = GetEndTime()-this->m_fCurrentTime;
		return l_fRestTime;
	}

	void	cCurveWithTime::SetCurrentPosition(Vector3 e_vPos)
	{
		this->m_vPreviousPosition = m_vCurrentPosition;
		m_vCurrentPosition = e_vPos;
	}

	cCurveWithTime	cCurveWithTime::GetOffsetCurve(float e_fStartTimeOffset,UT::sMinMaxData<float> e_fDirectionOffsetX,UT::sMinMaxData<float> e_fDirectionOffsetY,int e_iLOD)
	{
		std::vector<Vector3>l_PosList2 = m_OriginalPointList;
		std::vector<float>l_TimeList2 = m_OriginalTimeList;
		size_t	l_uiSize = l_PosList2.size();
		//
		for(size_t ii=1;ii<l_uiSize;++ii  )
		{
			l_TimeList2[ii] += e_fStartTimeOffset;
		}
		float	l_fDis = 0.f;
		for(size_t ii=1;ii<l_uiSize;++ii  )
		{
			float	l_fOffsetValueX = e_fDirectionOffsetX.Rand();
			float	l_fOffsetValueY = e_fDirectionOffsetY.Rand();
			Vector3	l_vDistance = (l_PosList2[ii-1]-l_PosList2[ii]);
			Vector3	l_vDirection = l_vDistance.Normalize();
			l_vDirection.x += frand(-l_fOffsetValueX,l_fOffsetValueX);
			l_vDirection.y += frand(-l_fOffsetValueY,l_fOffsetValueY);
			l_vDirection.NormalizeIt();
			float	l_fTimeDis = l_TimeList2[ii]-l_TimeList2[ii-1];
			l_TimeList2[ii] = l_fTimeDis+l_TimeList2[ii-1]+frand(0,0.3f);
			l_PosList2[ii] = l_PosList2[ii-1]-(l_vDistance.Length()*l_vDirection);
		}
		cCurveWithTime	l_Path;
		l_Path.SetOriginalTimeList(l_TimeList2);
		l_Path.SetOriginalPointList(l_PosList2);
		l_Path.SetLOD(e_iLOD);
		l_Path.DoLOD();
		return l_Path;
	}
}
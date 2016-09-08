#include "../stdafx.h"
#include "LinerTemplateDataProcess.h"
#include "../Image/SimplePrimitive.h"
#include "../GameplayUT/GameApp.h"
namespace	FATMING_CORE
{
	cTimeAndDataLinerUpdateInterface::cTimeAndDataLinerUpdateInterface()
	{
		m_iLOD = 1;
		m_pLODTimeVector = 0;
		m_pCurrentTimeVector = &m_TimeVector;
		m_bUpdateWithLiner = true;
		m_fPastTime = 0.f;
		m_bLoop = false;
		m_iCurrentWorkingIndex = -1;
	}

	void	cTimeAndDataLinerUpdateInterface::InternalElementParse(TiXmlElement*e_pTiXmlElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,L"cTimeAndDataLinerUpdateInterface");
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("LOD")
			{
				m_iLOD = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("Loop")
			{
				m_bLoop = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("DoLiner")
			{
				m_bUpdateWithLiner = VALUE_TO_BOOLEAN;
			}
		PARSE_NAME_VALUE_END
	}

	TiXmlElement*	cTimeAndDataLinerUpdateInterface::ToTiXmlElement()
	{
		if( !m_bUpdateWithLiner || m_iLOD > 1 || m_bLoop )
		{
			TiXmlElement*l_pTiXmlElement = new TiXmlElement(L"cTimeAndDataLinerUpdateInterface");
			l_pTiXmlElement->SetAttribute(L"LOD",m_iLOD);
			l_pTiXmlElement->SetAttribute(L"Loop",m_bLoop?1:0);
			l_pTiXmlElement->SetAttribute(L"DoLiner",m_bUpdateWithLiner?1:0);
			return l_pTiXmlElement;
		}
		return nullptr;
	}

	cTimeAndDataLinerUpdateInterface::cTimeAndDataLinerUpdateInterface(cTimeAndDataLinerUpdateInterface*e_pcTimeAndDataLinerUpdateInterface)
	{
		m_iLOD = e_pcTimeAndDataLinerUpdateInterface->m_iLOD;
		//DoLOD will implement at its inhirents
		m_pLODTimeVector = 0;
		m_pCurrentTimeVector = &m_TimeVector;
		m_TimeVector = e_pcTimeAndDataLinerUpdateInterface->m_TimeVector;
		m_bUpdateWithLiner = e_pcTimeAndDataLinerUpdateInterface->m_bUpdateWithLiner;
		m_fPastTime = 0.f;
		m_bLoop = e_pcTimeAndDataLinerUpdateInterface->m_bLoop;
		m_iCurrentWorkingIndex = -1;
	}

	cTimeAndDataLinerUpdateInterface::~cTimeAndDataLinerUpdateInterface()
	{
		SAFE_DELETE(m_pLODTimeVector);
	}

	void	cTimeAndDataLinerUpdateInterface::Init()
	{
		m_iCurrentWorkingIndex = 0;
		m_fPastTime = 0.f;
		if( m_TimeVector.size() == 0 )
		{
			m_iCurrentWorkingIndex = -1;
		}
	}


	void	cTimeAndDataLinerUpdateInterface::DoLOD()
	{
		SAFE_DELETE(m_pLODTimeVector);
		if(m_iLOD <= 1 || m_TimeVector.size() == 1)
		{
			m_pCurrentTimeVector = &m_TimeVector;
			return;
		}
		m_pLODTimeVector = new std::vector<float>;
		*m_pLODTimeVector = this->m_TimeVector;
		m_pCurrentTimeVector = m_pLODTimeVector;
		for( int i=0;i<m_iLOD;++i )
			cTimeAndDataLinerUpdateInterface::IncreaseLod();	
	}

	void	cTimeAndDataLinerUpdateInterface::IncreaseLod()
	{
		TemplateDataIncreaseLod<float>(m_pLODTimeVector);
	}
	int		cTimeAndDataLinerUpdateInterface::UpdateTimeVectorDataIndex(float e_fElpaseTime,float*e_pfLerpValue,float*e_fRestTimeToNextStep,float*e_pfNextStepTimeDiff,bool*e_pEndPoint)
	{
		int	l_iSize = (int)m_pCurrentTimeVector->size();
		if( l_iSize == 0 )
			return -1;
		this->m_fPastTime += e_fElpaseTime;
		float	l_fEndTime = (*m_pCurrentTimeVector)[l_iSize-1];
		std::vector<float> l_fTime = *m_pCurrentTimeVector;
		if( l_iSize == 1 || l_fEndTime <= 0.f )
		{
			*e_pfLerpValue = 1.f;
			*e_pEndPoint = true;
			m_fPastTime = l_fEndTime;
			return 0;
		}

		if( m_fPastTime >= l_fEndTime )
		{
			if( m_bLoop )
				m_fPastTime = UT::GetFloatModulus(m_fPastTime,l_fEndTime);
			else
			{
				int	l_iEndDataIndex = l_iSize-1;
				*e_pfLerpValue = 1.f;
				*e_pEndPoint = true;
				m_fPastTime = l_fEndTime;
				return l_iEndDataIndex;
			}
		}

		for( int i=l_iSize-1;i>-1;--i )
		{
			if( m_fPastTime >= (*m_pCurrentTimeVector)[i] )
			{
				int	l_iEndDataIndex = l_iSize-1;
				if( i != l_iEndDataIndex )
				{
					float	l_fPreviousTime = (*m_pCurrentTimeVector)[i];
					float	l_fTimeOverdValue = m_fPastTime-l_fPreviousTime;
					float	l_fTimeGap = (*m_pCurrentTimeVector)[i+1]-l_fPreviousTime;
					*e_fRestTimeToNextStep = l_fTimeGap-l_fTimeOverdValue;
					*e_pfNextStepTimeDiff = l_fTimeGap;
					if( l_fTimeGap != 0.f )
					{
						*e_pfLerpValue = l_fTimeOverdValue/l_fTimeGap;
					}
					else
					{
						*e_pfLerpValue = 1.f;
					}
				}
				else
				{
					*e_pfLerpValue = 1.f;
				}
				return i;
			}
		}
		//impossible
		assert(0 &&" GetTimeVectorDataIndex");
		return -1;
	}

	void	cTimeAndDataLinerUpdateInterface::Rearrange(float e_fNewTime)
	{
		float	l_fLastTime = GetEndTime();
		if( l_fLastTime >= 0.f )
		{
			float	l_fPercent = e_fNewTime/l_fLastTime;
			int	l_iSize = (int)m_TimeVector.size();
			for( int i=0;i<l_iSize;++i )
				m_TimeVector[i] *= l_fPercent;
			this->DoLOD();
		}
	}

	float	cTimeAndDataLinerUpdateInterface::GetEndTime(){	int	l_iSize = (int)m_TimeVector.size();	if( l_iSize > 0 ) return m_TimeVector[l_iSize-1] ;return 0.f;}
	void	cTimeAndDataLinerUpdateInterface::ChangeTime(int e_iIndex,float e_fTime)
	{
		if( (int)m_TimeVector.size() > e_iIndex )
			m_TimeVector[e_iIndex] = e_fTime; 	
	}

	float	cTimeAndDataLinerUpdateInterface::GetCurrentLerpValue()
	{
		float	l_fLastTime = GetEndTime();
		if( l_fLastTime >= 0.f )
		{
			return m_fPastTime/l_fLastTime;
		}
		return 1.f;
	}

	void	cTimeAndDataLinerUpdateInterface::UpdateByGlobalTime(float e_fElpaseTime)
	{
		if( m_fPastTime == e_fElpaseTime )
		{
			return;
		}
		this->Init();
		m_fPastTime = 0.f;
		Update(e_fElpaseTime);
	}

	void	cTimeAndDataLinerUpdateInterface::DoTimeLiner()
	{
		size_t	l_uiSize = this->m_TimeVector.size();
		if( l_uiSize < 2 )
			return;
		float	l_fStart = m_TimeVector[0];
		float	l_fEnd = m_TimeVector[l_uiSize-1];
		float	l_fGap = (l_fEnd-l_fStart)/l_uiSize;
		for(size_t i=0;i<l_uiSize;++i)
		{
			m_TimeVector[i] = l_fStart;
			l_fStart += l_fGap;
		}
		this->DoLOD();
	}

	template <class T>
	cLinerDataProcessor<T>::cLinerDataProcessor()
	{
		m_CurrentData = m_CurrentData-m_CurrentData;
		m_pCurrentLinerDataVector = &m_LinerDataVector;
		m_pCurrentTimeVector = &m_TimeVector;
		m_pLODDataVector = 0;
	}
//=========================
//
//=========================
	template <class T>
	cLinerDataProcessor<T>::cLinerDataProcessor(cLinerDataProcessor*e_pLinerDataProcessor):cTimeAndDataLinerUpdateInterface(e_pLinerDataProcessor)
	{
		m_pLODDataVector = 0;
		m_LinerDataVector = e_pLinerDataProcessor->m_LinerDataVector;
		m_pCurrentLinerDataVector = 0;
		m_pCurrentTimeVector = 0;
		this->DoLOD();

	}
//=========================
//
//=========================
	template <class T>
	cLinerDataProcessor<T>::~cLinerDataProcessor()
	{
		SAFE_DELETE(m_pLODDataVector);
	}
//=========================
//
//=========================
	template <class T>
	cTimeAndDataLinerUpdateInterface*	cLinerDataProcessor<T>::Clone()
	{
		cTimeAndDataLinerUpdateInterface*l_pLinerDataProcessor = new cLinerDataProcessor<T>(this);
		return l_pLinerDataProcessor;
	}
//=========================
//
//=========================
	template <class T>
	void cLinerDataProcessor<T>::IncreaseLod()
	{
		TemplateDataIncreaseLod<T>(m_pLODDataVector);
	}
//=========================
//
//=========================
	template <class T>
	void	cLinerDataProcessor<T>::DoLOD()
	{
		cTimeAndDataLinerUpdateInterface::DoLOD();
		SAFE_DELETE(m_pLODDataVector);
		if(m_iLOD <= 1 || m_LinerDataVector.size() == 1)
		{
			m_pCurrentLinerDataVector = &m_LinerDataVector;
			m_pCurrentTimeVector = &m_TimeVector;
			return;
		}
		m_pLODDataVector = new std::vector<T>;
		*m_pLODDataVector = this->m_LinerDataVector;
		m_pCurrentLinerDataVector = m_pLODDataVector;
		for( int i=0;i<m_iLOD;++i )
			IncreaseLod();

	}
	template <class T>
	void	cLinerDataProcessor<T>::DumpTo(cLinerDataProcessor<T>*e_pTarget)
	{
		e_pTarget->m_TimeVector = this->m_TimeVector;
		e_pTarget->m_bUpdateWithLiner = m_bUpdateWithLiner;
		e_pTarget->m_LinerDataVector = m_LinerDataVector;
		e_pTarget->m_iLOD = m_iLOD;
		e_pTarget->DoLOD();
		
	}
//=========================
//
//=========================
	template <class T>
	void*	cLinerDataProcessor<T>::GetDataPointerByIndex(int e_iIndex)
	{
		if(IsLegalIndexInVector<T>(&m_LinerDataVector,e_iIndex))
		{
			return (void*)&m_LinerDataVector[e_iIndex];
		}
		return 0;
	}
//=========================
//
//=========================
	template <class T>
	int		cLinerDataProcessor<T>::GetLOD(){return m_iLOD;}
//=========================
//
//=========================
	template <class T>
	void	cLinerDataProcessor<T>::SetLOD(int e_iLOD)
	{
		if( this->m_iLOD != e_iLOD )
		{
			m_iLOD = e_iLOD;
			DoLOD();
		}
	}
//=========================
//
//=========================
	template <class T>
	cLinerDataProcessor<T>::cLinerDataProcessor(TiXmlElement*e_pElement)
	{
		m_CurrentData = m_CurrentData-m_CurrentData;
		m_pCurrentLinerDataVector = &m_LinerDataVector;
		m_pCurrentTimeVector = &m_TimeVector;
		m_pLODDataVector = 0;
		m_iLOD = 1;
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,L"cLinerDataProcessor");
		//l_pAttribute->Value(
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pElement)
			const WCHAR*l_strValue = e_pElement->Value();
			COMPARE_VALUE("Time")
			{
				TiXmlAttribute*	l_pAttribute = e_pElement->FirstAttribute();
				const WCHAR*l_strTimeData = l_pAttribute->Value();
				this->m_TimeVector = GetValueListByCommaDivide<float>(l_strTimeData);
			}
			else
			COMPARE_VALUE("Data")
			{
				TiXmlAttribute*	l_pAttribute = e_pElement->FirstAttribute();
				while(l_pAttribute)
				{
					const WCHAR*l_strData = l_pAttribute->Value();
					if( l_strData )
					{
						T	l_Data = T(l_strData);
						this->m_LinerDataVector.push_back(l_Data);
					}
					l_pAttribute = l_pAttribute->Next();
				}
			}
			else
			COMPARE_VALUE("cTimeAndDataLinerUpdateInterface")
			{
				this->InternalElementParse(e_pElement);
			}
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pElement)
		if(this->m_iLOD >1)
			this->DoLOD();
	}
//=========================
//
//=========================
	template <class T>
	TiXmlElement*	cLinerDataProcessor<T>::ToTiXmlElement()
	{
		int	l_iSize = m_TimeVector.size();
		if( l_iSize == 0 || ( m_LinerDataVector.size() != m_TimeVector.size() ) )
			return 0;
		TiXmlElement*	l_pLinerTemplateDataProcessElement = new TiXmlElement(L"cLinerDataProcessor");
		l_pLinerTemplateDataProcessElement->SetAttribute(L"LOD",this->m_iLOD);
		TiXmlElement*	l_pTime = new TiXmlElement(L"Time");
		TiXmlElement*	l_pAnimationData = new TiXmlElement(L"Data");
		std::wstring	l_strTimeXmlElementData;
		std::wstring	l_strAnimationXmlElementData;
		for( int i=0;i<l_iSize;++i )
		{
			l_strTimeXmlElementData += ValueToStringW(*GetTime(i));
			l_strAnimationXmlElementData = ValueToStringW(*GetData(i));
			std::wstring	l_strData = L"Data";
			l_strData += ValueToStringW(i);
			if( i != l_iSize-1 )
			{
				l_strTimeXmlElementData += L",";
			}
			l_pAnimationData->SetAttribute(l_strData,l_strAnimationXmlElementData);
		}
		l_pTime->SetAttribute(L"Time",l_strTimeXmlElementData.c_str());
		l_pLinerTemplateDataProcessElement->LinkEndChild(l_pTime);
		l_pLinerTemplateDataProcessElement->LinkEndChild(l_pAnimationData);
		TiXmlElement*l_pInternalElement = cTimeAndDataLinerUpdateInterface::ToTiXmlElement();
		if( l_pInternalElement )
			l_pLinerTemplateDataProcessElement->LinkEndChild(l_pInternalElement);
		return l_pLinerTemplateDataProcessElement;
	}
//=========================
//
//=========================
	template <class T>
	std::vector<T>*				cLinerDataProcessor<T>::GetLinerDataVector()
	{
		return &this->m_LinerDataVector;
		//return  m_pCurrentLinerDataVector;
	}
//=========================
//
//=========================
	template <class T>
	T*							cLinerDataProcessor<T>::GetData(int e_iIndex){if((int)m_LinerDataVector.size()>e_iIndex && e_iIndex!= -1)return &m_LinerDataVector[e_iIndex];return 0;}
	template <class T>
	bool						cLinerDataProcessor<T>::GetLastData(T&e_Data)
	{
		size_t l_uiSize = m_LinerDataVector.size();
		if(l_uiSize)
		{
			e_Data = m_LinerDataVector[l_uiSize-1];
			return true;
		}
		return false;
	}
//=========================
//
//=========================
	template <class T>
	void				cLinerDataProcessor<T>::Init()
	{
		m_iCurrentWorkingIndex = 0;
		m_fPastTime = 0.f;
		if( m_TimeVector.size() == 0 )
		{
			m_iCurrentWorkingIndex = -1;
		}
	}
//=========================
//
//=========================
	template <class T>
	void				cLinerDataProcessor<T>::Update(float e_fElpaseTime)
	{
		if( m_iCurrentWorkingIndex == -1)
			return;
		//for linear interpolation
		float	l_fRestTimeToNextStep = 0.f;
		float	l_fNextStepTimeDiff = 0.f;
		float	l_fCurrentStepLerpValue = 0.f;
		bool	l_bEndPoint = false;
		//get current working point
		m_iCurrentWorkingIndex = UpdateTimeVectorDataIndex(e_fElpaseTime,&l_fCurrentStepLerpValue,&l_fRestTimeToNextStep,&l_fNextStepTimeDiff,&l_bEndPoint);
		if( m_iCurrentWorkingIndex != -1 )
		{
			if( !this->m_bUpdateWithLiner || l_bEndPoint )
			{
				m_CurrentData = (*this->m_pCurrentLinerDataVector)[m_iCurrentWorkingIndex];
			}
			else
			{
				m_CurrentData = ((*this->m_pCurrentLinerDataVector)[m_iCurrentWorkingIndex+1]-(*this->m_pCurrentLinerDataVector)[m_iCurrentWorkingIndex])*l_fCurrentStepLerpValue;
				m_CurrentData += (*this->m_pCurrentLinerDataVector)[m_iCurrentWorkingIndex];
			}
		}
	}
//=========================
//
//=========================
	template <class T>
	void				cLinerDataProcessor<T>::Clear(){ m_LinerDataVector.clear();m_TimeVector.clear();	SAFE_DELETE(m_pLODDataVector); SAFE_DELETE(m_pLODTimeVector); }
//=========================
//
//=========================
	template <class T>
	void				cLinerDataProcessor<T>::InvertOrder()
	{
		std::vector<T>		l_LinerDataVector = m_LinerDataVector;
		std::vector<float>	l_TimeVector = m_TimeVector;
		int	l_iSize = (int)m_TimeVector.size();
		int	l_iSize2 = (int)m_TimeVector.size()-1;
		for( int i=0;i<l_iSize;++i )
		{
			m_TimeVector[i] = l_TimeVector[l_iSize2-i];
			m_LinerDataVector[i] = l_LinerDataVector[l_iSize2-i];
		}
		this->DoLOD();
	}
//=========================
//
//=========================
	template <class T>
	T							cLinerDataProcessor<T>::GetCurrentData(){ return m_CurrentData; }
//=========================
//
//=========================
	template <class T>
	void				cLinerDataProcessor<T>::AddData(T e_Data,float e_fTime)
	{
		m_LinerDataVector.push_back(e_Data); 
		m_TimeVector.push_back(e_fTime);
		this->DoLOD(); 
		assert(m_LinerDataVector.size() ==m_TimeVector.size()&&"cLinerDataProcessor::AddData size not same");
	}
//=========================
//
//=========================
	template <class T>
	void				cLinerDataProcessor<T>::SetData(std::vector<float>e_TimeVector,std::vector<T>e_LinerDataVector){m_LinerDataVector = e_LinerDataVector;m_TimeVector = e_TimeVector;this->DoLOD();}
//=========================
//
//=========================
	template <class T>
	bool				cLinerDataProcessor<T>::ChangeData(int e_iIndex,T e_Data)
	{
		if(IsLegalIndexInVector<T>(&m_LinerDataVector,e_iIndex))
		{
			m_LinerDataVector[e_iIndex] = e_Data;
			this->DoLOD();
			return true;
		}
		return false;		
	}
//=========================
//
//=========================
	template <class T>
	bool				cLinerDataProcessor<T>::ChangeDataWithTime(int e_iIndex,T e_Data,float e_fTime)
	{
		if(IsLegalIndexInVector<T>(&m_LinerDataVector,e_iIndex))
		{
			m_LinerDataVector[e_iIndex] = e_Data; m_TimeVector[e_iIndex] = e_fTime; 
			this->DoLOD();
			return true;
		}
		return false;
	}
//=========================
//
//=========================
	template <class T>
	bool				cLinerDataProcessor<T>::InsertDataWithLiner(int e_iIndex,int e_iCount)
	{
		if(e_iCount != 0 && IsLegalIndexInVector<T>(&m_LinerDataVector,e_iIndex))
		{
			int		l_iInsertIndex = e_iIndex+1;
			float	l_fEndTime	= m_TimeVector[e_iIndex];
			T		l_EndData	= m_LinerDataVector[e_iIndex];
			if( m_LinerDataVector.size() == l_iInsertIndex )
			{//last index push to back
				for( int i=0;i<e_iCount;++i )
				{
					m_LinerDataVector.push_back(l_EndData);
					this->m_TimeVector.push_back(l_fEndTime);					
				}
			}
			else
			{
				l_fEndTime = this->m_TimeVector[l_iInsertIndex];
				l_EndData = this->m_LinerDataVector[l_iInsertIndex];
				float	l_fTimeGap = l_fEndTime-m_TimeVector[e_iIndex];
				l_fTimeGap /= (e_iCount+1);
				T		l_GapData = l_EndData-m_LinerDataVector[e_iIndex];
				l_GapData = l_GapData/(float)(e_iCount+1);
				for( int i=0;i<e_iCount;++i )
				{
					l_EndData = l_EndData-l_GapData;
					l_fEndTime -= l_fTimeGap;
					m_LinerDataVector.insert(m_LinerDataVector.begin()+l_iInsertIndex,l_EndData);
					m_TimeVector.insert(m_TimeVector.begin()+l_iInsertIndex,l_fEndTime);
				}
			}
			return true;
		}
		return true;
	}
//=========================
//
//=========================
	template <class T>
	bool				cLinerDataProcessor<T>::InsertData(int e_iIndex,T e_Data,float e_fTime)
	{
		if(IsLegalIndexInVector<T>(&m_LinerDataVector,e_iIndex))
		{
			m_LinerDataVector.insert(m_LinerDataVector.begin()+e_iIndex,e_Data); 
			m_TimeVector.insert(m_TimeVector.begin()+e_iIndex,e_fTime); 
			this->DoLOD();
			return true;
		}
		return false;
	}
//=========================
//
//=========================
	template <class T>
	bool				cLinerDataProcessor<T>::RemoveDtaa(int e_iIndex)
	{
		if(IsLegalIndexInVector<T>(&m_LinerDataVector,e_iIndex))
		{
			m_LinerDataVector.erase(m_LinerDataVector.begin()+e_iIndex);
			m_TimeVector.erase(m_TimeVector.begin()+e_iIndex);
			this->DoLOD();
			return true;
		}
		return false;
	}
//=========================
//
//=========================
	template <class T>
	bool						cLinerDataProcessor<T>::DoDataLiner(T e_StartData,T e_EndData)
	{
		int	l_iSize = (int)m_TimeVector.size();
		if( l_iSize < 2 )
			return false;
		T	l_FirstData = m_LinerDataVector[0];
		T	l_EndData = m_LinerDataVector[l_iSize-1];
		T	l_Gap = l_EndData-l_FirstData;
		l_Gap /= (float)l_iSize;
		T	l_AddValue = T::Zero;
		for( int i=0;i<l_iSize-1;++i )
		{
			m_LinerDataVector[i] = l_FirstData+l_AddValue;
			l_AddValue += l_Gap;
		}
		m_LinerDataVector[l_iSize-1] = l_EndData;
		return true;
	}
//=========================
//
//=========================
	template <class T>
	bool						cLinerDataProcessor<T>::DoDataLiner(bool e_bUpToDownLiner)
	{
		int	l_iSize = (int)m_TimeVector.size();
		if( l_iSize < 2 )
			return false;
		T	l_Data1 = e_bUpToDownLiner?m_LinerDataVector[0]:m_LinerDataVector[l_iSize-1];
		T	l_Data2 = e_bUpToDownLiner?m_LinerDataVector[l_iSize-1]:m_LinerDataVector[0];
		return DoDataLiner(l_Data1,l_Data2);
	}
//=========================
//
//=========================
	template <class T>
	int		cLinerDataProcessor<T>::GetClosetPointIndex(T e_vComparePoint,float e_fScanDis)
	{
		return GetClosestPointIndex(e_vComparePoint,&this->m_LinerDataVector,e_fScanDis);
	}
//=========================
//
//=========================
	template <class T>
	T				cLinerDataProcessor<T>::GetCurveCenter(float*e_pfRightDownToLeftUpperLength)
	{
		return UT::GetCurveCenter(&this->m_LinerDataVector,e_pfRightDownToLeftUpperLength);
	}
	//scale all curve as vector(length as 1),then resize curve
//=========================
//
//=========================
	template <class T>
	void				cLinerDataProcessor<T>::Scale(float e_fScale)
	{
		int	l_iNum = (int)m_LinerDataVector.size();
		if( l_iNum)
		{
			float	l_fLength = 0.f;
			T	l_vCenter = GetCurveCenter(&l_fLength);
			for( int i=0;i<l_iNum;++i )
			{
				//scale the point as vector length 1
				T	*l_pvPos = &m_LinerDataVector[i];
				//point minus center
				T	l_vTargetVector = *l_pvPos - l_vCenter;
				//set to proper position
				l_vTargetVector *= e_fScale;
				//final position we expect
				*l_pvPos = l_vTargetVector+l_vCenter;
			}
			this->DoLOD();
		}		
	}
//=========================
//
//=========================
	template <class T>
	void cLinerDataProcessor<T>::TransformCurve(cMatrix44 e_mat)
	{
		size_t	l_uiSize = this->m_LinerDataVector.size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			Vector3	l_vPos(m_LinerDataVector[i]);
			Vector3	l_vNewPos = e_mat.TransformVector(l_vPos);
			m_LinerDataVector[i] = T((float*)&l_vNewPos);
		}
		this->DoLOD();	    
	}
//=========================
//
//=========================
	template <class T>
	void				cLinerDataProcessor<T>::Translate(T e_vPos)
	{
		size_t	l_uiSize = this->m_LinerDataVector.size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			T	l_vNewPos = m_LinerDataVector[i]+e_vPos;
			m_LinerDataVector[i] = l_vNewPos;
		}
		this->DoLOD();			
	}
//=========================
//
//=========================
	//template <class T>
	//void				cLinerDataProcessor<T>::RotateCurveWithSpecificCenterPosition(Vector3 e_vCenterPos,Quaternion e_Quaternion)
	//{
	//	size_t	l_uiSize = m_OriginalPointList.size();
	//	for( size_t i =0;i<l_uiSize;++i )
	//	{
	//		T	l_vOffset = m_OriginalPointList[i]-e_vCenterPos;
	//		Vector3	l_vOffsetPos(l_vOffset);
	//		cMatrix44	l_mat = e_Quaternion.ToMatrix()*cMatrix44::TranslationMatrix(l_vOffsetPos);
	//		m_OriginalPointList[i] = l_mat.GetAxis(MyMath::W)+e_vCenterPos;
	//	}
	//	this->DoLOD();	
	//}
//=========================
//
//=========================
	template <class T>
	void				cLinerDataProcessor<T>::RotateCurveWithCurveCenter(cMatrix44 e_mat)
	{
		int	l_iNum = (int)this->m_LinerDataVector.size();
		if( l_iNum)
		{
			float	l_fLength = 0.f;
			T	l_vCenter = GetCurveCenter(&l_fLength);
			if( l_vCenter == T::Zero )
			{//to cheat compiler			
			}
			else
			{
				for( int i=0;i<l_iNum;++i )
				{
					T	*l_pvPos = &m_LinerDataVector[i];
					*l_pvPos = *l_pvPos-l_vCenter;
					*l_pvPos = e_mat.TransformVector(*l_pvPos);
					*l_pvPos += l_vCenter;
				}
			}
			this->DoLOD();
		}	
	}
//=========================
//
//=========================
	template <class T>
	float			cLinerDataProcessor<T>::GetTotalDistance()
	{
		float	l_fDis = 0;
		int	l_iSize = (int)this->m_LinerDataVector.size()-1;
		for( int i=0;i<l_iSize;++i )
		{
			T	l_vDistance = m_LinerDataVector[i+1]-m_LinerDataVector[i];
			l_fDis += l_vDistance.Length();
		}
		return l_fDis;	
	}
//=========================
//
//=========================
	template <class T>
	void	cLinerDataProcessor<T>::DebugRender(bool e_bRenderPoint,bool e_bRenderIndex,Vector4 e_vColor,cMatrix44 e_mat)
	{
		if( !m_pCurrentLinerDataVector )
			return;
		if( m_LinerDataVector.size() < 1  )
			return;
		GLRender::RenderLine((std::vector<T>*)m_pCurrentLinerDataVector,e_vColor,e_mat,false,false);
		if( e_bRenderIndex )
		{
			size_t	l_uiSize = this->m_LinerDataVector.size();
			for( size_t i=0;i<l_uiSize;++i )
			{
				Vector3	l_vPos(m_LinerDataVector[i]);
				cGameApp::RenderFont(l_vPos.x,l_vPos.y,ValueToStringW(i).c_str());
			}
		}
		if( e_bRenderPoint )
			RenderPoints(&(this->m_LinerDataVector)[0],m_LinerDataVector.size(),15,e_vColor,e_mat);
		//if( e_bRenderIndex )
		//{
		//	size_t	l_uiSize = m_LinerDataVector.size();
		//	for( size_t i=0;i<l_uiSize;++i )
		//	{
		//		Vector3	l_vPos(m_LinerDataVector[i]);
		//		l_vPos = e_mat.TransformCoordinate(l_vPos);
		//		cGameApp::RenderFont(l_vPos.x,l_vPos.y,ValueToStringW(i).c_str());
		//	}
		//}
	}
	//hey define the new member here or function can not write in cpp file
	template class	cLinerDataProcessor<Vector2>;
	template class	cLinerDataProcessor<Vector3>;
	template class	cLinerDataProcessor<Vector4>;

}
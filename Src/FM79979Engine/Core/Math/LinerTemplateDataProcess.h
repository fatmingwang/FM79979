#pragma once

#include <vector>
#include "../XML/tinyxml.h"
#include "../XML/StringToStructure.h"
#include "../NamedTypedObject.h"

namespace UT
{
	extern float	GetFloatModulus(float e_fValue,float e_fDivision);
};

namespace	FATMING_CORE
{
	template<class T>
	void	TemplateDataIncreaseLod(std::vector<T>*e_pData)
	{
		std::vector<T>	l_OldData = *e_pData;
		e_pData->clear();
		// keep the first point
		e_pData->push_back(l_OldData[0]);
		size_t	l_uiSize = l_OldData.size()-1;
		for(unsigned int i=0;i<l_uiSize;++i)
		{

			//for data
			const T& l_OldDataValue1 = l_OldData[i];
			const T& l_OldDataValue2 = l_OldData[i+1];
			// average the 2 original points to create 2 new points. For each
			// CV, another 2 verts are created.
			T l_NewData1 = 0.75f*l_OldDataValue1 + 0.25f*l_OldDataValue2;
			T l_NewData2 = 0.25f*l_OldDataValue1 + 0.75f*l_OldDataValue2;

			e_pData->push_back(l_NewData1);
			e_pData->push_back(l_NewData2);

		}
		// keep the last point
		e_pData->push_back(l_OldData[l_OldData.size()-1]);	
	}

	class	cTimeAndDataLinerUpdateInterface:public NamedTypedObject
	{
		virtual	void						InternalInit() = 0;
	protected:
		//call by whom inherent cTimeAndDataLinerUpdateInterface
		void								InternalElementParse(TiXmlElement*e_pTiXmlElement);
		virtual	TiXmlElement*				ToTiXmlElement();
		//
		//
		virtual	void						DoLOD();
		virtual	void						IncreaseLod();
		int									m_iCurrentWorkingIndex;
		bool								m_bLoop;
		float								m_fPastTime;
		std::vector<float>					m_TimeVector;
		std::vector<float>*					m_pLODTimeVector;
		std::vector<float>*					m_pCurrentTimeVector;
		//increade or decrease liner data 
		int									m_iLOD;
		int									UpdateTimeVectorDataIndex(float e_fElpaseTime,float*e_pfLerpValue,float*e_fRestTimeToNextStep,float*e_pfNextStepTimeDiff,bool*e_pEndPoint);
		GET_SET_DEC(bool,m_bUpdateWithLiner,IsUpdateWithLiner,SetUpdateWithLiner);
	public:
		cTimeAndDataLinerUpdateInterface();
		cTimeAndDataLinerUpdateInterface(cTimeAndDataLinerUpdateInterface*e_pcTimeAndDataLinerUpdateInterface);
		virtual	cTimeAndDataLinerUpdateInterface*	Clone() = 0;
		virtual	~cTimeAndDataLinerUpdateInterface();
		//lazy to do this now lah
		//virtual	void						DumpTo(cTimeAndDataLinerUpdateInterface*e_pSrc) = 0;
		//play done or init is not called yet.
		bool								IsDuringWorking(){ if( m_iCurrentWorkingIndex != -1 )return true;return false; }
		void								Init();
		virtual	void						Update(float e_fElpaseTime) = 0;
		virtual	void						UpdateByGlobalTime(float e_fElpaseTime);
		virtual	void						Clear() = 0;
		virtual	void						Rearrange(float e_fNewTime);
		virtual	void						InvertOrder() = 0;
		virtual	float						GetEndTime();
		virtual	float						GetCurrentLerpValue();
		void								ChangeTime(int e_iIndex,float e_fTime);
		void								SetTimeVector(std::vector<float>e_TimeVector){ m_TimeVector = e_TimeVector; }
		float*								GetTime(int e_iIndex){if((int)m_TimeVector.size()>e_iIndex && e_iIndex!= -1)return &m_TimeVector[e_iIndex];return 0;}
		int									Count(){ return (int)m_TimeVector.size(); }
		virtual	bool						RemoveDtaa(int e_iIndex) = 0;
		virtual	bool						InsertDataWithLiner(int e_iIndex,int e_iCount) = 0;
		virtual	void*						GetDataPointerByIndex(int e_iIndex){ assert(0&&"please override this" );return 0; }
		void								DoTimeLiner();
		void								SetLoop(bool e_bLoop){ m_bLoop = e_bLoop; }
		bool								IsLoop(bool e_bLoop){ return m_bLoop; }
	};

	template<class T>class	cLinerDataProcessor:public cTimeAndDataLinerUpdateInterface
	{
		virtual	void				InternalInit()override{}
		//check void cCurve::IncreaseLod()
		virtual	void				IncreaseLod()override;
	protected:
		//find biggest  value,for debug render.
		T*							m_pvValueForSmallestBiggestAndDis;
		//the data for current update data.
		T							m_CurrentData;
		//
		std::vector<T>				m_LinerDataVector;
		//won'talways exists if SetLOD is not called.
		std::vector<T>*				m_pLODDataVector;
		//indicate the original data.
		std::vector<T>*				m_pCurrentLinerDataVector;
	public:
		DEFINE_TYPE_INFO()
		cLinerDataProcessor();
		cLinerDataProcessor(cLinerDataProcessor*e_pLinerDataProcessor);
		virtual ~cLinerDataProcessor();
		cLinerDataProcessor(TiXmlElement*e_pElement);
		static cLinerDataProcessor*GetMe(TiXmlElement*e_pElement);
		virtual	cTimeAndDataLinerUpdateInterface*	Clone()override;
		virtual	TiXmlElement*						ToTiXmlElement()override;
		//
		virtual	void								DumpTo(cLinerDataProcessor<T>*e_pTarget);
		virtual	void*								GetDataPointerByIndex(int e_iIndex)override;
		int											GetLOD();
		void										SetLOD(int e_iLOD);
		std::vector<T>*								GetLinerDataVector();
		T*											GetData(int e_iIndex);
		bool										GetLastData(T&e_Data);
		virtual	void								Init();
		//for debug won't be called in Init()
		void										InitValueForSmallestBiggestAndDis();
		virtual	void								Update(float e_fElpaseTime)override;
		virtual	void								Clear()override;
		virtual	void								InvertOrder()override;
		T											GetCurrentData();
		virtual	void								AddData(T e_Data,float e_fTime);
		virtual	void								SetData(std::vector<float>e_TimeVector,std::vector<T>e_LinerDataVector);
		virtual	bool								ChangeData(int e_iIndex,T e_Data);
		virtual	bool								ChangeDataWithTime(int e_iIndex,T e_Data,float e_fTime);
		virtual	bool								InsertDataWithLiner(int e_iIndex,int e_iCount)override;
		virtual	bool								InsertData(int e_iIndex,T e_Data,float e_fTime);
		virtual	bool								RemoveDtaa(int e_iIndex)override;
		//if data is changed call this for necessary data change.
		virtual	void								DoLOD()override;
		bool										DoDataLiner(T e_StartData,T e_EndData);
		bool										DoDataLiner(bool e_bUpToDownLiner);
		int											GetClosetPointIndex(T e_vComparePoint,float e_fScanDis = 20.f);
		T											GetCurveCenter(float*e_pfRightDownToLeftUpperLength = 0);
		//scale all curve as vector(length as 1),then resize curve
		void										Scale(float e_fScale);
		void										TransformCurve(cMatrix44 e_mat);
		void										Translate(T e_vPos);
		void										RotateCurveWithCurveCenter(cMatrix44 e_mat);
		//this one is slow.
		float										GetTotalDistance();
		void										DebugRender(bool e_bRenderPoint = false,bool e_bRenderIndex = false,Vector4 e_vColor = Vector4::One,cMatrix44 e_mat = cMatrix44::Identity);
		void										DebugRenderWithMaximumValue(T e_vMaximumValue,bool e_bRenderPoint = false,bool e_bRenderIndex = false,Vector4 e_vColor = Vector4::One,cMatrix44 e_mat = cMatrix44::Identity);
		//for max and min dis
		T*											GetMaxValueDis();
		//Vector3(0,0,0),Vector3(1,2,0),0.1,0.025,0.1
		//NewStee = e_fSteepGredient;
		//0,0,0
		//0,0,0+(1,2,0)*(1+(index*e_fGredient)+for(NewStee+=e_fSteepGredient))
		static cLinerDataProcessor<T>*				GererateDataWithPreviousData(int e_iNumPoints,T e_vStartPos,T e_vStepPos,float e_fGredient = 0.1f,float e_fSteepGredient = 0.025f,float e_fTimeDiff = 0.1f);
		static cLinerDataProcessor<T>*				GererateData(int e_iNumPoints,T e_vStartPos,T e_vStepPos,float e_fGredient = 0.1f,float e_fSteepGredient = 0.025f,float e_fTimeDiff = 0.1f);
	};

	//just a quick update interface
	class	cLinerDataContainer
	{
	public:
		cLinerDataContainer(){}
		cLinerDataContainer(cLinerDataContainer*e_pLinerDataContainer)
		{
			int	l_iSize = (int)e_pLinerDataContainer->m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				cTimeAndDataLinerUpdateInterface*l_pData = e_pLinerDataContainer->m_ContainerVector[i]->Clone();
				this->m_ContainerVector.push_back(l_pData);
			}
		}
		virtual	~cLinerDataContainer()
		{
			Destroy();
		}
		std::vector<cTimeAndDataLinerUpdateInterface*>	m_ContainerVector;
		void											AddData(cTimeAndDataLinerUpdateInterface*e_pData){ m_ContainerVector.push_back(e_pData); }
		//
		void	Init()
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->Init();
			}
		}
		void	Update(float e_fElpaseTime)
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->Update(e_fElpaseTime);
			}
		}
		void	UpdateByGlobalTime(float e_fElpaseTime)
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->UpdateByGlobalTime(e_fElpaseTime);
			}	
		}

		void	Rearrange(float e_fNewTime)
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->Rearrange(e_fNewTime);
			}
		}

		float	GetEndTime()
		{
			float	l_fLastTime = 0.f;
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				float l_fLastTime2 = m_ContainerVector[i]->GetEndTime();
				if( l_fLastTime < l_fLastTime2 )
				{
					l_fLastTime = l_fLastTime2;
				}
			}
			return l_fLastTime;
		}

		virtual	void				InvertOrder()
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->InvertOrder();
			}	
		}

		virtual	void				Clear()
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->Clear();
			}
		}

		virtual	void				Destroy()
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				cTimeAndDataLinerUpdateInterface*l_pData = m_ContainerVector[i];
				SAFE_DELETE(l_pData);
			}
			m_ContainerVector.clear();
		}
	};
//end namespace
}
//endif
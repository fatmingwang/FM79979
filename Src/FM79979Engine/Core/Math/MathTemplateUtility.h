#pragma once

namespace UT
{
	extern std::vector<int>	GenerateRandomTable(int e_iCount,int e_iMaxValue);
	extern Vector3	rand(Vector3 e_vMin,Vector3 e_vMax);
	extern Vector2	rand(Vector2 e_vMin,Vector2 e_vMax);
	extern int		rand(int	e_iMin,	int		e_iMax);
	extern int		rand(long	e_iMin,	long	e_iMax);
	extern float	rand(float	e_fMin,	float	e_fMax);
	extern POINT	rand(POINT	e_Min,	POINT	e_Max);
	//generate a random vector from input data
	template<class T> std::vector<T>	GenerateRandomTable(std::vector<T>*e_pData)
	{
		int	l_Size = (int)e_pData->size();
		std::vector<int>l_RandomTable = GenerateRandomTable(l_Size,l_Size);
		std::vector<T>l_ResultTable;
		l_ResultTable.resize(l_Size);
		for( int i=0;i<l_Size;++i )
		{
			l_ResultTable[i] = ((*e_pData)[l_RandomTable[i]]);
		}
		return l_ResultTable;
	}

	template <class TYPE>inline	TYPE	LERP(TYPE e_fSrc,TYPE e_fDest,float e_fLERPValue )
	{
		return (TYPE)((e_fDest-e_fSrc)*e_fLERPValue+e_fSrc);
	}
	//if position is smaller than start pos return 0
	//if position is bigger than end pos return 2
	template<class TYPE>inline float	GetPosPercentageInRange(TYPE e_Pos,TYPE e_Start,TYPE e_End)
	{
		if(e_Pos<e_Start)
			return 0.f;
		if(e_Pos>e_End)
			return 1.f;
		TYPE l_Pos = e_Pos-e_Start;
		TYPE l_Length = e_End-e_Start;
		float l_fPercentage = (float)l_Pos/l_Length;
		return l_fPercentage;
	}

	//for each
	//template<class InputIterator, class Function>Function for_each(InputIterator first, InputIterator last, Function f)
	//{
	//	for ( ; first!=last; ++first ) f(*first);
	//		return f;
	//}
	//!!now please use Vector 2instead this.!!
	template <class T>	struct	sMinMaxData
	{
		T			Min;
		T			Max;
		//T			Value;
		sMinMaxData(){  }
		sMinMaxData(Vector2 e_Value)
		{
			Min = (T)e_Value.x;
			Max = (T)e_Value.y;
		}
		//inline sMinMaxData<T> operator =(Vector2 e_vData)
		//{
		//	sMinMaxData<T>	l_Data;
		//	l_Data.Min = (T)e_vData.x;
		//	l_Data.Max = (T)e_vData.y;
		//	return l_Data;
		//}
		inline	T	Rand()
		{
			//Value = rand(Min,Max);
			//return Value;
			return rand((T)Min,(T)Max);
		}
	};
	//sample
	//sProbabilityVector<float>	m_ProbabilityValue;
	//sProbabilityVector<int>	m_ProbabilityValue;
	template <class NUMBER_TYPE>struct	sProbabilityVector
	{
		std::vector<NUMBER_TYPE>	ProbabilityVector;
		NUMBER_TYPE					m_TotalProbabiliy;
		void						CalculateTotalValue()
		{
			m_TotalProbabiliy = 0;
			for (int i = 0; i<(int)ProbabilityVector.size(); ++i)
				m_TotalProbabiliy += ProbabilityVector[i];
		}
		void						SetupProbabilityData(std::vector<NUMBER_TYPE> e_ProbabilityVector)
		{
			ProbabilityVector = e_ProbabilityVector; m_TotalProbabiliy = 0; for( int i=0;i<(int)ProbabilityVector.size();++i ) m_TotalProbabiliy += ProbabilityVector[i];
		}
		int							GetIndexByProbability()
		{
			int	l_iSize = (int)ProbabilityVector.size();
			std::vector<int>l_RandomTable = GenerateRandomTable(l_iSize,l_iSize);
			NUMBER_TYPE	l_ProbabilityValue = 0;
			NUMBER_TYPE	l_RandProbabilityValue = rand((NUMBER_TYPE)0,(NUMBER_TYPE)m_TotalProbabiliy);
			for( int i=0;i<l_iSize;++i )
			{
				auto l_AddValue = ProbabilityVector[l_RandomTable[i]];
				//probability is 0 dont need this.
				if (l_AddValue == 0)
					continue;
				l_ProbabilityValue += l_AddValue;
				if( l_ProbabilityValue >= l_RandProbabilityValue )
					return l_RandomTable[i];
			}
			if (l_ProbabilityValue == 0)
				return -1;
			return l_iSize-1;
		}
		void						Clear(){ ProbabilityVector.clear(); m_TotalProbabiliy = 0; }
	};
	//sample
	//struct	sHoldTimeAndType
	//{
	//	UT::sTimeCounter			TC;
	//	eFAICharacterBehaviodStatus	FAICharacterBehaviodStatus;
	//	UT::sMinMaxData<float>		RandHoldTime;
	//};
	//sProbabilityWithValue<float,sHoldTimeAndType>	m_ProbabilityValue;
	//sProbabilityWithValue<float,int>	m_ProbabilityValue;
	template <class NUMBER_TYPE,class VALUE>struct	sProbabilityWithValue
	{
		sProbabilityWithValue(){ m_TotalProbabiliy = 0; }
		~sProbabilityWithValue(){}
		std::vector<VALUE>			m_ValueVector;
		std::vector<NUMBER_TYPE>	m_ProbabilityVector;
		NUMBER_TYPE					m_TotalProbabiliy;
		void						CalculateTotalValue()
		{
			m_TotalProbabiliy = 0;
			for (int i = 0; i<(int)m_ProbabilityVector.size(); ++i)
				m_TotalProbabiliy += m_ProbabilityVector[i];
		}
		void						SetupProbabilityData(std::vector<NUMBER_TYPE> e_ProbabilityVector) { m_ProbabilityVector = e_ProbabilityVector; CalculateTotalValue(); }
		void						SetupValueData(std::vector<VALUE> e_ValueVector){ m_ValueVector = e_ValueVector; }
		void						SetupData(std::vector<NUMBER_TYPE> e_ProbabilityVector,std::vector<VALUE> e_ValueVector){SetupProbabilityData(e_ProbabilityVector);SetupValueData(e_ValueVector);}
		int							GetIndexByProbability()
		{
			int	l_iSize = (int)m_ProbabilityVector.size();
			std::vector<int>l_RandomTable = GenerateRandomTable(l_iSize,l_iSize);
			NUMBER_TYPE	l_ProbabilityValue = 0;
			NUMBER_TYPE	l_RandProbabilityValue = rand((NUMBER_TYPE)0,(NUMBER_TYPE)m_TotalProbabiliy);
			for( int i=0;i<l_iSize;++i )
			{
				l_ProbabilityValue += m_ProbabilityVector[l_RandomTable[i]];
				if( l_ProbabilityValue > l_RandProbabilityValue )
					return i;
			}
			return l_iSize-1;
		}
		VALUE*						GetValueByProbability()
		{
			int	l_iIndex = GetIndexByProbability();
			if( l_iIndex != -1 )
			{
				return &m_ValueVector[l_iIndex];
			}
			return nullptr;
		}
		void						Clear(){ m_ProbabilityVector.clear(); m_TotalProbabiliy = 0; m_ValueVector.clear(); }
	};
//end namespace UT
}
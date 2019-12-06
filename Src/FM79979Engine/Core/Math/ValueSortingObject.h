#ifndef _VALUE_SORTING_OBJECT_H_
#define _VALUE_SORTING_OBJECT_H_
namespace	FATMING_CORE
{
	/*template<class T> */
	class cSortObject
	{
		void DoBigger(cSortObject*e_pObject);
		void DoSmaller(cSortObject*e_pObject);
	public:
		cSortObject*m_pSmaller;
		cSortObject*m_pBigger;
		float m_fValue;
		int m_iCompareTime;
		//T* m_pSrc;
		cSortObject(float e_fValue);
		void Sort(cSortObject*e_pObject);
		cSortObject*GetSmallest();

	};

	//rand range is 0~65536.
	//srand(222222);	
	//std::vector<cSortObject*> l_TestVector;
	//for( int i=0;i<100;++i )
	//{
	//cSortObject *l_p = new cSortObject(frand(-55,55));
	//Dump(l_p->m_fValue);
	//l_TestVector.push_back(l_p);
	//if( i>0 )
	////l_TestVector[0]->Sort(l_p);
	//l_TestVector[(l_TestVector.size()-1)/2]->Sort(l_p);
	//}
	//FMLog::LogWithFlag(L"\n");
	//Dump(l_TestVector[0]);

//end namespace	FATMING_CORE
}
#endif
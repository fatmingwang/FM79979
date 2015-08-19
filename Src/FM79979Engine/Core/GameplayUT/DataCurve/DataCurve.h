#ifndef _PROBABILITY_RESULT_CURVE_H_
#define _PROBABILITY_RESULT_CURVE_H_

#include "../BinaryFile.h"

namespace FATMING_CORE
{
	//for quick view data to chart.take a look at ProbabilityStatics.
	//ex: game probability and profits chart
	class	cDataAndCurveListWithXMlSechma;
	class	cDataAndCurve:public NamedTypedObject
	{
		Vector2				m_vRealMousePoint;
		Vector2				m_vMouseHitPoint;
		std::wstring		m_strCurrentPointData;
		float				m_fMaxMinScale;
		friend class cDataAndCurveListWithXMlSechma;
	public:
		struct	sDataAndCount
		{
			int		iOriginalCount;
			//double for store big enough data
			double*	pdbOriginalfData;
			sDataAndCount(int e_iCount);
			~sDataAndCount(){ iOriginalCount = 0;SAFE_DELETE(pdbOriginalfData);}
		};
	private:
		//
		Vector2*		m_pRenderLineData;
		//original data
		sDataAndCount*	m_pOriginalData;
		//compress data,ofinal render data
		sDataAndCount*	m_pCompressData;
		//
		double			m_dbTotalValue;
		//for render data
		double			m_dbMaxValue;
		double			m_dbMinValue;
		double			m_dbAeverageValue;
		double			m_dbFirstPointValue;
		//
		double			m_dbMaxValueForRealData;
		double			m_dbMinValueForRealData;
		//
		void			AssignMinMaxValue(sDataAndCount*e_pData);
		Vector4			m_vRenderRect;
	public:
		cDataAndCurve(int e_iCount);
		cDataAndCurve(double*e_pData,int e_iCount);
		virtual ~cDataAndCurve();
		//
		void			CalMinMaxValue();
		//
		void			CreateBuffer(int e_iCount);
		void			SetData(double*e_pData,int e_iCount);
		void			AssignData(int e_iIndex,double e_fData);
		//compress pOriginalData to pCompressData,so we could show needed sample
		void			Compress(double e_fPercentage);
		//
		sDataAndCount*	GetData();
		//
		void			Render(float e_fMaxMinScale,Vector4 e_vColor,Vector4 e_vRenderRect,bool e_bDrawRect);
		void			Render(float e_fMaxMinScale,int e_iStartCount,int e_iEndCount,Vector4 e_vColor,Vector4 e_vRenderRect,bool e_bDrawRect);
		sDataAndCount*	GetDataAndCount(){return m_pOriginalData;}
		void			MouseMove(float e_fPosX,float e_fPosY);
	};

	//<Root>
	//	<Header	Name="" Size=""	/>
	//	<Data	Name="xxxx" Size="1" />
	//	<Data	Name="xxxx" Size="1" />
	//	<Data	Name="xxxx" Size="1" />
	//	<Data	Name="xxxx" Size="1" />
	//</Root>

	class	cDataAndCurveListWithXMlSechma:public cNamedTypedObjectVector<cDataAndCurve>
	{
		struct	sData
		{
			std::wstring	strName;
			//int				iSize;
			eDataType		DataType;
		};
		cBinaryFile*m_pFile;
		void		ProcessRootData(TiXmlElement*e_pTiXmlElement);
		sData		ProcessData(TiXmlElement*e_pTiXmlElement);
		sData		m_Header;
		double			m_dbMaxValue;
		double			m_dbMinValue;
		double			m_dbAeverageValue;
	public:
		cDataAndCurveListWithXMlSechma(const char*e_strXMLSechemaName,const char*e_strBinaryFile);
		virtual ~cDataAndCurveListWithXMlSechma();
		void		Render(float e_fScale,int e_iIndex,Vector4 e_vRenderRect,Vector4 e_vColor,bool e_bDrawRect);
		void		Render(wchar_t*e_strName,Vector4 e_vRenderRect);
		void		Render(std::vector<int>e_RenderObjectList,Vector4 e_vRenderRect);
		void		RenderAll(Vector4 e_vRenderRect);
		void		CalMinMaxValue();
		void		CalMinMaxValue(std::vector<int>e_SelectIndices);
	};
}
#endif
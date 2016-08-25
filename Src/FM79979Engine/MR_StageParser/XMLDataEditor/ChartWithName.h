#pragma once
#include <vector>

class cChartWithName
{
public:
	bool	m_bShowValue;
	int		m_ObjectIndexDistance;
	int		m_iHeightScale;
	cChartWithName();
	~cChartWithName();
	struct sNameAndData
	{
		cLinerDataProcessor<Vector3>*	m_pData;
		std::wstring					m_Name;
		sNameAndData(){m_pData = new cLinerDataProcessor<Vector3>();}
		~sNameAndData(){SAFE_DELETE(m_pData);}
	};
	std::vector<int>					m_RenderIndex;
	std::vector<sNameAndData*>			m_DataVector;
	void								AddData(sNameAndData* e_pNameAndData);
	void								Render();
};
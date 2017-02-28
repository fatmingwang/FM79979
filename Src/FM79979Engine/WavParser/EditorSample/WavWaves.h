#pragma once


class cWaveFile;
class cWavWaves
{
	struct sChannelCurve
	{
		int						iCount;
		int						iMax;
		int						iAverage;
		sChannelCurve(){ iMax = iCount = iAverage = 0; }
		std::vector<Vector3>	LinesPoint;
		void					AssignData(unsigned char*e_pData,int e_iCount,int e_iDataStep);
		void					SetAverageAndMax(int e_iMax,int e_iAverage);
		void					Render(Vector2 e_vPos);
	};

	std::vector<sChannelCurve*>		m_ChannelCurveVector;
	cWaveFile*						m_pWaveInfo;
	//
	void	RenderChannel(int e_iChannelIndex);
public:
	cWavWaves();
	~cWavWaves();
	void	SetData(cWaveFile*e_pWaveInfo);
	void	Render();
private:

};
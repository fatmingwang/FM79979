#pragma once


class cWaveInfo;
class cWavWaves
{
	unsigned char*	m_pData;
	int				m_iLength;
	int				m_iDataBlock;

	std::vector<Vector3>	m_LinesPoint;
public:
	cWavWaves();
	~cWavWaves();
	void	SetData(cWaveInfo*e_pWaveInfo);
	void	Render();
private:

};
#pragma once

//https://www.itread01.com/content/1543374489.html
class cOpenCVTest
{
	std::string		m_strCameraURL;
	std::mutex		m_CameraReadMutex;
	std::mutex		m_FrameMutex;
	struct sMatWithFlag
	{
		bool						bNewData;
		class cv::Mat* pFrame;
		vector < unsigned char >	EncodedVector;
		sMatWithFlag();
		~sMatWithFlag();
	};
	sMatWithFlag*				m_pFrame;
	FATMING_CORE::cBaseImage*	m_pVideoImage;
	class cv::VideoCapture*		m_pVideoCapture;
	void	CameraReadThread(float e_fElpaseTime);
public:
	cOpenCVTest();
	~cOpenCVTest();
	void	OpenCamera(const char* e_strCameraURL = nullptr);
	void	Update(float e_fElpaseTime);
	void	Render();
};
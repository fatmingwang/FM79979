#pragma once

#include "opencv2/opencv.hpp"

class OpenCVTestBase
{	
public:
	struct sMatWithFlag
	{
		bool						bNewData;
		class cv::Mat* pFrame;
		vector < unsigned char >	EncodedVector;
		sMatWithFlag()
		{
			pFrame = new cv::Mat;
			bNewData = false;
		}
		~sMatWithFlag()
		{
			SAFE_DELETE(pFrame);
		}
	};
	virtual void	OpenCamera(const char* e_strCameraURL = nullptr) = 0;
	virtual void	Update(float e_fElpaseTime) = 0;
	virtual void	Render() = 0;
};
#pragma once
#include "opencv2/opencv.hpp"
class OpenCVTestBase:public NamedTypedObject
{	
public:
	OpenCVTestBase();
	struct sMatWithFlag
	{
		bool						bNewData;
		class cv::Mat* pFrame;
		vector < unsigned char >	EncodedVector;
		sMatWithFlag();
		~sMatWithFlag();
	};
	virtual void	OpenCamera(const char* e_strCameraURL = nullptr) = 0;
	virtual void	Update(float e_fElpaseTime) = 0;
	virtual void	Render() = 0;
};
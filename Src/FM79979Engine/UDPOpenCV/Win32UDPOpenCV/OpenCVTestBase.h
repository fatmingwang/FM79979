#pragma once
#include "opencv2/opencv.hpp"
//https://stackoverflow.com/questions/7153935/how-to-convert-utf-8-stdstring-to-utf-16-stdwstring/7154226
std::wstring utf8_to_utf16(const std::string& utf8);

class OpenCVTestBase:public NamedTypedObject
{
public:
	OpenCVTestBase();
	struct sMatWithFlag
	{
		bool						bNewData;
		cv::Mat						Frame;
		vector < unsigned char >	EncodedVector;
		sMatWithFlag();
		~sMatWithFlag();
	};
	virtual void	OpenCamera(const char* e_strCameraURL = nullptr) = 0;
	virtual void	Update(float e_fElpaseTime) = 0;
	virtual void	Render() = 0;
};
#ifdef WIN32
#include "pch.h"
#else
#include "stdafx.h"
#endif
#include "OpenCVTestBase.h"

OpenCVTestBase::sMatWithFlag::sMatWithFlag()
{
	pFrame = new cv::Mat;
	bNewData = false;
}

OpenCVTestBase::sMatWithFlag::~sMatWithFlag()
{
	SAFE_DELETE(pFrame);
}

OpenCVTestBase::OpenCVTestBase()
{
	std::string l_strOpenCVInfo = "OpenCV version :";	l_strOpenCVInfo += CV_VERSION;
	l_strOpenCVInfo += "\nMajor version : ";			l_strOpenCVInfo += ValueToString(CV_MAJOR_VERSION);
	l_strOpenCVInfo += "\nMinor version : ";			l_strOpenCVInfo += ValueToString(CV_MINOR_VERSION);
	l_strOpenCVInfo += "\nSubminor version : ";			l_strOpenCVInfo += ValueToString(CV_SUBMINOR_VERSION);
	FMLog::Log(l_strOpenCVInfo.c_str(), false);
}
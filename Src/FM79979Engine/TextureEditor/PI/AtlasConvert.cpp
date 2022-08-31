#include "stdafx.h"
#include "AtlasConvert.h"
#include <sstream>

bool sAtlasData::Export(const char* e_strFileName)
{
	std::string l_strResult;
	l_strResult = strFileName;		l_strResult += "\n";
	l_strResult += strSize;			l_strResult += "\n";
	l_strResult += strFormat;		l_strResult += "\n";
	l_strResult += strFilter;		l_strResult += "\n";
	l_strResult += strRepeat;		l_strResult += "\n";
	for (int i = 0; i < (int)m_ImageDataVector.size(); ++i)
	{
		l_strResult += m_ImageDataVector[i].ToString();
	}
	return UT::SaveTxtToFile(e_strFileName, l_strResult.c_str(), (int)l_strResult.length());
}

void sAtlasData::Parse(const char* e_strText, int e_iIndex, float e_fScale)
{
	if (e_iIndex == eAltasHeader::eAH_NAME)
	{
		strFileName = e_strText;
	}
	else
	if (e_iIndex == eAltasHeader::eAH_SIZE)
	{
		POINT l_Point = GetPoint(e_strText);
		l_Point.x /= 2;
		l_Point.y /= 2;
		strSize = ValueToString(l_Point);
	}
	else
	if (e_iIndex == eAltasHeader::eAH_FORMAT)
	{
		strFormat = e_strText;
	}
	else
	if (e_iIndex == eAltasHeader::eAH_FILTER)
	{
		strFilter = e_strText;
	}
	else
	if (e_iIndex == eAltasHeader::eAH_REPEAT)
	{
		strRepeat = e_strText;
	}
	else
	{
		if (m_CurrentImageData.Parse(e_strText, e_fScale))
		{
			m_ImageDataVector.push_back(m_CurrentImageData);
		}
	}
}

bool sAtlasData::sImageData::Parse(const char* e_strText, float e_Scale)
{
	std::string l_strText = e_strText;
	std::string l_strData;
	POINT		l_Size = {-1,-1};
	auto l_DataStartPos = l_strText.find(":");
	if (l_DataStartPos != -1)
	{
		l_strData = l_strText.substr(l_DataStartPos, l_strText.length() - l_DataStartPos);
		if (m_iCurrentImageDataParseIndex != eDataType::eDT_NAME ||
			m_iCurrentImageDataParseIndex != eDataType::eDT_INDEX)
		{
			l_Size = GetPoint(l_strData);
			l_Size.x = (long)(l_Size.x * e_Scale);
			l_Size.y = (long)(l_Size.y * e_Scale);
		}
	}
	if (m_iCurrentImageDataParseIndex == eDataType::eDT_NAME)
	{
		strName = e_strText;
	}
	else
	if (m_iCurrentImageDataParseIndex == eDataType::eDT_ROTATE)
	{
		strRotate = e_strText;
	}
	else
	if (m_iCurrentImageDataParseIndex == eDataType::eDT_POS_XY)
	{
		strPosXY = "  xy: ";
		strPosXY += ValueToString(l_Size);
	}
	else
	if (m_iCurrentImageDataParseIndex == eDataType::eDT_TRIMMED_SIZE)
	{
		strTrimmedSize = "  size: ";
		strTrimmedSize += ValueToString(l_Size);
	}
	else
	if (m_iCurrentImageDataParseIndex == eDataType::eDT_ORIG_SIZE)
	{
		strOrigSize = "  orig: ";
		strOrigSize += ValueToString(l_Size);
	}
	else
	if (m_iCurrentImageDataParseIndex == eDataType::eDT_OFFSET)
	{
		strOffset = "  offset: ";
		strOffset += ValueToString(l_Size);
	}
	else
	if (m_iCurrentImageDataParseIndex == eDataType::eDT_INDEX)
	{
		strIndex = e_strText;
		m_iCurrentImageDataParseIndex = 0;
		return true;
	}
	++m_iCurrentImageDataParseIndex;
	return false;
}

std::string sAtlasData::sImageData::ToString()
{
	std::string l_strResult;
	l_strResult = strName;			l_strResult += "\n";
	l_strResult = strRotate;		l_strResult += "\n";
	l_strResult = strPosXY;			l_strResult += "\n";
	l_strResult = strTrimmedSize;	l_strResult += "\n";
	l_strResult = strName;			l_strResult += "\n";
	l_strResult = strOffset;		l_strResult += "\n";
	l_strResult = strIndex;			l_strResult += "\n";
	return l_strResult;
}

bool sAtlasData::DoScaleThenExport(const char* e_strSrcFileName, const char* e_strDestFileName,float e_fScale)
{
	if(UT::IsFileExists(e_strSrcFileName))
	{
		auto l_strText = UT::GetTxtFileContent(e_strSrcFileName);
		//https://stackoverflow.com/questions/46962888/splitting-a-string-in-c-using-strtok-separated-by-a-comma-deliminator-and-u
		//https://en.cppreference.com/w/cpp/string/basic_string/getline
		std::stringstream ss(l_strText);
		std::string to;
		std::vector<std::string>	l_AllTextVector;
		int l_iIndex = 0;
		while (std::getline(ss, to, '\n'))
		{
			Parse(to.c_str(), l_iIndex);
			l_AllTextVector.push_back(to);
		}
		return this->Export(e_strDestFileName);
	}
	return false;
}

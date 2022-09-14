#pragma once

#include <string>
#include <vector>


//DNCT::resizeImage
struct sAtlasData
{
	enum eAltasHeader
	{
		eAH_NAME = 0,
		eAH_SIZE,
		eAH_FORMAT,
		eAH_FILTER,
		eAH_REPEAT,
		eAH_MAX
	};


//img/connect_00002
//  rotate: true
//  xy: 982, 127
//  size: 117, 108
//  orig: 120, 120
//  offset: 1, 0
//  index: -1
	struct sImageData
	{
		enum eDataType
		{
			eDT_NAME = 0,
			eDT_ROTATE,
			eDT_POS_XY,
			eDT_TRIMMED_SIZE,
			eDT_ORIG_SIZE,
			eDT_OFFSET,
			eDT_INDEX,
			eDT_MAX
		};
		std::string strName;
		std::string strRotate = "rotate: false";
		std::string	strPosXY;//this is trimmed pos,original start point = PosXY-Offset
		std::string	strTrimmedSize;
		std::string	strOrigSize;
		std::string	strOffset;
		std::string	strIndex = "index: -1";
		bool		Parse(const char* e_strText,float e_Scale = 0.5f);//return true is finish try next
		int			m_iCurrentImageDataParseIndex = 0;
		std::string ToString();
	};
	struct sAtlas
	{
		std::string strFileName;
		std::string strSize;
		std::string	strFormat = "format: RGBA8888";
		std::string	strFilter = "filter: Linear,Linear";
		std::string	strRepeat = "repeat: none";
		std::vector<sImageData> m_ImageDataVector;
	};
	std::vector<sAtlas>		m_AtlasVector;
	sAtlas*					m_pCurrentAtlas = nullptr;

	sImageData				m_CurrentImageData;
//
	static void	PITosAtlasData(const char* e_strFileName);
	bool		Export(const char* e_strFileName);
	void		Parse(const char*e_strText,int e_iIndex, float e_fScale = 0.5);
	bool		DoScaleThenExport(const char* e_strSrcFileName, const char* e_strDestFileName,float e_fScale = 0.5);
};
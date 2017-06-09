#pragma once

//std::wstring strSongName; is name
class  cMusicInfo:public NamedTypedObject
{
public:
	std::wstring m_strFileName;
	std::wstring m_strInfo;
	cMusicInfo(){}
	virtual ~cMusicInfo(){}

};
#include "stdafx.h"
#include "DragonBoneJsonConvert.h"
#include <fstream>

using namespace nlohmann;

//"height": 1141,
//"name" : "B",
//"imagePath" : "B_tex.png"
//"width" : 2032,
//
//
//"SubTexture" : [
//{
//        "frameWidth": 720,
//        "y" : 1,
//        "frameHeight" : 1280,
//        "width" : 295,
//        "frameX" : -31,
//        "height" : 279,
//        "name" : "megawin_m",
//        "frameY" : -151,
//        "x" : 1727
//},
//std::string str(R"({"json": "beta"})");
//nlohmann::json js = json::parse(str);

void	AssignWithScale(nlohmann::json& e_Json, const char* e_strKey, float e_fScale)
//#define AssignWithScale(e_Json,e_fScale)			
{
	if (e_Json.contains(e_strKey))
	{
		auto l_Json = e_Json.at(e_strKey);
		if (!l_Json.is_null())
		{
			auto l_Value = l_Json.get<int>();
			l_Value = (int)(l_Value * e_fScale);
			e_Json[e_strKey] = l_Value;
		}
	}
}

void cDragonBoneJsonConvert::ParseThenExport(const char* e_strTargetFileName, const char* e_strExportFileName, float e_fScale)
{
	std::ifstream ifs(e_strTargetFileName);
	nlohmann::json jf = json::parse(ifs);


	AssignWithScale(jf,"width", e_fScale);
	AssignWithScale(jf,"height", e_fScale);
	auto l_SubTextureArray = jf["SubTexture"];
	for (size_t i = 0; i< l_SubTextureArray.size();++i)
	//for (auto it  = l_SubTextureArray.begin(); it != l_SubTextureArray.end(); ++it)
	{
		//auto l_Key = it.key();
		//it.value();
		//auto l_SubTexture = l_SubTextureArray.at(i);
		//auto l_SubTexture = it.value();
		auto l_SubTexture = l_SubTextureArray.at(i);
		AssignWithScale(l_SubTexture, "frameWidth", e_fScale);
		AssignWithScale(l_SubTexture, "frameHeight", e_fScale);
		AssignWithScale(l_SubTexture, "x", e_fScale);
		AssignWithScale(l_SubTexture, "y", e_fScale);
		AssignWithScale(l_SubTexture, "frameX", e_fScale);
		AssignWithScale(l_SubTexture, "frameY", e_fScale);
		AssignWithScale(l_SubTexture, "width", e_fScale);
		AssignWithScale(l_SubTexture, "height", e_fScale);
		l_SubTextureArray.at(i) = l_SubTexture;
		FMLog::Log(l_SubTexture.dump().c_str(),false);
		//it.value() = l_SubTexture;
	}
	jf["SubTexture"] = l_SubTextureArray;
	std::ofstream file(e_strExportFileName);
	file << jf;
}

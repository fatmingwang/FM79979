#include "stdafx.h"
#include "FishLockPoints.h"

//<ImageCollisionData Name = "ChinaDragon_0006_1" Count = "33" PICount = "2" ImageName = "ChinaDragon_0006_1.pi,ChinaDragon_0006_2.pi">
//	<CollisionData Name = "DragonNormalMove_00000" Count = "6">
//		<Shape Type = "Convex" DataCount = "6" Data = "-144.00,-45.00,0.00,-152.00,-167.00,0.00,-62.00,-247.00,0.00,223.00,-31.00,0.00,158.00,7.00,0.00,49.00,-3.00,0.00" / >
//	</CollisionData>
//</ImageCollisionData>
bool cPIUnitAndPointsVector::MyParse(TiXmlElement * e_pRoot)
{
	e_pRoot = e_pRoot->FirstChildElement();
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
		auto l_strName = ATTRIBUT_VALUE(e_pRoot, "Name");
		if(l_strName)
		{
			cPIUnitAndPoints*l_pPIUnitAndPoints = new cPIUnitAndPoints();
			l_pPIUnitAndPoints->SetName(l_strName);
			auto l_pShapeData = e_pRoot->FirstChildElement();
			auto l_strData = ATTRIBUT_VALUE(l_pShapeData, "Data");
			if (!l_strData)
			{
				UT::ErrorMsg(l_strName,L"cPIUnitAndPointsVector::MyParse Data not exists!");
				continue;
			}
			l_pPIUnitAndPoints->Points = StringToVector3Vector(l_strData);
			this->AddObjectNeglectExist(l_pPIUnitAndPoints);
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	return false;
}

cPIUnitAndPointsVector::cPIUnitAndPointsVector(const char* e_strFileName):cSmartObject(this)
{
	if (!this->ParseWithMyParse(e_strFileName))
	{
		UT::ErrorMsg(e_strFileName,"file pare error");
	}
}

cPIUnitAndPointsVector::~cPIUnitAndPointsVector()
{
}

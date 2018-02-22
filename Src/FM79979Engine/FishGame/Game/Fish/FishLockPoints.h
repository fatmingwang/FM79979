#pragma once

struct cPIUnitAndPoints:public NamedTypedObject
{
	std::vector<Vector3> Points;
};

///////////////////////////////////////////////////////////////////////////////////////////
//the file content as same as collision.
class cPIUnitAndPointsVector:public cNamedTypedObjectVector<cPIUnitAndPoints>,public cNodeISAX,public cSmartObject
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot)override;
public:
	cPIUnitAndPointsVector(const char* e_strFileName);
	virtual ~cPIUnitAndPointsVector();
};
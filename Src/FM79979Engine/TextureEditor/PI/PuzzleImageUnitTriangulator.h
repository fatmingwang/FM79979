#pragma once
#include "poly2tri\poly2tri.h"
//vector<p2t::Point*>*polyline = new vector<p2t::Point*>;
//polyline->push_back(new p2t::Point(0, 0));
//polyline->push_back(new p2t::Point(10, 0));
//polyline->push_back(new p2t::Point(0, 10));
//polyline->push_back(new p2t::Point(10, 10));
//
//CDT* cdt = new CDT(*polyline);
//cdt->Triangulate();
////auto triangles = cdt->GetTriangles();
//delete cdt;
//DELETE_POINTER_VECTOR(polyline, p2t::Point*);

enum ePointsToTriangulatorType
{
	ePTPT_ADD = 0,
	ePTPT_DELETE,
	ePTPT_MOVE,
	ePTPT_MAX,
};

class cPuzzleImageUnitTriangulator:public NamedTypedObject
{
	std::vector<Vector2>		m_PointVector;
	std::vector<Vector2>		m_TriangleVector;

	GET_SET_DEC(ePointsToTriangulatorType,m_ePointsToTriangulatorType,GetPointsToTriangulatorType,SetPointsToTriangulatorType);
	GET_SET_DEC(cUIImage*,m_pTargetImage,GetTargetImage,SetTargetImage);
	int							GetClosestPoint(Vector2 e_vPos);
	int							m_iFocusPoint;				//which one point is closest point
	void						GenerateTriangle();
	sMouseMoveData				m_MouseMoveData;
	enum eMouseBehavior
	{
		eMB_DOWN = 0,
		eMB_MOVE,
		eMB_UP
	};
	void						PointsToTriangulatorAddMouseDown(int e_iPosX, int e_iPosY, eMouseBehavior e_eMouseBehavior);
	void						PointsToTriangulatorDeleteMouseDown(int e_iPosX, int e_iPosY, eMouseBehavior e_eMouseBehavior);
	void						PointsToTriangulatorMoveMouseDown(int e_iPosX, int e_iPosY, eMouseBehavior e_eMouseBehavior);
public:
	cPuzzleImageUnitTriangulator(cUIImage*e_pTargetImage);
	//cPuzzleImageUnitTriangulator(TiXmlElement*e_pTiXmlElement);
	~cPuzzleImageUnitTriangulator();
	void    		MouseDown(int e_iPosX, int e_iPosY);
	void    		MouseMove(int e_iPosX, int e_iPosY);
	void    		MouseUp(int e_iPosX, int e_iPosY);
	void			Render();
};


class cPuzzleImageUnitTriangulatorManager:public cNamedTypedObjectVector<cPuzzleImageUnitTriangulator>
{
	using cNamedTypedObjectVector<cPuzzleImageUnitTriangulator>::GetObject;
	using cNamedTypedObjectVector<cPuzzleImageUnitTriangulator>::RemoveObject;
public:
	cPuzzleImageUnitTriangulatorManager();
	~cPuzzleImageUnitTriangulatorManager();
	cPuzzleImageUnitTriangulator*			GetObject(cUIImage*e_pUIImage);
	void									RemoveObject(cUIImage*e_pUIImage);
};
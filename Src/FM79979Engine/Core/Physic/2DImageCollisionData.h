#ifndef _2D_IMAGE_COLLISION_DATA_H
#define _2D_IMAGE_COLLISION_DATA_H

#include "BulletShape.h"
#include "../Image/PuzzleImage.h"
#include "../Math/CurveManager.h"
#include "../AllImageInclude.h"
namespace FATMING_CORE
{
	enum	eCollisionType
	{
		eCT_Sphere = 0,//Sphere
		eCT_CONVEX,//Convex
		eCT_MAX,//empty and else
	};
	eCollisionType	StringToCollisionType(const wchar_t*e_strValue);
//<ImageCollisionData Name="08" Count="16">
//    <CollisionData Name="08fish-run0000" Count="0" />
//    <CollisionData Name="08fish-run0001" Count="2">
//        <Shape Type="Sphere" DataCount="1" Data="194.00,122.00,0.00," />
//        <Shape Type="Sphere" DataCount="1" Data="191.00,310.00,0.00," />
//    </CollisionData>
//    <CollisionData Name="08fish-run0002" Count="2">
//        <Shape Type="Convex" DataCount="5" Data="238.00,476.00,0.00,84.00,289.00,0.00,203.00,20.00,0.00,308.00,307.00,0.00,239.00,465.00,0.00," />
//        <Shape Type="Empty" />
//    </CollisionData>
//</ImageCollisionData>
	class	c2DImageCollisionDataForEditor:public ISAXCallback
	{
		Vector3	m_vMouseDownPos;
		void	ProcessImageCollisionData();
		void	ProcessCollisionData();
		void	ProcessShapeData();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);

		//cPuzzleImage*m_pPuzzleImage;
		//if true m_pPuzzleImage will be parse.
		//bool m_bParseImage;
		//for editor using
		int	m_iSelectedIndex;//for 
		int	m_iSelectedCollsiionDataIndex;//
		bool	m_bMouseDown;
		//btTransform	*m_mat;
	public:
		//this objec will be original position and no rotation,it wonly should work on editor
		struct	sImageCollisionData
		{
			struct	sCollisionData
			{
				//we mught need to drag the point,and this is what index is it(Sphere do not to consider this)
				int	iFocusPoint;
				//what shpae is it?sphere or convex,it should instead by eCollisionType
				bool	bConvex;
				//a vector contain a convex shape position data
				std::vector<Vector3>	ConvexHullPointData;
				sSphere*				pSphereData;
				//return the index has collided
				bool	Collide(float e_fRadius,float *e_pfMatrix);
				void	Render(Vector4 e_vColor);
				sCollisionData(){ pSphereData = 0; iFocusPoint = -1; }
				~sCollisionData(){ SAFE_DELETE(pSphereData); }
				sCollisionData(sCollisionData*e_pCollisionData);
			};  
			std::vector<sCollisionData*>	CollisionDataVector;
			sImageCollisionData(sImageCollisionData*e_pImageCollisionData);
			sImageCollisionData(){	iCollisionDataIndex = -1; pCurrentCollisionData = 0; }
			~sImageCollisionData(){ DELETE_VECTOR(CollisionDataVector,sCollisionData*); }

			void	Render(Vector4 e_vColor);
			int		Collide(float e_fRadius,float *e_pfMatrix);
			sCollisionData*pCurrentCollisionData;
			int		iCollisionDataIndex;
			void	SetSelectedIndex(int e_iIndex){	iCollisionDataIndex = e_iIndex;if( iCollisionDataIndex == -1 ){pCurrentCollisionData = 0;}else{pCurrentCollisionData = CollisionDataVector[iCollisionDataIndex];}}
			int		GetSelectedIndex(){ return iCollisionDataIndex; }
			//for editor puzzleImageUnit
			std::wstring	strName;
		};
		c2DImageCollisionDataForEditor();
		virtual ~c2DImageCollisionDataForEditor();
		void	ClearImageCollisionDataVector();
		void	Render(Vector4 e_vColor);
		bool	Collide(float e_fRadius,float *e_pfMatrix);
		//for game use
		cbtShapeCollision*	GetShapeCollisionByIndex(int e_iIndex);
		//for editor use
		std::vector<sImageCollisionData*>m_ImageCollisionDataVector;
		void	Export(char*e_strFileName,cPuzzleImage*e_pPuzzleImage);
		wchar_t	m_strForCollidedpiFile[MAX_PATH];
		//for editor use
		//0 for add 1 for fix
		sImageCollisionData*m_pCurrentImageCollisionData;
		void	SetSelectedIndex(int e_iIndex);
		int		GetSelectedIndex(){ return m_iSelectedIndex; }
		void	MouseDown(POINT e_Pos,int e_iMode);
		void	MouseUp(POINT e_Pos,int e_iMode);
		void	MouseMove(POINT e_Pos,int e_iMode);
		//for editor,if artist change the pi file it's possible to re-order the index.
		void	ReOrderIndex(cPuzzleImage*e_pPuzzleImage);	
	};
//=================================================
//
//=================================================
	class	c2DImageCollisionData:public ISAXCallback,public cNamedTypedObjectVector<cbtShapeCollision>
	{
		//for temp store data
		cbtConcaveShapeList*m_pbtConcaveShapeList;
		void	ProcessImageCollisionData();
		void	ProcessCollisionData();
		void	ProcessShapeData();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	public:
		DEFINE_TYPE_INFO();
		c2DImageCollisionData();
		virtual ~c2DImageCollisionData();
	};

//end namespace FATMING_CORE
}
#endif
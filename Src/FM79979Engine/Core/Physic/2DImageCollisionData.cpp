#include "../stdafx.h"
#include "2DImageCollisionData.h"
#include "../GLSL/Shader.h"

#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"

#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"

#include "BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
#include "BulletCollision/NarrowPhaseCollision/btPointCollector.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"


#include "../XML/XMLLoader.h"
#include "../XML/AtgXmlWriter.h"
namespace FATMING_CORE
{
	eCollisionType	StringToCollisionType(const wchar_t*e_strValue)
	{
		if(!wcscmp(e_strValue,L"Sphere"))
		{
			return eCT_Sphere;
		}
		else
		if(!wcscmp(e_strValue,L"Convex"))
		{
			return eCT_CONVEX;
		}
		return eCT_MAX;	
	}

	const wchar_t*         c2DImageCollisionData::TypeID( L"c2DImageCollisionData" );

	c2DImageCollisionDataForEditor::sImageCollisionData::sCollisionData::sCollisionData(sCollisionData*e_pCollisionData)
	{
		this->bConvex = e_pCollisionData->bConvex;
		pSphereData = 0;
		if( !this->bConvex )
		{
			this->pSphereData = new sSphere;
			*pSphereData = *e_pCollisionData->pSphereData;
		}
		else
		{
			this->ConvexHullPointData = e_pCollisionData->ConvexHullPointData;
		}
	}

	bool	c2DImageCollisionDataForEditor::sImageCollisionData::sCollisionData::Collide(float e_fRadius,float* e_pfMatrix)
	{
		if(this->bConvex&&ConvexHullPointData.size()>1)
		{
			cMatrix44	l_mat = cMatrix44::Identity;
			if(ConvexShapeCollideSphere(e_pfMatrix,e_fRadius,l_mat,(float*)&(ConvexHullPointData[0]),ConvexHullPointData.size(),sizeof(Vector3)))
				return true;
		}
		else
		{
			if( pSphereData )
			{
				cMatrix44	l_mat = cMatrix44::TranslationMatrix(pSphereData->vCenter);
				if(SphereCollideSphere(e_pfMatrix,e_fRadius,l_mat,pSphereData->fRadius))
					return true;
			}
		}
		return false;
	}

	void	c2DImageCollisionDataForEditor::sImageCollisionData::sCollisionData::Render(Vector4 e_vColor)
	{
		cMatrix44 l_mat = cMatrix44::Identity;
		if(this->bConvex)
		{
			RenderLine(&ConvexHullPointData,e_vColor,l_mat,true);
		}
		else
		{
			if( pSphereData )
			{
				sSphere l_Sphere = *pSphereData;
				cMatrix44	l_matTranslation = cMatrix44::TranslationMatrix(l_Sphere.vCenter);
				l_mat = l_mat*l_matTranslation;
				//l_mat.SetTranslation(l_mat.GetTranslation()+l_matTranslation.GetTranslation());
				l_Sphere.vCenter.x = l_mat.r[3].x;
				l_Sphere.vCenter.y = l_mat.r[3].y;
					RenderSphere(Vector2(l_Sphere.vCenter.x,l_Sphere.vCenter.y),l_Sphere.fRadius,e_vColor);
			}
		}
	}

	c2DImageCollisionDataForEditor::sImageCollisionData::sImageCollisionData(sImageCollisionData*e_pImageCollisionData)
	{
		iCollisionDataIndex = -1;
		pCurrentCollisionData = 0;
		for(int i=0;i<(int)e_pImageCollisionData->CollisionDataVector.size();++i)
		{
			sCollisionData*l_pCollisionData = e_pImageCollisionData->CollisionDataVector[i];
			sCollisionData*l_pNewCollisionData = new sCollisionData(l_pCollisionData);
			this->CollisionDataVector.push_back(l_pNewCollisionData);
		}
	}

	void	c2DImageCollisionDataForEditor::sImageCollisionData::Render(Vector4 e_vColor)
	{
		int	l_iIndex = 0;
		UINT	l_uiSize = CollisionDataVector.size();
		for( UINT i=0;i<l_uiSize;++i)
		{
			if( this->iCollisionDataIndex != i )
				CollisionDataVector[i]->Render(e_vColor);
			else
			{
				Vector4	l_vNewColor = e_vColor;
				l_vNewColor.x/=2;
				CollisionDataVector[i]->Render(l_vNewColor);
			}
		}
	}
	int		c2DImageCollisionDataForEditor::sImageCollisionData::Collide(float e_fRadius,float *e_pfMatrix)
	{
		int	l_iIndex = 0;
		UINT	l_uiSize = CollisionDataVector.size();
		for( UINT i=0;i<l_uiSize;++i)
		{
			if(CollisionDataVector[i]->Collide(e_fRadius,e_pfMatrix))
				return i;
		}
		return -1;
	}

	c2DImageCollisionDataForEditor::c2DImageCollisionDataForEditor():ISAXCallback(true)
	{
		m_iSelectedIndex = -1;
		m_iSelectedCollsiionDataIndex = -1;
		m_pCurrentImageCollisionData = 0;
		m_bMouseDown = false;
	}

	c2DImageCollisionDataForEditor::~c2DImageCollisionDataForEditor()
	{
		//SAFE_DELETE(m_pCurveLoader);
		//DELETE_VECTOR(m_btShapeCollisionVector,cbtShapeCollision*);
		DELETE_VECTOR(m_ImageCollisionDataVector,sImageCollisionData*);
	}

	//void	c2DImageCollisionDataForEditor::OpenFile(char*e_strFileName)
	//{
	//	m_iSelectedIndex = -1;
	//	SAFE_DELETE(m_pPuzzleImage);
	//	SAFE_DELETE(m_pCurveLoader);
	//	DELETE_VECTOR(m_btShapeCollisionVector,cbtShapeCollision*);
	//	m_pCurveLoader = new cCurveLoader();
	//	if(m_pCurveLoader->Parse(e_strFileName))
	//	{
	//		if( m_pCurveLoader->Count() )
	//		{
	//			btVector3	l_vCollisionPos[100];
	//			for(int i=0;i<m_pCurveLoader->Count();++i)
	//			{
	//				cCurveWithTime*l_pCurvesWithTime = (*m_pCurveLoader)[i];
	//				std::vector<Vector2> l_PointDataVector = l_pCurvesWithTime->GetOriginalPointList();
	//				int	l_iNumPoint = (int)l_pCurvesWithTime->GetOriginalPointList().size();
	//				assert(l_iNumPoint<100&&"too much collide points");
	//				for( int j=0;j<l_iNumPoint;++j )
	//				{
	//					l_vCollisionPos[j] = btVector3(l_PointDataVector[j].x,l_PointDataVector[j].y,0);
	//				}
	//				cbtShapeCollision*l_pbtShapeCollision = new cbtConvexHullShape((float*)l_vCollisionPos,l_iNumPoint);
	//				m_btShapeCollisionVector.push_back(l_pbtShapeCollision);
	//			}

	//			cImageParser	l_ImageParser;
	//			l_ImageParser.SetFromResource(true);
	//			char l_strFileNameForPI[MAX_PATH];
	//			std::string	l_strDirectory = UT::GetDirectoryWithoutFileName(e_strFileName);
	//			std::string	l_strFileName = UT::GetFileNameWithoutFullPath(e_strFileName);
	//			sprintf(l_strFileNameForPI,"%s%s.pi\0",l_strDirectory.c_str(),l_strFileName.c_str());
	//			if(l_ImageParser.Parse(l_strFileNameForPI))
	//			{
	//				m_pPuzzleImage = (cPuzzleImage*)l_ImageParser[0];
	//			}
	//		}
	//	}
	//}
	void	c2DImageCollisionDataForEditor::ClearImageCollisionDataVector()
	{
		DELETE_VECTOR(m_ImageCollisionDataVector,sImageCollisionData*);
	}

	void	c2DImageCollisionDataForEditor::Render(Vector4 e_vColor)
	{
		if( m_iSelectedIndex != -1 )
		{
			m_ImageCollisionDataVector[m_iSelectedIndex]->Render(e_vColor);
		}
	}

	bool	c2DImageCollisionDataForEditor::Collide(float e_fRadius,float *e_pfMatrix)
	{
		cbtConcaveShapeList	*l_btConcaveShapeList = new cbtConcaveShapeList;
		btTransform l_matTransform = btTransform::getIdentity();
		for( int i=0;i<(int)this->m_pCurrentImageCollisionData->CollisionDataVector.size();++i )
		{
			cbtShapeCollision*l_pbtCollisionShape = 0;
			if( m_pCurrentImageCollisionData->CollisionDataVector[i]->bConvex )
			{
				std::vector<Vector3>l_vVerticesData = m_pCurrentImageCollisionData->CollisionDataVector[i]->ConvexHullPointData;
				assert(l_vVerticesData.size()>0);
				l_pbtCollisionShape = new cbtConvexHullShape((float*)&(l_vVerticesData[0]),l_vVerticesData.size(),sizeof(Vector3));
			}
			else
			{
				sSphere*l_pSphere = m_pCurrentImageCollisionData->CollisionDataVector[i]->pSphereData;
				Vector3	l_vPos = l_pSphere->vCenter;
				l_pbtCollisionShape = new cbtSphereShape(l_pSphere->fRadius,l_pSphere->vCenter);
			}
			//l_pbtCollisionShape->SetName(L"79979");
			l_btConcaveShapeList->AddObjectNeglectExist(l_pbtCollisionShape);
		}
		cbtSphereShape*l_pbtSphereShapeTarget = new cbtSphereShape(e_fRadius);
		cMatrix44	lll = *(cMatrix44*)e_pfMatrix;
		l_pbtSphereShapeTarget->SetTransform(e_pfMatrix);
		bool	l_b = l_btConcaveShapeList->Collide(l_pbtSphereShapeTarget);
		delete l_btConcaveShapeList;
		delete l_pbtSphereShapeTarget;
		return l_b;
	}

	//cbtShapeCollision*	c2DImageCollisionDataForEditor::GetShapeCollisionByIndex(int e_iIndex)
	//{
	//	return (*this)[e_iIndex];
	//}

	void	c2DImageCollisionDataForEditor::SetSelectedIndex(int e_iIndex)
	{
		m_iSelectedIndex = e_iIndex;
		if( m_iSelectedIndex == -1 )
		{
			m_pCurrentImageCollisionData = 0;
		}
		else
		{
			if( (int)m_ImageCollisionDataVector.size() > m_iSelectedIndex )
				m_pCurrentImageCollisionData = m_ImageCollisionDataVector[m_iSelectedIndex];
			else
			{
				m_iSelectedIndex = -1;
				m_pCurrentImageCollisionData = 0;
			}
		}
	}

	void	c2DImageCollisionDataForEditor::MouseDown(POINT e_Pos,int e_iMode)
	{
		m_vMouseDownPos.x = (float)e_Pos.x;
		m_vMouseDownPos.y = (float)e_Pos.y;
		m_vMouseDownPos.z = 0;
		m_bMouseDown = true;
		if( m_pCurrentImageCollisionData && m_pCurrentImageCollisionData->iCollisionDataIndex != -1 )
		{
			Vector3	l_vPos = Vector3((float)e_Pos.x,(float)e_Pos.y,0.f);
			//m_pCurrentImageCollisionData->CollisionDataVector;
			if( m_pCurrentImageCollisionData->pCurrentCollisionData->bConvex )
			{
				switch(e_iMode)
				{
					case 0:
						m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData.push_back(l_vPos);
						break;
					case 1:
						m_pCurrentImageCollisionData->pCurrentCollisionData->iFocusPoint = UT::GetClosestPointIndex(l_vPos,&m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData);
						break;
				}
			}
			else
			{
				if(!m_pCurrentImageCollisionData->pCurrentCollisionData->pSphereData)
				{
					m_pCurrentImageCollisionData->pCurrentCollisionData->pSphereData = new sSphere();
					m_pCurrentImageCollisionData->pCurrentCollisionData->pSphereData->fRadius = 12.f;
					m_pCurrentImageCollisionData->pCurrentCollisionData->pSphereData->vCenter = l_vPos;
				}
				else
				{
					if( e_iMode != 3 )
						m_pCurrentImageCollisionData->pCurrentCollisionData->pSphereData->vCenter = l_vPos;
				}
			}
		}
	}

	void	c2DImageCollisionDataForEditor::MouseUp(POINT e_Pos,int e_iMode)
	{
		m_bMouseDown = false;
		if( m_pCurrentImageCollisionData && m_pCurrentImageCollisionData->iCollisionDataIndex != -1 )
		{
			if( e_iMode == 1 )
				m_pCurrentImageCollisionData->pCurrentCollisionData->iFocusPoint = -1;
		}
	}

	void	c2DImageCollisionDataForEditor::MouseMove(POINT e_Pos,int e_iMode)
	{
		if( m_bMouseDown )
		{
			if( e_iMode == 3 )
			{
				Vector3	l_vMovePos((float)e_Pos.x,(float)e_Pos.y,0);
				l_vMovePos -= m_vMouseDownPos;
				m_vMouseDownPos.x = (float)e_Pos.x;
				m_vMouseDownPos.y = (float)e_Pos.y;
				m_vMouseDownPos.z = 0;
				for( size_t i=0;i<m_pCurrentImageCollisionData->CollisionDataVector.size();++i )
				{
					sImageCollisionData::sCollisionData*l_pCollisionData = m_pCurrentImageCollisionData->CollisionDataVector[i];
					if( l_pCollisionData->bConvex )
					{
						for( size_t i=0; i<l_pCollisionData->ConvexHullPointData.size();++i)
						{
							l_pCollisionData->ConvexHullPointData[i] += l_vMovePos;
						}
					}
					else
					if( l_pCollisionData->pSphereData )
					{
						l_pCollisionData->pSphereData->vCenter += l_vMovePos;
					}
				}
			}
			else
			if( m_pCurrentImageCollisionData && m_pCurrentImageCollisionData->iCollisionDataIndex != -1 )
			{
				if( m_pCurrentImageCollisionData->pCurrentCollisionData->bConvex )
				{
					int	l_iSelectedIndex = m_pCurrentImageCollisionData->pCurrentCollisionData->iFocusPoint;
					if( e_iMode == 1 && m_pCurrentImageCollisionData->pCurrentCollisionData->iFocusPoint != -1 )
					{
						m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData[l_iSelectedIndex] = Vector3((float)e_Pos.x,(float)e_Pos.y,0.f);
					}
					else
					if( e_iMode == 2 ) 
					{
						Vector3	l_vMovePos((float)e_Pos.x,(float)e_Pos.y,0);
						l_vMovePos -= m_vMouseDownPos;
						for( size_t i=0; i<m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData.size();++i)
						{
							m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData[i] += l_vMovePos;
						}
						m_vMouseDownPos.x = (float)e_Pos.x;
						m_vMouseDownPos.y = (float)e_Pos.y;
						m_vMouseDownPos.z = 0;
					}
				}
				else
				if( m_pCurrentImageCollisionData->pCurrentCollisionData->pSphereData )
				{
					m_pCurrentImageCollisionData->pCurrentCollisionData->pSphereData->vCenter = Vector3((float)e_Pos.x,(float)e_Pos.y,0.f);
				}
			}
		}
	}

	void	c2DImageCollisionDataForEditor::ReOrderIndex(cPuzzleImage*e_pPuzzleImage)
	{
		//e_pPuzzleImage->Count()
	}

	void	c2DImageCollisionDataForEditor::Export(char*e_strFileName,cPuzzleImage*e_pPuzzleImage)
	{
#ifdef WIN32
		UINT l_iuiNumImageCollisionData = m_ImageCollisionDataVector.size();
		if( l_iuiNumImageCollisionData == 0 )
			return;
		ATG::XMLWriter	l_XMLWriter(e_strFileName);
		l_XMLWriter.StartElement("ImageCollisionData");
		l_XMLWriter.AddAttribute("Name",e_pPuzzleImage->GetName());
		l_XMLWriter.AddAttribute("Count",e_pPuzzleImage->Count());
		for( UINT i=0;i<l_iuiNumImageCollisionData;++i )
		{
			UINT l_uiNumCollisionData = m_ImageCollisionDataVector[i]->CollisionDataVector.size();
			l_XMLWriter.StartElement("CollisionData");
			l_XMLWriter.AddAttribute("Name",(*e_pPuzzleImage)[i]->GetName());
			l_XMLWriter.AddAttribute("Count",(int)l_uiNumCollisionData);
			for( UINT j=0;j<l_uiNumCollisionData;++j )
			{
				sImageCollisionData::sCollisionData*l_pCollisionData = m_ImageCollisionDataVector[i]->CollisionDataVector[j];
				l_XMLWriter.StartElement("Shape");
				//here should list all posible shape type
				if(l_pCollisionData->bConvex)
				{
					l_XMLWriter.AddAttribute("Type","Convex");
					l_XMLWriter.AddAttribute("DataCount",(int)l_pCollisionData->ConvexHullPointData.size());
					l_XMLWriter.AddAttribute("Data",Vector3VectorToString(&l_pCollisionData->ConvexHullPointData).c_str());
				}
				else
				{
					if( l_pCollisionData->pSphereData )
					{
						l_XMLWriter.AddAttribute("Type","Sphere");
						l_XMLWriter.AddAttribute("DataCount",1);
						l_XMLWriter.AddAttribute("Data",ValueToString(l_pCollisionData->pSphereData->vCenter));
						l_XMLWriter.AddAttribute("Radius",l_pCollisionData->pSphereData->fRadius);
					}
					else
					{
						l_XMLWriter.AddAttribute("Type","Empty");//data is empty.
					}
				}
				l_XMLWriter.EndElement();//end CollisionData

			}
			l_XMLWriter.EndElement();//end ImageCollisionData
		}

		l_XMLWriter.EndElement();//end root
		l_XMLWriter.Close();
#endif
	}

	void	c2DImageCollisionDataForEditor::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const wchar_t*l_strValue = this->m_pCurrentTiXmlElement->Value();
		COMPARE_VALUE("ImageCollisionData")
		{
			ProcessImageCollisionData();
		}
		else
		COMPARE_VALUE("CollisionData")
		{
			ProcessCollisionData();
		}
		else
		COMPARE_VALUE("Shape")
		{
			ProcessShapeData();
		}	
	}

	//<ImageCollisionData Name="08" Count="16">
	void	c2DImageCollisionDataForEditor::ProcessImageCollisionData()
	{
		if(this->m_pCurrentTiXmlElement->m_bDone)
		{//fuck I am lazy to debug fuck!
			this->m_pCurrentImageCollisionData = 0;
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Name")
				{
					WCHAR_TO_WCHAR(l_strValue,m_strForCollidedpiFile);
				}
				else
				COMPARE_NAME("Count")
				{
					int	l_iCount = _wtoi(l_strValue);
					assert(l_iCount == this->m_ImageCollisionDataVector.size());
				}
			PARSE_NAME_VALUE_END
		}
		else
		{
			//clear data before we start to do this shit.
			DELETE_VECTOR(m_ImageCollisionDataVector,sImageCollisionData*);		
		}
	}
    //<CollisionData Name="08fish-run0000" Count="0" />
	void	c2DImageCollisionDataForEditor::ProcessCollisionData()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			sImageCollisionData*l_pImageCollisionData = new sImageCollisionData();
			m_pCurrentImageCollisionData = l_pImageCollisionData;
			this->m_ImageCollisionDataVector.push_back(l_pImageCollisionData);
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Name")
				{
					m_pCurrentImageCollisionData->strName = l_strValue;
				}
				else
				COMPARE_NAME("Count")
				{
					int	l_iCount = _wtoi(l_strValue);
					m_pCurrentImageCollisionData->CollisionDataVector.reserve(l_iCount);
				}
			PARSE_NAME_VALUE_END
		}
	}
//        <Shape Type="Sphere" DataCount="1" Data="194.00,122.00,0.00," />
//        <Shape Type="Convex" DataCount="5" Data="238.00,476.00,0.00,84.00,289.00,0.00,203.00,20.00,0.00,308.00,307.00,0.00,239.00,465.00,0.00," />
//        <Shape Type="Empty" />
	void	c2DImageCollisionDataForEditor::ProcessShapeData()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			eCollisionType	l_eCollisionType = eCT_MAX;
			int	l_iCount = 0;
			const wchar_t*l_strData = 0;
			float	l_fRadius = 0.f;
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Type")
				{
					l_eCollisionType = StringToCollisionType(l_strValue);
				}
				else
				COMPARE_NAME("DataCount")
				{
					l_iCount = _wtoi(l_strValue);
				}
				else
				COMPARE_NAME("Data")
				{
					l_strData = l_strValue;
				}
				else
				COMPARE_NAME("Radius")
				{
					l_fRadius = (float)_wtof(l_strValue);
				}
			PARSE_NAME_VALUE_END	
			if( l_eCollisionType == eCT_MAX )
				return;
			sImageCollisionData::sCollisionData*l_pCollisionData = new sImageCollisionData::sCollisionData();
			this->m_pCurrentImageCollisionData->CollisionDataVector.push_back(l_pCollisionData);
			switch(l_eCollisionType)
			{
				case eCT_Sphere:
					l_pCollisionData->pSphereData = new sSphere();
					l_pCollisionData->pSphereData->vCenter = GetVector3(l_strData);
					l_pCollisionData->pSphereData->fRadius = l_fRadius;
					l_pCollisionData->bConvex = false;
					break;
				case eCT_CONVEX:
					l_pCollisionData->ConvexHullPointData = StringToVector3Vector((wchar_t*)l_strData,l_iCount);
					l_pCollisionData->bConvex = true;
					break;					
			}
		}
	}


	c2DImageCollisionData::c2DImageCollisionData()
	{
		m_pbtConcaveShapeList = 0;
		InitBT();
	}

	c2DImageCollisionData::~c2DImageCollisionData()
	{
		ExitBT();
	}

	void	c2DImageCollisionData::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const wchar_t*l_strValue = this->m_pCurrentTiXmlElement->Value();
		COMPARE_VALUE("ImageCollisionData")
		{
			ProcessImageCollisionData();
		}
		else
		COMPARE_VALUE("CollisionData")
		{
			ProcessCollisionData();
		}
		else
		COMPARE_VALUE("Shape")
		{
			ProcessShapeData();
		}	
	}

	//<ImageCollisionData Name="08" Count="16">
	void	c2DImageCollisionData::ProcessImageCollisionData()
	{
		//fuck I am lazy to debug fuck!
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("Name")
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Count")
			{
				int	l_iCount = _wtoi(l_strValue);
				this->m_ObjectList.reserve(l_iCount);
			}
		PARSE_NAME_VALUE_END
	}
    //<CollisionData Name="08fish-run0000" Count="0" />
	void	c2DImageCollisionData::ProcessCollisionData()
	{
		cbtConcaveShapeList*l_pbtConcaveShapeList = new cbtConcaveShapeList();
		m_pbtConcaveShapeList = l_pbtConcaveShapeList;
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("Name")
			{
				l_pbtConcaveShapeList->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Count")
			{
				//int	l_iCount = _wtoi(l_strValue);
			}
		PARSE_NAME_VALUE_END
		bool	l_b = this->AddObject(l_pbtConcaveShapeList);
		assert(l_b);
	}
//        <Shape Type="Sphere" DataCount="1" Data="194.00,122.00,0.00," />
//        <Shape Type="Convex" DataCount="5" Data="238.00,476.00,0.00,84.00,289.00,0.00,203.00,20.00,0.00,308.00,307.00,0.00,239.00,465.00,0.00," />
//        <Shape Type="Empty" />
	void	c2DImageCollisionData::ProcessShapeData()
	{
		eCollisionType	l_eCollisionType = eCT_MAX;
		int	l_iCount = 0;
		const wchar_t*l_strData = 0;
		float l_fRadius = 0.f;
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("Type")
			{
				l_eCollisionType = StringToCollisionType(l_strValue);
			}
			else
			COMPARE_NAME("DataCount")
			{
				l_iCount = _wtoi(l_strValue);
			}
			else
			COMPARE_NAME("Data")
			{
				l_strData = l_strValue;
			}
			else
			COMPARE_NAME("Radius")
			{
				l_fRadius = (float)_wtof(l_strValue);
			}
		PARSE_NAME_VALUE_END
		if( l_eCollisionType == eCT_MAX )
			return;
		cbtShapeCollision*l_pbtShapeCollision = 0;
		switch(l_eCollisionType)
		{
			case eCT_Sphere:
				l_pbtShapeCollision = new cbtSphereShape(l_fRadius,GetVector3(l_strData));
				break;
			case eCT_CONVEX:
				{
					std::vector<Vector3>l_vPos = StringToVector3Vector((wchar_t*)l_strData,l_iCount);
					l_pbtShapeCollision = new cbtConvexHullShape((float*)&l_vPos[0],l_iCount,sizeof(float)*3);
				}
				break;					
		}
		m_pbtConcaveShapeList->AddObjectNeglectExist(l_pbtShapeCollision);
	}

//end namespace FATMING_CORE
}
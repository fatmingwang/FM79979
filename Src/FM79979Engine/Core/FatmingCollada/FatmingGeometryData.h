#ifndef FATMING_GEOMETRY_DATA_H
#define FATMING_GEOMETRY_DATA_H
#include "Mesh.h"
	struct sController;
	struct	sDataAndDataType
	{
		eDataType	eType;
		char*		pData;
		UINT		uiCount;
		inline	sDataAndDataType(eDataType e_eDataType,UINT e_uiCount)
		{
			eType = e_eDataType;
			uiCount = e_uiCount;
			int	l_iSize = 0;
			switch(eType)
			{
				case eDT_BYTE:
					l_iSize = sizeof(char);
					break;
				case eDT_FLOAT:
					l_iSize = sizeof(float);
					break;
				case eDT_DOUBLE:
					l_iSize = sizeof(double);
					break;
				case eDT_INT:
					l_iSize = sizeof(int);
					break;
				default:
					break;
			}
			pData = new char[e_uiCount*l_iSize];
		}
		~sDataAndDataType()
		{
			SAFE_DELETE(pData);
		}
	};

	struct	sGeometryMesh
	{
		enum	eVerticesType
		{
			eVT_NONE = 0,
			eVT_POS,
			eVT_NORMAL,
			eVT_TEX_COORDINATE,//  texture (s t p collada, opengl s t) collada max exports 3 componets, but only 2 are used
			//eVT_TEX_COORDINATE1,//  texture (s t p collada, opengl s t) collada max exports 3 componets, but only 2 are used
			eVT_TANGENT,
			eVT_BITANGENT,
			eVT_VERTICEX_COLOR,
			eVT_VERTICEX_MAX,
		};
		struct sSource
		{
			const WCHAR*strSource_technique_common__accessor_source;
			const WCHAR*strID;
			const WCHAR*strName;//FCollada
			const WCHAR*strFloat_array_ID;
			const WCHAR*pNameArray;
			int		iCount;
			int		iTechnique_common_Accesor_Count;
			float*	pBufferArray;
			int		iStride;
			int		iIndexOffset;//for non unified indices
			eVerticesType	eVT;
			sSource()
			{
				iTechnique_common_Accesor_Count = 0;
				pNameArray = 0;
				strSource_technique_common__accessor_source = 0;
				strID = 0;
				strName = 0;
				iCount = 0;
				pBufferArray = 0;
				iStride = 0;
				eVT = eVT_NONE;
				iIndexOffset = 0;
			}
			~sSource()
			{
				delete pBufferArray;
			}
		};
		struct sVertices
		{
			eVerticesType	eVT;
			const WCHAR* strID;
			struct	sInput
			{
				const WCHAR* strInputSemantic;
				const WCHAR* strInputSource;
				sInput()
				{
					strInputSemantic = 0;
					strInputSource = 0;
				}
			};
			std::vector<sInput*>	AllInput;
			const WCHAR*GetSourceNameByEsmantic(const WCHAR*e_strSemantic)
			{
				for( UINT i=0;i<AllInput.size();++i )
				{
					if( !wcscmp(AllInput[i]->strInputSemantic,e_strSemantic) )
						return AllInput[i]->strInputSource;
				}
				return 0;
			}
			sVertices()
			{
				strID = 0;
			}
			~sVertices()
			{
				DELETE_VECTOR(AllInput)
			}
		};
		struct	sTriangles
		{
			void	ProcessIndices(WCHAR* e_strIndices,UINT e_uiSize);
			const	WCHAR*strMaterial;
			int		iCount;//indices count
			int		iMaxIndex;//the max value for index,so we have to create as much vertices as this value
			int*	piIndexBuffer;//for <p> it's non unified index
			bool	bHasTexture;
			bool	bHasNormal;
			bool	bHasTangent;
			bool	bHasBitangent;
			bool	bHasDiffuse;
			//for optmise vertex buffer by draw indices
			//int	*	pReuseVertexPositions;
			//std::vector<std::vector<int>>	ReverseReuseVertexPositions;
			struct sInputData
			{
				const WCHAR*strSemantic;
				const WCHAR*strSource;
				int		iOffset;
				int		iSet;//to determi which texture coordinate we are going to use,if the object using above 1 texture.
			};
			std::vector<sInputData*>	AllInputDataList;
			sTriangles()
			{
				bHasTangent = false;
				bHasBitangent = false;
				bHasDiffuse = false;
				bHasTexture = false;
				bHasNormal = false;
				strMaterial = 0;
				piIndexBuffer = 0;
				//pReuseVertexPositions = 0;
			}
			~sTriangles()
			{
				//SAFE_DELETE(pReuseVertexPositions);
				DELETE_VECTOR(AllInputDataList);
				SAFE_DELETE(piIndexBuffer);
			}
		};
		//AllGeometryMeshSourceList and indices buffer relationship,it's possible two resource using same index,although non-unified index buffer
		int		m_iAllSourceUsingIndicesBuffer;
		int		m_iTotalBufferStride;			//all buffer element stride
		//bool	m_bHasTexture;
		//internal use,to keep current data as need on the top
		sSource*pCurrentGeometryMeshSource;
		sTriangles*pCurrentGeometryMeshTriangles;
		//
		sVertices	m_GeometryMeshVertices;
		std::vector<sTriangles*>	AllGeometryMeshTrianglesList;//all bone
		std::vector<sSource*>		AllGeometryMeshSourceList;
		std::vector<cMesh*>			AllMeshIndexList;
		cMesh*						GetMeshByName(const WCHAR*e_strName);
		//while parse data,we have to compare the name,if name is correspnd then assign it.
		void	AssignVertexType();
		cMesh	*GenerateMeshByTriangleData(sTriangles*e_pTriangles);
		bool	NearlyEquals(float* e_pfPositionVector1,float* e_pUV1,float* e_pfNormalVector1,float*e_pfTangentVector1,
			float* e_pfPositionVector2,float* e_pUV2,float* e_pfNormalVector2,float*e_pfTangentVector2);
		//std::vector<eVerticesType>			AllVerticesTypeList;
		sSource*	GetVerticesByType( eVerticesType e_eVerticesType );
		eVerticesType	GetVerticesType(const WCHAR* e_str);
		//position and texture coordinate and normal data is jeep in them,currently them are assigned in 
		//GenerateCallListByTriangle function but it should be put in to mesh data.
		//or to reuse to assign new unfied index data to generate triangle strip data.
		struct	sAllVertexData
		{
			//position's count as same as skinning's weight count.
			UINT	uiVertexCount;
			//it should be name as iIndicesCount that respond to piIndices.
			UINT	uiNumIndices;
			//the data for bone id usually is 4 unsigned byte to become 1 float.if the bone ID is bigger than 512,u have to ensure data swap problem
			//the data might be set nullptr at Mesh.cpp:where mesh's contruction
			//so memory malloc could be reduce,but might not useful for furture using.
			float**	ppVerticesData;
			UINT*	puiIndices;
			//below for weights and blending bone indices,store the indice from original position data,so we could generate the skinnging data.
			std::vector<int>	AllIndicesNeededInOriginalPosVertex;//using in GenerateUniqueIndicesAndVertexData_OPENGL
			sAllVertexData()
			{
				uiNumIndices = 0;
				puiIndices = 0;
				ppVerticesData = new float*[TOTAL_FVF];
				for( int i=0;i<TOTAL_FVF;++i )
					ppVerticesData[i] = 0;
				pReuseVertexPositions = 0;
				uiVertexCount = 0;
			}
			~sAllVertexData()
			{
				SAFE_DELETE(puiIndices);
				SAFE_DELETE(pReuseVertexPositions);
				if( ppVerticesData )
				{
					for( int i=0;i<TOTAL_FVF;++i )
						SAFE_DELETE(ppVerticesData[i]);
					SAFE_DELETE(ppVerticesData);
				}
			}
			// below for vertex optmize using,we will remove it if we do not need it anymore
			// index:0,2,5,1,4,3
			// data:0,1,2,3,4,5,6,7,8,9
			// so we store index(1) in pReuseVertexPositions and make ReverseReuseVertexPositionsVector[index(1)] add data 012
			// then we could search all data in pReuseVertexPositions then fetch index(0) so we could get related data (0,1,2)
			// finally we could check UV and normal and position,if one of them is same,then we get a new index into our piIndices buffer.
			// we also could use this to optmize skinning weight and bone index
			// Helpers to remember how we can reuse vertices for OptimizeVertexBuffer.
			// See below for more details.
			int*	pReuseVertexPositions;
			// Reverse reuse vertex positions, this one is even more important because
			// this way we can get a list of used vertices for a shared vertex pos.
			std::vector< std::vector<int> >	ReverseReuseVertexPositionsVector;
			//below is the vertex we need,it could be add more,like tangent bitangent skinning weight...else
			//the member in it,just like FVF see the top define
		};
		//because fucking Collada exporter using non-unified indices,
		//each vertex format is allow to have different vertex count,
		//but opengl and DirectX need to have same count for eaxh vertex format(pos,tex,normal....else)
		//GenerateSkinningTriangleBlendicesWeights will map indices for skinning weights and influnce bones again.
		void	GenerateUniqueIndicesAndVertexData_OPENGL(sAllVertexData*e_pAllVertexData);
		void	GenerateUniqueIndicesAndVertexData_DirectX(sAllVertexData*e_pAllVertexData);
		//fuck here is so wrong!!!
		std::map<sTriangles*,sAllVertexData*>	AllTriangleVertexData;
		//include pos,texture coordinate,normal,return value:how many vertex is created
		int	GenerateTriangleVertices(sTriangles*e_pTriangles);
		sGeometryMesh()
		{
			m_iAllSourceUsingIndicesBuffer = -1;
			m_iTotalBufferStride = 0;
			pCurrentGeometryMeshTriangles = 0;
		}
		~sGeometryMesh()
		{
			DEL_MAP(sTriangles*,sAllVertexData*,AllTriangleVertexData);
			DELETE_VECTOR(AllGeometryMeshSourceList);
			DELETE_VECTOR(AllGeometryMeshTrianglesList);
			DELETE_VECTOR(AllMeshIndexList);
		}
	};
	struct	sSpline
	{
		bool	bClosed;
		sGeometryMesh::sSource*					pCurrentSource;
		sGeometryMesh::sVertices				Splinecontrol_vertices;
		std::vector<sGeometryMesh::sSource*>	AllSplineSource;
		sSpline()
		{
			bClosed = false;
			pCurrentSource = 0;
		}
		~sSpline()
		{
			DELETE_VECTOR(AllSplineSource);
		}
		cCurve	ContertToCurve()
		{
			cCurve	l_Curve;
			int	l_iSize = (int)Splinecontrol_vertices.AllInput.size();
			for( int i=0;i<l_iSize;++i )
			{
				if(!wcscmp(Splinecontrol_vertices.AllInput[i]->strInputSemantic,L"POSITION"))
				{
					int	l_iNumSource = (int)AllSplineSource.size();
					for(int j=0;j<l_iNumSource;++j)
					{
						const WCHAR*l_str1 = AllSplineSource[j]->strID;
						const WCHAR*l_str2 = &Splinecontrol_vertices.AllInput[i]->strInputSource[1];
						if(!wcscmp(l_str1,l_str2))
						{
							sGeometryMesh::sSource*l_pSource = AllSplineSource[j];
							for(int k=0;k<l_pSource->iTechnique_common_Accesor_Count;++k)
							{
								Vector3	l_vPos = *(Vector3*)&l_pSource->pBufferArray[k*3];
								l_Curve.AddPoint(l_vPos);
							}
						}
					}
				}
			}
			return l_Curve;
		}
	};
	struct sColladaGeomatries
	{
		const WCHAR*	strMaterialName;
		const WCHAR*	strName;
		const WCHAR*	strID;
		sGeometryMesh*	pGeometryMesh;
		sSpline*		pSpline;
		sColladaGeomatries()
		{
			strMaterialName = 0;
			strName = 0;
			strID = 0;
			pGeometryMesh = 0;
			pSpline = 0;
		}
		~sColladaGeomatries()
		{
			SAFE_DELETE(pGeometryMesh);
			SAFE_DELETE(pSpline);
		}
		//while the mesh contian the controller we have to set data as new indices(sAllVertexData)
		bool	GenerateSkinningTriangleBlendicesWeights(sController*e_pController);
	};

#endif
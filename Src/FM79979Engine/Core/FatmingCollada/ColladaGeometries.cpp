#include "StdAfx.h"
#include "FatmingGeometryData.h"
#include "ColladaGeometries.h"
#include "ColladaControllers.h"
const wchar_t*           cLibraryGeometries::TypeID( L"cLibraryGeometries" );

bool	g_bInvertTextureAdress_V = false;//bmp texture adress V is invert

bool	sGeometryMesh::NearlyEquals(float* e_pfPositionVector1,float* e_pUV1,float* e_pfNormalVector1,float*e_pfTangentVector1,
				float* e_pfPositionVector2,float* e_pUV2,float* e_pfNormalVector2,float*e_pfTangentVector2)
{
	bool	l_bSame = false;
	// Ignore blend indices and blend weights, they are the same
	// anyway, because they are calculated from the bone distances.
	if( e_pfPositionVector1[0] != e_pfPositionVector2[0]||
		e_pfPositionVector1[1] != e_pfPositionVector2[1]||
		e_pfPositionVector1[2] != e_pfPositionVector2[2])
		return false;
	if( !e_pUV1||!e_pUV2 )
		return true;
	//if( e_pUV1[0]!=e_pUV2[0]||
	//	e_pUV1[1]!=e_pUV2[1])
	if( fabs(e_pUV1[0]-e_pUV2[0])>0.001f||
		fabs(e_pUV1[1]-e_pUV2[1])>0.001f)
		return false;
	if( !e_pfNormalVector1|| !e_pfNormalVector2 )
		return true;
	// Normals and tangents do not have to be very close, we can't see
	// any difference between small variations here, but by optimizing
	// similar vertices we can improve the overall rendering performance.
	Vector3	l_vN1 = Vector3(e_pfNormalVector1);
	Vector3	l_vN2 = Vector3(e_pfNormalVector2);
	if( (l_vN1-l_vN2).Length()>0.05f )
		return false;
	if( !e_pfTangentVector1||!e_pfTangentVector2 )
		return true;
	Vector3	l_vT1 = Vector3(e_pfTangentVector1);
	Vector3	l_vT2 = Vector3(e_pfTangentVector2);
	if( (l_vT1-l_vT2).Length()<0.05f )
	{
		return true;
	}
	return false;
}

void	sGeometryMesh::sTriangles::ProcessIndices(WCHAR* e_strIndices,UINT e_uiSize)
{
	assert( piIndexBuffer == 0 );
	piIndexBuffer = new int[e_uiSize];
	WCHAR*	l_str = wcstok(e_strIndices,L", ");
	UINT	l_uiMaxValue = 0;
	UINT	l_uiIndex = 0;
	while(l_str)
	{
		int	l_i = _wtoi(l_str);
		if( l_i>(int)l_uiMaxValue )
			l_uiMaxValue = l_i;
		piIndexBuffer[l_uiIndex] = l_i;
		l_str = wcstok(0,L", ");
		++l_uiIndex;
	}
	iMaxIndex = l_uiMaxValue;
#ifdef DEBUG
	UINT	l_iSize = l_uiIndex;
	assert(e_uiSize == l_iSize&&"we should to know all input offset,because it's possible using same indx by two source,like textangent and texbinormal,so we have to fix this." );
#endif
}

sGeometryMesh::eVerticesType	sGeometryMesh::GetVerticesType(const WCHAR* e_str)
{
	if(!wcscmp(e_str,L"POSITION")||!wcscmp(e_str,L"VERTEX"))
	{
		return eVT_POS;
	}
	else
	if(!wcscmp(e_str,L"NORMAL"))
	{
		return eVT_NORMAL;
	}
	else
	if(!wcscmp(e_str,L"TEXCOORD"))//  texture (s t p collada, opengl s t) collada max exports 3 componets, but only 2 are used
	{
		return eVT_TEX_COORDINATE;
	}
	else
	if(!wcscmp(e_str,L"TEXTANGENT"))
	{
		return eVT_TANGENT;
	}
	else
	if(!wcscmp(e_str,L"TEXBINORMAL"))
	{
		return eVT_BITANGENT;
	}
	else
	if(!wcscmp(e_str,L"COLOR"))
	{
		//void	cVBOBuffer::RenderByShader()
		//assert(0&&"ensure the color data type is byte not float,or fix this problem,because the shader is using float not byte????");
		return eVT_VERTICEX_COLOR;
	}
	assert(0&&"vertex format error");
	return eVT_NONE;
}

cMesh*	sGeometryMesh::GetMeshByName(const WCHAR*e_strName)
{
	for(UINT i=0;i<this->AllMeshIndexList.size();++i)
	{
		if( !wcscmp(e_strName,AllMeshIndexList[i]->GetName()) )
			return AllMeshIndexList[i];
	}
	return 0;
}

void	sGeometryMesh::AssignVertexType()
{
	//below is wrong
	//first we have assign the ID:vertex to ID:position
	UINT l_NumTriangle = this->AllGeometryMeshTrianglesList.size();
	for( UINT l_iIndex = 0;l_iIndex<l_NumTriangle;++l_iIndex )
	{
		sTriangles*l_pTriangles = AllGeometryMeshTrianglesList[l_iIndex];
		UINT	l_uiSize = l_pTriangles->AllInputDataList.size();
		for(UINT i=0;i<l_uiSize;++i)
		{
			sGeometryMesh::sTriangles::sInputData*l_pInputData = l_pTriangles->AllInputDataList[i];
			const WCHAR*l_strSourceName = 0;
			if(!wcscmp(l_pInputData->strSemantic,L"VERTEX"))
				l_strSourceName = m_GeometryMeshVertices.GetSourceNameByEsmantic( L"POSITION" );
			else
				l_strSourceName = m_GeometryMeshVertices.GetSourceNameByEsmantic( l_pInputData->strSemantic );
			if( l_strSourceName )
				l_pInputData->strSource = l_strSourceName;
		}
	}
	//texture coordinate source
	sSource*l_pTextureSource = 0;
	//second assign all source to correct format
	for( UINT l_iIndex = 0;l_iIndex<l_NumTriangle;++l_iIndex )
	{
		sTriangles*l_pTriangles = AllGeometryMeshTrianglesList[l_iIndex];
		UINT	l_uiSize = l_pTriangles->AllInputDataList.size();
		for(UINT i=0;i<l_uiSize;++i)
		{
			sGeometryMesh::sTriangles::sInputData*l_pInputData = l_pTriangles->AllInputDataList[i];
			assert(l_pInputData->strSource);
			const WCHAR*l_strSource = &l_pInputData->strSource[1];//we do not need #
			for(UINT j=0;j<AllGeometryMeshSourceList.size();++j)
			{
				sSource*l_pSource = AllGeometryMeshSourceList[j];
				if( l_pSource->eVT == eVT_NONE )
				{
					if(!wcscmp(l_pSource->strID,l_strSource))
					{
						l_pSource->eVT = GetVerticesType(l_pInputData->strSemantic);
						l_pSource->iIndexOffset = l_pInputData->iOffset;
						if(l_pSource->eVT == eVT_TEX_COORDINATE)
						{
							l_pTextureSource = l_pSource;
							l_pTriangles->bHasTexture = true;
						}
						else
						if(l_pSource->eVT == eVT_NORMAL)
						{
							l_pTriangles->bHasNormal = true;
						}
						else
						if(l_pSource->eVT == eVT_TANGENT)
						{
							l_pTriangles->bHasTangent = true;
						}
						else
						if(l_pSource->eVT == eVT_BITANGENT)
						{
							l_pTriangles->bHasBitangent = true;
						}
						else
						if(l_pSource->eVT == eVT_VERTICEX_COLOR)
						{
							l_pTriangles->bHasDiffuse = true;
						}
					}
				}
			}
		}
	}
}

cMesh*	sGeometryMesh::GenerateMeshByTriangleData(sTriangles*e_pTriangles)
{
	//GenerateTriangleVertices(e_pTriangles);
	//GLuint	l_ListOpenGL = glGenLists(1); 
	//glNewList(l_ListOpenGL,GL_COMPILE);
	//sAllVertexData*l_pAllVertexData = AllTriangleVertexData[e_pTriangles];
	//glVertexPointer(3, GL_FLOAT, 0, l_pAllVertexData->ppVerticesData[FVF_POS]);
	//glTexCoordPointer(2, GL_FLOAT, 0, l_pAllVertexData->ppVerticesData[FVF_TEX0]);
	////MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0,l_iNumVertices);
	//MY_GLDRAW_ELEMENTS(GL_TRIANGLES,l_pAllVertexData->iNumIndices, GL_UNSIGNED_INT,l_pAllVertexData->piIndices );
	//glEndList();
	//return l_ListOpenGL;
	GenerateTriangleVertices(e_pTriangles);
	sAllVertexData*l_pAllVertexData = AllTriangleVertexData[e_pTriangles];
	cMesh*l_pMesh = new cMesh(l_pAllVertexData->ppVerticesData,l_pAllVertexData->uiVertexCount,l_pAllVertexData->puiIndices,l_pAllVertexData->uiNumIndices);
	return l_pMesh ;
}

sGeometryMesh::sSource*	sGeometryMesh::GetVerticesByType( eVerticesType e_eVerticesType )
{
	for( UINT i=0;i<AllGeometryMeshSourceList.size();++i)
	{
		eVerticesType	l_eVerticesType = AllGeometryMeshSourceList[i]->eVT;
		if( e_eVerticesType == l_eVerticesType )
		{
			return AllGeometryMeshSourceList[i];
		}
	}
	return 0;
}
//https://collada.org/public_forum/viewtopic.php?f=12&t=1037&view=previous
//Look at the "offset" attribute of each <input> in <triangles>.
//The "vertices" (which just contain position info in this example) are offset 0 and the normals are offset 1.
//This means that the first value in the <p> is a position,
//the second value is a normal, and that makes one complete "index".
//The third value is a position again, and the fourth value is a normal.
//The fifth value is another position, and the sixth value is a normal,
//and that completes the first triangle. Repeat for the other 11 triangles. 
//The key thing that may be confusing is that Collada allows different indices per stream (I call them "non-unified indices").
//That is, a single logical "index" can refer to (e.g.) the 10th position and the 3rd normal.
//Graphics APIs like OpenGL and Direct3D only support unified indices,
//where each index is a single value specifying where to look in both the position and normal streams.
//Non-unified indices are more flexible and allow a more compact data representation. For example,
//using non-unified indices, a cube can be described with 8 positions and 6 normals,
//but with unified indices you need 24 positions and 24 normals. //
//Dealing with non-unified indices is one of the trickier aspects of writing a Collada importer,
//since most Collada exporters write data using non-unified indices, and most graphics APIs don't support them.
//You need to transform the data to unified index form. This code is tricky to write,
//but fortunately it's already been done for you.
//The confusingly-named "deindexer" conditioner in the Collada Refinery converts a Collada document to unified index form.
int	sGeometryMesh::GenerateTriangleVertices(sTriangles*e_pTriangles)
{
	sAllVertexData*l_pAllVertexData = new sAllVertexData;
	AllTriangleVertexData.insert(std::make_pair(e_pTriangles,l_pAllVertexData));
	//although here is quite stupid,but it's clear enough let anyone couyld understand what os this going to do!!
	sSource*l_pPosSource = GetVerticesByType(eVT_POS);
	sSource*l_pTexSource = GetVerticesByType(eVT_TEX_COORDINATE);
	sSource*l_pNormalSource = GetVerticesByType(eVT_NORMAL);
	sSource*l_pTangentSource = GetVerticesByType(eVT_TANGENT);
	sSource*l_pBitangentSource = GetVerticesByType(eVT_BITANGENT);
	sSource*l_pDiffuseSource = GetVerticesByType(eVT_VERTICEX_COLOR);
	int	l_iPosBufferOffset = l_pPosSource->iIndexOffset;
	int	l_iTexBufferOffset = -1;
	int	l_iNormalBufferOffset = -1;
	int	l_iTangentBufferOffset = -1;
	int	l_iBiTangentBufferOffset = -1;
	int	l_iDiffuseBufferOffset = -1;
	float*l_pPosBuffer = l_pPosSource->pBufferArray;
	float*l_pTexBuffer = 0;
	float*l_pNormalBuffer = 0;
	float*l_pTangentBuffer = 0;
	float*l_pBitangentBuffer = 0;
	float*l_pDiffuseBuffer = 0;
	if(e_pTriangles->bHasTexture)
	{
		l_iTexBufferOffset = l_pTexSource->iIndexOffset;
		l_pTexBuffer = l_pTexSource->pBufferArray;
	}
	if(e_pTriangles->bHasNormal)
	{
		l_iNormalBufferOffset = l_pNormalSource->iIndexOffset;
		l_pNormalBuffer = l_pNormalSource->pBufferArray;
	}
	if(e_pTriangles->bHasTangent)
	{
		l_iTangentBufferOffset = l_pTangentSource->iIndexOffset;
		l_pTangentBuffer = l_pTangentSource->pBufferArray;
	}
	if(e_pTriangles->bHasBitangent)
	{
		l_iBiTangentBufferOffset = l_pBitangentSource->iIndexOffset;
		l_pBitangentBuffer = l_pBitangentSource->pBufferArray;
	}
	if(e_pTriangles->bHasDiffuse)
	{
		assert(l_pDiffuseSource->iStride == 3&&"vertex color not rgb???");
		//assert(0&&"I am lazy to do this right now!!because vertex color 2 4 ubyte but I use wrong data.");
		l_iDiffuseBufferOffset = l_pDiffuseSource->iIndexOffset;
		l_pDiffuseBuffer = l_pDiffuseSource->pBufferArray;
	}

	//assume we have 3 source(pos tex nor),p1,n1,t1,p2,n2,t2,p3,n3,t3
	//if we wanna to fetch the next vertex data we could do this
	//first get offset index,ex position its offset index is 0,
	//if we wanna to fetch next position index should like this
	//pIndexBuffer[currentindex+PosOffsetIndex+Numource*i]
	int	l_iNumSource = this->m_iAllSourceUsingIndicesBuffer;
	sTriangles*l_pTriangles = e_pTriangles;
	l_pAllVertexData->pReuseVertexPositions = new int[l_pTriangles->iCount*3];
	l_pAllVertexData->ReverseReuseVertexPositionsVector.resize(l_pPosSource->iCount/3);
	l_pAllVertexData->ppVerticesData[FVF_POS] = new float[l_pTriangles->iCount*3*g_iFVF_DataStride[FVF_POS]];
	int	l_iOneTriangle3Vertex = 3;
	if( e_pTriangles->bHasTexture )
		l_pAllVertexData->ppVerticesData[FVF_TEX0]  = new float[l_pTriangles->iCount*l_iOneTriangle3Vertex*g_iFVF_DataStride[FVF_TEX0]];//stride is 2
	if( e_pTriangles->bHasNormal )
		l_pAllVertexData->ppVerticesData[FVF_NORMAL]  = new float[l_pTriangles->iCount*l_iOneTriangle3Vertex*g_iFVF_DataStride[FVF_NORMAL]];//stride is 3
	if(e_pTriangles->bHasTangent)
	{
		l_pAllVertexData->ppVerticesData[FVF_TAGENT]  = new float[l_pTriangles->iCount*l_iOneTriangle3Vertex*g_iFVF_DataStride[FVF_TAGENT]];//stride is 3
	}
	if(e_pTriangles->bHasBitangent)
	{
		l_pAllVertexData->ppVerticesData[FVF_BITAGENT]  = new float[l_pTriangles->iCount*l_iOneTriangle3Vertex*g_iFVF_DataStride[FVF_BITAGENT]];//stride is 3
	}
	if(e_pTriangles->bHasDiffuse)
	{//
		int	l_iTotalColorLength = l_pTriangles->iCount*l_iOneTriangle3Vertex*g_iFVF_DataStride[FVF_DIFFUSE];
		l_pAllVertexData->ppVerticesData[FVF_DIFFUSE]  = new float[l_iTotalColorLength];//stride is 4
		//set all color as 1.
		memset(l_pAllVertexData->ppVerticesData[FVF_DIFFUSE],1,sizeof(float)*l_iTotalColorLength);
	}
	int	l_iCurrentVertexCount = 0;
	int	l_iNormalDataLength = 0;
	if( l_pNormalSource )
		l_iNormalDataLength = sizeof(float)*l_pNormalSource->iStride;
	int	l_iTagentLength = 0;
	if( l_pTangentSource )
		l_iTagentLength = sizeof(float)*l_pTangentSource->iStride;
	int	l_iBitagentLength = 0;
	if( l_pBitangentSource )
		l_iBitagentLength = sizeof(float)*l_pBitangentSource->iStride;
	int	l_iDiffuseLength = 0;
	if( l_pDiffuseSource )
		l_iDiffuseLength = sizeof(float)*l_pDiffuseSource->iStride;
	int	l_iPosLength = sizeof(float)*l_pPosSource->iStride;
	for( int i=0;i<l_pTriangles->iCount;++i )
	{
		int*l_pIndices  = &l_pTriangles->piIndexBuffer[i*l_iNumSource*3];//each time just 3 vertex
		for(int j=0;j<3;++j)
		{
			int	l_iCurrentPosVertexIndex = i*3*3+j*3;//3 for xyz
			int	l_iCurrentTexVertexIndex = i*3*2+j*2;//2 for UV
			if(e_pTriangles->bHasTexture)
			{
				int	l_iTexIndex1 = l_pIndices[l_iTexBufferOffset+j*l_iNumSource];
				float*l_pTex = &l_pTexBuffer[l_iTexIndex1*l_pTexSource->iStride];
				//glTexCoord2f(l_pTex[0],1-l_pTex[1]);
				//the texture we need usually just 2 not s element.
				l_pAllVertexData->ppVerticesData[FVF_TEX0][l_iCurrentTexVertexIndex] = l_pTex[0];
				if( g_bInvertTextureAdress_V )
					l_pAllVertexData->ppVerticesData[FVF_TEX0][l_iCurrentTexVertexIndex+1] = 1-l_pTex[1];//if the texture is bmp the texture V is invert
				else
					l_pAllVertexData->ppVerticesData[FVF_TEX0][l_iCurrentTexVertexIndex+1] = l_pTex[1];//if the texture is bmp the texture V is invert
			}
			if( e_pTriangles->bHasNormal )
			{
				int	l_iNormalIndxe = l_pIndices[l_iNormalBufferOffset+j*l_iNumSource];
				//glNormal3fv(&l_pNormalBuffer[l_iNormalIndxe*l_pNormalSource->iStride]);
				memcpy(&l_pAllVertexData->ppVerticesData[FVF_NORMAL][l_iCurrentPosVertexIndex],&l_pNormalBuffer[l_iNormalIndxe*l_pNormalSource->iStride],l_iNormalDataLength);
			}
			if(e_pTriangles->bHasTangent)
			{
				int	l_iTangentIndex = l_pIndices[l_iTangentBufferOffset+j*l_iNumSource];
				memcpy(&l_pAllVertexData->ppVerticesData[FVF_TAGENT][l_iCurrentPosVertexIndex],&l_pTangentBuffer[l_iTangentIndex*l_pTangentSource->iStride],l_iTagentLength);
			}
			if(e_pTriangles->bHasBitangent)
			{
				int	l_iBitangentIndex = l_pIndices[l_iBiTangentBufferOffset+j*l_iNumSource];
				memcpy(&l_pAllVertexData->ppVerticesData[FVF_BITAGENT][l_iCurrentPosVertexIndex],&l_pBitangentBuffer[l_iBitangentIndex*l_pBitangentSource->iStride],l_iBitagentLength);
			}
			if(e_pTriangles->bHasDiffuse)
			{
				int	l_iCurrentColorVertexIndex = i*3*4+j*4;//4 for rgba
				int	l_iDiffuseIndxe = l_pIndices[l_iDiffuseBufferOffset+j*l_iNumSource];
				//if( l_iDiffuseIndxe>=l_pTriangles->iCount*l_iOneTriangle3Vertex*g_iFVF_DataStride[FVF_DIFFUSE]
				//||l_iCurrentColorVertexIndex>=l_pTriangles->iCount*l_iOneTriangle3Vertex*g_iFVF_DataStride[FVF_DIFFUSE])
				//{
				//	int a=0;
				//}
				//rgba for 4 element
				//l_pDiffuseSource->iStride should not be multiply because rgba equal 1 flaot.
				//because Collada is RGB not RGBA so filled it to 4 element
				//assert(0&&"fuck here ocur crash!");
				memcpy(&l_pAllVertexData->ppVerticesData[FVF_DIFFUSE][l_iCurrentColorVertexIndex],&l_pDiffuseBuffer[l_iDiffuseIndxe*l_pDiffuseSource->iStride],l_iDiffuseLength);
			}
			int	l_iPosIndex = l_pIndices[l_iPosBufferOffset+j*l_iNumSource];
			assert(l_pPosSource->iCount>=l_iPosIndex*l_pPosSource->iStride&&"impossible!!the vertex count is smaller than index value!?");
			memcpy(&l_pAllVertexData->ppVerticesData[FVF_POS][l_iCurrentPosVertexIndex],&l_pPosBuffer[l_iPosIndex*l_pPosSource->iStride],l_iPosLength);
			//here to store the index to optmize the vertex data
			l_pAllVertexData->pReuseVertexPositions[l_pAllVertexData->uiNumIndices] = l_iPosIndex/3;
			l_pAllVertexData->ReverseReuseVertexPositionsVector[l_iPosIndex/3].push_back(l_pAllVertexData->uiNumIndices);
			++l_pAllVertexData->uiNumIndices;
		}
	}
	GenerateUniqueIndicesAndVertexData_OPENGL(l_pAllVertexData);
	return l_pAllVertexData->uiVertexCount;//final vertex count
}

void	sGeometryMesh::GenerateUniqueIndicesAndVertexData_DirectX(sAllVertexData*e_pAllVertexData)
{
	//u haveto flip triangle render order!!
	//clock wise to anti colock wise
}

void	sGeometryMesh::GenerateUniqueIndicesAndVertexData_OPENGL(sAllVertexData*e_pAllVertexData)
{
	int	l_iCurrentVertexCount = 0;
	std::vector<int>	l_NewIndices;
	l_NewIndices.reserve(e_pAllVertexData->uiNumIndices);
	assert(e_pAllVertexData->AllIndicesNeededInOriginalPosVertex.size() == 0);
	e_pAllVertexData->AllIndicesNeededInOriginalPosVertex.reserve(e_pAllVertexData->uiNumIndices);
	//create a temp vertex data,store new data into it,and swap it at last.
	float	**l_ppVerticesData = new float*[TOTAL_FVF];
	for( int i=0;i<TOTAL_FVF;++i )
	{
		if(e_pAllVertexData->ppVerticesData[i])
			l_ppVerticesData[i] = new float[e_pAllVertexData->uiNumIndices*g_iFVF_DataStride[i]];//the texture fvf should only be 2,but whatever...
		else
			l_ppVerticesData[i] = 0;
	}
	// Go over all vertices (indices are currently 1:1 with the vertices)
	//here just to test 2 triangle is close or not so does't need to fetch othere vertices data
	for (UINT j = 0; j < e_pAllVertexData->uiNumIndices; ++j)
	{
		float*l_pPosData1 = &e_pAllVertexData->ppVerticesData[FVF_POS][j*3];
		float*l_pTexData1 = e_pAllVertexData->ppVerticesData[FVF_TEX0];
		float*l_pNorData1 = e_pAllVertexData->ppVerticesData[FVF_NORMAL];
		float*l_pTangentData1 = e_pAllVertexData->ppVerticesData[FVF_TAGENT];
		if( l_pTexData1 )
			l_pTexData1 = &l_pTexData1[j*2];//stride2
		if( l_pNorData1 )
			l_pNorData1 = &l_pNorData1[j*3];//stride3
		if( l_pTangentData1 )
			l_pTangentData1 = &l_pTangentData1[j*3];//stride3
		bool l_bReusedExistingVertex = false;
		// Find out which position index was used, then we can compare
		// all other vertices that share this position. They will not
		// all be equal, but some of them can be merged.
		int l_iSharedPos = e_pAllVertexData->pReuseVertexPositions[j];
		for(int i=0;i<(int)e_pAllVertexData->ReverseReuseVertexPositionsVector[l_iSharedPos].size();++i)
		{
			int	l_iIndex = e_pAllVertexData->ReverseReuseVertexPositionsVector[l_iSharedPos][i];
			// Only check the indices that have already been added!
			if (l_iIndex != j &&
				// Make sure we already are that far in our new index list
				l_iIndex < (int)l_NewIndices.size() &&
				// And make sure this index has been added to newVertices yet!
				l_NewIndices[l_iIndex] < (int)l_NewIndices.size())
			{
				int	l_iNewVertexDest = l_NewIndices[l_iIndex];
				float*l_pPosData2 = &l_ppVerticesData[FVF_POS][l_iNewVertexDest*3];
				float*l_pTexData2 = l_ppVerticesData[FVF_TEX0];
				float*l_pNorData2 = l_ppVerticesData[FVF_NORMAL];
				float*l_pTangentData2 = l_ppVerticesData[FVF_TAGENT];
				if( l_pTexData2 )
					l_pTexData2 = &l_pTexData2[l_iNewVertexDest*2];
				if( l_pNorData2 )
					l_pNorData2 = &l_pNorData2[l_iNewVertexDest*3];
				if( l_pTangentData2 )
					l_pTangentData2 = &l_pTangentData2[l_iNewVertexDest*3];
				// Then finally compare vertices (this call is slow, but thanks to
				// all the other optimizations we don't have to call it that often)
				if(	NearlyEquals(l_pPosData1,l_pTexData1,l_pNorData1,l_pTangentData1,
							l_pPosData2,l_pTexData2,l_pNorData2,l_pTangentData2))
				{
					// Reuse the existing vertex, don't add it again, just
					// add another index for it!
					l_NewIndices.push_back(l_NewIndices[l_iIndex]);
					l_bReusedExistingVertex = true;
					break;
				}
			} // if (TangentVertex.NearlyEquals)
		} // foreach (otherVertexIndex)

		if (l_bReusedExistingVertex == false)
		{
			// Add the currentVertex and set it as the current index
			l_NewIndices.push_back(l_iCurrentVertexCount);
			e_pAllVertexData->AllIndicesNeededInOriginalPosVertex.push_back(j);//store original index
			for( int k=0;k<TOTAL_FVF;++k )
			{
				if(e_pAllVertexData->ppVerticesData[k])
				{
					assert(k!=FVF_SKINNING_WEIGHT&&k!=FVF_SKINNING_BONE_INDEX);//skinnign should do later not do here
					memcpy(&l_ppVerticesData[k][l_iCurrentVertexCount*g_iFVF_DataStride[k]],&e_pAllVertexData->ppVerticesData[k][j*g_iFVF_DataStride[k]],sizeof(float)*g_iFVF_DataStride[k]);
				}
			}
			l_iCurrentVertexCount++;
		} // if (reusedExistingVertex)
	} // for (num)
	assert((UINT)e_pAllVertexData->uiNumIndices>=e_pAllVertexData->AllIndicesNeededInOriginalPosVertex.size());
	//copy new data into store
	assert((UINT)l_iCurrentVertexCount<=e_pAllVertexData->uiNumIndices&&"after optmize original vertex count should bigger than optmized");
	UINT	l_uiNewIndicesCount = l_NewIndices.size();
	assert(l_uiNewIndicesCount == e_pAllVertexData->uiNumIndices);
	e_pAllVertexData->puiIndices = new UINT[l_uiNewIndicesCount];
	memcpy(e_pAllVertexData->puiIndices,&l_NewIndices[0],sizeof(int)*l_uiNewIndicesCount);
	e_pAllVertexData->uiVertexCount = l_iCurrentVertexCount;
	for( int i=0;i<TOTAL_FVF;++i )
	{
		if(e_pAllVertexData->ppVerticesData[i])
		{
			delete e_pAllVertexData->ppVerticesData[i];
			//int	l_iTotalDataLength = l_iCurrentVertexCount*g_iFVF_DataStride[i];
			//e_pAllVertexData->ppVerticesData[i] = new float[l_iTotalDataLength];
			//memcpy(e_pAllVertexData->ppVerticesData[i],l_ppVerticesData[i],sizeof(float)*l_iTotalDataLength);
			//SAFE_DELETE(l_ppVerticesData[i]);
			e_pAllVertexData->ppVerticesData[i] = l_ppVerticesData[i];
		}
	}
	SAFE_DELETE(l_ppVerticesData);
}

sColladaGeomatries*cLibraryGeometries::GetColladaGeomatries(const WCHAR*e_strID)
{
	for(UINT i=0;i<this->m_ColladaGeomatryList.size();++i)
	{
		if(!wcscmp(m_ColladaGeomatryList[i]->strID,e_strID))
			return m_ColladaGeomatryList[i];
	}
	return 0;
}

bool	sColladaGeomatries::GenerateSkinningTriangleBlendicesWeights(sController*e_pController)
{
	if(!wcscmp(this->strID,&e_pController->Skin.strSource[1])||
		!wcscmp(this->strName,&e_pController->Skin.strSource[1]))
	{
		sGeometryMesh::sTriangles*l_pTriangles = pGeometryMesh->pCurrentGeometryMeshTriangles;
		if(  this->pGeometryMesh->AllTriangleVertexData.size() == 1 )
		{
			assert(this->pGeometryMesh->AllTriangleVertexData.size() == 1&&"currently I just know how to parse one mesh one triangle");
			if( !l_pTriangles )
			{
				UT::ErrorMsg((WCHAR*)this->strName,L"does't has no mesh");
				return false;
			}
			int	l_iTotalSizeForSkinningData = l_pTriangles->iCount*3*MAX_RELEATED_BONE_COUNT;
			sGeometryMesh::sAllVertexData*l_pAllVertexData = this->pGeometryMesh->AllTriangleVertexData[l_pTriangles];
			assert(l_pAllVertexData->ppVerticesData[FVF_SKINNING_WEIGHT] == 0&&"has been assigned");
			assert(l_pAllVertexData->ppVerticesData[FVF_SKINNING_BONE_INDEX] == 0&&"has been assigned");
			//each triangle has 3 vertices and each vertex has 4 blending weight data
			//one vertex 3float(xyz)
			l_pAllVertexData->ppVerticesData[FVF_SKINNING_WEIGHT] = new float[l_iTotalSizeForSkinningData];
			memset(l_pAllVertexData->ppVerticesData[FVF_SKINNING_WEIGHT],0,sizeof(float)*l_iTotalSizeForSkinningData);
			//each triangle has 3 vertices and each vertex has 4 blending indices data,one indices is byte
			//so we do not need to multiply 4
			if(e_pController->Skin.pInflunceBonesForVertex->eType == eDT_BYTE)
			{
				l_pAllVertexData->ppVerticesData[FVF_SKINNING_BONE_INDEX] = new float[l_pTriangles->iCount*3];
				memset(l_pAllVertexData->ppVerticesData[FVF_SKINNING_BONE_INDEX],0,sizeof(float)*l_pTriangles->iCount*3);
			}
			else
			{//one int size is equal one float size.
				l_pAllVertexData->ppVerticesData[FVF_SKINNING_BONE_INDEX] = new float[l_iTotalSizeForSkinningData];
				memset(l_pAllVertexData->ppVerticesData[FVF_SKINNING_BONE_INDEX],0,sizeof(float)*l_iTotalSizeForSkinningData);
			}
	#ifdef DEBUG
			sGeometryMesh::sSource*l_pSource = this->pGeometryMesh->GetVerticesByType(sGeometryMesh::eVT_POS);
			assert(e_pController->Skin.pInflunceBonesForVertex->uiCount/MAX_RELEATED_BONE_COUNT == l_pSource->iCount/3&&"vertex count must match the mesh vertex count");
			assert(e_pController->Skin.pInflunceWeightsForVertex->uiCount/MAX_RELEATED_BONE_COUNT == l_pSource->iCount/3&&"vertex count must match the mesh vertex count");
			assert(l_pAllVertexData->AllIndicesNeededInOriginalPosVertex.size()<=(UINT)(l_pTriangles->iCount*3));
	#endif
			//first to generate the count as many indices and assign weights and influnce joints by vertex index
			//second generate the count as indeed by optmize,and assign data again.
			//
			//start to make weights and blending bone indices data as triangle's sort then resize it as draw indices just do as vertex resize(GenerateTriangleVertices,GenerateUniqueIndicesAndVertexData_OPENGL)
			sGeometryMesh::sSource*l_pPosSource = this->pGeometryMesh->GetVerticesByType(sGeometryMesh::eVT_POS);
			int	l_iPosBufferOffset = l_pPosSource->iIndexOffset;
			int	l_iNumSource = this->pGeometryMesh->m_iAllSourceUsingIndicesBuffer;
			//fetch collada reference data
			assert(e_pController->Skin.pInflunceBonesForVertex->eType == eDT_BYTE);
			float	*l_pfWeightsData = (float*)e_pController->Skin.pInflunceWeightsForVertex->pData;
			char	*l_pBlendingBoneIndicesData = (char*)e_pController->Skin.pInflunceBonesForVertex->pData;
			int	l_iWeightsDataLength = sizeof(float)*MAX_RELEATED_BONE_COUNT;
			int	l_iBlendingDataLength = e_pController->Skin.pInflunceBonesForVertex->eType == eDT_BYTE?sizeof(char)*MAX_RELEATED_BONE_COUNT:sizeof(int)*MAX_RELEATED_BONE_COUNT;
			//fetech store data to store
			//l_pDestFVF_SKINNING_BONE_INDEXData type is  char not float
			char*l_pDestFVF_SKINNING_BONE_INDEXData = (char*)l_pAllVertexData->ppVerticesData[FVF_SKINNING_BONE_INDEX];
			float*l_pDestFVF_SKINNING_WEIGHTData = l_pAllVertexData->ppVerticesData[FVF_SKINNING_WEIGHT];
			//for original position vertex index
			std::vector<int>	l_OriginalIndex;
			l_OriginalIndex.resize(l_pTriangles->iCount*3);
 			for( int i=0;i<l_pTriangles->iCount;++i )
			{
				int*l_pIndices  = &l_pTriangles->piIndexBuffer[i*l_iNumSource*3];//each time just 3 vertex,get indicex for specific vertex index
				for(int j=0;j<3;++j)
				{
					int	l_iCurrentPosVertexIndex = i*3+j;//xyz,3xyz for one triangle
					assert(l_pTriangles->iCount*3>=l_iCurrentPosVertexIndex);
					l_iCurrentPosVertexIndex*=MAX_RELEATED_BONE_COUNT;
					int	l_iPosIndex = l_pIndices[l_iPosBufferOffset+j*l_iNumSource];
					l_OriginalIndex[i*3+j] = l_iPosIndex;
					l_iPosIndex*=MAX_RELEATED_BONE_COUNT;//skinning and weight,multiply MAX_RELEATED_BONE_COUNT to get correct index,each weight and joint are four for one vertex
					assert((UINT)l_iPosIndex<=e_pController->Skin.pInflunceBonesForVertex->uiCount&&"weights data length is over range");
					memcpy(&l_pDestFVF_SKINNING_WEIGHTData[l_iCurrentPosVertexIndex],&l_pfWeightsData[l_iPosIndex],l_iWeightsDataLength);
					memcpy(&l_pDestFVF_SKINNING_BONE_INDEXData[l_iCurrentPosVertexIndex],&l_pBlendingBoneIndicesData[l_iPosIndex],l_iBlendingDataLength);
	//#ifdef DEBUG
	//			float	l_fForWeights[4] = {l_pDestFVF_SKINNING_WEIGHTData[l_iCurrentPosVertexIndex],
	//										l_pDestFVF_SKINNING_WEIGHTData[l_iCurrentPosVertexIndex+1],
	//										l_pDestFVF_SKINNING_WEIGHTData[l_iCurrentPosVertexIndex+2],
	//										l_pDestFVF_SKINNING_WEIGHTData[l_iCurrentPosVertexIndex+3]};
	//			char	l_fForJoints[4] = {l_pDestFVF_SKINNING_BONE_INDEXData[l_iCurrentPosVertexIndex],
	//										l_pDestFVF_SKINNING_BONE_INDEXData[l_iCurrentPosVertexIndex+1],
	//										l_pDestFVF_SKINNING_BONE_INDEXData[l_iCurrentPosVertexIndex+2],
	//										l_pDestFVF_SKINNING_BONE_INDEXData[l_iCurrentPosVertexIndex+3]};
	//			float	l_fWeights = l_fForWeights[0]+l_fForWeights[1]+l_fForWeights[2]+l_fForWeights[3];
	//			assert(l_fWeights<=1.001f&&l_fWeights>=0.9f);
	//#endif
				}
			}
	//#ifdef DEBUG
	//		for( int i=0;i<l_pTriangles->iCount*3;++i )
	//		{
	//			float	l_fWeight = l_pDestFVF_SKINNING_WEIGHTData[i*4]+
	//				l_pDestFVF_SKINNING_WEIGHTData[i*4+1]+
	//				l_pDestFVF_SKINNING_WEIGHTData[i*4+2]+
	//				l_pDestFVF_SKINNING_WEIGHTData[i*4+3];
	//			char	l_iForJoints[4] = {l_pDestFVF_SKINNING_BONE_INDEXData[i*4],
	//										l_pDestFVF_SKINNING_BONE_INDEXData[i*4+1],
	//										l_pDestFVF_SKINNING_BONE_INDEXData[i*4+2],
	//										l_pDestFVF_SKINNING_BONE_INDEXData[i*4+3]};
	//			assert(l_fWeight>=0.9f&&l_fWeight<=1.1f);
	//		}
	//#endif
			//start to generate necessary size by draw indices(do the same thing as GenerateUniqueIndicesAndVertexData_OPENGL)
			UINT	l_uiSize = l_pAllVertexData->uiVertexCount;
			float*	l_pfFinalWeights = new float[l_uiSize*MAX_RELEATED_BONE_COUNT];
			char*	l_pFinalBlendingBondIndices = new char[l_uiSize*MAX_RELEATED_BONE_COUNT];
			int	l_iSourceIndex = -1;
			int	l_iDestndex = -1;
			UINT	l_uiDestIndex = -1;
			std::vector<int>	l_OriginalIndex2;
			l_OriginalIndex2.resize(l_uiSize);
			for( UINT i=0;i<l_uiSize;++i )
			{
				//l_uiDestIndex = l_pAllVertexData->puiIndices[l_pAllVertexData->AllIndicesNeededInOriginalPosVertex[i]];
				l_uiDestIndex = l_pAllVertexData->AllIndicesNeededInOriginalPosVertex[i];
				l_OriginalIndex2[i] = (l_OriginalIndex[l_uiDestIndex]);
				//l_uiDestIndex = 1156;
				assert(l_uiDestIndex<=(UINT)l_pTriangles->iCount*3);
				l_iSourceIndex = i*MAX_RELEATED_BONE_COUNT;
				l_iDestndex = l_uiDestIndex*MAX_RELEATED_BONE_COUNT;
				//xyz 3 floats,three vertex one triangle.so the size have to multiply 3
				memcpy(&l_pfFinalWeights[l_iSourceIndex],&l_pDestFVF_SKINNING_WEIGHTData[l_iDestndex],l_iWeightsDataLength);
				memcpy(&l_pFinalBlendingBondIndices[l_iSourceIndex],&l_pDestFVF_SKINNING_BONE_INDEXData[l_iDestndex],l_iBlendingDataLength);
	//#ifdef DEBUG
	//			float	l_fForWeights = l_pfFinalWeights[l_iSourceIndex]+l_pfFinalWeights[l_iSourceIndex+1]+l_pfFinalWeights[l_iSourceIndex+2]+l_pfFinalWeights[l_iSourceIndex+3];
	////				char	l_fForJoints[4] = {l_pDestFVF_SKINNING_BONE_INDEXData[l_uiDestIndex*MAX_RELEATED_BONE_COUNT],l_pDestFVF_SKINNING_BONE_INDEXData[l_uiDestIndex*MAX_RELEATED_BONE_COUNT+1],l_pDestFVF_SKINNING_BONE_INDEXData[l_uiDestIndex*MAX_RELEATED_BONE_COUNT+2],l_pDestFVF_SKINNING_BONE_INDEXData[l_uiDestIndex*MAX_RELEATED_BONE_COUNT+3]};
	//			assert(l_fForWeights<=1.001f&&l_fForWeights>0.9f);
	//#endif
			}
			delete e_pController->Skin.pInflunceWeightsForVertex->pData;
			delete e_pController->Skin.pInflunceBonesForVertex->pData;
			e_pController->Skin.pInflunceWeightsForVertex->pData = (char*)l_pfFinalWeights;
			e_pController->Skin.pInflunceBonesForVertex->pData = (char*)l_pFinalBlendingBondIndices;
			for(UINT i=0;i<pGeometryMesh->AllMeshIndexList.size();++i)
			{
				if(!wcscmp(pGeometryMesh->AllMeshIndexList[i]->GetGeoID().c_str(),&e_pController->Skin.strSource[1]))
				{
					pGeometryMesh->AllMeshIndexList[i]->AssignSkinningData(
						(float*)e_pController->Skin.pInflunceWeightsForVertex->pData,
						(float*)e_pController->Skin.pInflunceBonesForVertex->pData,
						l_uiSize);
					return true;
				}
			}
		}
		else
		{//triangle is bigger than 2 now assign all weight as 1 for it.
			assert(this->pGeometryMesh->AllTriangleVertexData.size() == 1&&"currently I just know how to parse one mesh one triangle");
			for(UINT i=0;i<pGeometryMesh->AllMeshIndexList.size();++i)
			{
				if(!wcscmp(pGeometryMesh->AllMeshIndexList[i]->GetName(),&e_pController->Skin.strSource[1]))
				{
					//pGeometryMesh->AllMeshIndexList[i]->AssignSkinningData(
					//	(float*)e_pController->Skin.pInflunceWeightsForVertex->pData,
					//	(float*)e_pController->Skin.pInflunceBonesForVertex->pData,
					//	l_uiSize);
					return true;
				}
			}
		}
	}
	//assert(0);
	//fuck a mesh could have a lot triangles,change it!.
	return false;
}

cLibraryGeometries::cLibraryGeometries()
{
	m_pCurrentColladaGeomatry = 0;
}

cLibraryGeometries::~cLibraryGeometries()
{
	ClearData();
}

std::vector<cCurve>	cLibraryGeometries::ProcessCurveData()
{
	std::vector<cCurve>	l_Curve;
	int	l_iSize = (int)m_ColladaGeomatryList.size();
	for( int i=0;i<l_iSize;++i )
	{
		sColladaGeomatries*l_pColladaGeomatries = m_ColladaGeomatryList[i];
		if(l_pColladaGeomatries->pSpline)
		{
			l_Curve.push_back(l_pColladaGeomatries->pSpline->ContertToCurve());
		}
	}
	return l_Curve;
}

bool	cLibraryGeometries::GenerateSkinningTriangleBlendicesWeights(sController*e_pController)
{
	for( UINT i=0;i<this->m_ColladaGeomatryList.size();++i )
	{
		if( m_ColladaGeomatryList[i]->GenerateSkinningTriangleBlendicesWeights(e_pController) )
			return true;
	}
	return false;
}

sColladaGeomatries*	cLibraryGeometries::GetGeometryByID(const WCHAR*e_strID)
{
	for(UINT i=0;i<m_ColladaGeomatryList.size();++i)
	{
		if(!wcscmp(m_ColladaGeomatryList[i]->strID,e_strID))
			return m_ColladaGeomatryList[i];
	}
	return 0;
}

void	cLibraryGeometries::ParseData(TiXmlElement*e_pTiXmlElement)
{
	DELETE_VECTOR(m_ColladaGeomatryList,sColladaGeomatries*);
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		sColladaGeomatries*l_pCurrentColladaGeomatry = new sColladaGeomatries();
		m_pCurrentColladaGeomatry = l_pCurrentColladaGeomatry;
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry]))
		{
			Parsegeometry(l_pChildTiXmlElement);
		}
		else
		{
			assert(0);
		}
		m_ColladaGeomatryList.push_back(l_pCurrentColladaGeomatry);
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryGeometries::Parsegeometry(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"name"))
		{
			m_pCurrentColladaGeomatry->strName = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentColladaGeomatry->strID = l_strValue;
			if( g_strParsingDataList )
			{
				std::string	l_str;
				l_str.append("Geometry ");
				l_str.append(UT::WcharToChar(l_strValue));
				g_strParsingDataList->push_back(l_str);
			}
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh]))
		{
			Parsegeometry_mesh( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_spline]))
		{
			Parsegeometry_spline( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
			cGameApp::OutputDebugInfoString(l_strName);
			cGameApp::OutputDebugInfoString(L"	Fucking Lazy to do this now!!!\n");
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
	//fuck here should call generate mesh not at Parsegeometry_mesh_triangles_p
}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	assert(m_pCurrentColladaGeomatry->pGeometryMesh==0);
	m_pCurrentColladaGeomatry->pGeometryMesh = new sGeometryMesh();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_source]))
		{
			Parsegeometry_mesh_source( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_vertices]))
		{
			Parsegeometry_mesh_vertices( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_triangles]))
		{
			Parsegeometry_mesh_triangles( l_pChildTiXmlElement );
		}
		else
		{
			UT::ErrorMsg(l_strName,L"not support yet");
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh_source(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sGeometryMesh::sSource*l_pGeometryMeshSource = new sGeometryMesh::sSource();
	m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource = l_pGeometryMeshSource;
	this->m_pCurrentColladaGeomatry->pGeometryMesh->AllGeometryMeshSourceList.push_back(l_pGeometryMeshSource);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
			l_pGeometryMeshSource->strID = l_pTiXmlAttribute->Value();
		}
		else
		if(!wcscmp(l_strName,L"name"))
		{
			l_pGeometryMeshSource->strName = l_pTiXmlAttribute->Value();
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_source_float_array]))
		{
			Parsegeometry_mesh_source_float_array( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_source_technique_common]))
		{
			Parsegeometry_mesh_source_technique_common( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh_source_float_array(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource->iCount = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource->strFloat_array_ID = l_pTiXmlAttribute->Value();
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	if( m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource->iCount )
	{
		assert(l_strText);
		m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource->pBufferArray = ParseDataToGenerateIntPointer<float>((WCHAR*)l_strText,m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource->iCount);
	}
}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh_source_technique_common(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_source_technique_common_accessor]))
		{
			Parsegeometry_mesh_source_technique_common_accessor( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh_source_technique_common_accessor(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource->iTechnique_common_Accesor_Count = VALUE_TO_INT;
			//assert(m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource->iCount==_wtoi(l_strValue)*3);
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource->strSource_technique_common__accessor_source = l_pTiXmlAttribute->Value();
		}
		else
		if(!wcscmp(l_strName,L"stride"))
		{
			m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshSource->iStride = _wtoi(l_strValue);
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_source_technique_common_accessor_param]))
		{
			Parsegeometry_mesh_source_technique_common_accessor_param( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh_source_technique_common_accessor_param(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"name"))
		{
			//l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"type"))
		{
			assert(wcscmp(l_strValue,L"float") == 0);
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh_vertices(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
			assert( m_pCurrentColladaGeomatry->pGeometryMesh->m_GeometryMeshVertices.strID == 0 );
			m_pCurrentColladaGeomatry->pGeometryMesh->m_GeometryMeshVertices.strID = l_pTiXmlAttribute->Value();
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_triangles_input]))
		{
			Parsegeometry_mesh_vertices_input( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh_vertices_input(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sGeometryMesh::sVertices::sInput*l_pInput = new sGeometryMesh::sVertices::sInput;
	m_pCurrentColladaGeomatry->pGeometryMesh->m_GeometryMeshVertices.AllInput.push_back(l_pInput);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"semantic"))
		{
			l_pInput->strInputSemantic = l_pTiXmlAttribute->Value();
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			l_pInput->strInputSource = l_pTiXmlAttribute->Value();
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh_triangles(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sGeometryMesh::sTriangles*l_pTriangles = new sGeometryMesh::sTriangles;
	this->m_pCurrentColladaGeomatry->pGeometryMesh->AllGeometryMeshTrianglesList.push_back(l_pTriangles);
	this->m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshTriangles = l_pTriangles;
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			this->m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshTriangles->iCount = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"material"))
		{
			this->m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshTriangles->strMaterial = l_pTiXmlAttribute->Value();
			if( g_strParsingDataList )
			{
				std::string	*l_pstr = &(*g_strParsingDataList)[g_strParsingDataList->size()-1];
				l_pstr->append(" using material");
				l_pstr->append(UT::WcharToChar(l_strValue));
				l_pstr->append("\n");
			}
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_triangles_input]))
		{
			Parsegeometry_mesh_triangles_input( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_mesh_triangles_p]))
		{
			Parsegeometry_mesh_triangles_p( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_mesh_triangles_input(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sGeometryMesh::sTriangles::sInputData*l_pInputData = new sGeometryMesh::sTriangles::sInputData;
	this->m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshTriangles->AllInputDataList.push_back(l_pInputData);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"set"))
		{//texture destination index,it oculd be multi texture for one model
			l_pInputData->iSet = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"offset"))
		{
			l_pInputData->iOffset = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"semantic"))
		{
			l_pInputData->strSemantic = l_pTiXmlAttribute->Value();
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			l_pInputData->strSource = l_pTiXmlAttribute->Value();
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}

//
//Look at the "offset" attribute of each <input> in <triangles>. The "vertices" (which just contain position info in this example)
//are offset 0 and the normals are offset 1. This means that the first value in the <p> is a position, the second value is a normal,
//and that makes one complete "index". The third value is a position again, and the fourth value is a normal. The fifth value is another
//position, and the sixth value is a normal, and that completes the first triangle. Repeat for the other 11 triangles. 
//The key thing that may be confusing is that Collada allows different indices per stream (I call them "non-unified indices").
//That is, a single logical "index" can refer to (e.g.) the 10th position and the 3rd normal. Graphics APIs like OpenGL and Direct3D
//only support unified indices, where each index is a single value specifying where to look in both the position and normal streams.
//Non-unified indices are more flexible and allow a more compact data representation. For example, using non-unified indices, a cube
//can be described with 8 positions and 6 normals, but with unified indices you need 24 positions and 24 normals. 
//Dealing with non-unified indices is one of the trickier aspects of writing a Collada importer, since most Collada
//exporters write data using non-unified indices, and most graphics APIs don't support them. You need to transform the
//data to unified index form. This code is tricky to write, but fortunately it's already been done for you. The confusingly-named 
//"deindexer" conditioner in the Collada Refinery converts a Collada document to unified index form.
//https://collada.org/public_forum/viewtopic.php?p=3037&sid=0973780713238182da5d492866774507
void	cLibraryGeometries::Parsegeometry_mesh_triangles_p(TiXmlElement*e_pTiXmlElement)
{
	//assign correct vertex format
	m_pCurrentColladaGeomatry->pGeometryMesh->AssignVertexType();
	m_pCurrentColladaGeomatry->pGeometryMesh->m_iTotalBufferStride = 0;
	int	*l_piAllOffsetIndex = new int[ m_pCurrentColladaGeomatry->pGeometryMesh->AllGeometryMeshSourceList.size() ];
	//it's possible using same index buffer,so we have to know how many offset  we needed
	m_pCurrentColladaGeomatry->pGeometryMesh->m_iAllSourceUsingIndicesBuffer = 0;
	memset(l_piAllOffsetIndex,-1,sizeof(int)*m_pCurrentColladaGeomatry->pGeometryMesh->AllGeometryMeshSourceList.size());
	sGeometryMesh::sTriangles*l_pTriangles = this->m_pCurrentColladaGeomatry->pGeometryMesh->pCurrentGeometryMeshTriangles;
	for( UINT i=0;i<l_pTriangles->AllInputDataList.size();++i )
	{
		int	l_iIndexOffset = l_pTriangles->AllInputDataList[i]->iOffset;
		l_piAllOffsetIndex[i] = l_iIndexOffset;
		//if over two index is same,using same index bffer data
		int	l_iSame = 0;
		for( UINT j=0;j<i;++j )
		{
			if(l_piAllOffsetIndex[j] == l_iIndexOffset)
			{
				l_iSame++;
			}
		}
		if( !l_iSame )
		{
			m_pCurrentColladaGeomatry->pGeometryMesh->m_iTotalBufferStride += m_pCurrentColladaGeomatry->pGeometryMesh->AllGeometryMeshSourceList[i]->iStride;
			m_pCurrentColladaGeomatry->pGeometryMesh->m_iAllSourceUsingIndicesBuffer++;
		}
	}
	delete []l_piAllOffsetIndex;
	assert(l_pTriangles->piIndexBuffer == 0);
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);
	//it could be quad(4),but now triangle(3)
	int	l_iTriangleVerticesCount = 3;
	UINT	l_uiIndicesCount = l_pTriangles->iCount
								*l_iTriangleVerticesCount*
								m_pCurrentColladaGeomatry->pGeometryMesh->m_iAllSourceUsingIndicesBuffer;
	l_pTriangles->ProcessIndices((WCHAR*)l_strText,l_uiIndicesCount);
	cMesh*l_pMesh = m_pCurrentColladaGeomatry->pGeometryMesh->GenerateMeshByTriangleData(l_pTriangles);
	m_pCurrentColladaGeomatry->pGeometryMesh->AllMeshIndexList.push_back(l_pMesh);
	l_pMesh->SetName(m_pCurrentColladaGeomatry->strName);
	l_pMesh->SetGeoID(m_pCurrentColladaGeomatry->strID);
}
//
//
//
void	cLibraryGeometries::Parsegeometry_spline(TiXmlElement*e_pTiXmlElement)
{
	this->m_pCurrentColladaGeomatry->pSpline = new sSpline();
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"closed"))
		{
			this->m_pCurrentColladaGeomatry->pSpline->bClosed = VALUE_TO_BOOLEAN;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_spline_source]))
		{
			Parsegeometry_spline_source( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_spline_control_vertices]))
		{
			Parsegeometry_spline_control_vertices( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_spline_source(TiXmlElement*e_pTiXmlElement)
{
	sGeometryMesh::sSource*l_pSource = new sGeometryMesh::sSource;
	m_pCurrentColladaGeomatry->pSpline->pCurrentSource = l_pSource;
	this->m_pCurrentColladaGeomatry->pSpline->AllSplineSource.push_back(l_pSource);
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
			l_pSource->strID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_spline_source_float_array]))
		{
			Parsegeometry_spline_source_float_array( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_spline_source_technique_common]))
		{
			Parsegeometry_spline_source_technique_common( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_spline_source_Name_array]))
		{
			Parsegeometry_spline_source_Name_array( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_spline_source_float_array(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			m_pCurrentColladaGeomatry->pSpline->pCurrentSource->iCount = VALUE_TO_INT;
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentColladaGeomatry->pSpline->pCurrentSource->strFloat_array_ID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	if( m_pCurrentColladaGeomatry->pSpline->pCurrentSource->iCount )
	{
		assert(l_strText);
		m_pCurrentColladaGeomatry->pSpline->pCurrentSource->pBufferArray = ParseDataToGenerateIntPointer<float>((WCHAR*)l_strText,m_pCurrentColladaGeomatry->pSpline->pCurrentSource->iCount);
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_spline_source_technique_common(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_spline_source_technique_common_accessor]))
		{
			Parsegeometry_spline_source_technique_common_accessor( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_spline_source_technique_common_accessor(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			m_pCurrentColladaGeomatry->pSpline->pCurrentSource->iTechnique_common_Accesor_Count = VALUE_TO_INT;
			//l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			m_pCurrentColladaGeomatry->pSpline->pCurrentSource->strSource_technique_common__accessor_source = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"stride"))
		{
			m_pCurrentColladaGeomatry->pSpline->pCurrentSource->iStride = VALUE_TO_INT;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_spline_source_technique_common_accessor_param]))
		{
			Parsegeometry_spline_source_technique_common_accessor_param( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_spline_source_technique_common_accessor_param(TiXmlElement*e_pTiXmlElement)
{
	//const WCHAR*l_strName = 0;
	//const WCHAR*l_strValue = 0;
	//TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	//while( l_pTiXmlAttribute )
	//{
	//	l_strName = l_pTiXmlAttribute->Name();
	//	l_strValue = l_pTiXmlAttribute->Value();
	//	if(!wcscmp(l_strName,L"name"))
	//	{
	//		m_pCurrentColladaGeomatry->pSpline->pCurrentSource->eVT
	//		l_strValue;
	//	}
	//	else
	//	if(!wcscmp(l_strName,L"type"))
	//	{
	//		l_strValue;
	//	}
	//	else
	//	{
	//		assert(0);
	//	}
	//	l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	//}
}
//
//
//
void	cLibraryGeometries::Parsegeometry_spline_source_Name_array(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			this->m_pCurrentColladaGeomatry->pSpline->pCurrentSource->iCount = VALUE_TO_INT;
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			this->m_pCurrentColladaGeomatry->pSpline->pCurrentSource->strID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	this->m_pCurrentColladaGeomatry->pSpline->pCurrentSource->pNameArray = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryGeometries::Parsegeometry_spline_control_vertices(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryGeometriesElement[eLG_geometry_spline_control_vertices_input]))
		{
			Parsegeometry_spline_control_vertices_input( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryGeometries::Parsegeometry_spline_control_vertices_input(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sGeometryMesh::sVertices::sInput*l_pInputData = new sGeometryMesh::sVertices::sInput;
	m_pCurrentColladaGeomatry->pSpline->Splinecontrol_vertices.AllInput.push_back(l_pInputData);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"semantic"))
		{
			l_pInputData->strInputSemantic = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			l_pInputData->strInputSource = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}

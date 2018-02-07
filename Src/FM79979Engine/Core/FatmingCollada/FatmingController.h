#ifndef COLLADA_CONTROLLER
#define COLLADA_CONTROLLER
#include "FatmingGeometryData.h"
//each bone only could have 4 related bone animation,
//this could be no-limit,but u have to care about vertex assigned and shader code,and I am so lazy to do it :)
#define MAX_RELEATED_BONE_COUNT	4
//====================================
//data structure reference from xml format
//====================================
	struct	sController
	{
		struct	sSkin
		{
			enum	eControllerType
			{
				eCT_NONE = 0,
				eCT_JOINT,
				eCT_BINDPOSE,
				eCT_SKINWEIGHTS,
			};
			//just a collada data reference not the final data store
			//if u want to fetch final weights and blending bone indices u should fetch
			//pInflunceWeightsForVertex,pInflunceBonesForVertex
			struct	sSource
			{
				eControllerType	ControllerType;
				const WCHAR*	strID;
				const WCHAR*	strArrayID;
				int	iCount;				//weights count must as same coutn as vertex(position) count,
				std::vector<std::wstring>strAllJointName;	//for skin joint,reference to visual scene joints node.
				float*pData;			//for weights
				cMatrix44	*pMatrcies;	//for bind pose
				sSource()
				{
					pData = 0;
					ControllerType = eCT_NONE;
					strID = 0;
					strArrayID = 0;
					iCount = 0;
					pMatrcies = 0;
				}
				~sSource()
				{
					SAFE_DELETE(pData);
					SAFE_DELETE(pMatrcies);
				}
			};
			struct	sJoints
			{
				struct	sInput
				{
					const WCHAR*strSemantic;
					const WCHAR*strSource;
				};
				std::vector<sInput*>	InputList;
				~sJoints(){ DELETE_VECTOR(InputList); }
			};
			struct	sVertexWeights
			{
				struct	sInput
				{
					const WCHAR*strSemantic;
					const WCHAR*strSource;
					int	  iOffset;
				};
				int		iCount;	//pVcount's count
				int*	pVcount;//all vertex influnce bone count
				int*	pV;		//related bones and weights,the count as same as pVcount's data.
				UINT		uiAllVertexInfluncebones;
				std::vector<sInput*>	InputList;
				sInput*	GetInputyType(WCHAR*e_strSemantic)
				{
					for( UINT i=0;i<InputList.size();++i )
						if(!wcscmp(InputList[i]->strSemantic,e_strSemantic))
							return InputList[i];
					return 0;
				}
				sVertexWeights(){ pVcount = 0; pV = 0; iCount = 0; uiAllVertexInfluncebones  =0 ;  }
				~sVertexWeights(){ 	DELETE_VECTOR(InputList);  SAFE_DELETE(pVcount); SAFE_DELETE(pV);  }
			};
			sSkin(){ pInflunceWeightsForVertex = 0; pInflunceBonesForVertex = 0; strSource = 0; }
			~sSkin(){ DELETE_VECTOR(AllSource); 	SAFE_DELETE(pInflunceBonesForVertex);	SAFE_DELETE(pInflunceWeightsForVertex); }
			//internal data
			std::vector<sSource*>	AllSource;
			sJoints					Joints;
			sVertexWeights			VertexWeights;
			//
			sSource*				GetSourceByType(eControllerType e_eControllerType);
			sSource*				GetSourceBySourceID(const WCHAR*e_strSourceID);
													
			void	GenerateWeightsAndBonesData();//via vertex_weights element's vcount and v,we could generate the bones and weights as we needed
			int		VisualSceneJointNodeNameToIndex(const WCHAR*e_strNodeName);//-1 as noe found,for visual scene node check,so we could fethch matrix quickly
			//final usable data
			//!!this data has make max four joints as related,to be final skinning weight data,as we did this we have to assign four joints as related in joints data assigned.
			//as same as all vertex count(VertexWeights.icount*4) ,because one vertex relation 4 joints,4 weights,one joint one weight.
			//for final data output,currently maxima bones influnce is 4.
			sDataAndDataType*		pInflunceWeightsForVertex;//
			sDataAndDataType*		pInflunceBonesForVertex;//u have to take a look at sSource,because the joint's index is correspond to sSource's strAllJointName name

			const WCHAR*	strSource;//geometry ID in liberary_geoetries.
			cMatrix44	matBindShapeMatrix;
		};
		sSkin	Skin;
		const WCHAR*strID;
		const WCHAR*strName;
		sController(){ strID = 0; strName = 0; }
	};

#endif
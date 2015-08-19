#ifndef FATMING_VISUAL_SCENE_DATA_H
#define FATMING_VISUAL_SCENE_DATA_H

class	cBone;
//it could have more than one mesh on this visual node,
struct sVisualSceneLibrary
{
	struct	sNode
	{
		enum	eNodeType
		{
			eNT_NONE = 0,
			eNT_JOINT,
			eNT_NODE,
		};

		enum	eIndtanceType
		{
			eIT_NONE = 0,
			eIT_GEOMETRY,
			eIT_CONTROLLER,
		};

		struct sInstance
		{
			struct	sBineVertexInput
			{
				const WCHAR*strSemantic;
				const WCHAR*strInputSemantic;
				const WCHAR*strInputSet;
				sBineVertexInput()
				{
					strSemantic = 0;
					strInputSemantic = 0;
					strInputSet = 0;
				}
			};
			sBineVertexInput	BineVertexInput;
			eIndtanceType IndtanceType;
			//coorespond to visuall scene's controller instance's id
			const WCHAR*strUrl;
			std::vector<const WCHAR*>strSkeletonVector;
			const WCHAR*strMaterialStmbol;
			const WCHAR*strMaterialTarget;
			sInstance()
			{
				IndtanceType = eIT_NONE;
				strUrl = 0;
				//strSkeleton = 0;
				strMaterialStmbol = 0;
				strMaterialTarget = 0;
			}
		};
		eNodeType NodeType;
		const WCHAR*strName;
		const WCHAR*strID;
		const WCHAR*strSID;
		const WCHAR*strMatrixSid;
		const WCHAR*strInstanceCamera;
		sInstance*pInstance;
		cMatrix44	mat;
		//link
		sNode*pSibling;
		sNode*pParent;
		sNode*pFirstChild;
		bool	IsContain(const WCHAR*e_strName)
		{
			bool	l_bContain = false;
			if( !wcscmp(e_strName,strID) )// || (e_pNode->strSID&&!wcscmp(e_strName,e_pNode->strSID))
			{
				return true;
			}
			if( pFirstChild )
				IsContain(pFirstChild,e_strName,&l_bContain);
			return l_bContain;
		}
		void	IsContain(sNode*e_pNode,const WCHAR*e_strName,bool*e_plContain)
		{
			if( !wcscmp(e_strName,e_pNode->strID) )// || (e_pNode->strSID&&!wcscmp(e_strName,e_pNode->strSID))
			{
				*e_plContain = true;
				return;
			}
			sNode*l_pCurrentNode = e_pNode->pFirstChild;
			if( l_pCurrentNode&&*e_plContain == false )
			{
				IsContain(l_pCurrentNode,e_strName,e_plContain);
			}
			l_pCurrentNode = e_pNode->pSibling;
			if( l_pCurrentNode&& *e_plContain == false )
			{
				IsContain(l_pCurrentNode,e_strName,e_plContain);
			}
		}
		sNode(sNode*e_pParent)
		{
			mat = cMatrix44::Identity;
			strInstanceCamera = 0;
			strMatrixSid = 0;
			strName = 0;
			strID = 0;
			strSID = 0;
			pInstance = 0;
			pSibling = 0;
			pParent = e_pParent;
			pFirstChild = 0;
			NodeType = eNT_NONE;
		}
		~sNode()
		{
			if( pInstance )
				delete pInstance;
		}
	};
	//mesh node contian a mesh data and bone node vector,it all got from external,so do not delete it
	struct	sSkeletonNode
	{
		sNode*	pRootNode;
		//
		cBone*	pRootBone;
		cBone*	AddChildBone(sNode*e_pMeshNode,cBone*e_pParentBone);
		cBone*	AddSiblingBone(sNode*e_pMeshNode,cBone*e_pPreviousBone);
		void	GenerateSkeleton(sNode*e_pMeshNode,cBone*e_pParent);//internal use,quickly generate data for GenerateSkeleton.
		void	GenerateSkeleton();//via pMeshNode to generate hierachy for children
		cNamedTypedObjectVector<cBone>	m_AllBoneData;//all bone data,releate to SkeltonRootBonevector
		std::vector<sNode*>	AllNodesOfSkeletonNodeVector;	//all noded of SkeletonNode store in this vector,the order is same as m_AllBoneData
		sSkeletonNode();
		~sSkeletonNode();
		bool	ContainThisNodeName(const WCHAR*e_strName);
		void	ContainThisNodeName(sNode*e_pNode,WCHAR*e_strName,bool*e_pbValue);
	};
	//remove main root bone's sibling,we just wanna a skelton system here
	//assign skeleton toot into mesh noe
	void		GenerateSkeltonRootBone();
	const WCHAR*	strVisualSceneName;		//MaxScene
	const WCHAR*	strVisualSceneID;		//MaxScene
	cMatrix44	matRoot;
	sNode*		pRootNode;
	float		fStartTime;
	float		fEndTime;
	float		fFPrameRate;
	std::vector<sSkeletonNode*>	AllSkeletonNodeVector;//if node is sNode::eNT_JOINT then skeleton will be generate,the first level nodes under max scene and its children is a joint then it's a skeleton node.
	std::vector<sNode*>	AllNodesVector;			//all node store in this vector
	std::vector<sNode*>	AllInstanceNodesVector;	//all geometry and controller instance node,do not delete the data,because AllNodesVector has already store the instance data
	sVisualSceneLibrary()
	{
		strVisualSceneName = 0;
		strVisualSceneID = 0;
		pRootNode = 0;
		matRoot = cMatrix44::Identity;
	}
	~sVisualSceneLibrary()
	{
		DELETE_VECTOR(AllSkeletonNodeVector,sSkeletonNode*);
		DELETE_VECTOR(AllNodesVector,sNode*);
	}
	//count to show count info
	void				DumpNodeInfo(sNode*e_pNode,int *e_piCount);
	sSkeletonNode*		GetSkeletonNode(const WCHAR*e_strSkeletonName);
	static void			IsNodeContainJoints(sNode*e_pNode,bool *e_pbValue);
};
#endif
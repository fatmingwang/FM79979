#ifndef	FATMING_COLLADA_ANIMATIONS_DATA
#define FATMING_COLLADA_ANIMATIONS_DATA

	class	cBone;
	struct	sAnimationLibrary
	{
		struct sAnimation
		{
			const WCHAR*strID;
			struct sSource
			{
				const WCHAR*strID;
				int		iCount;
				const WCHAR*strArrayID;//for float array and namearray
				float*	pFloatArray;	//it's possible matrix array or time array,if it's matrix divide 16
				const WCHAR*pNameArray;
				struct	sTechniqueCommon
				{
					const WCHAR*strSource;
					int	iCount;
					int	iStride;
					const WCHAR*strParaName;
					const WCHAR*strParaType;
					sTechniqueCommon(){ strSource = 0; iCount = 0; iStride = 0; strParaName = 0; strParaType =0; }
				};
				struct	sTechnique
				{
					const WCHAR*strProfile;
					const WCHAR*strPreInfinity;
					const WCHAR*strPostInfinity;
					sTechnique(){ strProfile = 0; strPreInfinity = 0; strPostInfinity = 0; }
				};
				sSource(){strID = 0;iCount = 0;pFloatArray = 0;pNameArray = 0; pTechniqueCommon = 0; pTechnique = 0; }
				~sSource(){ SAFE_DELETE(pFloatArray); SAFE_DELETE(pTechniqueCommon); SAFE_DELETE(pTechnique); }
				sTechniqueCommon*	pTechniqueCommon;
				sTechnique*			pTechnique;
				//matrix stride is 16 or it's time data.
				bool				IsMatrix(){ return  pTechniqueCommon->iStride == 16?true:false; }
			};
			struct sSampler
			{
				const WCHAR*strID;
				struct	sInput
				{
					const WCHAR*strSemantic;
					const WCHAR*strSource;
					sInput(){ strSemantic = 0; strSource= 0; }
				};
				std::vector<sInput*>	InputVector;
				~sSampler(){ DELETE_VECTOR(InputVector,sInput*); }
			};
			struct sChannel
			{
				const WCHAR*strSource;
				const WCHAR*strTarget;
				sChannel(){ strSource = 0; strTarget = 0; }
			};
			std::vector<sSource*>	SourceVector;
			std::vector<sSampler*>	SamplerVector;
			std::vector<sChannel*>	ChannelVector;
			sAnimation(){  }
			~sAnimation(){ DELETE_VECTOR(SamplerVector,sSampler*); DELETE_VECTOR(ChannelVector,sChannel*); DELETE_VECTOR(SourceVector,sSource*); }
			std::map<float, cMatrix44>	GenerateKeyFrameMatrix(const WCHAR*e_strJointeName);
		};
		sAnimationLibrary(){}
		~sAnimationLibrary(){ ClearData(); }
		void	ClearData(){ DELETE_VECTOR(AnimationVector,sAnimation*); }
		std::vector<sAnimation*>	AnimationVector;
		inline	sAnimation*	GetAnimation(WCHAR*e_strName)
		{
			std::string l_strName = UT::WcharToChar(e_strName);
			const WCHAR*l_strCurrentCompareName = 0;
			size_t	l_AnimationVectorSize = AnimationVector.size();
			sAnimation*l_pAnimation = 0;
			for(size_t i=0;i<l_AnimationVectorSize;++i)
			{
				l_pAnimation = AnimationVector[i];
				size_t	l_ChannelVecotrSize = l_pAnimation->ChannelVector.size();
				for(size_t j=0;j<l_ChannelVecotrSize;++j)
				{
					l_strCurrentCompareName = l_pAnimation->ChannelVector[j]->strTarget;
					size_t	l_iLength = strlen(l_strName.c_str());
					if( wcslen(l_strCurrentCompareName)>=l_iLength )
					{
						bool l_bSameName = true;
						for( size_t j=0;j<l_iLength;++j )
						{
							if( l_strCurrentCompareName[j] != l_strName[j] )
							{
								l_bSameName = false;
								break;
							}
						}
						if( l_bSameName )
							return l_pAnimation;
					}
				}

			}
			return 0;
		}
		inline	sAnimation*	GetAnimation(const WCHAR*e_strAnimationID)
		{
			WCHAR*l_strrAnimationName = 0;
			for(UINT i=0;i<AnimationVector.size();++i)
			{
				sAnimation*l_pAnimation = AnimationVector[i];
				size_t	l_uiNumChannel = l_pAnimation->ChannelVector.size();
				for(size_t j=0;j<l_uiNumChannel;++j)
				{
					sAnimationLibrary::sAnimation::sChannel*l_pChannel = l_pAnimation->ChannelVector[j];
					l_strrAnimationName = wcstok((WCHAR*)l_pChannel->strTarget,L"/");
					if(!wcscmp(l_strrAnimationName,e_strAnimationID))
						return AnimationVector[i];
				}
			}
			return 0;
		}

	};

#endif
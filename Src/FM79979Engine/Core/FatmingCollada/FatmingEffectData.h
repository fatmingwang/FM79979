#ifndef FATMING_COLLADA_EFFECTS_H
#define FATMING_COLLADA_EFFECTS_H
	
	enum	eTechniqueType
	{
		eTL_NONE = 0,
		eTL_AMBIENT,	//just a color
		eTL_DIFFUSE,	//
		eTL_SPECULAR,	//
		eTL_BUMP,		//
		eTL_GLOW,		//currently I have no idea what data it will have,so ask artist,http://www.gamasutra.com/features/20040526/james_01.shtml
						//http://www.gamedev.net/community/forums/topic.asp?topic_id=318280
		eTL_TRANSPARENT,//basicly if RGB is zero,the destination color will be alpha,just using alpha formula to setup correct color.
		eTL_MAX
	};
	
	struct sEffectData
	{
		struct	sComonEffect
		{
			//===================
			//not all data has value depend what technique
			//we just using common texhnique here
			//===================
			struct	sTechniqueData
			{
				float	*pfFloatData;
				//
				float*	pColor;
				//
				eTechniqueType	eTL;
				//
				const WCHAR*strTextureName;
				//
				const WCHAR*strTexCoord;
				//
				//float*pfValue;not common technique,if I got time I will finish this.
			/*	struct	sTexParameter
				{
					
				};
				sTexParameter*pTexParameter;*/
				sTechniqueData()
				{
					//pTexParameter = 0;
					//pfValue = 0;
					pfFloatData = 0;
					pColor = 0;
					strTexCoord = 0;
					strTextureName = 0;
					eTL = eTL_NONE;
				}
				~sTechniqueData()
				{
					//SAFE_DELETE(pTexParameter);
					//pfValue = 0;
					SAFE_DELETE(pColor);
					SAFE_DELETE(pfFloatData);
				}

			};
			struct	sNewParam
			{
				const WCHAR*strSid;
				enum eNewParamType
				{
					eNPT_NONE = 0,
					eNPT_SURFACE,
					eNPT_SAMPLE2D,
				};
				struct	sSample2D
				{
					const WCHAR*strSource;
					const WCHAR*strWrap_w;
					const WCHAR*strWrap_t;
					const WCHAR*strWrap_s;
					const WCHAR*strMinFilter;
					const WCHAR*strMagFilter;
					const WCHAR*strMipfilter;
					sSample2D()
					{
						strSource = 0;
						strWrap_w = 0;
						strWrap_t = 0;
						strMinFilter = 0;
						strMagFilter = 0;
						strMipfilter = 0;
					}
				};
				struct sSurface
				{
					const WCHAR* strSid;
					const WCHAR* strType;
					const WCHAR* strInitForm;
					const WCHAR* strFormat;
					sSurface()
					{
						 strSid = 0;
						 strType = 0;
						 strInitForm = 0;
						 strFormat = 0;
					}
				};
				eNewParamType	eNPT;
				sSample2D	Sample2D;
				sSurface	Surface;
				sNewParam()
				{
					eNPT = eNPT_NONE;
				}
			};
			const WCHAR*strSid;
			const WCHAR*strId;
			const WCHAR* strTechniqueName;
			std::vector<sTechniqueData*>	AllTechniqueAndTextureName;//common technique and extra technique( ex:bump,technique under technique ),ask artist remove any useless technique,or programer would be confused.
			std::vector<sNewParam*>				AllNewParam;
			sComonEffect(){ strSid = 0; strTechniqueName = 0; }
			~sComonEffect()
			{
				DELETE_VECTOR(AllTechniqueAndTextureName,sTechniqueData*)
				DELETE_VECTOR(AllNewParam,sNewParam*);
			}
		};
		const WCHAR*m_strID;//effect file name( xxx.fx )
		const WCHAR*m_strName;
		sComonEffect	ComonEffect;
		sEffectData()
		{
			m_strID = 0;
			m_strName = 0;
		}
	};

#endif
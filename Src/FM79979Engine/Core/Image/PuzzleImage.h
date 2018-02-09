#ifndef PuzleImage_H
#define PuzleImage_H

namespace FATMING_CORE
{
	class	cPuzzleImage;
	class	cStaticAnimation;
	class	cNumeralImage;
	class	cBaseImage;
	//=======================
	//for puzzleimage unit
	//=======================
	struct	sUVData
	{
		float	fUV[4];			//texture uv.
		POINT	OffsetPos;		//offset position,via this we coul add alpha part to the detination texture while create texture.
		POINT	Size;			//the width and height we draw on screen.
		POINT	OriginalSize;	//orginal image size,animation image have same size for each image.
		POINT	ShowPosInPI;	//the left top pixel position in the pi image,this is for accurate to the position show on the pi,because UV it is possible to lost 1 pixel.
		inline	sUVData()
		{
			memset(this,0,sizeof(sUVData));
		}
		//the render position is center of image,but it's possible alpha pixels were strip,so we have to adjust the center.
		//it will like this:  half render size( strip size ) + GetCenterOffsetx
		//so it's final center offset size.
		//it has a serious problem...if the offset pixel is not same
		//it might occur visual error
		inline	Vector2	GetCenterOffsetxWithRenderSize( Vector2 e_vRenderSize )
		{
		    Vector2 l_vScalr( ((float)e_vRenderSize.x/Size.x),((float)e_vRenderSize.y/Size.y) );
		    Vector2 l_vCenterOffset( OriginalSize.x/2.f-OffsetPos.x,OriginalSize.y/2.f-OffsetPos.y );
		    l_vCenterOffset.x*=l_vScalr.x;
		    l_vCenterOffset.y*=l_vScalr.y;
		    return l_vCenterOffset;
		}
	};
	//derive from UV data with name
	struct	sPuzzleData:public sUVData
	{
		std::wstring	strFileName;
		sPuzzleData(){}
		inline	sPuzzleData(const wchar_t*e_strName,float*e_pfUV,POINT e_OffsetPos,POINT e_Size,POINT	e_OriginalSize,POINT e_ShowPosInPI)
		{
			ShowPosInPI = e_ShowPosInPI;
			OriginalSize = e_OriginalSize;
			strFileName = e_strName;
			memcpy(fUV,e_pfUV,sizeof(float)*4);
			OffsetPos = e_OffsetPos;
			Size = e_Size;
		}
	};


	//=======================
	//here is quite painful,because the machine is not good enough so we have to test any possible let image
	//be mip at one image,or just an image that stretch to pow of two,
	//basicly we just need puzzle image,but sometimes we still need to using stretch image to save loaduing time,and saving some memory.
	//=======================
	class	cPuzzleImageUnit:public cBaseImage
	{
		GET_SET_DEC(cPuzzleImageUnit*,m_pNext,GetNext,SetNext);
		GET_SET_DEC(cPuzzleImageUnit*,m_pPrior,GetPrior,SetPrior);
		sPuzzleData*m_pPuzzleData;
		//do we need this one?
		//GET_SET_DEC(std::vector<Vector2>*,m_pHintPointVectorFromPIEditor,GetHintPointVectorFromPIEditor,SetHintPointVectorFromPIEditor);
	public:
		DEFINE_TYPE_INFO()
		cPuzzleImageUnit(sPuzzleData*e_pPuzzleData,cPuzzleImage*e_pPuzzleImageParent);
		cPuzzleImageUnit(cPuzzleImageUnit*e_pPuzzleImageUnit);
		cPuzzleImageUnit(cBaseImage*e_pBaseImage);
		CLONE_MYSELF(cPuzzleImageUnit);
		virtual ~cPuzzleImageUnit();
		sPuzzleData*					GetPuzzleData(){return m_pPuzzleData;}
		static	cPuzzleImageUnit*		GetMe(TiXmlElement*e_pElement,bool e_bClone = true);
	};
	//================
	//this one for gather animation image data,
	//if we have those data we could simply assign data wjen we are editting the animation
	//
	//while calling remover object.....
	//ensure the parameter is index not name,or it's possible to kill wrong object
	//
	//================
	class cImageIndexOfAnimation:public NamedTypedObject
	{
		float*	m_pfEndTime;
	public:
		struct	sImageIndexAndTimeGap
		{
			int		iIndex;
			float	fTimeGap;
			sImageIndexAndTimeGap(){ iIndex = -1;fTimeGap = -1.f;}
			sImageIndexAndTimeGap(int e_iIndex,float e_fTimeGap)
			{
				iIndex = e_iIndex;
				fTimeGap = e_fTimeGap;
			}
		};
	    //while it is editor using this,in the game this one should delete or debug
	    std::vector<wstring>					*m_pNameList;
	    //while it's in the game using this
	    std::vector<sImageIndexAndTimeGap>          m_ImageAnimationDataList;
		cImageIndexOfAnimation(bool e_bNewNameList)
		{
		    m_pNameList = 0;
		    if(e_bNewNameList)
		        m_pNameList = new std::vector<std::wstring>;
			m_pfEndTime = 0;
		}
		//not a pointer reference it will new the data,but it's very rare to be called(ex:in the editor)
		cImageIndexOfAnimation(cImageIndexOfAnimation*e_pImageIndexOfAnimation);
		cImageIndexOfAnimation(cStaticAnimation*e_pStaticAnimation);
		CLONE_MYSELF(cImageIndexOfAnimation);
		virtual ~cImageIndexOfAnimation()
		{
	        SAFE_DELETE(m_pNameList);
			SAFE_DELETE(m_pfEndTime);
		}
		void    GenerateImageIndexByPI(cPuzzleImage*e_pPI,std::vector<std::wstring>   *e_pNameList);
		int     GetImageIndex(int e_iIndex,cPuzzleImage*e_pPI);
		void    Clear()
		{
		    if( m_pNameList )
		        m_pNameList->clear();
			m_ImageAnimationDataList.clear();
		}
		void    AddNameObject(const wchar_t*e_strName,int e_iIndex,float e_fTimeGap)
		{
		    std::wstring    l_strName = e_strName;
		    m_pNameList->push_back(l_strName);
			m_ImageAnimationDataList.push_back(sImageIndexAndTimeGap(e_iIndex,e_fTimeGap));
		}
		void    RemoveNameObject(int e_iIndex)
		{
		    m_pNameList->erase(m_pNameList->begin()+e_iIndex);
			m_ImageAnimationDataList.erase(m_ImageAnimationDataList.begin()+e_iIndex);
		}
		int Count()
		{
		    return (int)m_ImageAnimationDataList.size();
		}
		inline	float GetEndTime()
		{
			if( !m_pfEndTime )
			{
				m_pfEndTime = new float;
				*m_pfEndTime = 0.f;
				size_t	l_iSize = m_ImageAnimationDataList.size();
				for( size_t i = 0;i<l_iSize;++i )
				{
					*m_pfEndTime += m_ImageAnimationDataList[i].fTimeGap;
				}
			}
			return *m_pfEndTime;
		}

		int	GetImageIndexByCurrentTime( float e_fTime );
	};
	//if GeneratePuzzleimageUnit is 1,we will generate puzzleimageUnit,but it cost a tiny loading time.
	//!!but currently I do not make this actived!!!
	//<PuzzleImage Name="79979" Count="6" GeneratePuzzleimageUnit="0">
	//    <PuzzleUnit Name="ST2StonePart" UV="0,0,0.32875,0.1275," OffsetPos="9,9," Size="263,102" />
	//    <PuzzleUnit Name="ST3StonePart" UV="0.33,0,0.81875,0.11625," OffsetPos="4,4," Size="391,93" />
	//    <PuzzleUnit Name="ST4StonePart" UV="0,0.12875,0.3575,0.2525," OffsetPos="11,11," Size="286,99" />
	//    <PuzzleUnit Name="ST5StonePart" UV="0.35875,0.12875,0.885,0.25375," OffsetPos="52,52," Size="421,100" />
	//    <PuzzleUnit Name="ST6StonePart" UV="0,0.255,0.33375,0.39125," OffsetPos="30,30," Size="267,109" />
	//    <PuzzleUnit Name="ST7StonePart" UV="0.335,0.255,0.665,0.38375," OffsetPos="5,5," Size="264,103" />
	//</PuzzleImage>
	//==========================
	//uv is not store base image,it's in m_PuzzleDataList
	//do not add any object into list,or it could be occur error,
	//we have just GenerateAllPuzzleImageUnit while parse data
	//==========================
	class cPuzzleImage:virtual	public cBaseImage,virtual	public cNamedTypedObjectVector<cPuzzleImageUnit>
	{
	    //for animation in the pi image
		GET_SET_DEC(cNamedTypedObjectVector<cImageIndexOfAnimation>*,m_pImageIndexOfAnimation,GetImageIndexOfAnimationList,SetImageIndexOfAnimationList);
	    //for quickly to share all children's UV data,if only works while call GenerateAllUVToTriangleStrip
	    float*  m_pfAllChildrenTriangleStripUV;
	    //for quickly to share all children's UV data,if only works while call GenerateAllfUVToTwoTriangle
	    float*  m_pfAllChildrenTwoTriangleUV;
		//as how manay m_pPuzzleData
		GET_SET_DEC(int,m_iNumImage,GetNumImage,SetNumImage);
		//image data uv offset pos and name....
		GET_SET_DEC(sPuzzleData**,m_ppPuzzleData,GetPuzzleData,SetPuzzleData);

		//internal using,generate all image unit while parse data is done
		void	GenerateAllPuzzleImageUnit();
	public:
		DEFINE_TYPE_INFO()
		cPuzzleImage(cPuzzleImage*e_pPuzzleImage);
		cPuzzleImage(char*e_strName,std::vector<sPuzzleData> *e_pPuzzleDataList,bool e_bGenerateAllUnit = false,bool e_bFetchPixels = false);
		CLONE_MYSELF(cPuzzleImage);
		virtual ~cPuzzleImage();
		virtual	void	Render()override { cBaseImage::Render(); }
		void	Render(int e_iIndex);
		//dump data into destination,so we could delete the source puzzleImage.
		//void	DumpIntoPuzzleImage(cPuzzleImage*e_pPuzzleImage);
		//the count as many as GetNum,or instead call (*GetPuzzleDataList())[iIndex]
		sPuzzleData*	GetPuzzleData(int e_iIndex){ return m_ppPuzzleData[e_iIndex]; }
		sPuzzleData**	GetAllPuzzleData(){ return m_ppPuzzleData; }

		//make sure the Numeral image all have same size,
		//and it's sequence,and remember delete it after use it,
		//ensure u have no release puzzle image before delete Numeral image
		cNumeralImage*	GetNumeralImageByName(const wchar_t*e_strNumerImageName);
		cNumeralImage*	GetNumeralImageByName(const wchar_t*e_str0ImaneName,const wchar_t*e_str9ImaneName);
		float*   GetAllChildrenTriangleStripUV();
		float*   GetAllChildrenTwoTriangleUV();
		//
		static	std::string			GetFileName(const wchar_t*e_strObjectName);
		static	std::string			GetFileName(const char*e_strObjectName);
		//static	cPuzzleImage*		GetMe(TiXmlElement*e_pElement);
	};
}
#endif//end PuzleImage_H
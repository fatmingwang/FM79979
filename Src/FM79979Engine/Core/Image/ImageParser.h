#ifndef IMAGE_RESOURCE_MANAGER_H
#define IMAGE_RESOURCE_MANAGER_H
#include "BaseImage.h"
#include "../XML/XMLLoader.h"
namespace FATMING_CORE
{
	//=======================
	//perhap here come some new custome image format(HDR or...)
	//<Image>
	//	<Common Name="xxxx.bmp" Name1="xxx.png" />
	//</Image>
	//=======================
	enum	eImageType
	{
		eIT_COMMON = 0,//bmp,png....else
		eIT_UI,
		eIT_NUMERAL,		//cNumeralImage
		eIT_PUZZLE_IMAGE,	//cPuzzleImageUnit
		eIT_MAX
	};
    class   cPuzzleImage;
    class   cImageIndexOfAnimation;
	//=====================
	//there is no any cooresponf for multi thead
	//=====================
	class	cImageParser:public cNamedTypedObjectVector<NamedTypedObject>,public ISAXCallback
	{
		//if file is exist all parse will skip
		bool	m_bFileExist;
	    //for animation data,jsut a temp
	    cNamedTypedObjectVector<cImageIndexOfAnimation>*m_pCurrentImageIndexOfAnimationList;
		//if true will hold old image
		//unsigned char	m_ucOldImageKeepOrMergeOrKill;

		//for loading image file
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement)override;
		void	DistributeElementToLoaders();
		//	<Numeral Name="NumeralImage/CardNI.png" />
		void	ProcessNumeralTexture();
		//<Common Name="BackGround/BKImage.png" UV="0,0,1,1" />
		void	ProcessCommonTexture();
		//UI Image="xxx.png" Position="x,y"
		void	ProcessUITexture();
	//<PuzzleImage Name="79979" Count="6" >
	//    <PuzzleUnit Name="ST2StonePart" UV="0,0,0.32875,0.1275," OffsetPos="9,9," />
	//    <PuzzleUnit Name="ST3StonePart" UV="0.33,0,0.81875,0.11625," OffsetPos="4,4," />
	//    <PuzzleUnit Name="ST4StonePart" UV="0,0.12875,0.3575,0.2525," OffsetPos="11,11," />
	//    <PuzzleUnit Name="ST5StonePart" UV="0.35875,0.12875,0.885,0.25375," OffsetPos="52,52," />
	//    <PuzzleUnit Name="ST6StonePart" UV="0,0.255,0.33375,0.39125," OffsetPos="30,30," />
	//    <PuzzleUnit Name="ST7StonePart" UV="0.335,0.255,0.665,0.38375," OffsetPos="5,5," />
	//</PuzzleImage>
		void	ProcessPuzzleImageTexture();
		eImageType	m_eImageType;
		bool	m_bSortPIFileAsOriginal;
		//
		virtual	void	InternalParse()override;
		//
		virtual	void	RemoveResourceObject(NamedTypedObject*e_pObject)override;
	public:
		DEFINE_TYPE_INFO()
		DEFINE_FILE_EXTENSION_NAME_INFO()
		//DEFINE_TYPE_INFO()
		//because pi editor need original data sort.
		cImageParser(bool e_bSortPIFileAsOriginal = false);
		virtual ~cImageParser();

		cUIImage*	GetUIImage(int e_iIndex);
		cUIImage*	GetUIImage(const wchar_t*e_strName);

		cBaseImage*	GetBaseImage(const wchar_t*e_strName);

		cPuzzleImage*	GetPuzzleImageByFileName(const wchar_t*e_strName);
		cPuzzleImage*	GetPuzzleImage(const wchar_t*e_strName);
		cPuzzleImage*	GetPuzzleImage(int e_iIndex);
		cPuzzleImage*	GetPuzzleImage(const char*e_strFileName,const wchar_t*e_strPIName);
		void	Export(char*e_strFileName);
	};
	//only support PI editor(cUIImage)
	bool	IsObjectOverlap(cImageParser*e_pObject);
}//end namespace

#endif
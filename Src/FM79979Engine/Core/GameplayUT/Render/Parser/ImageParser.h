#ifndef IMAGE_RESOURCE_MANAGER_H
#define IMAGE_RESOURCE_MANAGER_H
#include "../CommonRender/BaseImage.h"
#include "../../../XML/XMLLoader.h"
namespace FATMING_CORE
{
    class   cPuzzleImage;
    class   cImageIndexOfAnimation;
	//=====================
	//there is no any cooresponf for multi thead
	//=====================
	class	cImageParser:public cNamedTypedObjectVector<NamedTypedObject>
	{
	public:
		DEFINE_TYPE_INFO()
		//DEFINE_TYPE_INFO()
		//because pi editor need original data sort.
		cImageParser();
		virtual ~cImageParser();

		cUIImage*	GetUIImage(int e_iIndex);
		cUIImage*	GetUIImage(const wchar_t*e_strName);

		cBaseImage*	GetBaseImage(const wchar_t*e_strName);

		cPuzzleImage*	GetPuzzleImageByFileName(const wchar_t*e_strFileName);
		cPuzzleImage*	GetPuzzleImageByFileName(const char*e_strFileName);
		cPuzzleImage*	GetPuzzleImage(const wchar_t*e_strName);
		cPuzzleImage*	GetPuzzleImage(int e_iIndex);
		cPuzzleImage*	GetPuzzleImage(const char*e_strFileName,const wchar_t*e_strPIName);
		virtual			NamedTypedObject* GetObjectByFileName(const char*e_strFileName)override;
		void			Export(char*e_strFileName);
	};
	//only support PI editor(cUIImage)
	bool	IsObjectOverlap(cImageParser*e_pObject);
}//end namespace

#endif
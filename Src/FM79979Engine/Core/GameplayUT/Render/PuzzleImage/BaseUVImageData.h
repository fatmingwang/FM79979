#pragma once
#include "PuzzleImage.h"
namespace FATMING_CORE
{
	//==================
	//one pi is not enough,so we need more texture but the texture size has  restriction...
	//although it might slow(?) but more images could be edited for MPDI the animation would be better
	//default we assume only one PI is exists,so the data(cBaseUVImageData::m_pPuzzleImage) from ascent
	//is the first data we have added!make sure the function has called is the way u want.
	//because all PI data are from external but all of it will store at cNamedTypedObjectVector<cBaseUVImageData>
	//if u want to change current m_pPuzzleImage make sure the one is what u want!.
	//==================
	class	cMulti_PI_Image
	{
	protected:
		cPuzzleImage* m_pPuzzleImage;
		bool	AddPIObjectIfNotExist(cPuzzleImage*e_pPuzzleImage)
		{
			m_pPuzzleImage = e_pPuzzleImage;
			return m_pPIList->AddObject(e_pPuzzleImage);
		}
		//all object only need one this,so it's a pointer only one instance for same MPDIList's children.
		//only cMPDIList has its instance!all cMPDIList's chilren(cCueToStartCurveWithTime,cMultiPathDynamicImage)
		//get from cMPDIList!
		GET_SET_DEC(cNamedTypedObjectVector<cPuzzleImage>*,m_pPIList,GetPIList,SetPIList);
	public:
		cMulti_PI_Image(){ m_pPIList = new cNamedTypedObjectVector<cPuzzleImage>(true);m_pPIList->SetFromResource(true); m_pPuzzleImage = 0; }
		cMulti_PI_Image(cMulti_PI_Image*e_pMulti_PI_Image)
		{
			assert(e_pMulti_PI_Image);
			m_pPIList = new cNamedTypedObjectVector<cPuzzleImage>();
			this->m_pPuzzleImage = e_pMulti_PI_Image->m_pPuzzleImage;
			m_pPIList->CopyListPointer(e_pMulti_PI_Image->m_pPIList);
		}
		virtual ~cMulti_PI_Image(){SAFE_DELETE(m_pPIList); }
		//make sure this one only call by original cMulti_PI_Image::m_pBaseUVImageDataList
		virtual	void	SetPuzzleImage(cPuzzleImage*e_pPuzzleImage)
		{
			AddPIObjectIfNotExist(e_pPuzzleImage);
		}
		virtual	void	Merge(cMulti_PI_Image*e_pMulti_PI_Image)
		{
			if( e_pMulti_PI_Image&& this == e_pMulti_PI_Image )
				return;
			m_pPIList->AddListToMe(e_pMulti_PI_Image->m_pPIList);
		}
		int				GetNumImage(){return m_pPuzzleImage->GetNumImage();}
		sPuzzleData*	GetAllPuzzleData(){return m_pPuzzleImage?m_pPuzzleImage->GetAllPuzzleData():0;}
		cPuzzleImage*	GetPuzzleImage(){return m_pPuzzleImage;}
		cPuzzleImage*	GetPuzzleImage(const wchar_t*e_strPIName){ return m_pPIList->GetObject(e_strPIName); }
		cPuzzleImage*	GetPuzzleImage(int e_iIndex){ return m_pPIList->GetObject(e_iIndex); }
		inline  void    RemovePIReference(cPuzzleImage*e_pPI)
		{
		    int l_iIndex = m_pPIList->GetObjectIndexByName(e_pPI->GetName());
		    if( l_iIndex != -1 )
		    {
		        if( this->m_pPuzzleImage == e_pPI )
		        {
		            m_pPuzzleImage = 0;
		        }
				m_pPIList->RemoveObjectWithoutDelete(l_iIndex);;
		    }
		}
		bool	IsContainPI(cPuzzleImage*e_pPI)
		{
			if( m_pPIList->GetObject(e_pPI->GetName()) )
				return true;
			return false;			
		}
	};
}//end namespace
#include "ParticleBase.h"
#include "ParticleEmitter.h"
#include "PrtTexture.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cPrtTextureActDynamicTexture);
	//usage explanation
	char*	cPrtTextureActDynamicTexture::UsageExplanation="select pi and it will apply image animation if pi contain animation,it could be edited in the SA";

	cPrtTextureActDynamicTexture::cPrtTextureActDynamicTexture()
	{
		m_pfAllUVToTwoTriangleData = 0;
		m_pImageIndexOfAnimation = 0;
	}
	
	cPrtTextureActDynamicTexture::~cPrtTextureActDynamicTexture()
	{
	}

	std::string	cPrtTextureActDynamicTexture::GetOutputDataString()
	{
		memset(m_sTemp,0,sizeof(char)*MAX_PATH);
		if( this->m_pEmitterOwner->GetBaseImage()->Type() == cPuzzleImage::TypeID )
		{
			if( this->m_pImageIndexOfAnimation )
			{
				sprintf(m_sTemp,"%s,",m_pImageIndexOfAnimation->GetCharName().c_str());
			}
		}
		return m_sTemp;
	}

	NamedTypedObject*  cPrtTextureActDynamicTexture::Clone()
	{
		cPrtTextureActDynamicTexture* l_p = new cPrtTextureActDynamicTexture; 
		assert(m_pImageIndexOfAnimation);
		l_p->m_pImageIndexOfAnimation = m_pImageIndexOfAnimation;
		l_p->SetName(GetName());
		l_p->SetPuzzleImage(this->m_pPuzzleImage);
		return l_p; 
	};

	void	cPrtTextureActDynamicTexture::SetPuzzleImage(cPuzzleImage*e_pPuzzleImage)
	{
	    cMulti_PI_Image::SetPuzzleImage(e_pPuzzleImage);
		assert(e_pPuzzleImage);
		m_pfAllUVToTwoTriangleData = e_pPuzzleImage->GetAllChildrenTwoTriangleUV();
	}

	std::string	cPrtTextureActDynamicTexture::GetDataInfo()
	{
		if( this->m_pImageIndexOfAnimation )
			sprintf(m_sTemp,"PIName:%s\nAnimationName:%s\n",this->m_pEmitterOwner->GetBaseImage()->GetCharName().c_str(),m_pImageIndexOfAnimation->GetCharName().c_str());
		else
			sprintf(m_sTemp,"change image index every second");
		return m_sTemp;
	}

	bool	cPrtTextureActDynamicTexture::SetDataByDataString(const char*e_pString)
	{
		char*	l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, ",",&l_pForStrtok_s);
		if( !l_pString )
			return false;
		assert(this->m_pEmitterOwner->GetBaseImage()->Type() == cPuzzleImage::TypeID&&"dynamic tecture must be PI texture!!");
		cPuzzleImage*l_pPI = dynamic_cast<cPuzzleImage*>(this->m_pEmitterOwner->GetBaseImage());
		this->SetPuzzleImage(l_pPI);
		assert(l_pPI->GetImageIndexOfAnimationList()&&"ensure this pi has animation!!");
		this->m_pImageIndexOfAnimation = l_pPI->GetImageIndexOfAnimationList()->GetObject(UT::CharToWchar(l_pString).c_str());
		assert(m_pImageIndexOfAnimation&&"can't find animation at this pi!!");
		return true;		
	}
//end namespace FATMING_CORE
}
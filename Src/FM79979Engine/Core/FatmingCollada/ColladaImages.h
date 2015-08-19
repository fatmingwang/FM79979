#ifndef COLLADA_IMAGES_H
#define COLLADA_IMAGES_H
enum eLIList
{
	eLI_image = 0,
	eLI_image_init_from,
	eLI_image_extra,
	eLI_MAX
};
static const WCHAR*g_scLibraryImagesElement[eLI_MAX] =
{
	L"image",
	L"init_from",
	L"extra"
};

struct	sLibraryImages
{
	char	strID[MAX_PATH];
	char	strName[MAX_PATH];
	char	strFileName[MAX_PATH];
	sLibraryImages()
	{
		memset(this,0,sizeof(sLibraryImages));
	}
};

class cLibraryImages :public NamedTypedObject
{
	char	m_strDirectory[MAX_PATH];
//
	void	Parseimage(TiXmlElement*e_pTiXmlElement);
//
	void	Parseimage_init_from(TiXmlElement*e_pTiXmlElement);
//
	void	Parseimage_extra(TiXmlElement*e_pTiXmlElement);
	//
	sLibraryImages*	m_pCurrentLibraryImages;
public:
	DEFINE_TYPE_INFO();
	cLibraryImages();
	~cLibraryImages();
	void	ParseData(TiXmlElement*e_pTiXmlElement);
	void	SetDirectory(char*e_strDirectory)
	{
		sprintf(m_strDirectory,"%s\0",e_strDirectory);
	}
	std::vector<sLibraryImages*>	AllLibraryImages;
	cImageParser*					m_pImageList;
};

#endif
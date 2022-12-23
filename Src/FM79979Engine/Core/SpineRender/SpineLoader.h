#pragma once
#include <string>
#include <map>
void	SpineInit();
void	SpineDestory();

void	DoSpineTest();
void	DoSpineRender();


//namespace spine
//{
//	class Atlas;
//}
//
//class cSpineAnimation :public cRenderObject
//{
//public:
//	void	Parse(const char* filename);
//	void	Update(float e_fElpaseTime)override;
//	void	Render()override;
//};

namespace spine
{
	class Atlas;
	class AnimationStateData;
	class AnimationState;
	class SkeletonData;
	class Skeleton;
}
class cTextureLoaderForSpine;

struct cSpineCache
{
	cSpineCache();
	~cSpineCache();
	cTextureLoaderForSpine*								m_pTextureLoaderForSpine;
	std::map<std::string, spine::SkeletonData*>			m_KeyAndSkeletonDataMap;
	std::map<std::string, spine::AnimationStateData*>	m_KeyAndAnimationStateDataMap;
	std::map<std::string, spine::Atlas*>				m_KeyAndAtlasMap;
	//std::map<std::string, spine::Atlas*>				m_KeyAndAtlasMap;
	spine::Atlas* GetAtlas(const char* e_strAtlasFile);
	std::tuple<spine::Skeleton*, spine::AnimationState*, spine::Atlas*>		GetSpineData(const char* e_strFileName);

	spine::SkeletonData*													GetSkeletonData(const char* e_strSkeletonFileName,const char* e_strAtlasFileName = nullptr);

	spine::Skeleton*														GetSkeleton(const char* e_strFileName);
	spine::AnimationState*													GetAnimationState(const char* e_strFileName);
};

extern  cSpineCache* g_pSpineCache;
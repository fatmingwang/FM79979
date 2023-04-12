#include "SpineLoader.h"
#include <spine/Debug.h>
#include <spine/spine.h>
#include <stdio.h>
#include <functional>
#include <tuple>
#include "SpineExtension.h"
#include "SkeletonRenderer.h"
#include "SkeletonAnimation.h"
#include "pch.h"

#include <filesystem>

using std::filesystem::directory_iterator;
//using std::filesystem::current_path;
#ifdef MSVC
#pragma warning(disable : 4710)
#endif

//https://github.com/GerogeChong/spine-sdl

class cExtensionForSpine : public spine::DefaultSpineExtension
{
public:
	cExtensionForSpine() {}

	virtual ~cExtensionForSpine() {}

protected:
	virtual char* _readFile(const spine::String& path, int* length)override
	{
		auto l_pData = UT::GetFileContent(path.buffer(), *length, "rb");
		auto bytes = SpineExtension::alloc<char>(*length, __FILE__, __LINE__);
		memcpy(bytes, l_pData, *length);
		delete l_pData;
		return bytes;
	}
};


class cTextureLoaderForSpine : public spine::TextureLoader, public NamedTypedObject
{
public:
	cTextureLoaderForSpine(){}

	virtual ~cTextureLoaderForSpine() {}
	virtual void load(spine::AtlasPage& page, const spine::String& path)
	{
		auto l_pTexture = cTextureManager::GetInstance()->GetObject(this, path.buffer());
		//CCASSERT(texture != nullptr, "Invalid image");
		if (l_pTexture)
		{
			//texture->setTexParameters(textureParams);
			page.setRendererObject(l_pTexture);
			page.width = l_pTexture->GetWidth();
			page.height = l_pTexture->GetHeight();
		}
	}
	virtual void unload(void* texture)
	{
		if (texture)
		{
			cTexture* l_pTexture = (cTexture*)texture;
			l_pTexture->Release(this);
		}
	}
};


cExtensionForSpine* g_pExtensionForSpine = nullptr;
DebugExtension* g_pSpineDebugExtension = nullptr;
cSpineCache::cSpineCache()
{
	m_pTextureLoaderForSpine = nullptr;
	m_pTextureLoaderForSpine = new cTextureLoaderForSpine();
#ifdef WASM
	g_pExtensionForSpine = new cExtensionForSpine();
	SpineExtension::setInstance(g_pExtensionForSpine);
#else
	#ifdef DEBUG
		//g_pSpineDebugExtension = new DebugExtension(SpineExtension::getInstance());
		//SpineExtension::setInstance(g_pSpineDebugExtension);
	#endif
#endif
}
cSpineCache::~cSpineCache()
{
	DELETE_MAP(m_KeyAndSkeletonDataMap);
	DELETE_MAP(m_KeyAndAnimationStateDataMap);
	DELETE_MAP(m_KeyAndAtlasMap);
	SAFE_DELETE(m_pTextureLoaderForSpine);
	SAFE_DELETE(g_pExtensionForSpine);
#ifdef DEBUG
	if (g_pSpineDebugExtension)
	{
		g_pSpineDebugExtension->reportLeaks();
		delete g_pSpineDebugExtension;
	}
#endif

}

spine::Atlas* cSpineCache::GetAtlas(const char* e_strAtlasFile)
{
	auto l_IT = m_KeyAndAtlasMap.find(e_strAtlasFile);
	if (l_IT == m_KeyAndAtlasMap.end())
	{
		int l_iFileLength = -1;
		char* l_pAtlasContent = GetFileContent(e_strAtlasFile, l_iFileLength, "rb");
		if (l_pAtlasContent)
		{
			//spine::Atlas* l_pAtlas = new (__FILE__, __LINE__) spine::Atlas(l_pAtlasContent, l_iFileLength,UT::GetDirectoryWithoutFileName(e_strAtlasFile).c_str(), m_pTextureLoaderForSpine);
			spine::Atlas* l_pAtlas = new (__FILE__, __LINE__) spine::Atlas(e_strAtlasFile, m_pTextureLoaderForSpine);
			m_KeyAndAtlasMap[e_strAtlasFile] = l_pAtlas;
			delete[] l_pAtlasContent;
			return l_pAtlas;
		}
	}
	return l_IT->second;
}
std::tuple<spine::Skeleton*, spine::AnimationState*, spine::Atlas*>  cSpineCache::GetSpineData(const char* e_strFileName)
{
	spine::Skeleton* l_pSkeleton = nullptr;
	spine::AnimationState* l_pAnimationState = nullptr;
	std::string l_strAtlasFileName = UT::ChangeFileExtensionName(e_strFileName, "atlas");
	auto l_pAtlas = GetAtlas(l_strAtlasFileName.c_str());
	if (l_pAtlas)
	{
		auto l_KeyAndSkeletonDataMapIT = m_KeyAndSkeletonDataMap.find(e_strFileName);
		auto l_KeyAndAnimationStateDataMapIT = m_KeyAndAnimationStateDataMap.find(e_strFileName);
		if (l_KeyAndSkeletonDataMapIT == m_KeyAndSkeletonDataMap.end())
		{
			bool l_bBinary = !UT::GetFileExtensionName(e_strFileName).compare("json") ? false : true;
			spine::SkeletonData*l_pSkeletonData = nullptr;
			if(l_bBinary)
			{

				spine::SkeletonBinary binary(l_pAtlas);
				l_pSkeletonData = binary.readSkeletonDataFile(e_strFileName);
			}
			else
			{
				spine::SkeletonJson json(l_pAtlas);
				l_pSkeletonData = json.readSkeletonDataFile(e_strFileName);
			}
			m_KeyAndSkeletonDataMap[e_strFileName] = l_pSkeletonData;
			l_KeyAndSkeletonDataMapIT = m_KeyAndSkeletonDataMap.find(e_strFileName);
		}
		if (l_KeyAndAnimationStateDataMapIT == m_KeyAndAnimationStateDataMap.end())
		{
			auto l_pStateData = new (__FILE__, __LINE__) spine::AnimationStateData(l_KeyAndSkeletonDataMapIT->second);
			m_KeyAndAnimationStateDataMap[e_strFileName] = l_pStateData;
			l_KeyAndAnimationStateDataMapIT = m_KeyAndAnimationStateDataMap.find(e_strFileName);
		}
		l_pSkeleton = new (__FILE__, __LINE__) spine::Skeleton(l_KeyAndSkeletonDataMapIT->second);
		l_pAnimationState = new (__FILE__, __LINE__) spine::AnimationState(l_KeyAndAnimationStateDataMapIT->second);
	}
	return std::make_tuple(l_pSkeleton, l_pAnimationState, l_pAtlas);
}

spine::SkeletonData* cSpineCache::GetSkeletonData(const char* e_strFileName, const char* e_strAtlasFileName)
{
	std::string l_strAtlasFileName;
	if (e_strAtlasFileName)
	{
		l_strAtlasFileName = e_strAtlasFileName;
	}
	else
	{
		l_strAtlasFileName = UT::ChangeFileExtensionName(e_strFileName, "atlas");
	}
	auto l_pAtlas = GetAtlas(l_strAtlasFileName.c_str());
	if (l_pAtlas)
	{
		FMLOG("get atlas %s ok", l_strAtlasFileName.c_str());
		auto l_KeyAndSkeletonDataMapIT = m_KeyAndSkeletonDataMap.find(e_strFileName);
		if (l_KeyAndSkeletonDataMapIT == m_KeyAndSkeletonDataMap.end())
		{
			bool l_bBinary = !UT::GetFileExtensionName(e_strFileName).compare("json") ? false : true;
			spine::SkeletonData* l_pSkeletonData = nullptr;
			if (l_bBinary)
			{
				spine::SkeletonBinary binary(l_pAtlas);
				l_pSkeletonData = binary.readSkeletonDataFile(e_strFileName);
			}
			else
			{
				spine::SkeletonJson json(l_pAtlas);
				l_pSkeletonData = json.readSkeletonDataFile(e_strFileName);
				if (json.getError().length())
				{
					UT::ErrorMsg(json.getError().buffer(), "Error!");
				}
			}
			if (l_pSkeletonData)
			{
				FMLOG("get skeleton data %s ok", e_strFileName);
			}
			else
			{
				FMLOG("get skeleton data %s failed", e_strFileName);
			}
			m_KeyAndSkeletonDataMap[e_strFileName] = l_pSkeletonData;
			l_KeyAndSkeletonDataMapIT = m_KeyAndSkeletonDataMap.find(e_strFileName);
		}
		return l_KeyAndSkeletonDataMapIT->second;
	}
	FMLOG("get atlas %s failed", l_strAtlasFileName.c_str());
	return nullptr;
}

spine::Skeleton* cSpineCache::GetSkeleton(const char* e_strFileName)
{
	spine::Skeleton* l_pSkeleton = nullptr;
	std::string l_strAtlasFileName = UT::ChangeFileExtensionName(e_strFileName, "atlas");
	auto l_pAtlas = GetAtlas(l_strAtlasFileName.c_str());
	if (l_pAtlas)
	{
		auto l_KeyAndSkeletonDataMapIT = m_KeyAndSkeletonDataMap.find(e_strFileName);
		if (l_KeyAndSkeletonDataMapIT == m_KeyAndSkeletonDataMap.end())
		{
			bool l_bBinary = !UT::GetFileExtensionName(e_strFileName).compare("json") ? false : true;
			spine::SkeletonData* l_pSkeletonData = nullptr;
			if (l_bBinary)
			{
				spine::SkeletonBinary binary(l_pAtlas);
				l_pSkeletonData = binary.readSkeletonDataFile(e_strFileName);
			}
			else
			{
				spine::SkeletonJson json(l_pAtlas);
				l_pSkeletonData = json.readSkeletonDataFile(e_strFileName);
			}
			m_KeyAndSkeletonDataMap[e_strFileName] = l_pSkeletonData;
			l_KeyAndSkeletonDataMapIT = m_KeyAndSkeletonDataMap.find(e_strFileName);
		}
		l_pSkeleton = new (__FILE__, __LINE__) spine::Skeleton(l_KeyAndSkeletonDataMapIT->second);
	}
	return l_pSkeleton;
}

spine::AnimationState* cSpineCache::GetAnimationState(const char* e_strFileName)
{
	spine::AnimationState* l_pAnimationState = nullptr;
	std::string l_strAtlasFileName = UT::ChangeFileExtensionName(e_strFileName, "atlas");
	auto l_pAtlas = GetAtlas(l_strAtlasFileName.c_str());
	if (l_pAtlas)
	{
		auto l_KeyAndSkeletonDataMapIT = m_KeyAndSkeletonDataMap.find(e_strFileName);
		auto l_KeyAndAnimationStateDataMapIT = m_KeyAndAnimationStateDataMap.find(e_strFileName);
		if (l_KeyAndAnimationStateDataMapIT == m_KeyAndAnimationStateDataMap.end())
		{
			auto l_pStateData = new (__FILE__, __LINE__) spine::AnimationStateData(l_KeyAndSkeletonDataMapIT->second);
			m_KeyAndAnimationStateDataMap[e_strFileName] = l_pStateData;
			l_KeyAndAnimationStateDataMapIT = m_KeyAndAnimationStateDataMap.find(e_strFileName);
		}
		l_pAnimationState = new (__FILE__, __LINE__) spine::AnimationState(l_KeyAndAnimationStateDataMapIT->second);
	}
	return l_pAnimationState;
}


cSpineCache* g_pSpineCache = nullptr;
//spine-runtimes-4.0\spine-cocos2dx\src\spine\spine-cocos2dx.cpp

//http://en.esotericsoftware.com/spine-applying-animations#Applying-Animations

void	SpineInit()
{
	if (!g_pSpineCache)
	{
		g_pSpineCache = new cSpineCache();
	}
}
SkeletonAnimation* g_pSkeletonAnimation = nullptr;
std::vector<std::string>*	g_pAllSpineFileNameVector = nullptr;
//testData.add(TestData("spine/goblins/goblins/goblins-pro.json", "spine/goblins/goblins/goblins-pro.skel",
//	"spine/goblins/goblins/goblins.atlas"));
int g_iCurrentSpineAnimationIndex = 0;
int g_iCurrentSpineSkinIndex = 0;
int g_iCurrentSpineFileIndex = 0;
std::wstring* g_pstrCurrentSpineInfo = nullptr;
void	DoSpineKeyUp(unsigned char e_ucKeyUp);
void ComposeInfo()
{
	if (g_pSkeletonAnimation)
	{
		auto l_AnimationVector = g_pSkeletonAnimation->getSkeleton()->getData()->getAnimations();
		auto l_Skins = g_pSkeletonAnimation->getSkeleton()->getData()->getSkins();
		auto l_strCurrentFileName = (*g_pAllSpineFileNameVector)[g_iCurrentSpineFileIndex];
		auto l_strSkinName = l_Skins[g_iCurrentSpineSkinIndex]->getName().buffer();
		auto l_strAnimationName = l_AnimationVector[g_iCurrentSpineAnimationIndex]->getName().buffer();
		auto l_strInfo = UT::ComposeMsgByFormat("Q,E change file,A,D change animation,W,S change skin\nFileName:%s(%d/%d)\nSkin:%s(%d/%d)\nAnimation:%s(%d/%d)",
			//UT::GetFileNameWithoutFullPath(l_strCurrentFileName).c_str(), g_iCurrentSpineFileIndex + 1, g_AllSpineFileNameVector.size(),
			l_strCurrentFileName.c_str(), g_iCurrentSpineFileIndex + 1, g_pAllSpineFileNameVector->size(),
			l_strSkinName, g_iCurrentSpineSkinIndex + 1, l_Skins.size(),
			l_strAnimationName, g_iCurrentSpineAnimationIndex + 1, l_AnimationVector.size());
		*g_pstrCurrentSpineInfo = ValueToStringW(l_strInfo);
	}
	else
	{
		*g_pstrCurrentSpineInfo = L"open spine/SpineTestDirectory.txt\n add diectory to this file.";
	}
}
void testLoading()
{
	//auto l_AllFiles = directory_iterator("spine");
	auto l_strTargetFile = "Spine/SpinePreviewer.txt";
	if (UT::IsFileExists(l_strTargetFile))
	{
		if (!g_pAllSpineFileNameVector)
		{
			g_pAllSpineFileNameVector = new std::vector<std::string>();
		}
		if (!g_pstrCurrentSpineInfo)
		{
			g_pstrCurrentSpineInfo = new std::wstring();
		}
		std::string	l_strTestDirectory = GetTxtFileContent(l_strTargetFile);
		try
		{
			auto l_AllFiles = std::filesystem::recursive_directory_iterator(l_strTestDirectory.c_str());
			for (const auto& file : l_AllFiles)
			{
				auto l_strPath = file.path().c_str();

				//CarServer_1__log_17_12_2021_10_57_56.txt
				auto l_strFileName = UT::GetFileExtensionName(l_strPath);
				//win32 use L?
				//if (l_strFileName.compare(L"json") == 0 || l_strFileName.compare(L"skel") == 0)
				if (l_strFileName.compare("json") == 0 || l_strFileName.compare("skel") == 0)
				{
					g_pAllSpineFileNameVector->push_back(ValueToString(l_strPath));
				}
			}
		}
		catch (std::exception e)
		{
			l_strTestDirectory = e.what() + l_strTestDirectory;
			UT::ErrorMsg(+ l_strTestDirectory.c_str(), "Error");
			
		}
	}
	if (g_pAllSpineFileNameVector->size())
	{
		g_iCurrentSpineFileIndex = 1;
		DoSpineKeyUp('Q');
	}
	else
	{
		//spine::SkeletonData* l_pTestSkeleton = nullptr;
		////l_pTestSkeleton = g_pSpineCache->GetSkeletonData("spine/dragon/dragon-ess.skel");
		////l_pTestSkeleton = g_pSpineCache->GetSkeletonData("spine/Anim_Main_Rear.json");
		//l_pTestSkeleton = g_pSpineCache->GetSkeletonData("spine/goblins/goblins-ess.skel","spine/goblins/goblins.atlas");
		////l_pTestSkeleton = g_pSpineCache->GetSkeletonData("spine/10027/Anim_Main.json");
		////auto l_pTestSkeleton = g_pSpineCache->GetSkeletonData("spine/10027/Anim_Jackpot_gold_ball_loop.json");


		////getError();
		////g_pSkeletonRenderer = SkeletonRenderer::createWithSkeleton(l_pTestSkeleton,true,true);
		//if (l_pTestSkeleton)
		//{
		//	g_pSkeletonAnimation = SkeletonAnimation::createWithData(l_pTestSkeleton, false);
		//	auto l_AnimationVector = g_pSkeletonAnimation->getSkeleton()->getData()->getAnimations();
		//	if (l_AnimationVector.size())
		//	{
		//		auto l_strName = l_AnimationVector[0]->getName().buffer();
		//		g_pSkeletonAnimation->setAnimation(0, l_strName, true);
		//		auto l_Skins = g_pSkeletonAnimation->getSkeleton()->getData()->getSkins();
		//		if (l_Skins.size())
		//		{
		//			g_pSkeletonAnimation->setSkin(l_Skins[l_Skins.size() > 1 ? 1 : 0]->getName().buffer());
		//		}
		//	}
		//}
	}
	ComposeInfo();
}

namespace spine
{
	SpineExtension* g_pDefaultExtension = nullptr;
	SpineExtension* getDefaultExtension()
	{
		if (!g_pDefaultExtension)
		{
			g_pDefaultExtension = new DefaultSpineExtension();
		}
		return g_pDefaultExtension;
	}
}//namespace spine

void	SpineDestory()
{
	SAFE_DELETE(g_pSkeletonAnimation);
	SAFE_DELETE(g_pSpineCache);
	SAFE_DELETE(spine::g_pDefaultExtension);
	SAFE_DELETE(g_pAllSpineFileNameVector);
	SAFE_DELETE(g_pstrCurrentSpineInfo);
}

void	DoSpineTest()
{
	testLoading();
}


void	DoSpineKeyUp(unsigned char e_ucKeyUp)
{
	int l_iNumFile = g_pAllSpineFileNameVector->size();
	if (l_iNumFile)
	//if (1)
	{
		int l_iPrevious = g_iCurrentSpineFileIndex;
		if (e_ucKeyUp == 'Q')
		{
			g_iCurrentSpineFileIndex -= 1;
			if (g_iCurrentSpineFileIndex < 0)
			{
				g_iCurrentSpineFileIndex = l_iNumFile - 1;
			}
		}
		else
		if (e_ucKeyUp == 'E')
		{
			g_iCurrentSpineFileIndex += 1;
			if (g_iCurrentSpineFileIndex >= l_iNumFile)
			{
				g_iCurrentSpineFileIndex = 0;
			}
		}
		if (g_iCurrentSpineFileIndex == 10)
		{
			int a = 0;
		}
		if (l_iPrevious != g_iCurrentSpineFileIndex)
		{
			SAFE_DELETE(g_pSkeletonAnimation);
			auto l_pTestSkeleton = g_pSpineCache->GetSkeletonData((*g_pAllSpineFileNameVector)[g_iCurrentSpineFileIndex].c_str());
			if (l_pTestSkeleton)
			{
				g_pSkeletonAnimation = SkeletonAnimation::createWithData(l_pTestSkeleton, false);
			}
			g_iCurrentSpineAnimationIndex = 0;
			g_iCurrentSpineSkinIndex = 0;
		}
	}
	else
	{
		return;
	}
	if (!g_pSkeletonAnimation)
	{
		return;
	}
	auto l_AnimationVector = g_pSkeletonAnimation->getSkeleton()->getData()->getAnimations();
	auto l_Skins = g_pSkeletonAnimation->getSkeleton()->getData()->getSkins();	
	int l_iAnimationCount = (int)l_AnimationVector.size();
	int l_iSkinCount = (int)l_Skins.size();
	if(l_iSkinCount)
	{
		if (e_ucKeyUp == 'W')
		{
			g_iCurrentSpineSkinIndex -= 1;
			if (g_iCurrentSpineSkinIndex < 0)
			{
				g_iCurrentSpineSkinIndex = l_iSkinCount - 1;
			}
		}
		else
		if (e_ucKeyUp == 'S')
		{
			g_iCurrentSpineSkinIndex += 1;
			if (g_iCurrentSpineSkinIndex >= l_iSkinCount)
			{
				g_iCurrentSpineSkinIndex = 0;
			}
		}
		g_pSkeletonAnimation->setSkin(l_Skins[g_iCurrentSpineSkinIndex]->getName().buffer());
	}
	if(l_iAnimationCount)
	{
		if (e_ucKeyUp == 'A')
		{
			g_iCurrentSpineAnimationIndex -= 1;
			if (g_iCurrentSpineAnimationIndex < 0)
			{
				g_iCurrentSpineAnimationIndex = l_iAnimationCount - 1;
			}
		}
		else
		if (e_ucKeyUp == 'D')
		{
			g_iCurrentSpineAnimationIndex += 1;
			if (g_iCurrentSpineAnimationIndex >= l_iAnimationCount)
			{
				g_iCurrentSpineAnimationIndex = 0;
			}
		}
		auto l_strName = l_AnimationVector[g_iCurrentSpineAnimationIndex]->getName().buffer();
		g_pSkeletonAnimation->setAnimation(0, l_strName, true);
	}
	auto l_strCurrentFileName = (*g_pAllSpineFileNameVector)[g_iCurrentSpineFileIndex];
	auto l_strSkinName = l_Skins[g_iCurrentSpineSkinIndex]->getName().buffer();
	auto l_strAnimationName = l_AnimationVector[g_iCurrentSpineAnimationIndex]->getName().buffer();
	auto l_strInfo = UT::ComposeMsgByFormat("Q,E change file,A,D change animation,W,S change skin\nFileName:%s(%d/%d)\nSkin:%s(%d/%d)\nAnimation:%s(%d/%d)", 
		//UT::GetFileNameWithoutFullPath(l_strCurrentFileName).c_str(), g_iCurrentSpineFileIndex+1, g_AllSpineFileNameVector.size(),
		l_strCurrentFileName.c_str(), g_iCurrentSpineFileIndex + 1, g_pAllSpineFileNameVector->size(),
		l_strSkinName,g_iCurrentSpineSkinIndex+1, l_Skins.size(),
		l_strAnimationName,g_iCurrentSpineAnimationIndex+1,l_AnimationVector.size());
	*g_pstrCurrentSpineInfo = ValueToStringW(l_strInfo);
}

void	DoSpineRender()
{
	if (g_pSkeletonAnimation)
	{
		//g_pSkeletonAnimation->SetWorldPosition(Vector3(1920, 1080, 0));
		//g_pSkeletonAnimation->SetWorldPosition(Vector3(500, 500, 0));
		//g_pSkeletonAnimation->SetWorldTransform(cMatrix44::TranslationMatrix(Vector3(1920,750, 0))*cMatrix44::ZAxisRotationMatrix(D3DX_PI) * cMatrix44::ScaleMatrix(Vector3(2, 2, 2)));
		g_pSkeletonAnimation->SetWorldTransform(cMatrix44::TranslationMatrix(Vector3(1320, 750, 0)) * cMatrix44::ZAxisRotationMatrix(D3DX_PI)* cMatrix44::YAxisRotationMatrix(D3DX_PI) * cMatrix44::ScaleMatrix(Vector3(1,1,1)));// 
		//g_pSkeletonAnimation->SetWorldTransform(cMatrix44::TranslationMatrix(Vector3(900, 750, 0)) * cMatrix44::ZAxisRotationMatrix(D3DX_PI) * cMatrix44::ScaleMatrix(Vector3(2, 2, 2)));
		g_pSkeletonAnimation->Update(0.016f);
		g_pSkeletonAnimation->Render();
		//g_pSkeletonAnimation->DebugRender();
	}
	if (cGameApp::m_spGlyphFontRender)
	{
		cGameApp::m_spGlyphFontRender->SetScale(1.5);
		cGameApp::RenderFont(320, 0, ValueToStringW(*g_pstrCurrentSpineInfo));
		cGameApp::m_spGlyphFontRender->SetScale(1);
	}
}
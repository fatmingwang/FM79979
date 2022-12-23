#include <algorithm>
#include <limits>
#include "SkeletonRenderer.h"
#include "SpineExtension.h"
#include "SpineLoader.h"

//reference
//https://github.com/EsotericSoftware/spine-runtimes/blob/3.8/spine-sfml/cpp/src/spine/spine-sfml.cpp#L89
// 
//
//
sTrianglesAndIndicesForSpine::sTrianglesAndIndicesForSpine()
{
	Resize(100);
	//pTexture = e_pTexture;
	//if (pTexture)
	//{
	//	pTexture->AddRef(this);
	//}
}

sTrianglesAndIndicesForSpine::~sTrianglesAndIndicesForSpine()
{
}

//void sTrianglesAndIndicesForSpine::AppendVertex(int e_iIndex, unsigned short e_usDrawIndexValue, Vector2 e_vPos, Vector2 e_vUV, Vector4 e_vColor,Vector4* e_pvTintColor)
//{
//	vPosVector[e_iIndex] = e_vPos;
//	vColorVector[e_iIndex] = e_vUV;	
//	vUVVector[e_iIndex] = e_vColor;
//	if (e_pvTintColor)
//	{
//		vTintColorVector[e_iIndex] = *e_pvTintColor;
//	}
//	ucIndicexVector[e_iIndex] = e_usDrawIndexValue;
//}

void	sTrianglesAndIndicesForSpine::AppendVertex(Vector2 e_vPos,Vector2 e_vUV, Vector4 e_vColor)
{
	vPosVector[iCurrentIndex] = e_vPos;
	vColorVector[iCurrentIndex] = e_vColor;
	vUVVector[iCurrentIndex] = e_vUV;
	++iCurrentIndex;
	if (iCurrentIndex + 1 >= vPosVector.size())
	{
		Resize((iCurrentIndex+1) * 2);
	}
}

void sTrianglesAndIndicesForSpine::Resize(int e_iCount)
{
	if (e_iCount != (int)vPosVector.size())
	{
		vPosVector.resize(e_iCount);
		vColorVector.resize(e_iCount);
		vUVVector.resize(e_iCount);
		vTintColorVector.resize(e_iCount);
	}
}
void sTrianglesAndIndicesForSpine::Reset()
{
	iCurrentIndex = 0;
}
void sTrianglesAndIndicesForSpine::Render(cMatrix44 e_Mat)
{
	//do Render.
	GLRender::RenderTrianglesWithMatrix((float*)&vPosVector[0],
		(float*)&vUVVector[0], (float*)&vColorVector[0],
		e_Mat, 2, iCurrentIndex / 3);
}
namespace 
{

	int computeTotalCoordCount(Skeleton &skeleton, int startSlotIndex, int endSlotIndex);
	Vector4 computeBoundingRect(const float *coords, int vertexCount);
	//void interleaveCoordinates(float *dst, const float *src, int vertexCount, int dstStride);
	sBlendfunction makeBlendFunc(BlendMode blendMode, bool premultipliedAlpha);
	void transformWorldVertices(float *dstCoord, int coordCount, Skeleton &skeleton, int startSlotIndex, int endSlotIndex);
	bool slotIsOutRange(Slot &slot, int startSlotIndex, int endSlotIndex);
	bool nothingToDraw(Slot &slot, int startSlotIndex, int endSlotIndex);
}// namespace
// C Variable length array
#ifdef _MSC_VER
// VLA not supported, use _malloca
#define VLA(type, arr, count) \
type *arr = static_cast<type *>(_malloca(sizeof(type) * count))
#define VLA_FREE(arr) \
do { _freea(arr); } while (false)
#else
#define VLA(type, arr, count) \
type arr[count]
#define VLA_FREE(arr)
#endif

SkeletonRenderer *SkeletonRenderer::createWithSkeleton(Skeleton *skeleton, bool ownsSkeleton, bool ownsSkeletonData)
{
	SkeletonRenderer *node = new SkeletonRenderer(skeleton, ownsSkeleton, ownsSkeletonData);
	return node;
}
SkeletonRenderer *SkeletonRenderer::createWithData(SkeletonData *skeletonData, bool ownsSkeletonData)
{
	SkeletonRenderer *node = new SkeletonRenderer(skeletonData, ownsSkeletonData);
	return node;
}
SkeletonRenderer *SkeletonRenderer::createWithFile(const std::string &skeletonDataFile, Atlas *atlas, float scale)
{
	SkeletonRenderer *node = new SkeletonRenderer(skeletonDataFile, atlas, scale);
	return node;
}
SkeletonRenderer *SkeletonRenderer::createWithFile(const std::string &skeletonDataFile, const std::string &atlasFile, float scale)
{
	SkeletonRenderer *node = new SkeletonRenderer(skeletonDataFile, atlasFile, scale);
	return node;
}
void SkeletonRenderer::initialize()
{
	_clipper = new (__FILE__, __LINE__) SkeletonClipping();
	setOpacityModifyRGB(true);

	setTwoColorTint(false);

	_skeleton->setToSetupPose();
	_skeleton->updateWorldTransform();
}
void SkeletonRenderer::setupGLProgramState(bool twoColorTintEnabled)
{
	if (twoColorTintEnabled)
	{
//#if COCOS2D_VERSION < 0x00040000
//			setGLProgramState(SkeletonTwoColorBatch::getInstance()->getTwoColorTintProgramState());
//#endif
		return;
	}

	cTexture*texture = nullptr;
	for (size_t i = 0, n = _skeleton->getSlots().size(); i < n; i++)
	{
		Slot *slot = _skeleton->getDrawOrder()[i];
		Attachment *const attachment = slot->getAttachment();
		if (!attachment)
		{
			continue;
		}
		if (attachment->getRTTI().isExactly(RegionAttachment::rtti)) 
		{
			RegionAttachment *regionAttachment = static_cast<RegionAttachment *>(attachment);
			//texture = static_cast<l_pRenderObjectOfSpineAttachment *>(regionAttachment->getRendererObject())->_texture;
		}
		else 
		if (attachment->getRTTI().isExactly(MeshAttachment::rtti)) 
		{
			MeshAttachment *meshAttachment = static_cast<MeshAttachment *>(attachment);
			//texture = static_cast<l_pRenderObjectOfSpineAttachment *>(meshAttachment->getRendererObject())->_texture;
		}
		else
		{
			continue;
		}
		if (texture != nullptr)
		{
			break;
		}
	}

	//setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP, texture));
}

void SkeletonRenderer::setSkeletonData(SkeletonData *skeletonData, bool ownsSkeletonData)
{
	_skeleton = new (__FILE__, __LINE__) Skeleton(skeletonData);
	_ownsSkeletonData = ownsSkeletonData;
}
SkeletonRenderer::SkeletonRenderer()//_attachmentLoader(nullptr),_atlas(nullptr),
	: _timeScale(1), _effect(nullptr), _debugSlots(false), _debugBones(false), _debugMeshes(false), _debugBoundingRect(false), _startSlotIndex(0), _endSlotIndex(INT_MAX)
{
}
SkeletonRenderer::SkeletonRenderer(Skeleton *skeleton, bool ownsSkeleton, bool ownsSkeletonData, bool ownsAtlas)//_attachmentLoader(nullptr),_atlas(nullptr),  , _effect(nullptr)
	: _timeScale(1), _effect(nullptr), _debugSlots(false), _debugBones(false), _debugMeshes(false), _debugBoundingRect(false), _startSlotIndex(0), _endSlotIndex(INT_MAX)
{
	initWithSkeleton(skeleton, ownsSkeleton, ownsSkeletonData, ownsAtlas);
}
SkeletonRenderer::SkeletonRenderer(SkeletonData *skeletonData, bool ownsSkeletonData)//_attachmentLoader(nullptr),_atlas(nullptr),  , _effect(nullptr)
	: _timeScale(1), _effect(nullptr), _debugSlots(false), _debugBones(false), _debugMeshes(false), _debugBoundingRect(false), _startSlotIndex(0), _endSlotIndex(INT_MAX)
{
	initWithData(skeletonData, ownsSkeletonData);
}
SkeletonRenderer::SkeletonRenderer(const std::string &skeletonDataFile, Atlas *atlas, float scale)//_attachmentLoader(nullptr),_atlas(nullptr),  , _effect(nullptr)
	: _timeScale(1), _effect(nullptr), _debugSlots(false), _debugBones(false), _debugMeshes(false), _debugBoundingRect(false), _startSlotIndex(0), _endSlotIndex(INT_MAX)
{
	initWithJsonFile(skeletonDataFile, atlas, scale);
}
SkeletonRenderer::SkeletonRenderer(const std::string &skeletonDataFile, const std::string &atlasFile, float scale)//_attachmentLoader(nullptr),_atlas(nullptr),  , _effect(nullptr)
	: _timeScale(1), _effect(nullptr), _debugSlots(false), _debugBones(false), _debugMeshes(false), _debugBoundingRect(false), _startSlotIndex(0), _endSlotIndex(INT_MAX)
{
	initWithJsonFile(skeletonDataFile, atlasFile, scale);
}
SkeletonRenderer::~SkeletonRenderer()
{
	if (_ownsSkeletonData) delete _skeleton->getData();
	if (_ownsSkeleton) delete _skeleton;
	//if (_ownsAtlas && _atlas) delete _atlas;
	//if (_attachmentLoader) delete _attachmentLoader;
	delete _clipper;
}
void SkeletonRenderer::initWithSkeleton(Skeleton *skeleton, bool ownsSkeleton, bool ownsSkeletonData, bool ownsAtlas)
{
	_skeleton = skeleton;
	_ownsSkeleton = ownsSkeleton;
	_ownsSkeletonData = ownsSkeletonData;
	//_ownsAtlas = ownsAtlas;
	initialize();
}
void SkeletonRenderer::initWithData(SkeletonData *skeletonData, bool ownsSkeletonData) 
{
	_ownsSkeleton = true;
	setSkeletonData(skeletonData, ownsSkeletonData);
	initialize();
}
void SkeletonRenderer::initWithJsonFile(const std::string &skeletonDataFile, Atlas *atlas, float scale)
{
	//_atlas = atlas;
	//_attachmentLoader = new (__FILE__, __LINE__) Cocos2dAtlasAttachmentLoader(_atlas);

	//SkeletonJson json(_attachmentLoader);
	//json.setScale(scale);
	//SkeletonData *skeletonData = json.readSkeletonDataFile(skeletonDataFile.c_str());
	//CCASSERT(skeletonData, (!json.getError().isEmpty() ? json.getError().buffer() : "Error reading skeleton data."));
	auto l_pskeletonData = g_pSpineCache->GetSkeletonData(skeletonDataFile.c_str());
	_ownsSkeleton = true;
	setSkeletonData(l_pskeletonData, true);

	initialize();
}
void SkeletonRenderer::initWithJsonFile(const std::string &skeletonDataFile, const std::string &atlasFile, float scale)
{
	//_atlas = new (__FILE__, __LINE__) Atlas(atlasFile.c_str(), &textureLoader, true);
	//CCASSERT(_atlas, "Error reading atlas file.");

	//_attachmentLoader = new (__FILE__, __LINE__) Cocos2dAtlasAttachmentLoader(_atlas);

	//SkeletonJson json(_attachmentLoader);
	//json.setScale(scale);
	//SkeletonData *skeletonData = json.readSkeletonDataFile(skeletonDataFile.c_str());
	//CCASSERT(skeletonData, (!json.getError().isEmpty() ? json.getError().buffer() : "Error reading skeleton data."));
	//_ownsAtlas = true;
	auto l_pskeletonData = g_pSpineCache->GetSkeletonData(skeletonDataFile.c_str());
	_ownsSkeleton = true;
	setSkeletonData(l_pskeletonData, true);

	initialize();
}
void SkeletonRenderer::initWithBinaryFile(const std::string &skeletonDataFile, Atlas *atlas, float scale)
{
	//_atlas = atlas;
	//_attachmentLoader = new (__FILE__, __LINE__) Cocos2dAtlasAttachmentLoader(_atlas);

	//SkeletonBinary binary(_attachmentLoader);
	//binary.setScale(scale);
	//SkeletonData *skeletonData = binary.readSkeletonDataFile(skeletonDataFile.c_str());
	//CCASSERT(skeletonData, (!binary.getError().isEmpty() ? binary.getError().buffer() : "Error reading skeleton data."));
	//_ownsSkeleton = true;
	//setSkeletonData(skeletonData, true);
	auto l_pskeletonData = g_pSpineCache->GetSkeletonData(skeletonDataFile.c_str());
	_ownsSkeleton = true;
	setSkeletonData(l_pskeletonData, true);

	initialize();
}
void SkeletonRenderer::initWithBinaryFile(const std::string &skeletonDataFile, const std::string &atlasFile, float scale) 
{
	//_atlas = new (__FILE__, __LINE__) Atlas(atlasFile.c_str(), &textureLoader, true);
	//CCASSERT(_atlas, "Error reading atlas file.");

	//_attachmentLoader = new (__FILE__, __LINE__) Cocos2dAtlasAttachmentLoader(_atlas);

	//SkeletonBinary binary(_attachmentLoader);
	//binary.setScale(scale);
	//SkeletonData *skeletonData = binary.readSkeletonDataFile(skeletonDataFile.c_str());
	//CCASSERT(skeletonData, (!binary.getError().isEmpty() ? binary.getError().buffer() : "Error reading skeleton data."));
	//_ownsSkeleton = true;
	//_ownsAtlas = true;
	//setSkeletonData(skeletonData, true);
	auto l_pskeletonData = g_pSpineCache->GetSkeletonData(skeletonDataFile.c_str());
	_ownsSkeleton = true;
	setSkeletonData(l_pskeletonData, true);
	initialize();
}
void SkeletonRenderer::Update(float e_fElpaseTime)
{
	if (_ownsSkeleton)
	{
		_skeleton->update(e_fElpaseTime);
	}
}
void SkeletonRenderer::Render()
{
	// Early exit if the skeleton is invisible.
	if (_skeleton->getColor().a == 0) 
	{
		return;
	}
	sBlendfunction l_OriginalBlendfunction;
	l_OriginalBlendfunction.GetStatus();
	auto l_matWorldTransform = this->GetWorldTransform();
	m_TrianglesAndIndicesForSpine.Reset();
	//only need to do once?
	//{
	//	const int coordCount = computeTotalCoordCount(*_skeleton, _startSlotIndex, _endSlotIndex);
	//	if (coordCount == 0)
	//	{
	//		return;
	//	}
	//	m_TrianglesAndIndicesForSpine.Resize(coordCount);
	//	assert(coordCount % 2 == 0);
	//}
	//transformWorldVertices((float*)&m_TrianglesAndIndicesForSpine.vPosVector[0],
	//	m_TrianglesAndIndicesForSpine.vPosVector.size(), *_skeleton,
	//	_startSlotIndex, _endSlotIndex);
	//const float *worldCoordPtr = (float*)&m_TrianglesAndIndicesForSpine.vPosVector[0];
	const bool hasSingleTint = (isTwoColorTint() == false);
	if (!hasSingleTint)
	{
		int a = 0;
	}
	else
	{
		int a = 0;
	}
	//stride = 2,vertex only inpuy position(Vector2)
	const int l_ciVertexStride = 2;
	Color l_CurrentSlotAttachementColor = {0,0,0,0};

	Vector<float>*			l_pUVVector = nullptr;
	Vector<unsigned short>* l_pIndicesVector = nullptr;
	Vector<float>			l_PosVector;
	Vector<unsigned short>	l_QUADIndicesVector;
	Vector<Color>			l_TempColors;
	//my
	//0    1
	//
	//2    3
	//012,213
	l_QUADIndicesVector.add(0);
	l_QUADIndicesVector.add(1);
	l_QUADIndicesVector.add(2);
	l_QUADIndicesVector.add(2);
	l_QUADIndicesVector.add(3);
	l_QUADIndicesVector.add(0); 
	//spine
	//2   3
	//
	//1   0
	//
	//2   3
	//l_QUADIndicesVector.add(0);
	//l_QUADIndicesVector.add(1);
	//l_QUADIndicesVector.add(2);
	//l_QUADIndicesVector.add(2);
	//l_QUADIndicesVector.add(3);
	//l_QUADIndicesVector.add(0);
	int l_iCurrentSlotAttachementVerticesCount = 0;
	int l_iCurrentSlotAttachementIndicesCount = 0;
	cTexture* l_pTexture = nullptr;
	cTexture* l_pLastTexture = nullptr;
	if (_effect)
	{
		_effect->begin(*_skeleton);
	}
	auto l_uiSize = _skeleton->getSlots().size();
	for (size_t i = 0, n = l_uiSize; i < n; ++i)
	{
		Slot *slot = _skeleton->getDrawOrder()[i];
		if (nothingToDraw(*slot, _startSlotIndex, _endSlotIndex))
		{
			_clipper->clipEnd(*slot);
			continue;
		}

		if (slot->getAttachment()->getRTTI().isExactly(RegionAttachment::rtti))
		{
			RegionAttachment *attachment = static_cast<RegionAttachment *>(slot->getAttachment());
			l_pTexture = (cTexture*)((AtlasRegion*)attachment->getRendererObject())->page->getRendererObject();
			l_PosVector.setSize(8,0);
			attachment->computeWorldVertices(slot->getBone(), l_PosVector, 0, l_ciVertexStride);
			l_pUVVector = &attachment->getUVs();
			l_pIndicesVector = &l_QUADIndicesVector;
			l_CurrentSlotAttachementColor = attachment->getColor();
			l_iCurrentSlotAttachementVerticesCount = 4;
			l_iCurrentSlotAttachementIndicesCount = 6;
#ifdef DEBUG
			Vector2* l_vpPos = (Vector2*)l_PosVector.buffer();
			//Vector2  l_vPos4[4] = { l_vpPos[0],l_vpPos[1], l_vpPos[2], l_vpPos[3] };
			Vector2  l_vPos4[4] = { l_vpPos[3],l_vpPos[2], l_vpPos[1], l_vpPos[0] };
			Vector2* l_vpUV = (Vector2*)l_pUVVector->buffer();
			Vector2  l_vUV4[4] = { l_vpUV[0],l_vpUV[1], l_vpUV[2], l_vpUV[3] };
			//if (l_pTexture)
			//{
			//	l_pTexture->ApplyImage();
			//}
			//GLRender::RenderQuadWithMatrix((float*)l_vPos4, (float*)l_vpUV, Vector4::One, this->GetWorldTransform(), 2, 1);
			float l_fWidth = (l_vPos4[0] - l_vPos4[1]).Length();
			float l_fHeight = (l_vPos4[0] - l_vPos4[2]).Length();
			//GLRender::RenderRectangle(l_vPos4[0]+this->GetWorldTransform().GetTranslation()-Vector2(l_fWidth, l_fHeight), l_fWidth, l_fHeight, Vector4::Red);
			int a = 0;
#endif
		} 
		else
		if (slot->getAttachment()->getRTTI().isExactly(MeshAttachment::rtti))
		{
			MeshAttachment* l_pMeshAttachment = (MeshAttachment*)slot->getAttachment();
			l_pTexture = (cTexture*)((AtlasRegion*)l_pMeshAttachment->getRendererObject())->page->getRendererObject();
			l_PosVector.setSize(l_pMeshAttachment->getWorldVerticesLength(), 0);
			l_pMeshAttachment->computeWorldVertices(*slot, 0, l_pMeshAttachment->getWorldVerticesLength(), l_PosVector, 0, l_ciVertexStride);
			l_CurrentSlotAttachementColor = l_pMeshAttachment->getColor();
			l_iCurrentSlotAttachementVerticesCount = (int)l_pMeshAttachment->getWorldVerticesLength()/ l_ciVertexStride;
			l_pUVVector = &l_pMeshAttachment->getUVs();
			l_pIndicesVector = &l_pMeshAttachment->getTriangles();
			l_iCurrentSlotAttachementIndicesCount = (int)l_pMeshAttachment->getTriangles().size();
		}
		else
		if (slot->getAttachment()->getRTTI().isExactly(ClippingAttachment::rtti))
		{
			ClippingAttachment *clip = (ClippingAttachment *) slot->getAttachment();
			_clipper->clipStart(*slot, clip);
			continue;
		}
		else 
		{
			//need this one?
			_clipper->clipEnd(*slot);
			continue;
		}
		if (l_pLastTexture != l_pTexture)
		{
			if (l_pLastTexture)
			{
				l_pLastTexture->ApplyImage();
				_blendFunc.Render();
				m_TrianglesAndIndicesForSpine.Render(l_matWorldTransform);
				m_TrianglesAndIndicesForSpine.Reset();
			}
			l_pLastTexture = l_pTexture;
		}
		l_CurrentSlotAttachementColor.a *= _skeleton->getColor().a * slot->getColor().a;
		if (l_CurrentSlotAttachementColor.a == 0)
		{
			_clipper->clipEnd(*slot);
			continue;
		}
		l_CurrentSlotAttachementColor.r *= _skeleton->getColor().r * slot->getColor().r;
		l_CurrentSlotAttachementColor.g *= _skeleton->getColor().g * slot->getColor().g;
		l_CurrentSlotAttachementColor.b *= _skeleton->getColor().b * slot->getColor().b;
		if (_premultipliedAlpha)
		{
			l_CurrentSlotAttachementColor.r *= l_CurrentSlotAttachementColor.a;
			l_CurrentSlotAttachementColor.g *= l_CurrentSlotAttachementColor.a;
			l_CurrentSlotAttachementColor.b *= l_CurrentSlotAttachementColor.a;
		}
		//const sBlendfunction blendFunc = makeBlendFunc(slot->getData().getBlendMode(), l_pRenderObjectOfSpineAttachment->_texture->hasPremultipliedAlpha());
		sBlendfunction l_OldBlendFunc = _blendFunc;
		_blendFunc = makeBlendFunc(slot->getData().getBlendMode(), false);
		if (i != 0)
		{
			if(l_OldBlendFunc.eSrcBlendingMode != _blendFunc.eSrcBlendingMode ||
				l_OldBlendFunc.eDestBlendingMode != _blendFunc.eDestBlendingMode)
			{
				if (l_pTexture)
				{
					l_pTexture->ApplyImage();
				}
				_blendFunc.Render();
				m_TrianglesAndIndicesForSpine.Render(l_matWorldTransform);
				m_TrianglesAndIndicesForSpine.Reset();
			}
		}
		if (_clipper->isClipping())
		{
			_clipper->clipTriangles(l_PosVector, *l_pIndicesVector, *l_pUVVector, l_ciVertexStride);
			if (_clipper->getClippedTriangles().size() == 0)
			{
				_clipper->clipEnd(*slot);
				continue;
			}
			l_PosVector.clear();
			l_PosVector.addAll(_clipper->getClippedVertices());

			l_pUVVector = &_clipper->getClippedUVs();
			l_pIndicesVector = &_clipper->getClippedTriangles();
			l_iCurrentSlotAttachementIndicesCount = (int)_clipper->getClippedTriangles().size();
			l_iCurrentSlotAttachementVerticesCount = (int)_clipper->getClippedVertices().size() / l_ciVertexStride;
		}
		if (_effect)
		{
			l_TempColors.clear();
			Color l_darkColorTemp;
			for (int ii = 0; ii < l_iCurrentSlotAttachementVerticesCount; ii++)
			{
				Color vertexColor = l_CurrentSlotAttachementColor;
				int index = ii << 1;
				float x = l_PosVector[index];
				float y = l_PosVector[index + 1];
				float u = (*l_pUVVector)[index];
				float v = (*l_pUVVector)[index + 1];
				_effect->transform(x, y, u, v, vertexColor, l_darkColorTemp);
				l_PosVector[index] = x;
				l_PosVector[index + 1] = y;
				l_TempColors.add(vertexColor);
			}

			for (int ii = 0; ii < l_iCurrentSlotAttachementIndicesCount; ++ii)
			{
				int index = (*l_pIndicesVector)[ii] << 1;
				Vector2 l_vPos(l_PosVector[index], l_PosVector[index + 1]);
				Vector2 l_vUV((*l_pUVVector)[index], (*l_pUVVector)[index + 1]);
				auto l_vSpineColor = l_TempColors[index >> 1];
				Vector4 l_vColor(l_vSpineColor.r, l_vSpineColor.g, l_vSpineColor.b, l_vSpineColor.a);
				m_TrianglesAndIndicesForSpine.AppendVertex(l_vPos, l_vUV, l_vColor);
			}
		}
		else
		{
			for (int ii = 0; ii < l_iCurrentSlotAttachementIndicesCount; ++ii)
			{
				int index = (*l_pIndicesVector)[ii] * l_ciVertexStride;
				Vector2 l_vPos(l_PosVector[index], l_PosVector[index + 1]);
				Vector2 l_vUV((*l_pUVVector)[index], (*l_pUVVector)[index + 1]);
				Vector4 l_vColor(l_CurrentSlotAttachementColor.r,
					l_CurrentSlotAttachementColor.g,
					l_CurrentSlotAttachementColor.b,
					l_CurrentSlotAttachementColor.a);
				m_TrianglesAndIndicesForSpine.AppendVertex(l_vPos, l_vUV, l_vColor);
			}
		}
		_clipper->clipEnd(*slot);
	}
	_clipper->clipEnd();
	if (_effect)
	{
		_effect->end();
	}
	_blendFunc.Render();
	if (l_pTexture)
	{
		l_pTexture->ApplyImage();
	}
	m_TrianglesAndIndicesForSpine.Render(this->GetWorldTransform());
	l_OriginalBlendfunction.Restore();



}

void SkeletonRenderer::DebugRender()
{
	auto l_mWorldTransform = this->GetWorldTransform();
	// Draw bounding rectangle
	if (_debugBoundingRect) 
	{
		//glLineWidth(2);
		//auto brect = getBoundingBox();
		//const Vec2 points[4] =
		//		{
		//				brect.origin,
		//				{brect.origin.x + brect.size.width, brect.origin.y},
		//				{brect.origin.x + brect.size.width, brect.origin.y + brect.size.height},
		//				{brect.origin.x, brect.origin.y + brect.size.height}};
		//drawNode->drawPoly(points, 4, true, Color4F::GREEN);
	}

	//if (_debugSlots)
	{
		// Slots.
		// DrawPrimitives::setDrawColor4B(0, 0, 255, 255);
		glLineWidth(2);
		for (size_t i = 0, n = _skeleton->getSlots().size(); i < n; i++)
		{
			Slot *slot = _skeleton->getDrawOrder()[i];

			if (!slot->getBone().isActive())
			{
				continue;
			}
			if (!slot->getAttachment() || !slot->getAttachment()->getRTTI().isExactly(RegionAttachment::rtti))
			{
				continue;
			}

			if (slotIsOutRange(*slot, _startSlotIndex, _endSlotIndex))
			{
				continue;
			}

			RegionAttachment *attachment = (RegionAttachment *) slot->getAttachment();
			float worldVertices[8];
			attachment->computeWorldVertices(slot->getBone(), worldVertices, 0, 2);
			const Vector2 l_Points[5] =
					{
							{worldVertices[0], worldVertices[1]},
							{worldVertices[2], worldVertices[3]},
							{worldVertices[4], worldVertices[5]},
							{worldVertices[6], worldVertices[7]},
							{worldVertices[0], worldVertices[1]} };
			GLRender::RenderLine((float*)&l_Points,5,Vector4::Green, 2, l_mWorldTransform);
		}
	}

	//if (_debugBones)
	{
		glLineWidth(2);
		for (size_t i= 0, n = _skeleton->getBones().size(); i < n; i++)
		{
			Bone *bone = _skeleton->getBones()[i];
			if (!bone->isActive()) continue;
			float x = bone->getData().getLength() * bone->getA() + bone->getWorldX();
			float y = bone->getData().getLength() * bone->getC() + bone->getWorldY();
			Vector2 l_vPoints[2] = { Vector2(bone->getWorldX(), bone->getWorldY()), Vector2(x, y) };
			GLRender::RenderLine((float*)&l_vPoints, 2, Vector4::Green, 2, l_mWorldTransform);
		}
		// Bone origins.
		Vector4 l_vColor = Vector4::One;
		for (size_t i = 0, n = _skeleton->getBones().size(); i < n; i++)
		{
			Bone *bone = _skeleton->getBones()[i];
			if (!bone->isActive())
			{
				continue;
			}
			auto l_vPos = l_mWorldTransform.TransformCoordinate(Vector2(bone->getWorldX(), bone->getWorldY()));
			GLRender::RenderPoint(l_vPos,15, l_vColor);
			if (i == 0)
			{
				l_vColor = Vector4::Green;
			}
		}
	}

	//if (_debugMeshes)
	{
		// Meshes.
		glLineWidth(2);
		for (size_t i = 0, n = _skeleton->getSlots().size(); i < n; ++i)
		{
			Slot *slot = _skeleton->getDrawOrder()[i];
			if (!slot->getBone().isActive()) continue;
			if (!slot->getAttachment() || !slot->getAttachment()->getRTTI().isExactly(MeshAttachment::rtti)) continue;
			MeshAttachment *const mesh = static_cast<MeshAttachment *>(slot->getAttachment());
			VLA(float, worldCoord, mesh->getWorldVerticesLength());
			mesh->computeWorldVertices(*slot, 0, mesh->getWorldVerticesLength(), worldCoord, 0, 2);
			for (size_t t = 0; t < mesh->getTriangles().size(); t += 3) {
				// Fetch triangle indices
				const int idx0 = mesh->getTriangles()[t + 0];
				const int idx1 = mesh->getTriangles()[t + 1];
				const int idx2 = mesh->getTriangles()[t + 2];
				const Vector2 l_vPoints[3] =
				{
								worldCoord + (idx0 * 2),
								worldCoord + (idx1 * 2),
								worldCoord + (idx2 * 2)
				};
				const Vector2 l_vTransformedPoints[3] =
				{
					l_mWorldTransform.TransformCoordinate(l_vPoints[0]),
					l_mWorldTransform.TransformCoordinate(l_vPoints[1]),
					l_mWorldTransform.TransformCoordinate(l_vPoints[2])
				};
				GLRender::RenderLine((float*)&l_vTransformedPoints, 3, Vector4::Yellow, 2);
			}
			VLA_FREE(worldCoord);
		}
	}
}
const	cBound* SkeletonRenderer::GenerateBound()
{
	SAFE_DELETE(m_pLocalBound);
	RECT l_Rect = { 0, 0, 0, 0 };
	this->m_pLocalBound = new cBound();
	const int coordCount = computeTotalCoordCount(*_skeleton, _startSlotIndex, _endSlotIndex);
	if (coordCount == 0)
	{
		//m_pLocalBound->SetRect(l_Rect);
	}
	else
	{
		VLA(float, worldCoords, coordCount);
		transformWorldVertices(worldCoords, coordCount, *_skeleton, _startSlotIndex, _endSlotIndex);
		auto bb = computeBoundingRect(worldCoords, coordCount / 2);
		VLA_FREE(worldCoords);
		l_Rect.left = (int)bb.x;
		l_Rect.top = (int)bb.y;
		l_Rect.right = (int)bb.z;
		l_Rect.bottom = (int)bb.w;
	}
	m_pLocalBound->SetRect(l_Rect);
	return m_pLocalBound;
}
void SkeletonRenderer::updateWorldTransform()
{
	_skeleton->updateWorldTransform();
}
void SkeletonRenderer::setToSetupPose() {
	_skeleton->setToSetupPose();
}
void SkeletonRenderer::setBonesToSetupPose() 
{
	_skeleton->setBonesToSetupPose();
}
void SkeletonRenderer::setSlotsToSetupPose() 
{
	_skeleton->setSlotsToSetupPose();
}
Bone *SkeletonRenderer::findBone(const std::string &boneName) const 
{
	return _skeleton->findBone(boneName.c_str());
}
Slot *SkeletonRenderer::findSlot(const std::string &slotName) const 
{
	return _skeleton->findSlot(slotName.c_str());
}
void SkeletonRenderer::setSkin(const std::string &skinName) 
{
	_skeleton->setSkin(skinName.empty() ? 0 : skinName.c_str());
}
void SkeletonRenderer::setSkin(const char *skinName) 
{
	_skeleton->setSkin(skinName);
}
Attachment *SkeletonRenderer::getAttachment(const std::string &slotName, const std::string &attachmentName) const 
{
	return _skeleton->getAttachment(slotName.c_str(), attachmentName.c_str());
}
bool SkeletonRenderer::setAttachment(const std::string &slotName, const std::string &attachmentName) 
{
	bool result = _skeleton->getAttachment(slotName.c_str(), attachmentName.empty() ? 0 : attachmentName.c_str()) ? true : false;
	_skeleton->setAttachment(slotName.c_str(), attachmentName.empty() ? 0 : attachmentName.c_str());
	return result;
}
bool SkeletonRenderer::setAttachment(const std::string &slotName, const char *attachmentName) 
{
	bool result = _skeleton->getAttachment(slotName.c_str(), attachmentName) ? true : false;
	_skeleton->setAttachment(slotName.c_str(), attachmentName);
	return result;
}
void SkeletonRenderer::setTwoColorTint(bool enabled)
{
	_twoColorTint = enabled;
}
bool SkeletonRenderer::isTwoColorTint()
{
	return _twoColorTint;
}
void SkeletonRenderer::setVertexEffect(VertexEffect *effect)
{
	this->_effect = effect;
}
void SkeletonRenderer::setSlotsRange(int startSlotIndex, int endSlotIndex)
{
	_startSlotIndex = startSlotIndex == -1 ? 0 : startSlotIndex;
	_endSlotIndex = endSlotIndex == -1 ? INT_MAX : endSlotIndex;
}
Skeleton *SkeletonRenderer::getSkeleton() const
{
	return _skeleton;
}
void SkeletonRenderer::setDebugSlotsEnabled(bool enabled)
{
	_debugSlots = enabled;
}
bool SkeletonRenderer::getDebugSlotsEnabled() const
{
	return _debugSlots;
}
void SkeletonRenderer::setDebugBonesEnabled(bool enabled)
{
	_debugBones = enabled;
}
bool SkeletonRenderer::getDebugBonesEnabled() const
{
	return _debugBones;
}
void SkeletonRenderer::setDebugMeshesEnabled(bool enabled)
{
	_debugMeshes = enabled;
}
bool SkeletonRenderer::getDebugMeshesEnabled() const
{
	return _debugMeshes;
}
void SkeletonRenderer::setDebugBoundingRectEnabled(bool enabled)
{
	_debugBoundingRect = enabled;
}
bool SkeletonRenderer::getDebugBoundingRectEnabled() const
{
	return _debugBoundingRect;
}
void SkeletonRenderer::setOpacityModifyRGB(bool value)
{
	_premultipliedAlpha = value;
}
bool SkeletonRenderer::isOpacityModifyRGB() const
{
	return _premultipliedAlpha;
}

namespace 
{
	Vector4 computeBoundingRect(const float *coords, int vertexCount)
	{
		assert(coords);
		assert(vertexCount > 0);

		const float *v = coords;
		float minX = v[0];
		float minY = v[1];
		float maxX = minX;
		float maxY = minY;
		for (int i = 1; i < vertexCount; ++i)
		{
			v += 2;
			float x = v[0];
			float y = v[1];
			minX = min(minX, x);
			minY = min(minY, y);
			maxX = max(maxX, x);
			maxY = max(maxY, y);
		}
		//return Vector4(minX, minY, maxX - minX, maxY - minY);
		return Vector4(minX, minY, maxX, maxY);
	}

	bool slotIsOutRange(Slot &slot, int startSlotIndex, int endSlotIndex)
	{
		const int index = slot.getData().getIndex();
		return startSlotIndex > index || endSlotIndex < index;
	}

	bool nothingToDraw(Slot &slot, int startSlotIndex, int endSlotIndex)
	{
		Attachment *attachment = slot.getAttachment();
		if (!attachment)
		{
			//FMLOG("%s skip",slot.getData().getName().buffer());
			return true;
		}
		if (slotIsOutRange(slot, startSlotIndex, endSlotIndex) || !slot.getBone().isActive())
		{
			return true;
		}
		const auto &attachmentRTTI = attachment->getRTTI();
		if (attachmentRTTI.isExactly(ClippingAttachment::rtti))
		{
			return false;
		}
		if (slot.getColor().a == 0)
		{
			return true;
		}
		if (attachmentRTTI.isExactly(RegionAttachment::rtti))
		{
			if (static_cast<RegionAttachment*>(attachment)->getColor().a == 0)
			{
				return true;
			}
		}
		else
		if (attachmentRTTI.isExactly(MeshAttachment::rtti))
		{
			if (static_cast<MeshAttachment*>(attachment)->getColor().a == 0)
			{
				return true;
			}
		}
		return false;
	}
	int computeTotalCoordCount(Skeleton &skeleton, int startSlotIndex, int endSlotIndex)
	{
		int coordCount = 0;
		for (size_t i = 0; i < skeleton.getSlots().size(); ++i)
		{
			Slot &slot = *skeleton.getSlots()[i];
			if (nothingToDraw(slot, startSlotIndex, endSlotIndex))
			{
				continue;
			}
			Attachment *const attachment = slot.getAttachment();
			if (attachment->getRTTI().isExactly(RegionAttachment::rtti))
			{
				coordCount += 8;
			}
			else
			if (attachment->getRTTI().isExactly(MeshAttachment::rtti))
			{
				MeshAttachment *const mesh = static_cast<MeshAttachment *>(attachment);
				coordCount += (int)mesh->getWorldVerticesLength();
			}
		}
		return coordCount;
	}
	void transformWorldVertices(float *dstCoord, int coordCount, Skeleton &skeleton, int startSlotIndex, int endSlotIndex)
	{
		float *dstPtr = dstCoord;
#ifndef NDEBUG
		float *const dstEnd = dstCoord + coordCount;
#endif
		for (size_t i = 0; i < skeleton.getSlots().size(); ++i)
		{
			/*const*/ Slot &slot = *skeleton.getDrawOrder()[i];// match the draw order of SkeletonRenderer::Draw
			if (nothingToDraw(slot, startSlotIndex, endSlotIndex))
			{
				continue;
			}
			Attachment *const attachment = slot.getAttachment();
			if (attachment->getRTTI().isExactly(RegionAttachment::rtti))
			{
				RegionAttachment *const regionAttachment = static_cast<RegionAttachment *>(attachment);
				assert(dstPtr + 8 <= dstEnd);
				regionAttachment->computeWorldVertices(slot.getBone(), dstPtr, 0, 2);
				dstPtr += 8;
			}
			else
			if (attachment->getRTTI().isExactly(MeshAttachment::rtti))
			{
				MeshAttachment *const mesh = static_cast<MeshAttachment *>(attachment);
				assert(dstPtr + mesh->getWorldVerticesLength() <= dstEnd);
				mesh->computeWorldVertices(slot, 0, mesh->getWorldVerticesLength(), dstPtr, 0, 2);
				dstPtr += mesh->getWorldVerticesLength();
			}
		}
		assert(dstPtr == dstEnd);
	}
	//void interleaveCoordinates(float *dst, const float *src, int count, int dstStride)
	//{
	//	if (dstStride == 2)
	//	{
	//		memcpy(dst, src, sizeof(float) * count * 2);
	//	}
	//	else
	//	{
	//		for (int i = 0; i < count; ++i)
	//		{
	//			dst[0] = src[0];
	//			dst[1] = src[1];
	//			dst += dstStride;
	//			src += 2;
	//		}
	//	}
	//}

	sBlendfunction makeBlendFunc(BlendMode blendMode, bool premultipliedAlpha)
	{
		sBlendfunction l_Blendfunction;
		switch (blendMode) 
		{
			case BlendMode_Additive:
				l_Blendfunction.eSrcBlendingMode = premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
				l_Blendfunction.eDestBlendingMode = GL_ONE;
				break;
			case BlendMode_Multiply:
				l_Blendfunction.eSrcBlendingMode = GL_DST_COLOR;
				l_Blendfunction.eDestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
				break;
			case BlendMode_Screen:
				l_Blendfunction.eSrcBlendingMode = GL_ONE;
				l_Blendfunction.eDestBlendingMode = GL_ONE_MINUS_SRC_COLOR;
				break;
			default:
				l_Blendfunction.eSrcBlendingMode = premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
				l_Blendfunction.eDestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
				break;
		}
		return l_Blendfunction;
	}

}// namespace

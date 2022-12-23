#pragma once

#include <string>
#include <spine/spine.h>
#include "./pch.h"
using namespace spine;

struct sTrianglesAndIndicesForSpine :public NamedTypedObject
{
	std::vector <Vector2>		vPosVector;
	std::vector <Vector4>		vColorVector;
	std::vector <Vector2>		vUVVector;
	//for tint(copy from cocos2dx)?
	std::vector <Vector4>		vTintColorVector;
	//cTexture* pTexture;
	int							iCurrentIndex;
	sTrianglesAndIndicesForSpine();
	~sTrianglesAndIndicesForSpine();
	//void						AppendVertex(int e_iIndex, unsigned short e_usDrawIndexValue, Vector2 e_vPos, Vector2 e_vUV, Vector4 e_vColor,Vector4*e_pvTintColor = nullptr);
	void						AppendVertex(Vector2 e_vPos, Vector2 e_vUV,Vector4 e_vColor);
	void						Resize(int e_iCount);
	void						Reset();
	void						Render(cMatrix44 e_Mat);
};

/* Draws a skeleton. */
class SkeletonRenderer : public cRenderObject
{
	sTrianglesAndIndicesForSpine	m_TrianglesAndIndicesForSpine;
public:
	static SkeletonRenderer *createWithSkeleton(Skeleton *skeleton, bool ownsSkeleton = false, bool ownsSkeletonData = false);
	static SkeletonRenderer *createWithData(SkeletonData *skeletonData, bool ownsSkeletonData = false);
	static SkeletonRenderer *createWithFile(const std::string &skeletonDataFile, Atlas *atlas, float scale = 1);
	static SkeletonRenderer *createWithFile(const std::string &skeletonDataFile, const std::string &atlasFile, float scale = 1);

	virtual void Update(float e_fElpaseTime) override;
	virtual void Render() override;
	virtual void DebugRender()override;
	virtual const	cBound* GenerateBound()override;

	Skeleton *getSkeleton() const;

	/*  */
	void setDebugSlotsEnabled(bool enabled);
	bool getDebugSlotsEnabled() const;

	void setDebugBonesEnabled(bool enabled);
	bool getDebugBonesEnabled() const;

	void setDebugMeshesEnabled(bool enabled);
	bool getDebugMeshesEnabled() const;

	void setDebugBoundingRectEnabled(bool enabled);
	bool getDebugBoundingRectEnabled() const;

	// --- Convenience methods for common Skeleton_* functions.
	void updateWorldTransform();

	void setOpacityModifyRGB(bool value);

	void setToSetupPose();
	void setBonesToSetupPose();
	void setSlotsToSetupPose();

	/* Returns 0 if the bone was not found. */
	Bone *findBone(const std::string &boneName) const;
	/* Returns 0 if the slot was not found. */
	Slot *findSlot(const std::string &slotName) const;

	/* Sets the skin used to look up attachments not found in the SkeletonData defaultSkin. Attachments from the new skin are
		* attached if the corresponding attachment from the old skin was attached.
		* @param skin May be empty string ("") for no skin.*/
	void setSkin(const std::string &skinName);
	/** @param skin May be 0 for no skin.*/
	void setSkin(const char *skinName);

	/* Returns 0 if the slot or attachment was not found. */
	Attachment *getAttachment(const std::string &slotName, const std::string &attachmentName) const;
	/* Returns false if the slot or attachment was not found.
		* @param attachmentName May be empty string ("") for no attachment. */
	bool setAttachment(const std::string &slotName, const std::string &attachmentName);
	/* @param attachmentName May be 0 for no attachment. */
	bool setAttachment(const std::string &slotName, const char *attachmentName);

	/* Enables/disables two color tinting for this instance. May break batching */
	void setTwoColorTint(bool enabled);
	/* Whether two color tinting is enabled */
	bool isTwoColorTint();

	/* Sets the vertex effect to be used, set to 0 to disable vertex effects */
	void setVertexEffect(VertexEffect *effect);

	/* Sets the range of slots that should be rendered. Use -1, -1 to clear the range */
	void setSlotsRange(int startSlotIndex, int endSlotIndex);

	SkeletonRenderer();
	SkeletonRenderer(Skeleton *skeleton, bool ownsSkeleton = false, bool ownsSkeletonData = false, bool ownsAtlas = false);
	SkeletonRenderer(SkeletonData *skeletonData, bool ownsSkeletonData = false);
	SkeletonRenderer(const std::string &skeletonDataFile, Atlas *atlas, float scale = 1);
	SkeletonRenderer(const std::string &skeletonDataFile, const std::string &atlasFile, float scale = 1);

	virtual ~SkeletonRenderer();

	void initWithSkeleton(Skeleton *skeleton, bool ownsSkeleton = false, bool ownsSkeletonData = false, bool ownsAtlas = false);
	void initWithData(SkeletonData *skeletonData, bool ownsSkeletonData = false);
	void initWithJsonFile(const std::string &skeletonDataFile, Atlas *atlas, float scale = 1);
	void initWithJsonFile(const std::string &skeletonDataFile, const std::string &atlasFile, float scale = 1);
	void initWithBinaryFile(const std::string &skeletonDataFile, Atlas *atlas, float scale = 1);
	void initWithBinaryFile(const std::string &skeletonDataFile, const std::string &atlasFile, float scale = 1);

	virtual void initialize();
	bool isOpacityModifyRGB() const;

protected:
	void setSkeletonData(SkeletonData *skeletonData, bool ownsSkeletonData);
	void setupGLProgramState(bool twoColorTintEnabled);

	bool _ownsSkeletonData;
	bool _ownsSkeleton;
	//bool _ownsAtlas = false;
	//Atlas *_atlas;
	//AttachmentLoader *_attachmentLoader;
	bool _premultipliedAlpha;
	Skeleton *_skeleton;
	float _timeScale;
	bool _debugSlots;
	bool _debugBones;
	bool _debugMeshes;
	bool _debugBoundingRect;
	SkeletonClipping *_clipper;
	VertexEffect *_effect;
	sBlendfunction _blendFunc;

	int _startSlotIndex;
	int _endSlotIndex;
	bool _twoColorTint;
};
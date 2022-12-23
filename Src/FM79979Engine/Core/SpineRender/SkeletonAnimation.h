#pragma once

#include "SkeletonRenderer.h"

using namespace spine;

class SkeletonAnimation;

typedef std::function<void(TrackEntry *entry)> StartListener;
typedef std::function<void(TrackEntry *entry)> InterruptListener;
typedef std::function<void(TrackEntry *entry)> EndListener;
typedef std::function<void(TrackEntry *entry)> DisposeListener;
typedef std::function<void(TrackEntry *entry)> CompleteListener;
typedef std::function<void(TrackEntry *entry, Event *event)> EventListener;
typedef std::function<void(SkeletonAnimation *node)> UpdateWorldTransformsListener;

/** Draws an animated skeleton, providing an AnimationState for applying one or more animations and queuing animations to be
* played later. */
class SkeletonAnimation : public SkeletonRenderer
{
public:
	static SkeletonAnimation *createWithData(SkeletonData *skeletonData, bool ownsSkeletonData = false);
	static SkeletonAnimation *createWithJsonFile(const std::string &skeletonJsonFile, Atlas *atlas, float scale = 1);
	static SkeletonAnimation *createWithJsonFile(const std::string &skeletonJsonFile, const std::string &atlasFile, float scale = 1);
	static SkeletonAnimation *createWithBinaryFile(const std::string &skeletonBinaryFile, Atlas *atlas, float scale = 1);
	static SkeletonAnimation *createWithBinaryFile(const std::string &skeletonBinaryFile, const std::string &atlasFile, float scale = 1);


	virtual void Update(float e_fElpaseTime) override;
	virtual void Render() override;

	void setAnimationStateData(AnimationStateData *stateData);
	void setMix(const std::string &fromAnimation, const std::string &toAnimation, float duration);

	TrackEntry *setAnimation(int trackIndex, const std::string &name, bool loop);
	TrackEntry *addAnimation(int trackIndex, const std::string &name, bool loop, float delay = 0);
	TrackEntry *setEmptyAnimation(int trackIndex, float mixDuration);
	void setEmptyAnimations(float mixDuration);
	TrackEntry *addEmptyAnimation(int trackIndex, float mixDuration, float delay = 0);
	Animation *findAnimation(const std::string &name) const;
	TrackEntry *getCurrent(int trackIndex = 0);
	void clearTracks();
	void clearTrack(int trackIndex = 0);

	void setStartListener(const StartListener &listener);
	void setInterruptListener(const InterruptListener &listener);
	void setEndListener(const EndListener &listener);
	void setDisposeListener(const DisposeListener &listener);
	void setCompleteListener(const CompleteListener &listener);
	void setEventListener(const EventListener &listener);
	void setPreUpdateWorldTransformsListener(const UpdateWorldTransformsListener &listener);
	void setPostUpdateWorldTransformsListener(const UpdateWorldTransformsListener &listener);

	void setTrackStartListener(TrackEntry *entry, const StartListener &listener);
	void setTrackInterruptListener(TrackEntry *entry, const InterruptListener &listener);
	void setTrackEndListener(TrackEntry *entry, const EndListener &listener);
	void setTrackDisposeListener(TrackEntry *entry, const DisposeListener &listener);
	void setTrackCompleteListener(TrackEntry *entry, const CompleteListener &listener);
	void setTrackEventListener(TrackEntry *entry, const EventListener &listener);

	virtual void onAnimationStateEvent(TrackEntry *entry, EventType type, Event *event);
	virtual void onTrackEntryEvent(TrackEntry *entry, EventType type, Event *event);

	AnimationState *getState() const;
	void setUpdateOnlyIfVisible(bool status);

	SkeletonAnimation();
	virtual ~SkeletonAnimation();
	virtual void initialize() override;

protected:
	AnimationState *_state;

	bool _ownsAnimationStateData;
	bool _updateOnlyIfVisible;
	bool _firstDraw;

	StartListener _startListener;
	InterruptListener _interruptListener;
	EndListener _endListener;
	DisposeListener _disposeListener;
	CompleteListener _completeListener;
	EventListener _eventListener;
	UpdateWorldTransformsListener _preUpdateListener;
	UpdateWorldTransformsListener _postUpdateListener;

private:
	typedef SkeletonRenderer super;
};
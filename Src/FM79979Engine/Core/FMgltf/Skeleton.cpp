#include "Skeleton.h"

cSkeleton::cSkeleton() { }

cSkeleton::cSkeleton(const Frame& rest, const Frame& bind)
{
	Set(rest, bind);
}

void cSkeleton::Set(const Frame& rest, const Frame& bind)
{
	mRestPose = rest;
	mBindPose = bind;
	UpdateInverseBindPose();
}

void cSkeleton::UpdateInverseBindPose()
{
	//unsigned int size = mBindPose.Size();
	//mInvBindPose.resize(size);
	bool l_bDo = false;
	if (l_bDo)
	{
		std::map<Frame*, cMatrix44>		l_InvBindPoseMap;
		mBindPose.GetAllInverseWorldTransform(l_InvBindPoseMap);
		for (auto l_IT : l_InvBindPoseMap)
		{
			mInvBindPose.push_back(l_IT.second);
		}
	}
	else
	{
		mBindPose.GetAllInverseWorldTransform(mInvBindPose);
	}
//	for (unsigned int i = 0; i < size; ++i)
//	{
//		Transform world = mBindPose.GetGlobalTransform(i);
//#if 1
//		mInvBindPose[i] = inverse(transformToMat4(world));
//#else
//		// This is a bit safer and more numerically stable
//		mInvBindPose[i] = transformToMat4(inverse(world));
//#endif
//	}
}


Frame& cSkeleton::GetBindPose()
{
	return mBindPose;
}

Frame& cSkeleton::GetRestPose() 
{
	return mRestPose;
}

std::vector<cMatrix44>& cSkeleton::GetInvBindPose()
{
	return mInvBindPose;
}
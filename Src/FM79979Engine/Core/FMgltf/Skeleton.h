#pragma once

#include <vector>
#include <string>
#include "../AllCoreInclude.h"
class cSkeleton 
{
protected:
	Frame mRestPose;
	Frame mBindPose;
	std::vector<cMatrix44>		mInvBindPose;
	std::vector<std::string>	mJointNames;
protected:
	void UpdateInverseBindPose();
public:
	cSkeleton();
	cSkeleton(const Frame& rest, const Frame& bind);

	void Set(const Frame& rest, const Frame& bind);

	Frame& GetBindPose();
	Frame& GetRestPose();
	std::vector<cMatrix44>& GetInvBindPose();

	std::vector<std::string>& GetJointNames();
	std::string& GetJointName(unsigned int index);
};

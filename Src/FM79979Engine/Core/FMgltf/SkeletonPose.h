#pragma once

#include <vector>
#include <string>
#include "../AllCoreInclude.h"

class cSkeletonPose
{
protected:
	Frame m_RestPose;
	Frame m_BindPose;
	std::vector<cMatrix44>		m_InvBindPose;
	std::vector<std::string>	m_JointNames;
protected:
	void UpdateInverseBindPose();
public:
	cSkeletonPose();
	cSkeletonPose(const Frame& rest, const Frame& bind);

	void Set(const Frame& rest, const Frame& bind);

	Frame& GetBindPose();
	Frame& GetRestPose();
	std::vector<cMatrix44>& GetInvBindPose();
};

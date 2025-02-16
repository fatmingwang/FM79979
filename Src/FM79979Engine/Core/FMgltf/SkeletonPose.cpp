#include "SkeletonPose.h"

cSkeletonPose::cSkeletonPose() { }

cSkeletonPose::cSkeletonPose(const Frame& rest, const Frame& bind)
{
	Set(rest, bind);
}

void cSkeletonPose::Set(const Frame& rest, const Frame& bind)
{
	m_RestPose = rest;
	m_BindPose = bind;
	UpdateInverseBindPose();
}

void cSkeletonPose::UpdateInverseBindPose()
{
	//unsigned int size = mBindPose.Size();
	//mInvBindPose.resize(size);
	bool l_bDo = false;
	if (l_bDo)
	{
		std::map<Frame*, cMatrix44>		l_InvBindPoseMap;
		m_BindPose.GetAllInverseWorldTransform(l_InvBindPoseMap);
		for (auto l_IT : l_InvBindPoseMap)
		{
			m_InvBindPose.push_back(l_IT.second);
		}
	}
	else
	{
		m_BindPose.GetAllInverseWorldTransform(m_InvBindPose);
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


Frame& cSkeletonPose::GetBindPose()
{
	return m_BindPose;
}

Frame& cSkeletonPose::GetRestPose()
{
	return m_RestPose;
}

std::vector<cMatrix44>& cSkeletonPose::GetInvBindPose()
{
	return m_InvBindPose;
}
#include "../stdAfx.h"
#include "FUSynchronized.h"

//
// cFUSynchronized
//

cFUSynchronized::~cFUSynchronized()
{
}

void cFUSynchronized::Lock()
{
	criticalSection.Enter();
}

void cFUSynchronized::Unlock()
{
	criticalSection.Leave();
}


void	cFUSynchronized::WaitCondition()
{
	m_bWaitingSingnale = true;
	bool	l_bWaitLeave = false;
	while( m_bWaitingSingnale )
	{
		if( l_bWaitLeave == false )
		{
			criticalSection.Leave();
			l_bWaitLeave  =true;
		}
	} 
	criticalSection.Enter();
}

void	cFUSynchronized::ConditionOk()
{
	m_bWaitingSingnale = false;
}
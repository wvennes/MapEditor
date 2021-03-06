//---------------------------------------------------------------------------
// Job.cpp
//---------------------------------------------------------------------------

#include "Engine/Threads/Job.hpp"


//---------------------------------------------------------------------------
Job::Job( JobPriority priority /* = AVERAGE */ )
	: m_isJobFinished( false )
	, m_jobPriority( priority )
{
	// base constructor - do nothing
}


//---------------------------------------------------------------------------
Job::~Job()
{
	// do nothing
}
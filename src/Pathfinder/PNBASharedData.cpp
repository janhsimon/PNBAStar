#include "tbb\spin_mutex.h"

#include "PNBASharedData.hpp"

void PNBASharedData::setFinished(bool finished)
{
	// lock
	tbb::spin_mutex::scoped_lock myLock(tbb::spin_mutex);

	this->finished = finished;

	// unlock
}

void PNBASharedData::setL(float L)
{
	// lock
	tbb::spin_mutex::scoped_lock myLock(tbb::spin_mutex);

	this->L = L;

	// unlock
}
// SP::cSPTimer: a stopwatch driven by a millisecond-time callback.

#include "game/types.h"
#include "SP/cSPTimer.h"

using namespace SP;

// @ 0x00bc30f0
void cSPTimer::Start()
{
    if (!mRunning) {
        mBaseTime = mGetMillisecondTime();
        mRunning = true;
    }
}

// @ 0x00bc3110
void cSPTimer::Stop()
{
    if (mRunning) {
        unsigned __int64 now = mGetMillisecondTime();
        mRunning = false;
        mElapsedTime += now - mBaseTime;
    }
}

// @ 0x00bc3130
void cSPTimer::Restart()
{
    mBaseTime = mGetMillisecondTime();
    mElapsedTime = 0;
    mRunning = true;
}

// @ 0x00bc3150
void cSPTimer::RollBack(unsigned __int64 delta)
{
    if (mRunning)
        mBaseTime += delta;
}

// @ 0x00bc3170
void cSPTimer::Reset()
{
    mBaseTime = 0;
    mElapsedTime = 0;
    mRunning = false;
}

// @ 0x00bc3190
unsigned __int64 cSPTimer::GetElapsedTime() const
{
    if (mRunning) {
        unsigned __int64 now = mGetMillisecondTime();
        return mElapsedTime + (now - mBaseTime);
    }
    return mElapsedTime;
}

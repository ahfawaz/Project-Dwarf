#include "../Application/stdafx.h"

#include "TimeManager.h"

///////////////////////////////////////////////////////////////////////////////////////////
//Clock Functions
///////////////////////////////////////////////////////////////////////////////////////////

CClock::CClock()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&m_lFreq);
	Start();
}

//Starts the timer. 
//If called again it will restart the timer.
void CClock::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&m_lStart);
	m_bActive = true;
}

//Returns the elapsed time and resets the timer.
double CClock::Tick()
{
	if (m_bActive)
	{
		long long finish;
		QueryPerformanceCounter((LARGE_INTEGER *)&finish);
		double time = (finish - m_lStart) / double(m_lFreq);
		m_lStart = finish;
		return time;
	}

	return 0.0;
}

//Returns the elapsed time with reseting the timer.
double CClock::Watch() const
{
	if (m_bActive)
	{
		long long finish;
		QueryPerformanceCounter((LARGE_INTEGER *)&finish);
		return (finish - m_lStart) / double(m_lFreq);
	}

	return 0.0;
}

//Returns the elapsed time and resets the timer and stops the timer.
double CClock::StopReset()
{
	if (m_bActive)
	{
		long long finish;
		QueryPerformanceCounter((LARGE_INTEGER *)&finish);
		m_bActive = false;
		return (finish - m_lStart) / double(m_lFreq);
	}

	return 0.0;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Time Manager Functions
///////////////////////////////////////////////////////////////////////////////////////////

CClock CTimeManager::m_Clock;
double CTimeManager::m_dElapsedTime;
double CTimeManager::m_dTimeDelta;

void CTimeManager::Initialize()
{
	m_Clock.Start();
}

void CTimeManager::Update()
{
	double prev_time = m_dElapsedTime;
	m_dElapsedTime = m_Clock.Watch();
	m_dTimeDelta = m_dElapsedTime - prev_time;
}

void CTimeManager::Shutdown() {}

float CTimeManager::GetElapsedTime()
{
	return (float)m_dElapsedTime;
}

float CTimeManager::GetTimeDelta()
{
#if _DEBUG
	if (m_dTimeDelta > 0.03)
		return 0.03f;
#endif

	return (float)m_dTimeDelta;
}

float CTimeManager::GetRawTimeDelta()
{
	return (float)m_dTimeDelta;
}

double CTimeManager::GetElapsedTimeD()
{
	return m_dElapsedTime;
}

double CTimeManager::GetTimeDeltaD()
{
	return m_dTimeDelta;
}

void CTimeManager::ResetDelta()
{
	m_dElapsedTime = m_Clock.Watch();
	m_dTimeDelta = 0.0;
}

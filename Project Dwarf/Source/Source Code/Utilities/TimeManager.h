#pragma once

//Most (if not all) of the functions made here are thanks to a friend on a previous Project (Andres Urquijo)

class CClock
{
	long long m_lFreq, m_lStart, m_lPrev;
	bool m_bActive;

public:
	CClock();

	//Starts the timer. 
	//If called again it will restart the timer.
	void Start();

	//Returns the elapsed time and resets the timer.
	double Tick();

	//Returns the elapsed time with reseting the timer.
	double Watch() const;

	//Returns the elapsed time and resets the timer and stops the timer.
	double StopReset();

};

class CTimeManager
{
	static CClock m_Clock;
	static double m_dElapsedTime;
	static double m_dTimeDelta;

public:

	void Initialize();
	void Update();
	void Shutdown();

	static float GetElapsedTime();
	static float GetTimeDelta();
	static float GetRawTimeDelta();

	static double GetElapsedTimeD();
	static double GetTimeDeltaD();

	static void ResetDelta();
};
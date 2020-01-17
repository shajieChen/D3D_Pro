#include "phc.h"
#include "CTimer.h"


DX::CTimer::CTimer():
		m_elapsedTicks(0), 
		m_totalTicks(0), 
		m_leftOverTicks(0),
		m_frameCount(0),
		m_framesPerSecond(0),
		m_frameThisSecond(0),
		m_qpcSecondCounter(0),
		m_isFixedTimeStep(false) , 
		m_targetElapsedTicks(TicksPerSecond / 60)
{
	if (!QueryPerformanceCounter(&m_qpcFrequency))
	{
		throw std::exception("QueryPerformaceFrequency"); 
	}
	if (!QueryPerformanceCounter(&m_qpcLastTime))
	{
		throw std::exception("QueryPerformaceFrequency");
	}

	//³õÊ¼»¯delta Îª1/10
	m_qpcMaxDelta = static_cast<uint64_t>(m_qpcFrequency.QuadPart / 10); 

}

//
void DX::CTimer::ResetElapsedTime()
{
	if (!QueryPerformanceCounter(&m_qpcLastTime))
	{
		throw std::exception("QueryPerformaceCounter");
	}

	m_leftOverTicks = 0; 
	m_framesPerSecond = 0; 
	m_frameThisSecond = 0; 
	m_qpcSecondCounter = 0; 
}





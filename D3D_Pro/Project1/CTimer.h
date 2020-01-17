#pragma once 
#include <cstdint>
#include <WTypesbase.h>

namespace DX
{ 
	class CTimer
	{
	public : 
		CTimer();

		//从上一个update开始 获取过程时间(ticks | seconds 单位)
		uint64_t GetElapsedTicks()	const { return m_elapsedTicks;					}
		double	 GetElapsedSeconds()const { return TicksToSeconds(m_elapsedTicks);	}

		//Total Time 
		uint64_t GetTotalTime()		const { return m_totalTicks;					}
		double	 GetTotalSeconds()	const { return TicksToSeconds(m_totalTicks);	}

		//FrameCount 
		uint32_t GetFrameCount()	const { return m_frameCount;					}
		double	GetFramePerSeconds()const { return m_framesPerSecond;				}

		//设置Timer的模式
		void SetFixedTimeStep(bool isFixedTimesStep) { m_isFixedTimeStep = isFixedTimesStep; }

		//在步长相同的情况下 多久回调Update
		void SetTargetElapsedTicks(uint64_t targetElapsed) { m_targetElapsedTicks = targetElapsed;  }
		void SetTargetElapsedSeconds(double targetElapsed) { m_targetElapsedTicks = SecondsToTicks(targetElapsed);  }
		 
		//ticks  100 毫微秒 每秒设定10 000 000ticks
		static const uint64_t TicksPerSecond = 10000000; 
		static inline double TicksToSeconds(uint64_t ticks) { return static_cast<double>(ticks) / TicksPerSecond; }
		static inline uint64_t SecondsToTicks(double seconds) { return static_cast<uint64_t>(seconds * TicksPerSecond);  }

		void ResetElapsedTime(); 
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			LARGE_INTEGER currentTime;  //请求当前时间 
			if (!QueryPerformanceCounter(&currentTime))
			{
				throw std::exception("QueryPerformaceCounter"); 
			}

			uint64_t timeDelta = static_cast<uint64_t>(currentTime.QuadPart - m_qpcLastTime.QuadPart); 

			m_qpcLastTime = currentTime;  
			m_qpcSecondCounter += timeDelta; 

			if (timeDelta > m_qpcMaxDelta)
			{
				timeDelta = m_qpcMaxDelta; 
			}

			timeDelta *= TicksPerSecond; 
			timeDelta /= static_cast<uint64_t>(m_qpcFrequency.QuadPart); 

			uint32_t lastFrameCount = m_frameCount; 

			if (m_isFixedTimeStep)//固定时间戳的模式
			{
				if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - m_targetElapsedTicks))) < TicksPerSecond / 4000)
				{
					timeDelta = m_targetElapsedTicks; 
				}
				m_leftOverTicks += timeDelta; 

				while (m_leftOverTicks >= m_targetElapsedTicks )
				{
					m_elapsedTicks = m_targetElapsedTicks; 
					m_totaltick += m_targetElapsedTicks; 
					m_leftOverTicks -= m_targetElapsedTicks; 
					m_frameCount += ; 

					update(); 
				}
			}
			else
			{
				m_elapsedTicks = timeDelta; 
				m_totalTicks += timeDelta;  
				m_leftOverTicks = 0; 
				m_frameCount++; 

				update(); 
			}

			if (m_frameCount != lastFrameCount)
			{
				m_frameThisSecond++; 
			}
			if (m_qpcSecondCounter >= static_cast<uint64_t>(m_qpcFrequency.QuadPart))
			{
				m_framesPerSecond = m_frameThisSecond; 
				m_frameThisSecond = 0; 
				m_qpcSecondCounter %= static_cast<uint64_t>(m_qpcFrequency.QuadPart); 
			}
		} 
	private : 
		LARGE_INTEGER m_qpcFrequency; 
		LARGE_INTEGER m_qpcLastTime;  
		uint64_t m_qpcMaxDelta; 


		uint64_t m_elapsedTicks; 
		uint64_t m_totalTicks; 
		uint64_t m_leftOverTicks; 

		uint32_t m_frameCount; 
		uint32_t m_framesPerSecond; 
		uint32_t m_frameThisSecond; 
		uint64_t m_qpcSecondCounter; 

		bool m_isFixedTimeStep; 
		uint64_t m_targetElapsedTicks; 

	};


};
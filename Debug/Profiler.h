#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <fstream>
#include <string>

namespace BINDU
{

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfilerResult
	{
		std::string Name;

		FloatingPointMicroseconds	StartTime;
		std::chrono::microseconds	ElapsedTime;
	};

	struct ProfilerSession
	{
		std::string	Name;
	};

	class Profiler
	{
	public:

		Profiler(const Profiler& other) = delete;
		Profiler(Profiler& other) = delete;
		Profiler& operator = (const Profiler& other) = delete;
		Profiler& operator = (Profiler& other) = delete;

		static Profiler& Get()
		{
			static Profiler profiler;
			return profiler;
		}

		void BeginSession(const std::string& name, const std::string& filepath = "profiler-result.json");

		void EndSession();

		void WriteProfile(const ProfilerResult& result);

	private:
		Profiler() = default;

		~Profiler()
		{
			EndSession();
		}

		void WriteHeader();

		void WriteFooter();

	private:
		ProfilerSession* m_currentSession{ nullptr };

		std::ofstream	 m_outputStream;
	};


	class ProfilerTimer
	{
	public:
		ProfilerTimer(const char* name) : m_name(name)
		{
			m_startTimePoint = std::chrono::steady_clock::now();
		}

		~ProfilerTimer()
		{
			if(!m_stopped)
			{
				Stop();
			}
		}

		void Stop()
		{
			auto endTimePoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_startTimePoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch() -
				std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch();

			Profiler::Get().WriteProfile({ m_name, highResStart, elapsedTime });

			m_stopped = true;
		}

	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::steady_clock>	m_startTimePoint;
		bool m_stopped{ false };
	};


	namespace ProfilerUtils
	{
		template<size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template<size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex{ 0 };
			size_t dstIndex{ 0 };

			while(srcIndex < N)
			{
				size_t matchIndex{ 0 };
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;

				if (matchIndex == K - 1)
					srcIndex += matchIndex;

				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}

			return result;
		}
	};

}

#define BINDU_PROFILE 0
#if BINDU_PROFILE
{

		#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define BINDU_FUNC_SIG __PRETTY_FUNCTION__
		#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define BINDU_FUNC_SIG __PRETTY_FUNCTION__
		#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define BINDU_FUNC_SIG __FUNCSIG__
		#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define BINDU_FUNC_SIG __FUNCTION__
		#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define BINDU_FUNC_SIG __FUNC__
		#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define BINDU_FUNC_SIG __func__
		#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define BINDU_FUNC_SIG __func__
		#else
		#define BINDU_FUNC_SIG "HZ_FUNC_SIG unknown!"
		#endif


		#define BINDU_PROFILE_BEGIN_SESSION(name, filepath)		::BINDU::Profiler::Get().BeginSession(name, filepath)
		#define BINDU_PROFILE_END_SESSION()						::BINDU::Profiler::Get().EndSession()
		#define BINDU_PROFILE_SCOPE_LINE2(name, line)	constexpr auto fixedName##line = ::BINDU::ProfilerUtils::CleanupOutputString(name, "__cdecl ");\
																						 ::BINDU::ProfilerTimer timer##line(fixedName##line.Data)
		#define BINDU_PROFILE_SCOPE_LINE(name, line) BINDU_PROFILE_SCOPE_LINE2(name, line)
		#define BINDU_PROFILE_SCOPE(name) BINDU_PROFILE_SCOPE_LINE(name, __LINE__)
		#define BINDU_PROFILE_FUNCTION() BINDU_PROFILE_SCOPE(BINDU_FUNC_SIG)

#else

		#define BINDU_PROFILE_BEGIN_SESSION(name, filepath)
		#define BINDU_PROFILE_END_SESSION()
		#define BINDU_PROFILE_SCOPE(name)
		#define BINDU_PROFILE_FUNCTION()

#endif

#endif
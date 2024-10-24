#include "Profiler.h"

#include <iomanip>
#include <sstream>

#include "../Logger/Logger.h"

namespace BINDU
{
	void Profiler::BeginSession(const std::string& name, const std::string& filepath)
	{
		// Check if a session is already open
		if(m_currentSession)
		{
			// Close it and create a new one
			if(Logger::GetCoreLogger())
			{
				BINDU_CORE_ERROR("Profiler::BeginSession('{0}') when session {1} is already open.", name, m_currentSession->Name);
			}

			EndSession();
		}

		m_outputStream.open(filepath);

		if(m_outputStream.is_open())
		{
			m_currentSession = new ProfilerSession({ name });
			WriteHeader();
		}
		else
		{
			if(Logger::GetCoreLogger())
			{
				BINDU_CORE_ERROR("Profiler could not open profiler file '{0}'.", filepath);
			}
		}
	}

	void Profiler::EndSession()
	{
		if(m_currentSession)
		{
			WriteFooter();
			m_outputStream.close();
			delete m_currentSession;
			m_currentSession = nullptr;
		}
	}

	void Profiler::WriteProfile(const ProfilerResult& result)
	{
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",\n{";
		json << "\n\t\"cat\":\"function\",";
		json << "\n\t\"dur\":" << (result.ElapsedTime.count()) << ',';
		json << "\n\t\"name\":\"" << result.Name << "\",";
		json << "\n\t\"ph\":\"X\",";
		json << "\n\t\"pid\":0,";
		json << "\n\t\"ts\":" << result.StartTime.count();
		json << "\n}";

		if(m_currentSession)
		{
			m_outputStream << json.str();
			m_outputStream.flush();
		}
	}

	void Profiler::WriteHeader()
	{
		m_outputStream << "{\"otherData\": {},\"traceEvent\":[{}";
		m_outputStream.flush();
	}

	void Profiler::WriteFooter()
	{
		m_outputStream << "]}";
		m_outputStream.flush();
	}
}

#include "glpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace GLCore {

	std::shared_ptr<spdlog::logger> Log::m_Logger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_Logger = spdlog::stdout_color_mt("GLCORE");
		m_Logger->set_level(spdlog::level::trace);
	}

}
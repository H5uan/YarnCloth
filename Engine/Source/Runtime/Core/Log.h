#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace GLCore {
    class Log {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetLogger() { return m_Logger; }

    private:
        static std::shared_ptr<spdlog::logger> m_Logger;
    };
}

// Client log macros
#define LOG_TRACE(...)         ::GLCore::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          ::GLCore::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::GLCore::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::GLCore::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::GLCore::Log::GetLogger()->critical(__VA_ARGS__)

// TODO: Change all the macros to template functions
template<typename... Args>
void LogTrace(const Args&... args) {
    GLCore::Log::GetLogger()->trace(args...);
}

template<typename... Args>
void LogInfo(const Args&... args) {
    GLCore::Log::GetLogger()->info(args...);
}

template<typename... Args>
void LogWarn(const Args&... args) {
    GLCore::Log::GetLogger()->warn(args...);
}

template<typename... Args>
void LogError(const Args&... args) {
    GLCore::Log::GetLogger()->error(args...);
}

template<typename... Args>
void LogCritical(const Args&... args) {
    GLCore::Log::GetLogger()->critical(args...);
}
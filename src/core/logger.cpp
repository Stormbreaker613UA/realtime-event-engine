// logger.cpp
#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace core {

Logger::Logger() = default;
Logger::~Logger() = default;

void Logger::set_level(LogLevel level) noexcept
{
	std::lock_guard<std::mutex> lk(mutex_);
	level_ = level;
}

LogLevel Logger::level() const noexcept
{
	return level_;
}

static const char* level_to_string(LogLevel l)
{
	switch (l) {
	case LogLevel::Trace: return "TRACE";
	case LogLevel::Debug: return "DEBUG";
	case LogLevel::Info:  return "INFO";
	case LogLevel::Warn:  return "WARN";
	case LogLevel::Error: return "ERROR";
	default: return "?";
	}
}

void Logger::log(LogLevel lvl, const std::string& msg)
{
	std::lock_guard<std::mutex> lk(mutex_);
	if (static_cast<int>(lvl) < static_cast<int>(level_)) return;

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
#if defined(_WIN32)
	localtime_s(&tm, &t);
#else
	localtime_r(&t, &tm);
#endif

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [" << level_to_string(lvl) << "] " << msg << '\n';
	std::cout << oss.str();
}

Logger& global_logger()
{
	static Logger g;
	return g;
}

} // namespace core

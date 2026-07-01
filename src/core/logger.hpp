// logger.hpp
#pragma once

#include <mutex>
#include <string>

namespace core {

enum class LogLevel { Trace = 0, Debug, Info, Warn, Error };

class Logger {
public:
	Logger();
	~Logger();

	// Non-copyable
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	void set_level(LogLevel level) noexcept;
	LogLevel level() const noexcept;

	void log(LogLevel lvl, const std::string& msg);

	// Convenience helpers
	void trace(const std::string& msg) { log(LogLevel::Trace, msg); }
	void debug(const std::string& msg) { log(LogLevel::Debug, msg); }
	void info(const std::string& msg) { log(LogLevel::Info, msg); }
	void warn(const std::string& msg) { log(LogLevel::Warn, msg); }
	void error(const std::string& msg) { log(LogLevel::Error, msg); }

private:
	LogLevel level_ = LogLevel::Info;
	std::mutex mutex_;
};

// Global logger accessor
Logger& global_logger();

} // namespace core

#include "Logger.h"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace dxe
{
	std::shared_ptr<spdlog::logger> Logger::CoreLogger;
	std::vector<spdlog::sink_ptr> Sinks;

	void Logger::Init()
	{
		Sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>()); // debug
		// sinks.emplace_back(std::make_shared<ImGuiConsoleSink_mt>()); // ImGuiConsole

		auto LogFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("thenascript.log", 1048576 * 5, 3);
		Sinks.emplace_back(LogFileSink); // Log file
		// create the loggers
		CoreLogger = std::make_shared<spdlog::logger>("ThenaScript", begin(Sinks), end(Sinks));
		spdlog::register_logger(CoreLogger);

		// configure the loggers
#ifdef DXE_LOG_TIMESTAMP
		spdlog::set_pattern("%^[%T] %v%$");
#else
		spdlog::set_pattern("%v%$");
#endif // DXE_LOG_TIMESTAMP
		CoreLogger->set_level(spdlog::level::trace);
	}

	void Logger::Release()
	{
		CoreLogger.reset();
		spdlog::shutdown();
	}

	void Logger::AddSink(spdlog::sink_ptr& sink)
	{
		CoreLogger->sinks().push_back(sink);
		CoreLogger->set_pattern("%v%$");
	}

}

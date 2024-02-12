#pragma once

#include "Common.h"

#define SPDLOG_EOL ""
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace dxe
{
	class Logger
	{
	public:
		static void init();
		static void release();
		static std::shared_ptr<spdlog::logger>& getCoreLogger() { return CoreLogger; }
		static void addSink(std::shared_ptr<spdlog::sinks::sink>& Sink);
	private:
		static std::shared_ptr<spdlog::logger> CoreLogger;
	};
}

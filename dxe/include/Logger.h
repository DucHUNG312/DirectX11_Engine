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
		static void Init();
		static void Release();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return CoreLogger; }
		static void AddSink(std::shared_ptr<spdlog::sinks::sink>& Sink);
	private:
		static std::shared_ptr<spdlog::logger> CoreLogger;
	};
}

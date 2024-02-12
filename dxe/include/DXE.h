#pragma once

#include "Common.h"

// comment this to disable GPU path
// #define DXE_BUILD_GPU_RENDERER 

// comment this to disable profiler
#define DXE_ENABLE_PROFILER 0

// comment this to disable logging
#define DXE_ENABLE_LOG 1

// enable log
#include "Logger.h"
// Core log macros
#ifdef DXE_DEBUG

#endif // DXE_DEBUG

#if DXE_ENABLE_LOG
#define DXE_LOG_TRACE(...) SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::trace, __VA_ARGS__)
#define DXE_LOG_DEBUG(...) SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::debug, __VA_ARGS__)
#define DXE_LOG_INFO(...) SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::info, __VA_ARGS__)
#define DXE_LOG_WARN(...) SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::warn, __VA_ARGS__)
#define DXE_LOG_ERROR(...) SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::err, __VA_ARGS__)
#define DXE_LOG_CRITICAL(...) SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::critical, __VA_ARGS__)

#define DXE_LOG_TRACE_NL(...) (SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::trace, __VA_ARGS__), DXE_LOG_TRACE('\n'))
#define DXE_LOG_DEBUG_NL(...) (SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::debug, __VA_ARGS__), DXE_LOG_TRACE('\n'))
#define DXE_LOG_INFO_NL(...) (SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::info, __VA_ARGS__), DXE_LOG_TRACE('\n'))
#define DXE_LOG_WARN_NL(...) (SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::warn, __VA_ARGS__), DXE_LOG_TRACE('\n'))
#define DXE_LOG_ERROR_NL(...) (SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::err, __VA_ARGS__), DXE_LOG_TRACE('\n'))
#define DXE_LOG_CRITICAL_NL(...) (SPDLOG_LOGGER_CALL(::dxe::Logger::getCoreLogger(), spdlog::level::level_enum::critical, __VA_ARGS__), DXE_LOG_TRACE('\n'))
#else
#define DXE_LOG_TRACE(...) ((void)0)
#define DXE_LOG_DEBUG(...) ((void)0)
#define DXE_LOG_INFO(...) ((void)0)
#define DXE_LOG_WARN(...) ((void)0)
#define DXE_LOG_ERROR(...) ((void)0)
#define DXE_LOG_CRITICAL(...) ((void)0)

#define DXE_LOG_TRACE_NL(...) ((void)0)
#define DXE_LOG_DEBUG_NL(...) ((void)0)
#define DXE_LOG_INFO_NL(...) ((void)0)
#define DXE_LOG_WARN_NL(...) ((void)0)
#define DXE_LOG_ERROR_NL(...) ((void)0)
#define DXE_LOG_CRITICAL_NL(...) ((void)0)
#endif


#ifdef DXE_PLATFORM_WINDOWS
#pragma warning(disable : 4251)
#ifdef DXE_DYNAMIC
#ifdef DXE_ENGINE
#define DXE_EXPORT __declspec(dllexport)
#else
#define DXE_EXPORT __declspec(dllimport)
#endif
#else
#define DXE_EXPORT
#endif
#define DXE_HIDDEN
#else
#define DXE_EXPORT __attribute__((visibility("default")))
#define DXE_HIDDEN __attribute__((visibility("hidden")))
#endif

#ifdef DXE_PLATFORM_WINDOWS
#define DXE_HAVE__ALIGNED_MALLOC
#endif // DXE_PLATFORM_WINDOWS


#ifdef DXE_PLATFORM_WINDOWS
#define DXE_BREAK() __debugbreak()
#else
#define DXE_BREAK() raise(SIGTRAP)
#endif

#if (defined(_CPPUNWIND) || defined(__EXCEPTIONS))
#define DXE_EXCEPTIONS
#endif

#if defined(_MSC_VER)
#define DXE_IS_MSVC
#define DXE_DISABLE_WARNING_PUSH __pragma(warning(push))
#define DXE_DISABLE_WARNING_POP __pragma(warning(pop))
#define DXE_DISABLE_WARNING(warningNumber) __pragma(warning(disable \
                                                        : warningNumber))
#define DXE_DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER DXE_DISABLE_WARNING(4100)
#define DXE_DISABLE_WARNING_UNREFERENCED_FUNCTION DXE_DISABLE_WARNING(4505)
#define DXE_DISABLE_WARNING_CONVERSION_TO_SMALLER_TYPE DXE_DISABLE_WARNING(4267)
#else
#define DXE_DISABLE_WARNING_PUSH
#define DXE_DISABLE_WARNING_POP
#define DXE_DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
#define DXE_DISABLE_WARNING_UNREFERENCED_FUNCTION
#define DXE_DISABLE_WARNING_CONVERSION_TO_SMALLER_TYPE
#endif

#ifndef DXE_FORCE_INLINE
#if defined(_MSC_VER)
#define DXE_FORCE_INLINE __forceinline
#else
#define DXE_FORCE_INLINE inline
#endif
#endif // !DXE_FORCE_INLINE

#ifndef DXE_ALWAYS_INLINE
#if defined(_MSC_VER)
#define DXE_ALWAYS_INLINE FORCE_INLINE
#else
#define DXE_ALWAYS_INLINE inline
#endif 
#endif // !DXE_ALWAYS_INLINE

#ifndef DXE_NODISCARD
#define DXE_NODISCARD [[nodiscard]]
#endif // !DXE_NODISCARD

#ifndef DXE_ALLOW_DISCARD
#define DXE_ALLOW_DISCARD (void)
#endif // !DXE_ALLOW_DISCARD

#if defined(_MSC_VER)
  // NOTE MSVC often gives C4127 warnings with compiletime if statements. See bug 1362.
  // This workaround is ugly, but it does the job.
#define DXE_CONST_CONDITIONAL(cond)  (void)0, cond
#else
#define DXE_CONST_CONDITIONAL(cond)  cond
#endif

#ifdef DXE_PLATFORM_WINDOWS
#define DXE_CPU_GPU_LAMBDA(...) [ =, *this ] DXE_CPU_GPU(__VA_ARGS__) mutable
#else
#define DXE_CPU_GPU_LAMBDA(...) [=] DXE_CPU_GPU(__VA_ARGS__)
#endif

// Define Cache Line Size Constant
#ifdef DXE_BUILD_GPU_RENDERER
#define DXE_L1_CACHE_LINE_SIZE 128
#else
#define DXE_L1_CACHE_LINE_SIZE 64
#endif

// enable check
#define DXE_ENABLE_CHECK
#ifdef DXE_ENABLE_CHECK
#define CHECK(x) (!(!(x) && (DXE_LOG_CRITICAL("Check failed: {}", #x), DXE_BREAK(), true)))
#define CHECK_MSG(x, msg) (!(!(x) && (DXE_LOG_ERROR(#msg), true)))
#define CHECK_IMPL(a, b, op) (!(!(a op b) && (DXE_LOG_CRITICAL("Check failed: {}{}{}", #a, #op, #b), DXE_BREAK(), true)))
#define EMPTY_CHECK \
	do {			\
	} while (false);
#define CHECK_EQ(a, b) CHECK_IMPL(a, b, ==)
#define CHECK_NE(a, b) CHECK_IMPL(a, b, !=)
#define CHECK_GT(a, b) CHECK_IMPL(a, b, >)
#define CHECK_GE(a, b) CHECK_IMPL(a, b, >=)
#define CHECK_LT(a, b) CHECK_IMPL(a, b, <)
#define CHECK_LE(a, b) CHECK_IMPL(a, b, <=)

#define DXE_CHECK(x) (CHECK(x))
#define DXE_CHECK_MSG(x, msg) (CHECK_MSG(x, msg))
#define DXE_CHECK_EQ(a, b) (CHECK_EQ(a, b))
#define DXE_CHECK_NE(a, b) (CHECK_NE(a, b))
#define DXE_CHECK_GT(a, b) (CHECK_GT(a, b))
#define DXE_CHECK_GE(a, b) (CHECK_GE(a, b))
#define DXE_CHECK_LT(a, b) (CHECK_LT(a, b))
#define DXE_CHECK_LE(a, b) (CHECK_LE(a, b))

#ifdef DXE_IS_GPU_CODE

#define DXE_CHECK_RARE(freq, condition)
#define DXE_DCHECK_RARE(freq, condition)
#else
#define DXE_CHECK_RARE(freq, condition)                                                 \
    static_assert(std::is_floating_point<decltype(freq)>::value,                        \
                  "Expected floating-point frequency as first argument to CHECK_RARE"); \
    static_assert(std::is_integral<decltype(condition)>::value,                         \
                  "Expected Boolean condition as second argument to CHECK_RARE");       \
    do {                                                                                \
        static thread_local int64_t numTrue, total;                                     \
        static StatRegisterer reg([](StatsAccumulator &accum) {                         \
            accum.ReportRareCheck(__FILE__ " " CHECK_RARE_EXPAND_AND_TO_STRING(         \
                                      __LINE__) ": CHECK_RARE failed: " #condition,     \
                                  freq, numTrue, total);                                \
            numTrue = total = 0;                                                        \
        });                                                                             \
        ++total;                                                                        \
        if (condition)                                                                  \
            ++numTrue;                                                                  \
    } while (0)

#ifdef DXE_DEBUG_BUILD
#define DXE_DCHECK_RARE(freq, condition) DXE_CHECK_RARE(freq, condition)
#else
#define DXE_DCHECK_RARE(freq, condition)
#endif

#endif  // DXE_IS_GPU_CODE
#else
#define DXE_CHECK(x) EMPTY_CHECK
#define DXE_CHECK_MSG(x, msg) EMPTY_CHECK
#define DXE_CHECK_MSG_RET(x, ret, msg) EMPTY_CHECK
#define DXE_CHECK_EQ(a, b) EMPTY_CHECK
#define DXE_CHECK_NE(a, b) EMPTY_CHECK
#define DXE_CHECK_GT(a, b) EMPTY_CHECK
#define DXE_CHECK_GE(a, b) EMPTY_CHECK
#define DXE_CHECK_LT(a, b) EMPTY_CHECK
#define DXE_CHECK_LE(a, b) EMPTY_CHECK
#endif // DXE_ENABLE_CHECK

// Profiler
#define EMPTY_PROFILE \
	do {			  \
	} while (false);
#if DXE_ENABLE_PROFILER
#ifdef DXE_PLATFORM_WINDOWS
#define TRACY_CALLSTACK 1
#endif

#define DXE_TRACK_MEMORY 1
#define DXE_PROFILE_LOW 0
#define DXE_PROFILE_GPU_TIMINGS 0

#include <tracy/Tracy.hpp>
#define DXE_PROFILE_SCOPE(name) ZoneScopedN(name)
#define DXE_PROFILE_FUNCTION() ZoneScoped
#define DXE_PROFILE_FRAMEMARKER() FrameMark
#define DXE_PROFILE_LOCK(type, var, name) TracyLockableN(type, var, name)
#define DXE_PROFILE_LOCKMARKER(var) LockMark(var)
#define DXE_PROFILE_SETTHREADNAME(name) tracy::SetThreadName(name)

#if DXE_PROFILE_LOW
#define DXE_PROFILE_FUNCTION_LOW() ZoneScoped
#define DXE_PROFILE_SCOPE_LOW(name) ZoneScopedN(name)
#else
#define DXE_PROFILE_FUNCTION_LOW()
#define DXE_PROFILE_SCOPE_LOW(name)
#endif

#else
#define DXE_PROFILE_SCOPE(name) EMPTY_PROFILE
#define DXE_PROFILE_FUNCTION() EMPTY_PROFILE
#define DXE_PROFILE_FRAMEMARKER() EMPTY_PROFILE
#define DXE_PROFILE_LOCK(type, var, name) EMPTY_PROFILE
#define DXE_PROFILE_LOCKMARKER(var) EMPTY_PROFILE
#define DXE_PROFILE_SETTHREADNAME(name) EMPTY_PROFILE
#define DXE_PROFILE_FUNCTION_LOW() EMPTY_PROFILE
#define DXE_PROFILE_SCOPE_LOW(name) EMPTY_PROFILE
#endif

#define DXE_UNIMPLEMENTED												    \
    {																		\
        DXE_LOG_CRITICAL("Unimplemented : {} : {}", __FILE__, __LINE__);   \
        DXE_BREAK();														\
    }

#define DXE_NONCOPYABLE(class_name)                           \
    class_name(const class_name&)            = delete;		  \
    class_name& operator=(const class_name&) = delete
#define DXE_NONCOPYABLEANDMOVE(class_name)                    \
    class_name(const class_name&)            = delete;		  \
    class_name& operator=(const class_name&) = delete;		  \
    class_name(class_name&&)                 = delete;		  \
    class_name& operator=(class_name&&)      = delete

#ifndef DXE_STR
#define DXE_STR(x) #x
#define DXE_MAKE_STR(x) STR(x)
#endif // !DXE_STR

#ifndef DXE_MAX_RECURSION
#define DXE_MAX_RECURSION 100
#endif // DXE_MAX_RECURSION

#ifndef DXE_BIT
#define DXE_BIT(x) (1 << x)
#endif // !DXE_BIT

#ifndef DXE_SHIFT_LEFT
#define DXE_SHIFT_LEFT(x) (std::size(1) << x)
#endif // 

#ifndef DXE_SHIFT_RIGHT
#define DXE_SHIFT_RIGHT(x) (std::size(1) >> x)
#endif // !DXE_SHIFT_RIGHT

#ifndef DXE_BIND_EVENT_FN
#define DXE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { \
    return this->fn(std::forward<decltype(args)>(args)...);          \
}
#endif // !DXE_BIND_EVENT_FN


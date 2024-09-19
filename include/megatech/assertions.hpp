#ifndef MEGATECH_ASSERTIONS_HPP
#define MEGATECH_ASSERTIONS_HPP

#if __has_include(<format>) && !defined(MEGATECH_ASSERTIONS_DISABLE_FORMAT)
  #define MEGATECH_ASSERTIONS_FORMAT_AVAILABLE (1)
#endif

#define MEGATECH_ASSERTION_TYPE_PRINTF (1)
#define MEGATECH_ASSERTION_TYPE_FORMAT (2)

#ifndef MEGATECH_DEFAULT_ASSERTION_TYPE
  #ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
    #define MEGATECH_DEFAULT_ASSERTION_TYPE MEGATECH_ASSERTION_TYPE_FORMAT
  #else
    #define MEGATECH_DEFAULT_ASSERTION_TYPE MEGATECH_ASSERTION_TYPE_PRINTF
  #endif
#endif

#if MEGATECH_DEFAULT_ASSERTION_TYPE == MEGATECH_ASSERTION_TYPE_FORMAT && !defined(MEGATECH_ASSERTIONS_FORMAT_AVAILABLE)
  #error "<format> based assertions are not available."
#endif

#ifndef MEGATECH_ASSERTIONS_ENABLED
  #ifndef NDEBUG
    #define MEGATECH_ASSERTIONS_ENABLED (1)
  #endif
#endif

#include <source_location>
#include <string_view>

#ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
  #include <cstddef>

  #include <format>
#endif

#ifdef MEGATECH_ASSERTIONS_ENABLED
  #define MEGATECH_ASSERT_MSG_PRINTF(exp, msg, ...) \
    (debug_assertion_printf(std::source_location::current(), (exp), (msg) __VA_OPT__(,) __VA_ARGS__))
  #define MEGATECH_PRECONDITION_MSG_PRINTF(exp, msg, ...) \
    MEGATECH_ASSERT_MSG_PRINTF((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #define MEGATECH_POSTCONDITION_MSG_PRINTF(exp, msg, ...) \
    MEGATECH_ASSERT_MSG_PRINTF((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #if MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
    #define MEGATECH_ASSERT_MSG_FORMAT(exp, msg, ...) \
      (debug_assertion_format(std::source_location::current(), (exp), (msg) __VA_OPT__(,) __VA_ARGS__))
    #define MEGATECH_PRECONDITION_MSG_FORMAT(exp, msg, ...) \
      MEGATECH_ASSERT_MSG_FORMAT((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
    #define MEGATECH_POSTCONDITION_MSG_FORMAT(exp, msg, ...) \
      MEGATECH_ASSERT_MSG_FORMAT((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #endif
  #if MEGATECH_DEFAULT_ASSERTION_TYPE == MEGATECH_ASSERTION_TYPE_PRINTF
    #define MEGATECH_ASSERT_MSG(exp, msg, ...) MEGATECH_ASSERT_MSG_PRINTF((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #elif MEGATECH_DEFAULT_ASSERTION_TYPE == MEGATECH_ASSERTION_TYPE_FORMAT
    #define MEGATECH_ASSERT_MSG(exp, msg, ...) MEGATECH_ASSERT_MSG_FORMAT((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #else
    #error "MEGATECH_DEFAULT_ASSERTION_TYPE must be defined to MEGATECH_ASSERTION_TYPE_PRINTF, MEGATECH_ASSERTION_TYPE_FORMAT, or undefined."
  #endif
  #define MEGATECH_ASSERT(exp) MEGATECH_ASSERT_MSG((exp), (#exp))
  #define MEGATECH_PRECONDITION_MSG(exp, msg, ...) MEGATECH_ASSERT_MSG((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #define MEGATECH_PRECONDITION(exp) MEGATECH_ASSERT_MSG((exp), (#exp))
  #define MEGATECH_POSTCONDITION_MSG(exp, msg, ...) MEGATECH_ASSERT_MSG((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #define MEGATECH_POSTCONDITION(exp) MEGATECH_ASSERT_MSG((exp), (#exp))
#else
  #define MEGATECH_ASSERT(exp) ((void) 0)
  #define MEGATECH_ASSERT_MSG(exp, msg, ...) ((void) 0)
  #define MEGATECH_ASSERT_MSG_PRINTF(exp, msg, ...) ((void) 0)
  #define MEGATECH_PRECONDITION(exp) ((void) 0)
  #define MEGATECH_PRECONDITION_MSG(exp, msg, ...) ((void) 0)
  #define MEGATECH_PRECONDITION_MSG_PRINTF(exp, msg, ...) ((void) 0)
  #define MEGATECH_POSTCONDITION(exp) ((void) 0)
  #define MEGATECH_POSTCONDITION_MSG(exp, msg, ...) ((void) 0)
  #define MEGATECH_POSTCONDITION_MSG_PRINTF(exp, msg, ...) ((void) 0)
  #if MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
    #define MEGATECH_ASSERT_MSG_FORMAT(exp, msg, ...) ((void) 0)
    #define MEGATECH_PRECONDITION_MSG_FORMAT(exp, msg, ...) ((void) 0)
    #define MEGATECH_POSTCONDITION_MSG_FORMAT(exp, msg, ...) ((void) 0)
  #endif
#endif

namespace megatech::internal::base {

  [[noreturn]]
  void dispatch_assertion_failure(const std::source_location& location, char* const message);

  [[noreturn]]
  void dispatch_assertion_error(const std::source_location& location);

}

namespace megatech {

  void debug_assertion_printf(const std::source_location& location, const bool condition,
                              const char *const format, ...);

#if MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
  void debug_assertion_format(const std::source_location& location, const bool condition,
                              const std::string_view& format, const std::size_t message_size, std::format_args&& args);

  template <typename... Args>
  void debug_assertion_format(const std::source_location& location, const bool condition,
                              const std::format_string<Args...>& format, Args&&... args) {
    try
    {
      const auto size = std::formatted_size(std::forward<std::format_string<Args...>>(format),
                                            std::forward<Args>(args)...);
      debug_assertion_format(location, condition, format, size, std::make_format_args(args...));
    }
    catch (...)
    {
      internal::base::dispatch_assertion_error(location);
    }
  }
#endif

}

#endif

#ifndef MEGATECH_ASSERTIONS_HPP
#define MEGATECH_ASSERTIONS_HPP

#if __has_include(<format>)
  #ifndef MEGATECH_ASSERTIONS_FORMAT_UNAVAILABLE
    #define MEGATECH_ASSERTIONS_FORMAT_AVAILABLE (1)
  #endif
#elif !defined(MEGATECH_ASSERTIONS_FORMAT_UNAVAILABLE)
  #define MEGATECH_ASSERTIONS_FORMAT_UNAVAILABLE (1)
#endif

#if !defined(MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_PRINTF) && !defined(MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_FORMAT)
  #ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
    #define MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_FORMAT (1)
  #else
    #define MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_PRINTF (1)
  #endif
#elif defined(MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_PRINTF) && defined(MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_FORMAT)
  #error "Only one formatter can be used as the default formatter."
#elif defined(MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_FORMAT) && !defined(MEGATECH_ASSERTIONS_FORMAT_AVAILABLE)
  #error "<format> based assertions are not available."
#endif

#if !defined(MEGATECH_ASSERTIONS_DISABLED) && !defined(NDEBUG)
  #define MEGATECH_ASSERTIONS_ENABLED (1)
#endif

#include <source_location>
#include <string_view>

#ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
  #include <cstddef>

  #include <format>
#endif

#ifdef MEGATECH_ASSERTIONS_ENABLED
  #define MEGATECH_ASSERT_MSG_PRINTF(exp, msg, ...) \
    (megatech::debug_assertion_printf(std::source_location::current(), (exp), (msg) __VA_OPT__(,) __VA_ARGS__))
  #define MEGATECH_PRECONDITION_MSG_PRINTF(exp, msg, ...) \
    MEGATECH_ASSERT_MSG_PRINTF((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #define MEGATECH_POSTCONDITION_MSG_PRINTF(exp, msg, ...) \
    MEGATECH_ASSERT_MSG_PRINTF((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
    #define MEGATECH_ASSERT_MSG_FORMAT(exp, msg, ...) \
      (megatech::debug_assertion_format(std::source_location::current(), (exp), (msg) __VA_OPT__(,) __VA_ARGS__))
    #define MEGATECH_PRECONDITION_MSG_FORMAT(exp, msg, ...) \
      MEGATECH_ASSERT_MSG_FORMAT((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
    #define MEGATECH_POSTCONDITION_MSG_FORMAT(exp, msg, ...) \
      MEGATECH_ASSERT_MSG_FORMAT((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #endif
  #if defined(MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_PRINTF)
    #define MEGATECH_ASSERT_MSG(exp, msg, ...) MEGATECH_ASSERT_MSG_PRINTF((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
  #elif defined(MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_FORMAT)
    #define MEGATECH_ASSERT_MSG(exp, msg, ...) MEGATECH_ASSERT_MSG_FORMAT((exp), (msg) __VA_OPT__(,) __VA_ARGS__)
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

#define MEGATECH_ASSERTIONS_AVAILABLE (1)

namespace megatech::internal::base {

  [[noreturn]]
  void dispatch_assertion_failure(const std::source_location& location, char* const message) noexcept;

  [[noreturn]]
  void dispatch_assertion_error(const std::source_location& location) noexcept;

#ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
  void debug_assertion_format(const std::source_location& location, const bool condition,
                              const std::string_view& format, const std::size_t message_size,
                              std::format_args&& args) noexcept;
#endif

}

namespace megatech {

  void debug_assertion_printf(const std::source_location& location, const bool condition,
                              const char *const format, ...) noexcept;

#ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
  template <typename... Args>
  void debug_assertion_format(const std::source_location& location, const bool condition,
                              const std::format_string<Args...>& format, Args&&... args) noexcept {
    try
    {
      const auto size = std::formatted_size(format, std::forward<Args>(args)...);
      internal::base::debug_assertion_format(location, condition, format.get(), size, std::make_format_args(args...));
    }
    catch (...)
    {
      internal::base::dispatch_assertion_error(location);
    }
  }
#endif

}

#endif

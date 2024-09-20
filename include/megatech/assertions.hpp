/**
 * @file assertions.hpp
 * @brief Run-Time Assertions
 * @details Assertions behave as they do in many programming languages. An assertion is any expression that **MUST** be
 *          true when executed. If an assertion fails, a diagnostic message is written to the standard error stream,
 *          and the program is aborted (i.e., the program exits abnormally). Assertions are intended purely as a
 *          development tool, and they should be disabled in release software. When disabled, every assertion provided
 *          here is equivalent to the expression `((void) 0)`.
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2024
 */
#ifndef MEGATECH_ASSERTIONS_HPP
#define MEGATECH_ASSERTIONS_HPP

// Don't define this outside doxygen.
#ifdef __DOXYGEN__
  /**
   * @def MEGATECH_ASSERTIONS_FORMAT_UNAVAILABLE
   * @brief If defined, "format"-style assertions will be disabled.
   * @details This can be defined by clients to control the availability of features based on the standard `format`
   *          header assertions. Unlike defining ::MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_PRINTF, this macro completely
   *          disables all "format" functionality. None of the `*_FORMAT` macros will be defined. None of the
   *          corresponding functions will be declared. `<format>` will not be included by default.
   */
  #define MEGATECH_ASSERTIONS_FORMAT_UNAVAILABLE

  /**
   * @def MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
   * @brief If defined, "format"-style assertions will be enabled.
   * @details This cannot be defined by clients.
   */
  #define MEGATECH_ASSERTIONS_FORMAT_AVAILABLE

  /**
   * @def MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_PRINTF
   * @brief If defined, this forces the ::MEGATECH_ASSERT family of macros to use "printf"-style formatting.
   * @details This can be defined by clients, but only one default formatter can be used at a time.
   */
  #define MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_PRINTF

  /**
   * @def MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_FORMAT
   * @brief If defined, this forces the ::MEGATECH_ASSERT family of macros to use "format"-style formatting.
   * @details This can be defined by clients, but only one default formatter can be used at a time.
   */
  #define MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_FORMAT

  /**
   * @def MEGATECH_ASSERTIONS_DISABLED
   * @brief If defined, assertions are disabled **as-if** `NDEBUG` was defined.
   * @details This can be defined by clients, but it is mutually exclusive with ::MEGATECH_ASSERTIONS_ENABLED.
   *          Although defining `NDEBUG` is sufficient to eliminate assertions, it can be a bit confusing. Especially,
   *          since it's a double negative (assertions are enabled only when the "no debug" macro **isn't** defined).
   *          This macro, along with ::MEGATECH_ASSERTIONS_ENABLED provides greater clarity. If
   *          ::MEGATECH_ASSERTIONS_DISABLED is defined, assertions are eliminated. If ::MEGATECH_ASSERTIONS_ENABLED is
   *          defined, assertions are executed. Both macros cannot be defined at the same time.
   */
  #define MEGATECH_ASSERTIONS_DISABLED

  /**
   * @def MEGATECH_ASSERTIONS_ENABLED
   * @brief If defined, assertions are enabled **as-if** `NDEBUG` was not defined.
   * @details This can be defined by clients, but it is mutually exclusive with ::MEGATECH_ASSERTIONS_DISABLED. By
   *          defining this macro, clients can signal their intent to use assertions regardless of `NDEBUG`.
   * @see ::MEGATECH_ASSERTIONS_DISABLED
   */
  #define MEGATECH_ASSERTIONS_ENABLED

  /**
   * @def MEGATECH_ASSERT_MSG
   * @brief Assert that an expression is true and provide a diagnostic message if it is false.
   * @details The provided message will use the default formatting syntax. Clients can query
   *          ::MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_PRINTF and ::MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_FORMAT to
   *          determine which syntax is the default.
   * @param exp The assertion's controlling expression. This **MUST** be convertible to `bool`.
   * @param msg A diagnostic message format to render and display if the assertion fails. This **SHOULD** be a string
   *            literal. Other types of inputs are not recommended even if they produce the expected results.
   * @param ... 0 or more parameters to use in rendering the diagnostic message.
   */
  #define MEGATECH_ASSERT_MSG(exp, msg, ...)

  /**
   * @def MEGATECH_ASSERT_MSG_PRINTF
   * @brief Assert that an expression is true and provide a diagnostic message if it is false.
   * @details This variant always uses the "printf"-style format syntax.
   * @param exp The assertion's controlling expression. This **MUST** be convertible to `bool`.
   * @param msg A diagnostic message format to render and display if the assertion fails. This **SHOULD** be a string
   *            literal. Other types of inputs are not recommended even if they produce the expected results.
   * @param ... 0 or more parameters to use in rendering the diagnostic message.
   * @see ::MEGATECH_ASSERT_MSG
   */
  #define MEGATECH_ASSERT_MSG_PRINTF(exp, msg, ...)

  /**
   * @def MEGATECH_ASSERT_MSG_FORMAT
   * @brief Assert that an expression is true and provide a diagnostic message if it is false.
   * @details This variant always uses the "format"-style format syntax.
   * @param exp The assertion's controlling expression. This **MUST** be convertible to `bool`.
   * @param msg A diagnostic message format to render and display if the assertion fails. This **SHOULD** be a string
   *            literal. Other types of inputs are not recommended even if they produce the expected results.
   * @param ... 0 or more parameters to use in rendering the diagnostic message.
   * @see ::MEGATECH_ASSERT_MSG
   */
  #define MEGATECH_ASSERT_MSG_FORMAT(exp, msg, ...)

  /**
   * @def MEGATECH_ASSERT
   * @brief Assert that an expression is true.
   * @details the non-`*_MSG*` variants of all assertion macros use the expression as their diagnostic message.
   * @param exp The assertion's controlling expression. This **MUST** be convertible to `bool`.
   */
  #define MEGATECH_ASSERT(exp)

  /**
   * @def MEGATECH_PRECONDITION_MSG
   * @brief Assert that a precondition is true and provide a diagnostic message if it is false.
   * @details Precondition macros behave identically to assertion macros. They exist merely to clarify the purpose
   *          of precondition assertions.
   * @param exp The precondition's controlling expression. This **MUST** be convertible to `bool`.
   * @param msg A diagnostic message format to render and display if the assertion fails. This **SHOULD** be a string
   *            literal. Other types of inputs are not recommended even if they produce the expected results.
   * @param ... 0 or more parameters to use in rendering the diagnostic message.
   * @see MEGATECH_ASSERT_MSG
   */
  #define MEGATECH_PRECONDITION_MSG(exp, msg, ...)

  /**
   * @def MEGATECH_PRECONDITION_MSG_PRINTF
   * @brief Assert that a precondition is true and provide a diagnostic message if it is false.
   * @details Precondition macros behave identically to assertion macros. They exist merely to clarify the purpose
   *          of precondition assertions. This variant always uses the "printf"-style format syntax.
   * @param exp The precondition's controlling expression. This **MUST** be convertible to `bool`.
   * @param msg A diagnostic message format to render and display if the assertion fails. This **SHOULD** be a string
   *            literal. Other types of inputs are not recommended even if they produce the expected results.
   * @param ... 0 or more parameters to use in rendering the diagnostic message.
   * @see MEGATECH_ASSERT_MSG
   */
  #define MEGATECH_PRECONDITION_MSG_PRINTF(exp, msg, ...)

  /**
   * @def MEGATECH_PRECONDITION_MSG_FORMAT
   * @brief Assert that a precondition is true and provide a diagnostic message if it is false.
   * @details Precondition macros behave identically to assertion macros. They exist merely to clarify the purpose
   *          of precondition assertions. This variant always uses the "format"-style format syntax.
   * @param exp The precondition's controlling expression. This **MUST** be convertible to `bool`.
   * @param msg A diagnostic message format to render and display if the assertion fails. This **SHOULD** be a string
   *            literal. Other types of inputs are not recommended even if they produce the expected results.
   * @param ... 0 or more parameters to use in rendering the diagnostic message.
   * @see MEGATECH_ASSERT_MSG
   */
  #define MEGATECH_PRECONDITION_MSG_FORMAT(exp, msg, ...)

  /**
   * @def MEGATECH_PRECONDITION
   * @brief Assert that a precondition is true.
   * @details the non-`*_MSG*` variants of all assertion macros use the expression as their diagnostic message.
   * @param exp The precondition's controlling expression. This **MUST** be convertible to `bool`.
   * @see ::MEGATECH_ASSERT
   */
  #define MEGATECH_PRECONDITION(exp)

  /**
   * @def MEGATECH_POSTCONDITION_MSG
   * @brief Assert that a postcondition is true and provide a diagnostic message if it is false.
   * @details Postcondition macros behave identically to assertion macros. They exist merely to clarify the purpose
   *          of postcondition assertions.
   * @param exp The postcondition's controlling expression. This **MUST** be convertible to `bool`.
   * @param msg A diagnostic message format to render and display if the assertion fails. This **SHOULD** be a string
   *            literal. Other types of inputs are not recommended even if they produce the expected results.
   * @param ... 0 or more parameters to use in rendering the diagnostic message.
   * @see MEGATECH_ASSERT_MSG
   */
  #define MEGATECH_POSTCONDITION_MSG(exp, msg, ...)

  /**
   * @def MEGATECH_POSTCONDITION_MSG_PRINTF
   * @brief Assert that a postcondition is true and provide a diagnostic message if it is false.
   * @details Postcondition macros behave identically to assertion macros. They exist merely to clarify the purpose
   *          of postcondition assertions. This variant always uses the "printf"-style format syntax.
   * @param exp The postcondition's controlling expression. This **MUST** be convertible to `bool`.
   * @param msg A diagnostic message format to render and display if the assertion fails. This **SHOULD** be a string
   *            literal. Other types of inputs are not recommended even if they produce the expected results.
   * @param ... 0 or more parameters to use in rendering the diagnostic message.
   * @see MEGATECH_ASSERT_MSG
   */
  #define MEGATECH_POSTCONDITION_MSG_PRINTF(exp, msg, ...)

  /**
   * @def MEGATECH_POSTCONDITION_MSG_FORMAT
   * @brief Assert that a postcondition is true and provide a diagnostic message if it is false.
   * @details Postcondition macros behave identically to assertion macros. They exist merely to clarify the purpose
   *          of postcondition assertions. This variant always uses the "format"-style format syntax.
   * @param exp The postcondition's controlling expression. This **MUST** be convertible to `bool`.
   * @param msg A diagnostic message format to render and display if the assertion fails. This **SHOULD** be a string
   *            literal. Other types of inputs are not recommended even if they produce the expected results.
   * @param ... 0 or more parameters to use in rendering the diagnostic message.
   * @see MEGATECH_ASSERT_MSG
   */
  #define MEGATECH_POSTCONDITION_MSG_FORMAT(exp, msg, ...)

  /**
   * @def MEGATECH_POSTCONDITION
   * @brief Assert that a postcondition is true.
   * @details the non-`*_MSG*` variants of all assertion macros use the expression as their diagnostic message.
   * @param exp The postcondition's controlling expression. This **MUST** be convertible to `bool`.
   * @see ::MEGATECH_ASSERT
   */
  #define MEGATECH_POSTCONDITION(exp)

  /**
   * @def MEGATECH_ASSERTIONS_AVAILABLE
   * @brief This macro indicates that the Megatech Assertion library is available to client programs.
   */
  #define MEGATECH_ASSERTIONS_AVAILABLE (1)

  #undef MEGATECH_ASSERTIONS_FORMAT_UNAVAILABLE
  #undef MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_PRINTF
  #undef MEGATECH_ASSERTIONS_DEFAULT_FORMATTER_FORMAT
  #undef MEGATECH_ASSERTIONS_DISABLED
  #undef MEGATECH_ASSERT_MSG
  #undef MEGATECH_ASSERT_MSG_PRINTF
  #undef MEGATECH_ASSERT_MSG_FORMAT
  #undef MEGATECH_ASSERT
  #undef MEGATECH_PRECONDITION_MSG
  #undef MEGATECH_PRECONDITION_MSG_PRINTF
  #undef MEGATECH_PRECONDITION_MSG_FORMAT
  #undef MEGATECH_PRECONDITION
  #undef MEGATECH_POSTCONDITION_MSG
  #undef MEGATECH_POSTCONDITION_MSG_PRINTF
  #undef MEGATECH_POSTCONDITION_MSG_FORMAT
  #undef MEGATECH_POSTCONDITION

  #include <cstddef>

  #include <source_location>
  #include <string_view>
  #include <format>
#endif

/// @cond
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

#ifndef MEGATECH_ASSERTIONS_ENABLED
  #if !defined(MEGATECH_ASSERTIONS_DISABLED) && !defined(NDEBUG)
    #define MEGATECH_ASSERTIONS_ENABLED (1)
  #endif
#endif

#if defined(MEGATECH_ASSERTIONS_ENABLED) && defined(MEGATECH_ASSERTIONS_DISABLED)
  #error "Assertions cannot be enabled and disabled at the same time."
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
/// @endcond

/// @cond INTERNAL
namespace megatech::internal::base {

  /**
   * @brief Emit a diagnostic message and abort the program.
   * @details This function sends assertion diagnostics to standard error and aborts the program. It never returns.
   *          Some effort is made to ensure that this function handles weird cases, but generally you can't expect it
   *          to work if you're out of memory or standard error is closed.
   * @param location The location at which the program failed.
   * @param message A diagnostic message formatted as a NUL-terminated string. This function takes ownership of the
   *                string and is responsible for freeing it.
   */
  [[noreturn]]
  void dispatch_assertion_failure(const std::source_location& location, char* const message) noexcept;

  /**
   * @brief Emit a diagnostic message indicating an assertion failed, but the corresponding message couldn't be
   *        written.
   * @details This function is called whenever an error occurs in assertion processing. Basically, it should never
   *          happen. However, if an invalid format is used or the system is out of memory this will attempt to provide
   *          some information nonetheless. This function never returns
   * @param location The location at which the program failed.
   */
  [[noreturn]]
  void dispatch_assertion_error(const std::source_location& location) noexcept;

#ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
  /**
   * @brief Internal handler for "format"-style assertions.
   * @param location The location at which the assertion is found.
   * @param condition Whether or not the assertion passed. If this is false the program will be aborted with a
   *                  diagnostic.
   * @param format The format of the diagnostic message associated with the assertion.
   * @param message_size The size of the formatted message without a NUL-terminator.
   * @param args A type-erased collection of format arguments.
   */
  void debug_assertion_format(const std::source_location& location, const bool condition,
                              const std::string_view& format, const std::size_t message_size,
                              std::format_args&& args) noexcept;
#endif

}
/// @endcond

namespace megatech {

  /**
   * @brief Process an assertion using the "printf"-style formatting syntax.
   * @param location The location at which the assertion is found.
   * @param condition Whether or not the assertion passed. If this is false the program will be aborted with a
   *                  diagnostic.
   * @param format The format of the diagnostic message associated with the assertion.
   * @param ... 0 or more formatting arguments to use when rendering the diagnostic message.
   */
  void debug_assertion_printf(const std::source_location& location, const bool condition,
                              const char *const format, ...) noexcept;

#ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
  /**
   * @brief Process an assertion using the "format"-style formatting syntax.
   * @param location The location at which the assertion is found.
   * @param condition Whether or not the assertion passed. If this is false the program will be aborted with a
   *                  diagnostic.
   * @param format The format of the diagnostic message associated with the assertion.
   * @param args 0 or more formatting arguments to use when rendering the diagnostic message.
   */
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

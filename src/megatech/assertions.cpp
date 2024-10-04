/**
 * @file assertions.cpp
 * @brief Run-Time Assertion Implementation
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2024
 */
#include "megatech/assertions.hpp"

#include "config.hpp"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include <array>
#include <iterator>

#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
  #define MEGATECH_ASSERTIONS_PT_DECL static thread_local

  #include <mutex>
  #include <condition_variable>
#else
  #define MEGATECH_ASSERTIONS_PT_DECL static
#endif

// This is the prefix format for assertion diagnostics.
// It needs to contain the file name, line number, and procedure name at the location where an assertion failed.
#define PREFIX_FORMAT "%s:%" PRIu32 ": %s: "

namespace {

#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
  static std::mutex sg_mtx{ };
  static std::condition_variable sg_condition{ };
  static std::size_t sg_unresolved_failures{ 0 };
#endif

#if CONFIG_ASSERTION_BUFFER_CHAR_SIZE
  // A per-thread buffer for formatted assertion messages.
  // Creating lots of threads, therefore, will consume a considerable amount of memory if this is very large.
  // The default size is 4001 characters (i.e., enough for 1000 4-byte UTF-8 code points and a NUL terminator) or
  // roughly 4KiB. If this memory cost is an issue you should tune the library configuration to meet your needs.
  // If either CONFIG_ASSERTION_BUFFER_SIZE or CONFIG_MAX_CODE_POINT_SIZE are 0, then this is disabled.
  MEGATECH_ASSERTIONS_PT_DECL std::array<char, CONFIG_ASSERTION_BUFFER_CHAR_SIZE> pt_assertion_buffer{ };

  // This is a truncating output iterator type. Basically, it writes into a buffer until some size has been exceeded.
  // After that, the incoming output is simply discarded. This behaves similiarly to types like
  // std::back_insert_iterator.
  template <typename Type>
  class truncating_iterator final {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = void;
    using pointer = void;
    using reference = void;
    using iterator_category = std::output_iterator_tag;
  private:
    std::size_t m_size{ };
    std::size_t m_current{ };
    Type* m_data{ };
  public:
    constexpr truncating_iterator(Type *const data, const std::size_t size) : m_size{ size }, m_data{ data } { }
    constexpr truncating_iterator(const truncating_iterator& other) = default;
    constexpr truncating_iterator(truncating_iterator&& other) = default;

    constexpr ~truncating_iterator() noexcept = default;

    constexpr truncating_iterator& operator=(const truncating_iterator& rhs) = default;
    constexpr truncating_iterator& operator=(truncating_iterator&& rhs) = default;
    constexpr truncating_iterator& operator=(const Type& rhs) {
      if (m_current < m_size)
      {
        m_data[m_current++] = rhs;
      }
      return *this;
    }

    constexpr truncating_iterator& operator++() {
      return *this;
    }

    constexpr truncating_iterator operator++(int) {
      return *this;
    }

    constexpr truncating_iterator& operator*() {
      return *this;
    }
  };
#endif

}

namespace megatech::internal::base {

#if CONFIG_ASSERTION_BUFFER_CHAR_SIZE
  void dispatch_assertion_failure_with_message(const std::source_location& location, const char* expression,
                                               const char* message) noexcept {
#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
      try
#endif
      {
#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
        {
          auto lock = std::lock_guard<std::mutex>{ sg_mtx };
          // We're in the middle of resolving the assertion.
          ++sg_unresolved_failures;
        }
#endif
        // If the caller didn't provide an expression, it should be an empty string.
        if (!expression)
        {
          expression = "";
        }
        if (!message)
        {
          message = "";
        }
        {
#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
          auto lock = std::lock_guard<std::mutex>{ sg_mtx };
#endif
          std::fprintf(stderr, PREFIX_FORMAT "The assertion \"%s\" failed with the message \"%s\".\n",
                       location.file_name(), location.line(), location.function_name(), expression, message);
        }
      }
#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
      catch (...)
      {
        dispatch_assertion_failure_with_error(location, expression, "A concurrency error occurred while processing an "
                                                                    "assertion failure. Locking the assertion mutex "
                                                                    "may have failed.");
      }
      try
      {
        {
          // Although the lock could be released before notifying, it isn't maximally safe. Assertions aren't a
          // performance feature. The program is about to fail hard, so this is fine.
          auto lock = std::lock_guard<std::mutex>{ sg_mtx };
          if (sg_unresolved_failures == 1)
          {
            // Notifying will wake up all waiting threads. Then they will immediately block on the lock that they can't
            // acquire.
            sg_condition.notify_all();
          }
          // The assertion is resolved.
          --sg_unresolved_failures;
        }
        // Wait for any pending assertions to resolve.
        auto lock = std::unique_lock<std::mutex>{ sg_mtx };
        sg_condition.wait(lock, [](){ return !sg_unresolved_failures; });
      }
      // Ignore post-processing errors, just abort.
      catch (...) { }
#endif
      std::abort();
  }
#endif

  void dispatch_assertion_failure(const std::source_location& location, const char* expression) noexcept {
#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
      try
#endif
      {
#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
        {
          auto lock = std::lock_guard<std::mutex>{ sg_mtx };
          ++sg_unresolved_failures;
        }
#endif
        if (!expression)
        {
          expression = "";
        }
        {
#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
          auto lock = std::lock_guard<std::mutex>{ sg_mtx };
#endif
          std::fprintf(stderr, PREFIX_FORMAT "The assertion \"%s\" failed.\n", location.file_name(), location.line(),
                       location.function_name(), expression);
        }
      }
#ifdef CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED
      catch (...)
      {
        dispatch_assertion_failure_with_error(location, expression, "A concurrency error occurred while processing an "
                                                                    "assertion failure. Llocking the assertion mutex "
                                                                    "may have failed.");
      }
      try
      {
        {
          auto lock = std::lock_guard<std::mutex>{ sg_mtx };
          if (sg_unresolved_failures == 1)
          {
            sg_condition.notify_all();
          }
          --sg_unresolved_failures;
        }
        auto lock = std::unique_lock<std::mutex>{ sg_mtx };
        sg_condition.wait(lock, [](){ return !sg_unresolved_failures; });
      }
      catch (...) { }
#endif
      std::abort();
  }

  void dispatch_assertion_failure_with_error(const std::source_location& location, const char* expression,
                                             const char* error) noexcept {
    if (!expression)
    {
      expression = "";
    }
    if (!error)
    {
      error = "";
    }
    std::fprintf(stderr, PREFIX_FORMAT "The assertion \"%s\" failed.\nThe following error occurred during assertion "
                         "failure processing: \"%s\"\n", location.file_name(), location.line(),
                 location.function_name(), expression, error);
    std::abort();
  }

#ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
  void debug_assertion_format(const std::source_location& location, const bool condition, const char *const expression,
                              const std::string_view& format, std::format_args&& args) noexcept {
// If the assertion buffer is disabled, immediately defer to a bufferless assertion.
#if CONFIG_ASSERTION_BUFFER_CHAR_SIZE
    if (!condition)
    {
      try
      {
        std::vformat_to(truncating_iterator<char>{ pt_assertion_buffer.data(), pt_assertion_buffer.size() - 1 },
                        format, args);
      }
      catch (const std::format_error& err)
      {
        dispatch_assertion_failure_with_error(location, expression, "A formatting error occurred.");
      }
      catch (...)
      {
        dispatch_assertion_failure_with_error(location, expression, "An unknown error occurred while formatting.");
      }
      dispatch_assertion_failure_with_message(location, expression, pt_assertion_buffer.data());
    }
#else
    (void) format;
    (void) args;
    debug_assertion(location, condition, expression);
#endif
  }
#endif

}

namespace megatech {

  void debug_assertion(const std::source_location& location, const bool condition,
                       const char *const expression) noexcept {
    if (!condition)
    {
      internal::base::dispatch_assertion_failure(location, expression);
    }
  }

  void debug_assertion_printf(const std::source_location& location, const bool condition, const char *const expression,
                              const char *const format, ...) noexcept {
#if CONFIG_ASSERTION_BUFFER_CHAR_SIZE
    if (!condition)
    {
      std::va_list args;
      va_start(args, format);
      const auto res = std::vsnprintf(pt_assertion_buffer.data(), pt_assertion_buffer.size(), format, args);
      va_end(args);
      if (res <= 0)
      {
        internal::base::dispatch_assertion_failure_with_error(location, expression, "A formatting error occurred.");
      }
      internal::base::dispatch_assertion_failure_with_message(location, expression, pt_assertion_buffer.data());
    }
#else
    (void) format;
    debug_assertion(location, condition, expression);
#endif
  }

}

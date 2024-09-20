#include "megatech/assertions.hpp"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#define MEGATECH_ASSERTION_FAILURE_FORMATTER ("%s:%" PRIu32 ": An assertion failed with the message \"%s\".\n")
#define MEGATECH_ASSERTION_ERROR_FORMATTER \
  ("%s:%" PRIu32 ": An assertion failed, but its message could not be formatted.\n")

namespace megatech::internal::base {

  void dispatch_assertion_failure(const std::source_location& location, char* const message) noexcept {
      // std::fprintf can, of course, fail. However, if this happens we are so screwed that it's not recoverable here.
      const auto res = std::fprintf(stderr, MEGATECH_ASSERTION_FAILURE_FORMATTER, location.file_name(),
                                    location.line(), message);
      std::free(message);
      if (res < 0)
      {
        // This is really the best I can do. If there was an error then, perhaps, perror will still function as
        // intended. Realistically, if you're in a scenario where you can't write to stderr (and you haven't
        // purposefully closed it) things are pretty bad.
        std::perror("megatech-assertions");
      }
      std::abort();
  }

  void dispatch_assertion_error(const std::source_location& location) noexcept {
    const auto res = std::fprintf(stderr, MEGATECH_ASSERTION_ERROR_FORMATTER, location.file_name(), location.line());
    if (res < 0)
    {
      std::perror("megatech-assertions");
    }
    std::abort();
  }

#ifdef MEGATECH_ASSERTIONS_FORMAT_AVAILABLE
  void debug_assertion_format(const std::source_location& location, const bool condition,
                              const std::string_view& format, const std::size_t message_size,
                              std::format_args&& args) noexcept {
    if (!condition)
    {
      auto message = static_cast<char*>(nullptr);
      try
      {
        // The reason to do this is to ensure that there is exactly one call for printing messages.
        // Although std::string is "safer" this is a trivial memory operation.
        // internal::base::dispatch_assertion_failure will always call std::free(message).
        message = reinterpret_cast<char*>(std::calloc(message_size + 1, sizeof(char)));
        if (!message)
        {
          dispatch_assertion_error(location);
        }
        std::vformat_to(&message[0], format, args);
        dispatch_assertion_failure(location, message);
      }
      catch (...)
      {
        if (message)
        {
          std::free(message);
        }
        dispatch_assertion_error(location);
      }
    }
  }
#endif

}

namespace megatech {

  void debug_assertion_printf(const std::source_location& location, const bool condition,
                              const char *const format, ...) noexcept {
    if (!condition)
    {
      // va_lists require this declaration style specifically.
      std::va_list args;
      // Calculate and allocate space for message.
      va_start(args, format);
      std::va_list args_cp;
      // The next call will destroy args so we need another copy.
      va_copy(args_cp, args);
      const auto sz = std::vsnprintf(nullptr, 0, format, args);
      if (sz < 0)
      {
        va_end(args);
        va_end(args_cp);
        internal::base::dispatch_assertion_error(location);
      }
      va_end(args);
      // +1 for the null terminator
      auto message = reinterpret_cast<char*>(std::calloc(sz + 1, sizeof(char)));
      if (!message)
      {
        va_end(args_cp);
        internal::base::dispatch_assertion_error(location);
      }
      if (auto res = std::vsnprintf(message, sz + 1, format, args_cp); res < 0)
      {
        va_end(args_cp);
        std::free(message);
        internal::base::dispatch_assertion_error(location);
      }
      va_end(args_cp);
      internal::base::dispatch_assertion_failure(location, message);
    }
  }

}

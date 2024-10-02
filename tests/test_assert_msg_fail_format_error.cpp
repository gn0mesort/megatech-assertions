#include <megatech/assertions.hpp>

struct foo final {};

template <>
struct std::formatter<foo, char> final {
  template <typename ParseContext>
  constexpr ParseContext::iterator parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  FormatContext::iterator format(foo, FormatContext&) const {
    throw std::format_error{ "" };
  }
};

int main() {
  const auto f = foo{ };
  MEGATECH_ASSERT_MSG_FORMAT(1 != 1, "{}", f);
  return 0;
}

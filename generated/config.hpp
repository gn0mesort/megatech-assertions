/**
 * @file config.hpp
 * @brief Megatech Assertions Configuration Header
 * @details This requires Meson to properly configure.
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2024
 */
#ifndef CONFIG_HPP
#define CONFIG_HPP

#mesondefine CONFIG_MAX_CODE_POINT_SIZE
#mesondefine CONFIG_ASSERTION_BUFFER_SIZE
#mesondefine CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED

#if (CONFIG_MAX_CODE_POINT_SIZE * CONFIG_ASSERTION_BUFFER_SIZE) != 0
  #define CONFIG_ASSERTION_BUFFER_CHAR_SIZE (CONFIG_MAX_CODE_POINT_SIZE * CONFIG_ASSERTION_BUFFER_SIZE + 1)
#else
  #define CONFIG_ASSERTION_BUFFER_CHAR_SIZE (0)
#endif

#endif

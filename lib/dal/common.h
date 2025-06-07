#pragma once

#include <iostream>

namespace dal {

namespace common {

class DebugLogWriter {
public:
    template <typename T>
    auto operator<<(T const &t) -> DebugLogWriter & {
#ifdef DEBUG
        std::cerr << t;
#endif
        return *this;
    }

    typedef std::basic_ostream<char, std::char_traits<char>> CoutType;
    typedef CoutType &(*StandardEndLine)(CoutType &);

    DebugLogWriter &operator<<(StandardEndLine manip) {
#ifdef DEBUG
        std::cerr << manip;
#endif
        return *this;
    }
};

// NOLINTNEXTLINE
auto assert(bool cmp) -> void {
    if (!cmp) {
        abort();
    }
}

} // namespace common

} // namespace dal

#define LOG() ::dal::common::DebugLogWriter()

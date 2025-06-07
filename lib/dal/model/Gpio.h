#pragma once

#include <cstdint>

namespace dal {

namespace model {

/**!
 * \brief Simple Gpio definition
 */
class Gpio {
public:
    uint32_t line = 0;
};

} // namespace model

} // namespace dal

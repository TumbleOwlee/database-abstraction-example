#pragma once

#include <cstdint>
#include <string>

namespace dal {

namespace model {

/**!
 * \brief Basic User definition
 */
struct User {
    std::string name;
    uint32_t age;
};

} // namespace model

} // namespace dal

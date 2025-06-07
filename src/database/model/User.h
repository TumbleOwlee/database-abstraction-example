#pragma once

#include <cstdint>
#include <string>

namespace persistence {

namespace model {

/**!
 * \brief Basic User definition
 */
struct User {
    std::string name;
    uint32_t age;
};

} // namespace model

} // namespace persistence

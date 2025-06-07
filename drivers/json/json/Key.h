#pragma once

#include "dal/interfaces/Key.h"

#include <nlohmann/json.hpp>

namespace dal {

namespace drivers {

namespace json {

/**!
 * \brief json object key definition
 */
class Key : public ::dal::interface::Key {
public:
    /**!
     * \brief Creates a key represented by the json id
     *
     * \param id The json id of the object
     */
    Key(::nlohmann::basic_json<>::size_type id) : _id(id) {}

    /**!
     * \brief Returns the internal json id
     *
     * \return The json id
     */
    auto get() -> ::nlohmann::basic_json<>::size_type { return _id; }

private:
    ::nlohmann::basic_json<>::size_type _id;
};

} // namespace json

} // namespace drivers

} // namespace dal

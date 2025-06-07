#pragma once

#include <hiberlite.h>

#include "../interfaces/Key.h"

namespace persistence {

namespace hiberlite {

/**!
 * \brief Hiberlite object key definition
 */
class Key : public ::persistence::interface::Key {
public:
    /**!
     * \brief Creates a key represented by the hiberlite id
     *
     * \param id The hiberlite id of the object
     */
    Key(::hiberlite::sqlid_t id) : _id(id) {}

    /**!
     * \brief Returns the internal hiberlite id
     *
     * \return The hiberlite id
     */
    auto get() -> ::hiberlite::sqlid_t { return _id; }

private:
    ::hiberlite::sqlid_t _id;
};

} // namespace hiberlite

} // namespace persistence

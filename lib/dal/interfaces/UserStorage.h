#pragma once

#include "../model/User.h"
#include "Key.h"

#include <memory>

namespace dal {

namespace interface {

/**!
 * \brief Interface representing the user specific view on the storage
 */
class UserStorage {
public:
    /**!
     * \brief Persist a user
     *
     * \param user The user to store
     *
     * \return The key handle associated with the stored user
     */
    virtual auto persist(::dal::model::User &&user) -> std::shared_ptr<::dal::interface::Key> = 0;

    /**!
     * \brief Load a user by its key id
     *
     * \param id The key id associated with the user
     *
     * \return The user associated with the given id
     */
    virtual auto load(std::shared_ptr<::dal::interface::Key> id) -> ::dal::model::User = 0;
};

} // namespace interface

} // namespace dal

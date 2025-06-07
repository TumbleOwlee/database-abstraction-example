#pragma once

#include "../model/User.h"
#include "Key.h"

#include <memory>

namespace persistence {

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
    virtual auto persist(::persistence::model::User &&user) -> std::shared_ptr<::persistence::interface::Key> = 0;

    /**!
     * \brief Load a user by its key id
     *
     * \param id The key id associated with the user
     *
     * \return The user associated with the given id
     */
    virtual auto load(std::shared_ptr<::persistence::interface::Key> id) -> ::persistence::model::User = 0;
};

} // namespace interface

} // namespace persistence

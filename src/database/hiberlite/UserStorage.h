#pragma once

#include "Key.h"

#include "../interfaces/IUserStorage.h"
#include "../model/User.h"

#include <hiberdefs.h>
#include <hiberlite.h>
#include <iostream>

#include "../common.h"

namespace persistence {

namespace hiberlite {

/**!
 * \brief User definition of the database representation
 *
 * \detail This user is a specialization of the model 'User' and extends
 *         it by providing the necessary implementation of methods required
 *         by the underlying backend (hiberlite)
 */
class User : public persistence::model::User {
private:
    friend class ::hiberlite::access;

    /**!
     * \brief Hiberlite specific required method to create columns
     */
    template <class Archive>
    void hibernate(Archive &archive) {
        archive & ::hiberlite::sql_nvp<decltype(name)>("name", name);
        archive & ::hiberlite::sql_nvp<decltype(age)>("age", age);
    }

public:
    /**!
     * \brief Consturctor
     */
    User() : persistence::model::User() {}

    /**!
     * \brief Consturctor
     *
     * \details Creates a hiberlite user from the model user
     *
     * \param user The user as given by the model
     */
    User(persistence::model::User &&user) : persistence::model::User(user) {}
};

/**!
 * \brief UserStorage providing the user specific database operations
 */
class UserStorage : virtual public persistence::interface::IUserStorage {
public:
    /**!
     * \brief Create a user storage view
     *
     * \param database  Underlying database backend
     */
    UserStorage(::hiberlite::Database &database) : _database(database) {}

    /**!
     * \brief Persists a user by storing it in the database
     *
     * \param user The user to store
     *
     * \return Key handle representing the stored user
     */
    auto persist(persistence::model::User &&user) -> std::shared_ptr<persistence::interface::IKey> override {
        LOG() << "Persisting user... ";
        auto bean = _database.copyBean(User(std::move(user)));
        LOG() << bean.get_id() << std::endl;
        return std::make_shared<persistence::hiberlite::Key>(bean.get_id());
    }

    /**!
     * \brief Loads a user from the database
     *
     * \param id  The id of the stored user that should be retrieved
     *
     * \throw std::invalid_argument if key isn't valid for the hiberlite backend
     *
     * \return The loaded user
     */
    auto load(std::shared_ptr<persistence::interface::IKey> id) -> persistence::model::User override {
        auto ptr = dynamic_cast<persistence::hiberlite::Key *>(id.get());
        if (ptr == nullptr) {
            throw std::invalid_argument("Provided pointer isn't valid key.");
        }
        auto sqlid = ptr->get();
        LOG() << "Load user " << sqlid << "..." << std::endl;
        auto user = _database.loadBean<User>(sqlid);
        return persistence::model::User(*user.get_object()->get());
    }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace persistence

namespace hiberlite {

/**!
 * \brief Hiberlite specific functionality required to be able to store users
 */
template <>
// NOLINTNEXTLINE
std::string Database::getClassName<persistence::hiberlite::User>() {
    std::string temp("User");
    std::replace(temp.begin(), temp.end(), ':', '_');
    return temp;
}

} // namespace hiberlite

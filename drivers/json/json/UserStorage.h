#pragma once

#include "JsonHandle.h"
#include "Key.h"

#include "dal/interfaces/UserStorage.h"
#include "dal/model/User.h"

#include <iostream>

#include "dal/common.h"

namespace dal {

namespace drivers {

namespace json {

class User : public ::dal::model::User {
public:
    User(::dal::model::User &&user) : ::dal::model::User(user) {}

    auto toJson() -> ::nlohmann::json {
        return {
            {"name", name},
            {"age",  age }
        };
    }

    static auto fromJson(::nlohmann::json const &obj) -> User {
        return User(::dal::model::User{obj["name"], obj["age"]});
    }
};

/**!
 * \brief UserStorage providing the user specific database operations
 */
class UserStorage : virtual public ::dal::interface::UserStorage {
public:
    /**!
     * \brief Create a user storage view
     *
     * \param database  Underlying database backend
     */
    UserStorage(::dal::drivers::json::JsonHandle &json) : _json(json) {}

    /**!
     * \brief Persists a user by storing it in the database
     *
     * \param user The user to store
     *
     * \return Key handle representing the stored user
     */
    auto persist(::dal::model::User &&user) -> std::shared_ptr<::dal::interface::Key> override {
        LOG() << "Persisting user...";
        auto id = _json.persist("user", User(std::move(user)));
        LOG() << id << std::endl;
        return std::make_shared<::dal::drivers::json::Key>(id);
    }

    /**!
     * \brief Loads a user from the database
     *
     * \param id  The id of the stored user that should be retrieved
     *
     * \throw std::invalid_argument if key isn't valid for the json backend
     *
     * \return The loaded user
     */
    auto load(std::shared_ptr<::dal::interface::Key> id) -> ::dal::model::User override {
        auto ptr = dynamic_cast<::dal::drivers::json::Key *>(id.get());
        if (ptr == nullptr) {
            throw std::invalid_argument("Provided pointer isn't valid key.");
        }
        auto index = ptr->get();
        LOG() << "Load user " << index << "..." << std::endl;
        auto user = _json.load<User>("user", index);
        return user;
    }

private:
    ::dal::drivers::json::JsonHandle &_json;
};

} // namespace json

} // namespace drivers

} // namespace dal

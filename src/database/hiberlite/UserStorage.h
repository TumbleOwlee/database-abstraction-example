#pragma once

#include "../IStorage.h"
#include "../model/User.h"
#include <hiberlite.h>
#include <iostream>

namespace persistence {

namespace hiberlite {

class UserStorage : virtual public persistence::storage::IUserStorage {
public:
    UserStorage(::hiberlite::Database &database) : _database(database) {}

    auto persist(persistence::model::User const &user) -> void override {
        std::cerr << "Persisting user..." << std::endl;
    }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace persistence

#pragma once

#include "../IStorage.h"
#include "../model/User.h"
#include <hiberlite.h>
#include <iostream>

namespace persistence {

namespace hiberlite {

class UserStorage : public virtual persistence::storage::IUserStorage {
public:
    UserStorage(::hiberlite::Database &database) : _database(database) {}

    auto persistUser(User const &user) -> void override { std::cerr << "Persisting user..." << std::endl; }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace persistence

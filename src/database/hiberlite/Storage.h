#pragma once

#include "GpioStorage.h"
#include "LogStorage.h"
#include "UserStorage.h"
#include <string>

namespace persistence {

namespace hiberlite {

class Storage : public virtual persistence::storage::IStorage,
                public persistence::hiberlite::UserStorage,
                public persistence::hiberlite::LogStorage,
                public persistence::hiberlite::GpioStorage {
public:
    Storage(std::string const &path)
        : persistence::hiberlite::UserStorage(_database), persistence::hiberlite::LogStorage(_database),
          persistence::hiberlite::GpioStorage(_database) {}

private:
    ::hiberlite::Database _database;
};

} // namespace hiberlite

} // namespace persistence

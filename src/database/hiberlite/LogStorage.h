#pragma once

#include "../IStorage.h"
#include "../model/Log.h"
#include <hiberlite.h>
#include <iostream>

namespace persistence {

namespace hiberlite {

class LogStorage : public virtual persistence::storage::ILogStorage {
public:
    LogStorage(::hiberlite::Database &database) : _database(database) {}

    auto persistLog(Log const &log) -> void override { std::cerr << "Persisting log..." << std::endl; }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace persistence

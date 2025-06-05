#pragma once

#include "../IStorage.h"
#include "../model/Gpio.h"
#include <hiberlite.h>
#include <iostream>

namespace persistence {

namespace hiberlite {

class GpioStorage : virtual public persistence::storage::IGpioStorage {
public:
    GpioStorage(::hiberlite::Database &database) : _database(database) {}

    auto persist(persistence::model::Gpio const &gpio) -> void override {
        std::cerr << "Persisting gpio..." << std::endl;
    }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace persistence

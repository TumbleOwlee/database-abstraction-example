#pragma once

#include "../interfaces/IGpioStorage.h"
#include "../interfaces/IKey.h"
#include "../model/Gpio.h"
#include <hiberlite.h>
#include <iostream>

namespace persistence {

namespace hiberlite {

/**!
 * \brief GpioStorage providing the gpio specific database operations
 */
class GpioStorage : virtual public persistence::interface::IGpioStorage {
public:
    /**!
     * \brief Create a gpio storage view
     *
     * \param database  Underlying database backend
     */
    GpioStorage(::hiberlite::Database &database) : _database(database) {}

    /**!
     * \brief Persists a gpio by storing it in the database
     *
     * \param gpio The gpio to store
     *
     * \return Key handle representing the stored gpio
     */
    auto persist(persistence::model::Gpio &&gpio) -> std::shared_ptr<persistence::interface::IKey> override {
        std::cerr << "Persisting gpio..." << std::endl;
        return 0;
    }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace persistence

#pragma once

#include "../interfaces/ILogStorage.h"
#include "../model/Log.h"
#include <hiberlite.h>
#include <iostream>

namespace persistence {

namespace hiberlite {

/**!
 * \brief LogStorage providing the log specific database operations
 */
class LogStorage : virtual public persistence::interface::ILogStorage {
public:
    /**!
     * \brief Create a log storage view
     *
     * \param database  Underlying database backend
     */
    LogStorage(::hiberlite::Database &database) : _database(database) {}

    /**!
     * \brief Persists a log by storing it in the database
     *
     * \param log The log to store
     *
     * \return Key handle representing the stored log
     */
    auto persist(persistence::model::Log &&log) -> std::shared_ptr<persistence::interface::IKey> override {
        std::cerr << "Persisting log..." << std::endl;
        return 0;
    }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace persistence

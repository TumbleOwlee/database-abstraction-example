#pragma once

#include "../interfaces/Transaction.h"
#include "GpioStorage.h"
#include "LogStorage.h"
#include "UserStorage.h"

#include <string>

#include "../common.h"

namespace persistence {

namespace hiberlite {

/**!
 * \brief Storage implementation for hiberlite backend
 */
class Storage final : virtual public ::persistence::interface::Storage,
                      virtual public ::persistence::hiberlite::UserStorage,
                      virtual public ::persistence::hiberlite::LogStorage,
                      virtual public ::persistence::hiberlite::GpioStorage {
public:
    /**!
     * \brief Transaction instance type
     *
     * \details You can specify a custom transaction type if necessary. If no special handling is necessary
     *          you can skip the implementation of your own type and simply use the provided default implementation
     *          of the interface. The default utilizes the implementation of `startTransaction()`, `submitTransaction()`
     *          and `rollbackTransaction()` of ::persistence::interfaces::Storage.
     */
    using Transaction = ::persistence::defaults::Transaction;

    /**!
     * \brief Create a new storage using the hiberlite backend
     *
     * \param path  Path to the database file
     */
    Storage(std::string const &path)
        : ::persistence::hiberlite::UserStorage(_database), ::persistence::hiberlite::LogStorage(_database),
          ::persistence::hiberlite::GpioStorage(_database) {
        LOG() << "Create model..." << std::endl;
        _database.open(path);
        _database.registerBeanClass<::persistence::hiberlite::User>();
        _database.registerBeanClass<::persistence::hiberlite::Log>();
        _database.registerBeanClass<::persistence::hiberlite::Gpio>();
        try {
            _database.createModel();
        } catch (::hiberlite::database_error &e) {
            LOG() << "Model already exists. Skip 'CREATE TABLE'." << std::endl;
        }
    }

    /**!
     * \brief Destructor that closes the database
     */
    ~Storage() override { _database.close(); }

    auto startTransaction() -> void override { _database.startTransaction(); }
    auto commitTransaction() -> void override { _database.commitTransaction(); }
    auto rollbackTransaction() -> void override { _database.rollbackTransaction(); }

private:
    ::hiberlite::Database _database;
};

} // namespace hiberlite

} // namespace persistence

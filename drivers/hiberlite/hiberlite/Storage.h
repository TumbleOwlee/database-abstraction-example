#pragma once

#include "GpioStorage.h"
#include "LogStorage.h"
#include "UserStorage.h"

#include "dal/common.h"
#include "dal/interfaces/Transaction.h"

#include <string>

namespace dal {

namespace drivers {

namespace hiberlite {

/**!
 * \brief Storage implementation for hiberlite backend
 */
class Storage final : virtual public ::dal::interface::Storage,
                      virtual public ::dal::drivers::hiberlite::UserStorage,
                      virtual public ::dal::drivers::hiberlite::LogStorage,
                      virtual public ::dal::drivers::hiberlite::GpioStorage {
public:
    /**!
     * \brief Transaction instance type
     *
     * \details You can specify a custom transaction type if necessary. If no special handling is necessary
     *          you can skip the implementation of your own type and simply use the provided default implementation
     *          of the interface. The default utilizes the implementation of `startTransaction()`, `submitTransaction()`
     *          and `rollbackTransaction()` of ::dal::interfaces::Storage.
     */
    using Transaction = ::dal::defaults::Transaction;

    /**!
     * \brief Create a new storage using the hiberlite backend
     *
     * \param path  Path to the database file
     */
    Storage(std::string const &path)
        : ::dal::drivers::hiberlite::UserStorage(_database), ::dal::drivers::hiberlite::LogStorage(_database),
          ::dal::drivers::hiberlite::GpioStorage(_database) {
        LOG() << "Create model..." << std::endl;
        _database.open(path);
        _database.registerBeanClass<::dal::drivers::hiberlite::User>();
        _database.registerBeanClass<::dal::drivers::hiberlite::Log>();
        _database.registerBeanClass<::dal::drivers::hiberlite::Gpio>();
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

} // namespace drivers

} // namespace dal

#pragma once

#include "GpioStorage.h"
#include "JsonHandle.h"
#include "LogStorage.h"
#include "UserStorage.h"

#include "dal/interfaces/Transaction.h"

#include <string>

namespace dal {

namespace drivers {

namespace json {

/**!
 * \brief Storage implementation for json backend
 */
class Storage final : virtual public ::dal::interface::Storage,
                      virtual public ::dal::drivers::json::UserStorage,
                      virtual public ::dal::drivers::json::LogStorage,
                      virtual public ::dal::drivers::json::GpioStorage {
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
     * \brief Create a new storage using the json backend
     *
     * \param path  Path to the database file
     */
    Storage(std::string const &path)
        : ::dal::drivers::json::UserStorage(_handle), ::dal::drivers::json::LogStorage(_handle),
          ::dal::drivers::json::GpioStorage(_handle), _handle(path) {}

    /**!
     * \brief Destructor that closes the database
     */
    ~Storage() override {}

    auto startTransaction() -> void override { _handle.startTransaction(); }

    auto commitTransaction() -> void override { _handle.commitTransaction(); }

    auto rollbackTransaction() -> void override { _handle.rollbackTransaction(); }

private:
    ::dal::drivers::json::JsonHandle _handle;
};

} // namespace json

} // namespace drivers

} // namespace dal

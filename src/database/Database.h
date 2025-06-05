#pragma once

#include "IStorage.h"
#include "ITransaction.h"

#include <functional>
#include <memory>

namespace persistence {

class Database {
public:
    /**!
     * Create database using the templated backend
     *
     * \param args  Arguments passed to the backend constructor
     *
     * \return Created database
     */
    template <class Storage, typename... Args>
    static auto create(Args... args) -> Database {
        auto create = [](std::shared_ptr<persistence::storage::IStorage> storage)
            -> std::unique_ptr<persistence::storage::ITransaction> {
            return std::make_unique<typename Storage::Transaction>(storage);
        };
        return Database(std::shared_ptr<persistence::storage::IStorage>(new Storage(args...)), create);
    }

    /**!
     * Start new transaction
     *
     * \return Created transaction
     */
    auto newTransaction() -> std::unique_ptr<persistence::storage::ITransaction> { return _create(_storage); }

    /**!
     * Returns access interface to tables
     *
     * \return Database table view
     */
    template <class Table>
    constexpr auto get() const -> std::shared_ptr<Table> {
        return _storage;
    }

private:
    using CreateFn = std::function<
        auto(std::shared_ptr<persistence::storage::IStorage>)->std::unique_ptr<persistence::storage::ITransaction>>;

    // Backend handle
    std::shared_ptr<persistence::storage::IStorage> _storage;
    // Function to create transaction
    CreateFn _create;

    /**!
     * Create database using the backend
     *
     * \param storage  Backend handle
     * \param create   Custom function to create transaction
     */
    Database(std::shared_ptr<persistence::storage::IStorage> storage, CreateFn create)
        : _storage(storage), _create(create) {}
};

} // namespace persistence

#pragma once

#include "interfaces/Storage.h"
#include "interfaces/Transaction.h"

#include <functional>
#include <memory>

namespace persistence {

class Database {
public:
    /**!
     * \brief Create database using the templated backend
     *
     * \param args  Arguments passed to the backend constructor
     *
     * \return Created database
     */
    template <class Storage, typename... Args>
    static auto create(Args... args) -> Database {
        auto create = [](std::shared_ptr<persistence::interface::IStorage> storage)
            -> std::unique_ptr<persistence::interface::ITransaction> {
            return std::unique_ptr<typename Storage::Transaction>(new typename Storage::Transaction(storage));
        };
        return Database(std::shared_ptr<persistence::interface::IStorage>(new Storage(args...)), create);
    }

    /**!
     * \brief Start new transaction
     *
     * \return Created transaction
     */
    auto newTransaction() -> std::unique_ptr<persistence::interface::ITransaction> { return _create(_storage); }

    /**!
     * \brief Returns access interface to tables
     *
     * \return Database table view
     */
    template <class View>
    constexpr auto get() const -> std::shared_ptr<View> {
        return _storage;
    }

private:
    using CreateFn = std::function<
        auto(std::shared_ptr<persistence::interface::IStorage>)->std::unique_ptr<persistence::interface::ITransaction>>;

    // Backend handle
    std::shared_ptr<persistence::interface::IStorage> _storage;
    // Function to create transaction
    CreateFn _create;

    /**!
     * \brief Create database using the backend
     *
     * \param storage  Backend handle
     * \param create   Custom function to create transaction
     */
    Database(std::shared_ptr<persistence::interface::IStorage> storage, CreateFn create)
        : _storage(storage), _create(create) {}
};

} // namespace persistence

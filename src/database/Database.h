#pragma once

#include "IStorage.h"
#include <memory>

namespace persistence {

class Database {
public:
    class Transaction {
    public:
    };

    template <class Storage, typename... Args>
    static auto create(Args... args) -> Database {
        return Database(std::shared_ptr<persistence::storage::IStorage>(new Storage(args...)));
    }

    auto startTransaction() -> Transaction;

    auto endTransaction(Transaction &&transaction) -> bool;

    template <class Table>
    constexpr auto get() -> std::shared_ptr<Table> {
        return _storage;
    }

private:
    std::shared_ptr<persistence::storage::IStorage> _storage;

    Database(std::shared_ptr<persistence::storage::IStorage> storage) : _storage(storage) {}
};

} // namespace persistence

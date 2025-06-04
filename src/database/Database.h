#pragma once

#include "IStorage.h"
#include <memory>

namespace persistence {

template <class Storage>
class Database {
public:
    class Transaction {
    public:
    };

    template <typename... Args>
    Database(Args... args) : _storage(new Storage(args...)) {}

    auto startTransaction() -> Transaction;

    auto endTransaction(Transaction &&transaction) -> bool;

    template <class Table>
    constexpr auto get() -> std::shared_ptr<Table> {
        return _storage;
    }

private:
    std::shared_ptr<persistence::storage::IStorage> _storage;
};

} // namespace persistence

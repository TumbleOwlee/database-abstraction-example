#pragma once

#include "IStorage.h"

#include <memory>

namespace persistence {

namespace storage {

class ITransaction {
public:
    ITransaction(std::shared_ptr<IStorage> storage) : _storage(storage) {}

    template <class Table>
    constexpr auto get() const -> std::shared_ptr<Table> {
        return _storage;
    }

    virtual ~ITransaction() {}

    virtual auto submit() -> void = 0;

protected:
    bool submitted = false;
    std::shared_ptr<IStorage> _storage;
};

} // namespace storage

} // namespace persistence

#pragma once

#include "../ITransaction.h"
#include "GpioStorage.h"
#include "LogStorage.h"
#include "UserStorage.h"

#include <memory>
#include <string>

namespace persistence {

namespace hiberlite {

class Storage final : virtual public persistence::storage::IStorage,
                      virtual public persistence::hiberlite::UserStorage,
                      virtual public persistence::hiberlite::LogStorage,
                      virtual public persistence::hiberlite::GpioStorage {
public:
    class Transaction final : public persistence::storage::ITransaction {
    public:
        Transaction(std::shared_ptr<storage::IStorage> storage) : persistence::storage::ITransaction(storage) {
            std::cerr << "Create transaction" << std::endl;
        }

        ~Transaction() override {
            if (!submitted) {
                std::cerr << "Cancel transaction" << std::endl;
            }
        }

        auto submit() -> void override {
            submitted = true;
            std::cerr << "Submit transaction" << std::endl;
        }
    };

    Storage(std::string const &path)
        : persistence::hiberlite::UserStorage(_database), persistence::hiberlite::LogStorage(_database),
          persistence::hiberlite::GpioStorage(_database) {}

private:
    ::hiberlite::Database _database;
};

} // namespace hiberlite

} // namespace persistence

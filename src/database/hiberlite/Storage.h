#pragma once

#include "../interfaces/ITransaction.h"
#include "GpioStorage.h"
#include "LogStorage.h"
#include "UserStorage.h"

#include <memory>
#include <string>

#include "../common.h"

namespace persistence {

namespace hiberlite {

/**!
 * \brief Storage implementation for hiberlite backend
 */
class Storage final : virtual public persistence::interface::IStorage,
                      virtual public persistence::hiberlite::UserStorage,
                      virtual public persistence::hiberlite::LogStorage,
                      virtual public persistence::hiberlite::GpioStorage {
public:
    /**!
     * \brief Transaction handling for hiberlite backend
     */
    class Transaction final : public persistence::interface::ITransaction {
    public:
        /**!
         * \brief Create/Start new transaction
         *
         * \param storage  Interface handle to access the underlying database
         *
         * \return Interface handle to the transaction
         */
        Transaction(std::shared_ptr<interface::IStorage> storage) : persistence::interface::ITransaction(storage) {
            LOG() << "Create transaction" << std::endl;
            _storage->startTransaction();
        }

        /**!
         * \brief Destructor - cancelling transaction if not submitted/active
         */
        ~Transaction() override {
            if (!submitted) {
                LOG() << "Cancel transaction" << std::endl;
                _storage->rollbackTransaction();
            }
        }

        /**!
         * \brief Submit the active transaction
         *
         * \throws ::hiberlite::database_error if transaction was already submitted
         */
        auto submit() -> void override {
            if (submitted) {
                throw ::hiberlite::database_error("Transaction already submitted.");
            }

            submitted = true;
            _storage->commitTransaction();
            LOG() << "Submit transaction" << std::endl;
        }

    private:
        bool submitted = false;
    };

    /**!
     * \brief Create a new storage using the hiberlite backend
     *
     * \param path  Path to the database file
     */
    Storage(std::string const &path)
        : persistence::hiberlite::UserStorage(_database), persistence::hiberlite::LogStorage(_database),
          persistence::hiberlite::GpioStorage(_database) {
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

#pragma once

#include "../interfaces/ITransaction.h"
#include "GpioStorage.h"
#include "LogStorage.h"
#include "UserStorage.h"

#include <memory>
#include <string>

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
            std::cerr << "Create transaction" << std::endl;
        }

        /**!
         * \brief Destructor - cancelling transaction if not submitted/active
         */
        ~Transaction() override {
            if (!submitted) {
                std::cerr << "Cancel transaction" << std::endl;
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
            std::cerr << "Submit transaction" << std::endl;
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
        std::cerr << "Create model..." << std::endl;
        _database.open(path);
        _database.registerBeanClass<::persistence::hiberlite::User>();
        try {
            _database.createModel();
        } catch (::hiberlite::database_error &e) {
            std::cerr << "Model already exists. Skip 'CREATE TABLE'." << std::endl;
        }
    }

    /**!
     * \brief Destructor that closes the database
     */
    ~Storage() override { _database.close(); }

private:
    ::hiberlite::Database _database;
};

} // namespace hiberlite

} // namespace persistence

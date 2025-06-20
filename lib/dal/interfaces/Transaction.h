#pragma once

#include "../common.h"
#include "Storage.h"

#include <memory>

namespace dal {

namespace interface {

/**!
 * \brief Basic interface for a database transaction
 */
class Transaction {
public:
    /**!
     * \brief Initialize new transaction
     *
     * \param storage  Handle to underlying storage
     */
    Transaction(std::shared_ptr<Storage> storage) : _storage(storage) {}

    /**!
     * \brief Return access view to the underlying storage
     *
     * \param View  The subview to retrieve on the storage
     *
     * \return Handle to the access view
     */
    template <class View>
    constexpr auto get() const -> std::shared_ptr<View> {
        return _storage;
    }

    /**!
     * \brief Destructor
     */
    virtual ~Transaction() {}

    /**!
     * \brief Submit the transaction
     *
     * \throws custom error if transaction was already submitted
     */
    virtual auto submit() -> void = 0;

protected:
    std::shared_ptr<Storage> _storage;
};

} // namespace interface

namespace defaults {

/**!
 * \brief Transaction handling for hiberlite backend
 */
class Transaction final : public ::dal::interface::Transaction {
public:
    /**!
     * \brief Create/Start new transaction
     *
     * \param storage  Interface handle to access the underlying database
     *
     * \return Interface handle to the transaction
     */
    Transaction(std::shared_ptr<interface::Storage> storage) : ::dal::interface::Transaction(storage) {
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
            throw std::logic_error("Transaction already submitted.");
        }

        submitted = true;
        _storage->commitTransaction();
        LOG() << "Submit transaction" << std::endl;
    }

private:
    bool submitted = false;
};

} // namespace defaults

} // namespace dal

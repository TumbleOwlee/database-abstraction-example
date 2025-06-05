#pragma once

#include "IStorage.h"

#include <memory>

namespace persistence {

namespace interface {

/**!
 * \brief Basic interface for a database transaction
 */
class ITransaction {
public:
    /**!
     * \brief Initialize new transaction
     *
     * \param storage  Handle to underlying storage
     */
    ITransaction(std::shared_ptr<IStorage> storage) : _storage(storage) {}

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
    virtual ~ITransaction() {}

    /**!
     * \brief Submit the transaction
     *
     * \throws custom error if transaction was already submitted
     */
    virtual auto submit() -> void = 0;

protected:
    std::shared_ptr<IStorage> _storage;
};

} // namespace interface

} // namespace persistence

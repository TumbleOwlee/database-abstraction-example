#pragma once

#include "GpioStorage.h"
#include "LogStorage.h"
#include "UserStorage.h"

namespace dal {

namespace interface {

/**!
 * \brief Interface combination of all sub-views
 */
class Storage : virtual public ::dal::interface::UserStorage,
                virtual public ::dal::interface::GpioStorage,
                virtual public ::dal::interface::LogStorage {
public:
    /**!
     * \brief Destructor
     */
    virtual ~Storage() {}

    virtual auto startTransaction() -> void = 0;
    virtual auto commitTransaction() -> void = 0;
    virtual auto rollbackTransaction() -> void = 0;
};

} // namespace interface

} // namespace dal

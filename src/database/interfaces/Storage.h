#pragma once

#include "GpioStorage.h"
#include "LogStorage.h"
#include "UserStorage.h"

namespace persistence {

namespace interface {

/**!
 * \brief Interface combination of all sub-views
 */
class Storage : virtual public ::persistence::interface::UserStorage,
                virtual public ::persistence::interface::GpioStorage,
                virtual public ::persistence::interface::LogStorage {
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

} // namespace persistence

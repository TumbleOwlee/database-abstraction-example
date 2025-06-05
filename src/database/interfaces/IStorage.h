#pragma once

#include "IGpioStorage.h"
#include "ILogStorage.h"
#include "IUserStorage.h"

namespace persistence {

namespace interface {

/**!
 * \brief Interface combination of all sub-views
 */
class IStorage : virtual public IUserStorage, virtual public IGpioStorage, virtual public ILogStorage {
public:
    /**!
     * \brief Destructor
     */
    virtual ~IStorage() {}
};

} // namespace interface

} // namespace persistence

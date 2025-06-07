#pragma once

#include "../model/Log.h"
#include "Key.h"

#include <memory>

namespace persistence {

namespace interface {

/**!
 * \brief Interface representing the log specific view on the storage
 */
class ILogStorage {
public:
    /**!
     * \brief Persist a log
     *
     * \param log The log to store
     *
     * \return The key handle associated with the stored log
     */
    virtual auto persist(persistence::model::Log &&log) -> std::shared_ptr<persistence::interface::IKey> = 0;

    /**!
     * \brief Load a log by its key id
     *
     * \param id The key id associated with the log
     *
     * \return The log associated with the given id
     */
    virtual auto load(std::shared_ptr<persistence::interface::IKey> id) -> persistence::model::Log = 0;
};

} // namespace interface

} // namespace persistence

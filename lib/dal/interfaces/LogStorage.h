#pragma once

#include "../model/Log.h"
#include "Key.h"

#include <memory>

namespace dal {

namespace interface {

/**!
 * \brief Interface representing the log specific view on the storage
 */
class LogStorage {
public:
    /**!
     * \brief Persist a log
     *
     * \param log The log to store
     *
     * \return The key handle associated with the stored log
     */
    virtual auto persist(::dal::model::Log &&log) -> std::shared_ptr<::dal::interface::Key> = 0;

    /**!
     * \brief Load a log by its key id
     *
     * \param id The key id associated with the log
     *
     * \return The log associated with the given id
     */
    virtual auto load(std::shared_ptr<::dal::interface::Key> id) -> ::dal::model::Log = 0;
};

} // namespace interface

} // namespace dal

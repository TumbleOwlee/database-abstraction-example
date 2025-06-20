#pragma once

#include "../model/Gpio.h"

#include "Key.h"

#include <memory>

namespace dal {

namespace interface {

/**!
 * \brief Interface representing the gpio specific view on the storage
 */
class GpioStorage {
public:
    /**!
     * \brief Persist a gpio
     *
     * \param gpio The gpio to store
     *
     * \return The key handle associated with the stored gpio
     */
    virtual auto persist(::dal::model::Gpio &&gpio) -> std::shared_ptr<::dal::interface::Key> = 0;

    /**!
     * \brief Load a gpio by its key id
     *
     * \param id The key id associated with the gpio
     *
     * \return The gpio associated with the given id
     */
    virtual auto load(std::shared_ptr<::dal::interface::Key> id) -> ::dal::model::Gpio = 0;
};

} // namespace interface

} // namespace dal

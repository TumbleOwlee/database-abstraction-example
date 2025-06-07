#pragma once

#include "JsonHandle.h"
#include "Key.h"

#include "dal/common.h"
#include "dal/interfaces/GpioStorage.h"
#include "dal/interfaces/Key.h"
#include "dal/model/Gpio.h"

#include <iostream>

namespace dal {

namespace drivers {

namespace json {

class Gpio : public ::dal::model::Gpio {
public:
    Gpio(::dal::model::Gpio &&gpio) : ::dal::model::Gpio(gpio) {}

    auto toJson() -> ::nlohmann::json {
        return {
            {"line", line}
        };
    }

    static auto fromJson(::nlohmann::json const &obj) -> Gpio { return Gpio(::dal::model::Gpio{obj["line"]}); }
};

/**!
 * \brief GpioStorage providing the gpio specific database operations
 */
class GpioStorage : virtual public ::dal::interface::GpioStorage {
public:
    /**!
     * \brief Create a gpio storage view
     *
     * \param database  Underlying database backend
     */
    GpioStorage(::dal::drivers::json::JsonHandle &json) : _json(json) {}

    /**!
     * \brief Persists a gpio by storing it in the database
     *
     * \param gpio The gpio to store
     *
     * \return Key handle representing the stored gpio
     */
    auto persist(::dal::model::Gpio &&gpio) -> std::shared_ptr<::dal::interface::Key> override {
        LOG() << "Persisting gpio...";
        auto id = _json.persist("gpio", Gpio(std::move(gpio)));
        LOG() << id << std::endl;
        return std::make_shared<::dal::drivers::json::Key>(id);
    }

    /**!
     * \brief Loads a gpio from the database
     *
     * \param id  The id of the stored gpio that should be retrieved
     *
     * \throw std::invalid_argument if key isn't valid for the json backend
     *
     * \return The loaded gpio
     */
    auto load(std::shared_ptr<::dal::interface::Key> id) -> ::dal::model::Gpio override {
        auto ptr = dynamic_cast<::dal::drivers::json::Key *>(id.get());
        if (ptr == nullptr) {
            throw std::invalid_argument("Provided pointer isn't valid key.");
        }
        auto index = ptr->get();
        LOG() << "Load gpio " << index << "..." << std::endl;
        auto gpio = _json.load<Gpio>("gpio", index);
        return gpio;
    }

private:
    ::dal::drivers::json::JsonHandle &_json;
};

} // namespace json

} // namespace drivers

} // namespace dal

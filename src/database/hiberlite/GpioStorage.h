#pragma once

#include "../interfaces/GpioStorage.h"
#include "../interfaces/Key.h"
#include "../model/Gpio.h"

#include <hiberlite.h>
#include <iostream>

#include "../common.h"
#include "database/hiberlite/Key.h"

namespace persistence {

namespace hiberlite {

/**!
 * \brief Gpio definition of the database representation
 *
 * \detail This gpio is a specialization of the model 'Gpio' and extends
 *         it by providing the necessary implementation of methods required
 *         by the underlying backend (hiberlite)
 */
class Gpio : public ::persistence::model::Gpio {
private:
    friend class ::hiberlite::access;

    /**!
     * \brief Hiberlite specific required method to create columns
     */
    template <class Archive>
    void hibernate(Archive &archive) {
        archive & ::hiberlite::sql_nvp<decltype(line)>("line", line);
    }

public:
    /**!
     * \brief Consturctor
     */
    Gpio() : ::persistence::model::Gpio() {}

    /**!
     * \brief Consturctor
     *
     * \details Creates a hiberlite gpio from the model gpio
     *
     * \param gpio The gpio as given by the model
     */
    Gpio(::persistence::model::Gpio &&gpio) : ::persistence::model::Gpio(gpio) {}
};

/**!
 * \brief GpioStorage providing the gpio specific database operations
 */
class GpioStorage : virtual public ::persistence::interface::GpioStorage {
public:
    /**!
     * \brief Create a gpio storage view
     *
     * \param database  Underlying database backend
     */
    GpioStorage(::hiberlite::Database &database) : _database(database) {}

    /**!
     * \brief Persists a gpio by storing it in the database
     *
     * \param gpio The gpio to store
     *
     * \return Key handle representing the stored gpio
     */
    auto persist(::persistence::model::Gpio &&gpio) -> std::shared_ptr<::persistence::interface::Key> override {
        LOG() << "Persisting gpio... ";
        auto bean = _database.copyBean(Gpio(std::move(gpio)));
        LOG() << bean.get_id() << std::endl;
        return std::make_shared<::persistence::hiberlite::Key>(bean.get_id());
    }

    /**!
     * \brief Loads a gpio from the database
     *
     * \param id  The id of the stored gpio that should be retrieved
     *
     * \throw std::invalid_argument if key isn't valid for the hiberlite backend
     *
     * \return The loaded gpio
     */
    auto load(std::shared_ptr<::persistence::interface::Key> id) -> ::persistence::model::Gpio override {
        auto ptr = dynamic_cast<::persistence::hiberlite::Key *>(id.get());
        if (ptr == nullptr) {
            throw std::invalid_argument("Provided pointer isn't valid key.");
        }
        auto sqlid = ptr->get();
        LOG() << "Load gpio " << sqlid << "..." << std::endl;
        auto gpio = _database.loadBean<Gpio>(sqlid);
        return ::persistence::model::Gpio(*gpio.get_object()->get());
    }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace persistence

namespace hiberlite {

/**!
 * \brief Hiberlite specific functionality required to be able to store gpios
 */
template <>
// NOLINTNEXTLINE
std::string Database::getClassName<persistence::hiberlite::Gpio>() {
    std::string temp("Gpio");
    std::replace(temp.begin(), temp.end(), ':', '_');
    return temp;
}

} // namespace hiberlite

#pragma once

#include "Key.h"

#include "dal/interfaces/LogStorage.h"
#include "dal/model/Log.h"

#include <hiberdefs.h>
#include <hiberlite.h>
#include <iostream>

#include "dal/common.h"

namespace dal {

namespace drivers {

namespace hiberlite {

/**!
 * \brief Log definition of the database representation
 *
 * \detail This log is a specialization of the model 'Log' and extends
 *         it by providing the necessary implementation of methods required
 *         by the underlying backend (hiberlite)
 */
class Log : public ::dal::model::Log {
private:
    friend class ::hiberlite::access;

    /**!
     * \brief Hiberlite specific required method to create columns
     */
    template <class Archive>
    void hibernate(Archive &archive) {
        archive & ::hiberlite::sql_nvp<decltype(timestamp)>("timestamp", timestamp);
    }

public:
    /**!
     * \brief Consturctor
     */
    Log() : ::dal::model::Log() {}

    /**!
     * \brief Consturctor
     *
     * \details Creates a hiberlite log from the model log
     *
     * \param log The log as given by the model
     */
    Log(::dal::model::Log &&log) : ::dal::model::Log(log) {}
};

/**!
 * \brief LogStorage providing the log specific database operations
 */
class LogStorage : virtual public ::dal::interface::LogStorage {
public:
    /**!
     * \brief Create a log storage view
     *
     * \param database  Underlying database backend
     */
    LogStorage(::hiberlite::Database &database) : _database(database) {}

    /**!
     * \brief Persists a log by storing it in the database
     *
     * \param log The log to store
     *
     * \return Key handle representing the stored log
     */
    auto persist(::dal::model::Log &&log) -> std::shared_ptr<::dal::interface::Key> override {
        auto bean = _database.copyBean(Log(std::move(log)));
        LOG() << "Persisting log...";
        LOG() << bean.get_id() << std::endl;
        return std::make_shared<::dal::drivers::hiberlite::Key>(bean.get_id());
    }

    /**!
     * \brief Loads a log from the database
     *
     * \param id  The id of the stored log that should be retrieved
     *
     * \throw std::invalid_argument if key isn't valid for the hiberlite backend
     *
     * \return The loaded log
     */
    auto load(std::shared_ptr<::dal::interface::Key> id) -> ::dal::model::Log override {
        auto ptr = dynamic_cast<::dal::drivers::hiberlite::Key *>(id.get());
        if (ptr == nullptr) {
            throw std::invalid_argument("Provided pointer isn't valid key.");
        }
        auto sqlid = ptr->get();
        LOG() << "Load log " << sqlid << "..." << std::endl;
        auto log = _database.loadBean<Log>(sqlid);
        return ::dal::model::Log(*log.get_object()->get());
    }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace drivers

} // namespace dal

namespace hiberlite {

/**!
 * \brief Hiberlite specific functionality required to be able to store logs
 */
template <>
// NOLINTNEXTLINE
std::string Database::getClassName<::dal::drivers::hiberlite::Log>() {
    std::string temp("Log");
    std::replace(temp.begin(), temp.end(), ':', '_');
    return temp;
}

} // namespace hiberlite

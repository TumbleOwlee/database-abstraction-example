#pragma once

#include "Key.h"

#include "../interfaces/LogStorage.h"
#include "../model/Log.h"

#include <hiberdefs.h>
#include <hiberlite.h>
#include <iostream>

#include "../common.h"

namespace persistence {

namespace hiberlite {

/**!
 * \brief Log definition of the database representation
 *
 * \detail This log is a specialization of the model 'Log' and extends
 *         it by providing the necessary implementation of methods required
 *         by the underlying backend (hiberlite)
 */
class Log : public persistence::model::Log {
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
    Log() : persistence::model::Log() {}

    /**!
     * \brief Consturctor
     *
     * \details Creates a hiberlite log from the model log
     *
     * \param log The log as given by the model
     */
    Log(persistence::model::Log &&log) : persistence::model::Log(log) {}
};

/**!
 * \brief LogStorage providing the log specific database operations
 */
class LogStorage : virtual public persistence::interface::ILogStorage {
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
    auto persist(persistence::model::Log &&log) -> std::shared_ptr<persistence::interface::IKey> override {
        auto bean = _database.copyBean(Log(std::move(log)));
        LOG() << "Persisting log...";
        LOG() << bean.get_id() << std::endl;
        return std::make_shared<persistence::hiberlite::Key>(bean.get_id());
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
    auto load(std::shared_ptr<persistence::interface::IKey> id) -> persistence::model::Log override {
        auto ptr = dynamic_cast<persistence::hiberlite::Key *>(id.get());
        if (ptr == nullptr) {
            throw std::invalid_argument("Provided pointer isn't valid key.");
        }
        auto sqlid = ptr->get();
        LOG() << "Load log " << sqlid << "..." << std::endl;
        auto log = _database.loadBean<Log>(sqlid);
        return persistence::model::Log(*log.get_object()->get());
    }

private:
    ::hiberlite::Database &_database;
};

} // namespace hiberlite

} // namespace persistence

namespace hiberlite {

/**!
 * \brief Hiberlite specific functionality required to be able to store logs
 */
template <>
// NOLINTNEXTLINE
std::string Database::getClassName<persistence::hiberlite::Log>() {
    std::string temp("Log");
    std::replace(temp.begin(), temp.end(), ':', '_');
    return temp;
}

} // namespace hiberlite

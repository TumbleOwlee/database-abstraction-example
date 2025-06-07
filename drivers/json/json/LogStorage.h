#pragma once

#include "JsonHandle.h"
#include "Key.h"

#include "dal/common.h"
#include "dal/interfaces/LogStorage.h"
#include "dal/model/Log.h"

#include <iostream>

namespace dal {

namespace drivers {

namespace json {

class Log : public ::dal::model::Log {
public:
    Log(::dal::model::Log &&log) : ::dal::model::Log(log) {}

    auto toJson() -> ::nlohmann::json {
        return {
            {"timestamp", timestamp}
        };
    }

    static auto fromJson(::nlohmann::json const &obj) -> Log { return Log(::dal::model::Log{obj["timestamp"]}); }
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
    LogStorage(::dal::drivers::json::JsonHandle &json) : _json(json) {}

    /**!
     * \brief Persists a log by storing it in the database
     *
     * \param log The log to store
     *
     * \return Key handle representing the stored log
     */
    auto persist(::dal::model::Log &&log) -> std::shared_ptr<::dal::interface::Key> override {
        LOG() << "Persisting log...";
        auto id = _json.persist("log", Log(std::move(log)));
        LOG() << id << std::endl;
        return std::make_shared<::dal::drivers::json::Key>(id);
    }

    /**!
     * \brief Loads a log from the database
     *
     * \param id  The id of the stored log that should be retrieved
     *
     * \throw std::invalid_argument if key isn't valid for the json backend
     *
     * \return The loaded log
     */
    auto load(std::shared_ptr<::dal::interface::Key> id) -> ::dal::model::Log override {
        auto ptr = dynamic_cast<::dal::drivers::json::Key *>(id.get());
        if (ptr == nullptr) {
            throw std::invalid_argument("Provided pointer isn't valid key.");
        }
        auto index = ptr->get();
        LOG() << "Load log " << index << "..." << std::endl;
        auto log = _json.load<Log>("log", index);
        return log;
    }

private:
    ::dal::drivers::json::JsonHandle &_json;
};

} // namespace json

} // namespace drivers

} // namespace dal

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace dal {

namespace drivers {

namespace json {

class JsonHandle {
public:
    JsonHandle(std::string const &path) : _path(path), _json(::nlohmann::json::object()) {
        if (std::filesystem::exists(path)) {
            std::fstream stream;
            stream.open(path);
            if (!stream.is_open()) {
                throw std::invalid_argument("File can't be opened.");
            }
            _json = ::nlohmann::json::parse(stream);
        }
    }

    ~JsonHandle() {
        std::fstream stream;
        stream.open(_path, std::ios::out);
        stream << _json.dump(4);
        stream.close();
    }

    auto startTransaction() -> void {
        if (_taActive) {
            throw std::logic_error("Transaction already active.");
        }
        _taActive = true;
    }

    auto commitTransaction() -> void {
        if (!_taActive) {
            throw std::logic_error("Transaction is not active.");
        }
        std::fstream stream;
        stream.open(_path, std::ios::out);
        stream << _json.dump(4);
        stream.close();
        _taActive = false;
    }

    auto rollbackTransaction() -> void {
        if (!_taActive) {
            throw std::logic_error("Transaction is not active.");
        }
        std::ifstream stream(_path);
        _json = ::nlohmann::json::parse(stream);
        _taActive = false;
    }

    template <typename T>
    auto persist(char const *const key, T &&obj) -> ::nlohmann::basic_json<>::size_type {
        if (_json.end() == _json.find(key)) {
            _json[key] = ::nlohmann::json::array();
        }

        ::nlohmann::json &array = _json[key];
        array.push_back(obj.toJson());

        if (!_taActive) {
            std::fstream stream;
            stream.open(_path, std::ios::out);
            stream << _json.dump(4);
            stream.close();
        }

        return array.size() - 1;
    }

    template <typename T>
    auto load(char const *const key, ::nlohmann::basic_json<>::size_type index) -> T {
        if (_json.find(key) == _json.end() || !_json[key].is_array() || !_json[key][index].is_object()) {
            throw std::logic_error("Can't access given element in JSON array.");
        }
        return T::fromJson(_json[key][index]);
    }

private:
    std::string _path;
    ::nlohmann::json _json;
    bool _taActive = false;
};

} // namespace json

} // namespace drivers

} // namespace dal

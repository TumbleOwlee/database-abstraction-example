#pragma once

#include "model/Gpio.h"
#include "model/Log.h"
#include "model/User.h"

namespace persistence {

namespace storage {

class IUserStorage {
public:
    virtual auto persist(persistence::model::User const &user) -> void = 0;
};

class ILogStorage {
public:
    virtual auto persist(persistence::model::Log const &log) -> void = 0;
};

class IGpioStorage {
public:
    virtual auto persist(persistence::model::Gpio const &gpio) -> void = 0;
};

class IStorage : public virtual IUserStorage, public virtual IGpioStorage, public virtual ILogStorage {};

} // namespace storage

} // namespace persistence

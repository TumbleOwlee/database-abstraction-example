# Database Abstraction Layer

[![CMake on Unix](https://github.com/TumbleOwlee/database-abstraction-example/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/TumbleOwlee/database-abstraction-example/actions/workflows/cmake-single-platform.yml)

This repository contains a quick example and proof-of-concept of an abstraction layer around any database backend. It is solely based on interfaces that can be implemented for the various possible storage libraries. This approach allows an application to replace the storage library with minimal need to change any part of the application code.

The repository currently only contains the necessary implementation of the interface to use the [hiberlite](https://github.com/tumbleowlee/hiberlite) project as the data storage. But the same interfaces can be implemented for any other storage library, e.g. simple file storage or more sphisticated databases.

## Example

As you can see in the following example, only the single included header file and the call to `Database::create<T>(..)` contain references to the actual used storage library (in this case hiberlite). All other calls only act on interfaces. Thus, changing the underlying storage library only require a change to the two lines while not affecting all other application code. Please refer to the implementation in `src/database/hiberlite` for an example of the interface implementations to support the use of hiberlite as the storage library.

```c++
#include "dal/Database.h"
#include "hiberlite/Storage.h"

using namespace ::dal;
using namespace ::dal::common;
using namespace ::dal::model;
using namespace ::dal::interface;

int main(int, char **) {
    std::cerr << ">> Running..." << std::endl;

    Database database = Database::create<::dal::drivers::hiberlite::Storage>("db.sqlite");

    /* Non-transaction persist */
    {
        std::cerr << ">> Perform non-transaction persist/load .." << std::endl;

        auto userStorage = database.get<UserStorage>();
        auto userId = userStorage->persist(User{"John Doe", 21});

        auto user = userStorage->load(userId);
        assert(user.name == "John Doe" && user.age == 21);

        auto gpioStorage = database.get<GpioStorage>();
        auto gpioId = gpioStorage->persist(Gpio{1});

        auto gpio = gpioStorage->load(gpioId);
        assert(gpio.line == 1);

        auto logStorage = database.get<LogStorage>();
        auto logId = logStorage->persist(Log{"Aug Fri 2025-03-12 11:21:00+02:00"});

        auto log = logStorage->load(logId);
        assert(log.timestamp == "Aug Fri 2025-03-12 11:21:00+02:00");
    }

    /* Drop transaction */
    {
        std::cerr << ">> Drop transaction .." << std::endl;

        auto transaction = database.newTransaction();

        auto userStorage = transaction->get<UserStorage>();
        userStorage->persist(User{"John Doene", 33});

        auto gpioStorage = database.get<GpioStorage>();
        auto gpioId = gpioStorage->persist(Gpio{2});

        auto logStorage = transaction->get<LogStorage>();
        logStorage->persist(Log{"Sep Tue 2024-08-21 17:24:34+02:00"});

        /* Transaction is dropped and not applied when moving out of scope */
    }

    /* Perform transaction to persist */
    {
        std::cerr << ">> Submit transaction .." << std::endl;

        auto transaction = database.newTransaction();

        auto userStorage = database.get<UserStorage>();
        auto userId = userStorage->persist(User{"Jane Doe", 25});

        auto gpioStorage = database.get<GpioStorage>();
        auto gpioId = gpioStorage->persist(Gpio{3});

        auto logStorage = database.get<LogStorage>();
        auto logId = logStorage->persist(Log{"Aug Fri 2025-04-02 12:30:00+02:00"});

        transaction->submit();

        auto user = userStorage->load(userId);
        assert(user.name == "Jane Doe" && user.age == 25);

        auto gpio = gpioStorage->load(gpioId);
        assert(gpio.line == 3);

        auto log = logStorage->load(logId);
        assert(log.timestamp == "Aug Fri 2025-04-02 12:30:00+02:00");
    }

    std::cerr << ">> Stopped" << std::endl;
    return 0;
}
```

Given the example, you can run the same application code with the JSON storage driver by just changing two lines:

```c++
#include "dal/Database.h"
#include "json/Storage.h" // <-- include the other storage driver

using namespace ::dal;
using namespace ::dal::common;
using namespace ::dal::model;
using namespace ::dal::interface;

int main(int, char **) {
    std::cerr << ">> Running..." << std::endl;

    Database database = Database::create<::dal::drivers::json::Storage>("db.json"); // <-- select the JSON storage driver

    /* Non-transaction persist */
    {
        std::cerr << ">> Perform non-transaction persist/load .." << std::endl;

        auto userStorage = database.get<UserStorage>();
        auto userId = userStorage->persist(User{"John Doe", 21});

        auto user = userStorage->load(userId);
        assert(user.name == "John Doe" && user.age == 21);

        auto gpioStorage = database.get<GpioStorage>();
        auto gpioId = gpioStorage->persist(Gpio{1});

        auto gpio = gpioStorage->load(gpioId);
        assert(gpio.line == 1);

        auto logStorage = database.get<LogStorage>();
        auto logId = logStorage->persist(Log{"Aug Fri 2025-03-12 11:21:00+02:00"});

        auto log = logStorage->load(logId);
        assert(log.timestamp == "Aug Fri 2025-03-12 11:21:00+02:00");
    }

    /* Drop transaction */
    {
        std::cerr << ">> Drop transaction .." << std::endl;

        auto transaction = database.newTransaction();

        auto userStorage = transaction->get<UserStorage>();
        userStorage->persist(User{"John Doene", 33});

        auto gpioStorage = database.get<GpioStorage>();
        auto gpioId = gpioStorage->persist(Gpio{2});

        auto logStorage = transaction->get<LogStorage>();
        logStorage->persist(Log{"Sep Tue 2024-08-21 17:24:34+02:00"});

        /* Transaction is dropped and not applied when moving out of scope */
    }

    /* Perform transaction to persist */
    {
        std::cerr << ">> Submit transaction .." << std::endl;

        auto transaction = database.newTransaction();

        auto userStorage = database.get<UserStorage>();
        auto userId = userStorage->persist(User{"Jane Doe", 25});

        auto gpioStorage = database.get<GpioStorage>();
        auto gpioId = gpioStorage->persist(Gpio{3});

        auto logStorage = database.get<LogStorage>();
        auto logId = logStorage->persist(Log{"Aug Fri 2025-04-02 12:30:00+02:00"});

        transaction->submit();

        auto user = userStorage->load(userId);
        assert(user.name == "Jane Doe" && user.age == 25);

        auto gpio = gpioStorage->load(gpioId);
        assert(gpio.line == 3);

        auto log = logStorage->load(logId);
        assert(log.timestamp == "Aug Fri 2025-04-02 12:30:00+02:00");
    }

    std::cerr << ">> Stopped" << std::endl;
    return 0;
}
```

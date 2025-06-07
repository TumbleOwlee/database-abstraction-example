#include "database/Database.h"
#include "database/hiberlite/Storage.h"

using namespace ::persistence;
using namespace ::persistence::common;
using namespace ::persistence::model;
using namespace ::persistence::interface;

int main(int, char **) {
    std::cerr << ">> Running..." << std::endl;

    Database database = Database::create<::persistence::hiberlite::Storage>("db.sqlite");

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

#include "database/Database.h"
#include "database/hiberlite/Storage.h"
#include <hiberlite.h>

using namespace persistence;
using namespace persistence::model;
using namespace persistence::storage;

int main(int, char **) {
    std::cerr << "Running..." << std::endl;

    Database database = Database::create<persistence::hiberlite::Storage>("db.sqlite");

    /* Non-transaction persist */
    {
        auto userStorage = database.get<IUserStorage>();
        userStorage->persist(User());

        auto gpioStorage = database.get<IGpioStorage>();
        gpioStorage->persist(Gpio());

        auto logStorage = database.get<ILogStorage>();
        logStorage->persist(Log());
    }

    /* Perform transaction to persist */
    {
        auto transaction = database.newTransaction();

        auto userStorage = transaction->get<IUserStorage>();
        userStorage->persist(User());

        auto logStorage = transaction->get<ILogStorage>();
        logStorage->persist(Log());

        transaction->submit();
    }

    /* Drop transaction */
    {
        auto transaction = database.newTransaction();

        auto userStorage = transaction->get<IUserStorage>();
        userStorage->persist(User());

        auto logStorage = transaction->get<ILogStorage>();
        logStorage->persist(Log());

        /* Transaction is dropped and not applied when moving out of scope */
    }

    std::cerr << "...Stopped" << std::endl;
    return 0;
}

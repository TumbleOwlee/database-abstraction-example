#include <hiberlite.h>

#include "database/Database.h"
#include "database/hiberlite/Storage.h"

auto assert(bool cmp) -> void {
    if (!cmp) {
        abort();
    }
}

using namespace persistence;
using namespace persistence::model;
using namespace persistence::interface;

int main(int, char **) {
    std::cerr << "Running..." << std::endl;

    Database database = Database::create<persistence::hiberlite::Storage>("db.sqlite");

    /* Non-transaction persist */
    {
        auto userStorage = database.get<IUserStorage>();
        auto id = userStorage->persist(User{"Max Mustermann", 21});

        auto user = userStorage->load(id);
        assert(user.name == "Max Mustermann" && user.age == 21);

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

#include "database/Database.h"
#include "database/hiberlite/Storage.h"
#include <hiberlite.h>

using namespace persistence;
using namespace persistence::model;
using namespace persistence::storage;

int main(int, char **) {
    std::cerr << "Running..." << std::endl;

    Database database = Database::create<persistence::hiberlite::Storage>("db.sqlite");

    auto userStorage = database.get<IUserStorage>();
    userStorage->persist(User());

    auto gpioStorage = database.get<IGpioStorage>();
    gpioStorage->persist(Gpio());

    auto logStorage = database.get<ILogStorage>();
    logStorage->persist(Log());

    std::cerr << "...Stopped" << std::endl;
    return 0;
}

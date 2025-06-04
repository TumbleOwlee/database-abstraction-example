#include "database/Database.h"
#include "database/hiberlite/Storage.h"
#include <hiberlite.h>

using namespace persistence;

int main(int, char **) {
    std::cerr << "Running..." << std::endl;

    Database<persistence::hiberlite::Storage> database("db.sqlite");

    auto userTable = database.get<storage::IUserStorage>();
    userTable->persistUser(User());

    auto gpioTable = database.get<storage::IGpioStorage>();
    gpioTable->persistGpio(Gpio());

    auto logTable = database.get<storage::ILogStorage>();
    logTable->persistLog(Log());

    std::cerr << "...Stopped" << std::endl;
    return 0;
}

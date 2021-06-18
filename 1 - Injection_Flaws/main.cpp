#include <string>

//#include "crow_all.h"
#include <pqxx/pqxx>

int main(int argc, char *argv[])
{
    //crow::SimpleApp app;
    //psql-userdb="host=<host> dbname=<database-name> user=<database-user> password=<database-user-password> connect_timeout=30"
    std::string databaseHost{"postgres-db"};
    std::string databaseName{std::getenv("POSTGRES_DB")};
    std::string databaseUser{std::getenv("POSTGRES_USER")};
    std::string databasePassword{std::getenv("POSTGRES_PASSWORD")};
    std::string databaseConnectionString{
        "host=" + databaseHost + " dbname=" + databaseName + " user=" + databaseUser + " password=" + databasePassword + " connect_timeout=30"
    };
    pqxx::connection conn{databaseConnectionString};
}
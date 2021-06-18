#include <string>
#include <iostream>

#include "crow_all.h"
#include <pqxx/pqxx>

int main(int argc, char *argv[])
{
    crow::SimpleApp app;
    //psql-userdb="host=<host> dbname=<database-name> user=<database-user> password=<database-user-password> connect_timeout=30"
    std::string databaseHost{"postgres-db"};
    std::string databaseName{std::getenv("POSTGRES_DB")};
    std::string databaseUser{std::getenv("POSTGRES_USER")};
    std::string databasePassword{std::getenv("POSTGRES_PASSWORD")};
    std::string databaseConnectionString{
        "host=" + databaseHost + " dbname=" + databaseName + " user=" + databaseUser + " password=" + databasePassword + " connect_timeout=30"
    };
    std::cout << "Database connection string is: " << databaseConnectionString << '\n';
    pqxx::connection databaseConnection{databaseConnectionString};
    pqxx::nontransaction nonTransation{databaseConnection};

    std::cout << "Database name is: " << databaseConnection.dbname() << '\n';

    CROW_ROUTE(app, "/api/training/healthCheck").methods(crow::HTTPMethod::Get)
    ([&databaseConnection, &nonTransation](const crow::request &req){
        return crow::response(200);
    });

    char *port = std::getenv("PORT");
	uint16_t iPort = static_cast<uint16_t>(port != nullptr ? std::stoi(port) : 18080);

	std::cout << "PORT = " << iPort << "\n";

	app.port(iPort).multithreaded().run();
	databaseConnection.close();
	return 0;
}
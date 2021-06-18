#include <string>
#include <iostream>

#include "crow_all.h"
#include <pqxx/pqxx>

int main(int argc, char *argv[])
{
    crow::SimpleApp app;
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
    ([&databaseConnection, &nonTransation](const crow::request &req) {
        return crow::response(200);
    });

    CROW_ROUTE(app, "/api/training/users").methods(crow::HTTPMethod::Get)
    ([&databaseConnection, &nonTransation](const crow::request &req) {
        // For POST methods
        //crow::json::rvalue data = crow::json::load(req.body);
        crow::json::wvalue dataTransferObject;
        std::string query{
            "SELECT * FROM gtworld_users;"
        };
        try
        {
            pqxx::result queryResult{ nonTransation.exec(query) };
            if (queryResult.size() < 1) 
            {
                dataTransferObject["status"] = true;
                dataTransferObject["message"] = "No users!";
                return crow::response(200, dataTransferObject);
            }
            else
            {
                dataTransferObject["status"] = true;
                dataTransferObject["message"] = "Request Successful!";
                std::vector<crow::json::wvalue> data;
                data.reserve(queryResult.size());
                for (const pqxx::row row : queryResult) 
                {
                    crow::json::wvalue user;
                    for (const pqxx::field field : row)
                    {
                        if (!field.is_null())
                        {
                            // use PostgreSQL oid datatype from pg_type() table
                            switch (field.type())
                            {
                                case 1043:
                                    user[field.name()] = field.as<std::string>();
                                    break;
                                case 23:  
                                    user[field.name()] = field.as<int>();                              
                                default:
                                    break;
                            }
                        }
                    }
                    data.push_back(std::move(user));
                }
                dataTransferObject["data"] = std::move(data);
                return crow::response(200, dataTransferObject);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return crow::response(400, "Please try again!");
        }     
    });

    char *port = std::getenv("PORT");
	uint16_t iPort = static_cast<uint16_t>(port != nullptr ? std::stoi(port) : 18080);

	std::cout << "PORT = " << iPort << "\n";

	app.port(iPort).multithreaded().run();
	databaseConnection.close();
	return 0;
}
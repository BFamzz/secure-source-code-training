#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "crow_all.h"
#include <pqxx/pqxx>

std::string getView(const std::string &filename, crow::mustache::context &x);

int main(int argc, char *argv[])
{
    crow::SimpleApp app;
    crow::mustache::set_base(".");
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

    CROW_ROUTE(app, "/api/users").methods(crow::HTTPMethod::Get)
    ([&databaseConnection, &nonTransation]() {
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
                std::vector<crow::json::wvalue> users;
                users.reserve(queryResult.size());
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
                    users.push_back(std::move(user));
                }
                dataTransferObject["users"] = std::move(users);
                //return crow::response(200, dataTransferObject);
                std::cout << "View is: " << getView("users", dataTransferObject) << '\n';
                return crow::response(200, getView("users", dataTransferObject));
            }
        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return crow::response(404, getView("error", dataTransferObject));
        }     
    });

    CROW_ROUTE(app, "/api/users/<string>").methods(crow::HTTPMethod::Get)
    ([&databaseConnection, &nonTransation] (const crow::request &req, std::string firstName) {
        crow::json::wvalue dataTransferObject;
        std::string query{
            "SELECT * FROM gtworld_users WHERE first_name = '" + firstName + "';" // Potential SQL Injection
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
                std::vector<crow::json::wvalue> users;
                users.reserve(queryResult.size());
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
                    users.push_back(std::move(user));
                }
                dataTransferObject["users"] = std::move(users);
                return crow::response(200, getView("users", dataTransferObject));
            }
        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return crow::response(404, getView("error", dataTransferObject));
        }
    });

    CROW_ROUTE(app, "/api/getFirstName").methods(crow::HTTPMethod::Get)
    ([&databaseConnection, &nonTransation] (const crow::request &req) {
        crow::json::wvalue dataTransferObject;
        std::string firstName = req.url_params.get("first_name");
        std::string query{
            "SELECT * FROM gtworld_users WHERE first_name = $1;" // Potential SQL Injection
        };

        try
        {
            databaseConnection.prepare("", query);
            pqxx::result queryResult{ nonTransation.exec_prepared("", firstName) };
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
                std::vector<crow::json::wvalue> users;
                users.reserve(queryResult.size());
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
                    users.push_back(std::move(user));
                }
                dataTransferObject["users"] = std::move(users);
                return crow::response(200, getView("users", dataTransferObject));
            }
        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return crow::response(404, getView("error", dataTransferObject));
        }
    });

    char *port = std::getenv("PORT");
	uint16_t iPort = static_cast<uint16_t>(port != nullptr ? std::stoi(port) : 18080);

	std::cout << "PORT = " << iPort << "\n";

	app.port(iPort).multithreaded().run();
	databaseConnection.close();
	return EXIT_SUCCESS;
}

std::string getView(const std::string &filename, crow::mustache::context &x)
{
    return crow::mustache::load("../html/" + filename + ".html").render(x);
}


//localhost/api/getFirstName?first_name=Drake
//URL-ENCODED//%27;CREATE%20TABLE%20hacked%20(name%20VARCHAR(10));--
//Drake';CREATE TABLE hacked (name VARCHAR(20));--
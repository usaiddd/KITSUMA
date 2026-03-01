#include<libpq-fe.h>
#include<iostream>
using namespace std;
int main() {

    const char* conninfo = 
        "host=localhost port=5432 dbname=your_db user=your_user password=your_password";

    // Create connection
    PGconn* conn = PQconnectdb(conninfo);

    // Check connection status
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection failed: " 
                  << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return 1;
    }

    std::cout << "Connected successfully!" << std::endl;
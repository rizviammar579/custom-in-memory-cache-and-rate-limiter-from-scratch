#include <iostream>
#include "../third_party/httplib.h"

using namespace std;

int main()
{
        httplib::Server server;

        server.Get("/", [](const httplib::Request &req,
                           httplib::Response &res)
                   { res.set_content("C++ HTTP Server is running!", "text/plain"); });

        cout << "Starting C++ HTTP Server..." << endl;
        cout << "Server listening on http://localhost:8081" << endl;

        if (!server.listen("0.0.0.0", 8081))
        {
                cerr << "Failed to start server!" << endl;
                return 1;
        }

        return 0;
}

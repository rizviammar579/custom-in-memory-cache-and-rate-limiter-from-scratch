#include <iostream>
#include <chrono>
#include <cstdlib>
#include <string>
#include "../third_party/httplib.h"
#include "../third_party/dotenv.h"
#include "./lru-cache/lru_cache.h"
#include "./sliding-window-rate-limiter/sliding_window_rate_limiter.h"

using namespace std;

int main()
{
        dotenv::init(".env");

        const char *secret = getenv("CPP_SERVER_SECRET_KEY");

        if (secret == nullptr)
        {
                cerr << "CPP Server Secret Key is not set!" << endl;
                return 1;
        }

        string secretKey(secret);

        httplib::Server server;

        LRUCache cache(100);

        SlidingWindowRateLimiter limiter(10, chrono::seconds(60));

        server.Get("/", [](const httplib::Request &req,
                           httplib::Response &res)
                   { res.set_content("C++ HTTP Server is running!", "text/plain"); });

        server.Get("/cache", [&](const httplib::Request &req,
                                 httplib::Response &res)
                   {
                        if(req.get_header_value("CPP_SERVER_SECRET_KEY") != secretKey)
                        {
                             res.status = 403;
                             res.set_content("Forbidden", "text/plain");
                             return;
                        }
                        
                           if (!req.has_param("ip") || !req.has_param("key"))
                           {
                                   res.status = 400;
                                   res.set_content("Bad Request : Missing IP or cache key", "text/plain");
                                   return;
                           }

                           string ip = req.get_param_value("ip");
                           string key = req.get_param_value("key");

                           if (!limiter.allowRequest(ip))
                           {
                                   res.status = 429;
                                   res.set_content("Too Many Requests : Rate limit exceeded", "text/plain");
                                   return;
                           }

                           auto value = cache.get(key);

                           if (!value.has_value())
                           {
                                   res.status = 404;
                                   res.set_content("Not Found : Cache miss", "text/plain");
                                   return;
                           }

                           res.status = 200;
                           res.set_content(value.value(), "application/json"); });

        server.Post("/cache", [&](const httplib::Request &req,
                                  httplib::Response &res)
                    {

                       if(req.get_header_value("CPP_SERVER_SECRET_KEY") != secretKey)
                        {
                             res.status = 403;
                             res.set_content("Forbidden", "text/plain");
                             return;
                        }


                            if (!req.has_param("key"))
                            {
                                    res.status = 400;
                                    res.set_content("Bad Request : Missing cache key", "text/plain");
                                    return;
                            }

                            string key = req.get_param_value("key");

                            cache.put(key, req.body);

                            res.status = 200;
                            res.set_content("Cache updated successfully", "text/plain"); });

        cout << "Starting C++ HTTP Server..." << endl;
        cout << "Server listening on http://localhost:8081" << endl;

        if (!server.listen("127.0.0.1", 8081))
        {
                cerr << "Failed to start server!" << endl;
                return 1;
        }

        return 0;
}

#include <iostream>
#include "sliding_window_rate_limiter.h"

using namespace std;

bool SlidingWindowRateLimiter::allowRequest(const string &clientID , TimePoint now){

    auto& clientRequests = requests[clientID];

    while(!clientRequests.empty() && (now - clientRequests.front() >= windowDuration)){
        clientRequests.pop_front();
    }

    if (clientRequests.size() < maxRequests){

        clientRequests.push_back(now);
        return true;

    } 
    else return false; 
    

}

void SlidingWindowRateLimiter::printDeque(const std::string& clientID)
{
    auto it = requests.find(clientID);

    if (it == requests.end() || it->second.empty()) return;
    
    const auto& clientRequests = it->second;

    auto start = clientRequests.front();

    cout << "Client: " << clientID << "\n";
    cout << "Deque: [";

    for (const auto& timestamp : clientRequests)
    {
        auto elapsed = chrono::duration_cast<chrono::seconds>(
            timestamp - start
        ).count();

        cout << elapsed << "s ";

    }

    cout << "]\n";
}
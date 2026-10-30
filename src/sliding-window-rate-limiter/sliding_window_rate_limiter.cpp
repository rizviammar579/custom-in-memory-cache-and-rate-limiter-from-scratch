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
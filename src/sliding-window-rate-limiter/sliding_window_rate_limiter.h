#ifndef SLIDING_WINDOW_RATE_LIMITER_H
#define SLIDING_WINDOW_RATE_LIMITER_H

#include <unordered_map>
#include <deque>
#include <string>
#include <chrono>
#include <stdexcept>

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;


class SlidingWindowRateLimiter
{
    int maxRequests;
    std::chrono::seconds windowDuration;

    std::unordered_map<std::string, std::deque<TimePoint>> requests;

public:

    SlidingWindowRateLimiter(int maxRequests, std::chrono::seconds windowDuration)
        : maxRequests(maxRequests), windowDuration(windowDuration)
    {

        if (maxRequests <= 0 || windowDuration <= std::chrono::seconds::zero())
            throw std::invalid_argument("Invalid Rate Limiter Configuration");
    }

    bool allowRequest(const std::string &clientID , TimePoint now = Clock::now());

    void printDeque(const std::string &clientID);

};

#endif
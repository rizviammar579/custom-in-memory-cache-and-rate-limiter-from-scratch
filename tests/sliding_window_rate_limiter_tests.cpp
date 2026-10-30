#include <iostream>
#include <cassert>
#include <stdexcept>

#include "../src/sliding-window-rate-limiter/sliding_window_rate_limiter.h"

using namespace std;

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;


// 1. Requests within the configured limit
void testRequestsWithinLimit()
{

    SlidingWindowRateLimiter limiter(3, chrono::seconds(10));
    auto start = Clock::now();

    
    assert(limiter.allowRequest("client-1", start));
    limiter.printDeque("client-1");

    
    assert(limiter.allowRequest("client-1", start + chrono::seconds(1)));
    limiter.printDeque("client-1");


    assert(limiter.allowRequest("client-1", start + chrono::seconds(2)));
    limiter.printDeque("client-1");

}


// 2. Reject requests after reaching the limit
void testRequestLimitExceeded()
{
    

    SlidingWindowRateLimiter limiter(3, chrono::seconds(10));
    auto start = Clock::now();

    for (int i = 0; i < 3; i++)
    {
        assert(limiter.allowRequest("client-1", start + chrono::seconds(i)));
        limiter.printDeque("client-1");
    }

    cout << "Request at 3s: ";
    assert(!limiter.allowRequest("client-1", start + chrono::seconds(3)));
    limiter.printDeque("client-1");

}


// 3. Expired timestamps should be removed
void testExpiredRequests()
{

    SlidingWindowRateLimiter limiter(3, chrono::seconds(10));
    auto start = Clock::now();

    assert(limiter.allowRequest("client-1", start));
    assert(limiter.allowRequest("client-1", start + chrono::seconds(2)));
    assert(limiter.allowRequest("client-1", start + chrono::seconds(4)));

    cout << "Deque before expiration:\n";
    limiter.printDeque("client-1");

    cout << "Request at 11s: ";
    assert(limiter.allowRequest("client-1", start + chrono::seconds(11)));
    cout << "Allowed (0s timestamp expired)\n";
    limiter.printDeque("client-1");

}


// 4. Different clients have independent limits
void testIndependentClients()
{

    SlidingWindowRateLimiter limiter(2, chrono::seconds(10));
    auto start = Clock::now();

    cout << "Client 1";
    assert(limiter.allowRequest("client-1", start));
    limiter.printDeque("client-1");

   
    assert(limiter.allowRequest("client-1", start + chrono::seconds(1)));
    limiter.printDeque("client-1");

   
    assert(!limiter.allowRequest("client-1", start + chrono::seconds(2)));
    limiter.printDeque("client-1");

    cout << "Client 2";
    assert(limiter.allowRequest("client-2", start + chrono::seconds(2)));
    limiter.printDeque("client-2");

    
    assert(limiter.allowRequest("client-2", start + chrono::seconds(3)));
    limiter.printDeque("client-2");

}

// 5. A rejected request must not extend the window
void testRejectedRequestNotRecorded()
{

    SlidingWindowRateLimiter limiter(2, chrono::seconds(10));
    auto start = Clock::now();

    assert(limiter.allowRequest("client-1", start));
    assert(limiter.allowRequest("client-1", start + chrono::seconds(2)));

    cout << "Deque before rejected request:\n";
    limiter.printDeque("client-1");

    cout << "Request at 3s: ";
    assert(!limiter.allowRequest("client-1", start + chrono::seconds(3)));
    cout << "Rejected\n";
    limiter.printDeque("client-1");

    cout << "Request at 10s: ";
    assert(limiter.allowRequest("client-1", start + chrono::seconds(10)));
    cout << "Allowed (0s timestamp expired)\n";
    limiter.printDeque("client-1");

}

// 6. Invalid configurations
void testInvalidConfiguration()
{

    bool exceptionThrown = false;

    try
    {
        SlidingWindowRateLimiter limiter(0, chrono::seconds(10));
    }
    catch (const invalid_argument&)
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
    cout << "Zero max requests correctly rejected\n";

    exceptionThrown = false;

    try
    {
        SlidingWindowRateLimiter limiter(3, chrono::seconds(0));
    }
    catch (const invalid_argument&)
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
    cout << "Zero window duration correctly rejected\n";
}


int main()
{
    testRequestsWithinLimit();
    testRequestLimitExceeded();
    testExpiredRequests();
    testIndependentClients();
    testRejectedRequestNotRecorded();
    testInvalidConfiguration();

    cout << "\nAll sliding window rate limiter tests passed!\n";

    return 0;
}

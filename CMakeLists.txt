cmake_minimum_required(VERSION 3.20)

project(CacheRateLimiter
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Main application
# add_executable(cache_rate_limiter
#     src/main.cpp
# )

# target_include_directories(cache_rate_limiter PRIVATE
#     src/lru-cache
#     src/sliding-window-rate-limiter
# )

# Separate test executable
add_executable(lru_cache_tests
    tests/lru_cache_tests.cpp
    src/lru-cache/lru_cache.cpp
)

target_include_directories(lru_cache_tests PRIVATE
    src/lru-cache
)

# Separate test executable
add_executable(sliding_window_rate_limiter_tests
    tests/sliding_window_rate_limiter_tests.cpp
    src/sliding-window-rate-limiter/sliding_window_rate_limiter.cpp
)

target_include_directories(sliding_window_rate_limiter_tests PRIVATE
    src/sliding-window-rate-limiter
)

enable_testing()
add_test(NAME LRUCacheTests COMMAND lru_cache_tests)
add_test(NAME SlidingWindowRateLimiterTests COMMAND sliding_window_rate_limiter_tests)

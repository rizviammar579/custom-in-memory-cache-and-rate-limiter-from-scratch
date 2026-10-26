cmake_minimum_required(VERSION 3.20)

project(CacheRateLimiter
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Main application
add_executable(cache_rate_limiter
    src/main.cpp
    src/lru-cache/lru_cache.cpp
)

target_include_directories(cache_rate_limiter PRIVATE
    src/lru-cache
)

# Separate test executable
add_executable(lru_cache_tests
    tests/lru_cache_tests.cpp
    src/lru-cache/lru_cache.cpp
)

target_include_directories(lru_cache_tests PRIVATE
    src/lru-cache
)

enable_testing()
add_test(NAME LRUCacheTests COMMAND lru_cache_tests)

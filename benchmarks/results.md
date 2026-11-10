# Performance Benchmarks

## 1. Overview

This document compares API response latency when retrieving product data through the PostgreSQL database and through the C++ in-memory cache.

The objective is to evaluate how caching reduces response time by avoiding repeated database queries.

## 2. Test Environment

| Component | Configuration |
|---|---|
| Backend | Node.js + Express + TypeScript |
| Cache Server | C++ HTTP Server |
| Database | Neon PostgreSQL |
| Cache Algorithm | LRU (Least Recently Used) |
| Operating System | Windows 11 |
| HTTP Client | Postman |
| Cache Capacity | 100 entries |

## 3. Benchmark Methodology

The following scenarios were tested:

- **Cache MISS:** A request for data not present in the cache. The Express server fetches the data from PostgreSQL and populates the cache.
- **Cache HIT:** A repeated request for the same data. The C++ cache returns the stored response without querying PostgreSQL.

Each scenario was tested 10 times using the same endpoint and dataset.

Average, minimum, and maximum response times were recorded in milliseconds.

## 4. Results

### Cache MISS vs Cache HIT



| Metric | Cache MISS | Cache HIT |
|---|---:|---:|
| Average response time | 1598 ms | 10.5 ms |
| Minimum response time | 1070 ms | 4 ms |
| Maximum response time | 2170 ms | 28 ms |

### Performance Improvement

Using the average response times:

- Cache MISS: 1598 ms
- Cache HIT: 10.5 ms

**Response time reduction:**

( (1598 - 10.5) / 1598 ) × 100 = **99.34%**

The cache HIT scenario demonstrates approximately **99.34% lower response latency** compared to the cache MISS scenario.



## 5. Observations

- Cache HIT requests avoid repeated PostgreSQL queries.
- The in-memory cache significantly reduces response latency for repeated requests.
- The LRU cache provides efficient key-based lookups.
- The C++ cache server and Express backend work together to serve cached responses.
- When the cache server is unavailable, the Express backend can fall back to PostgreSQL.

## 6. Conclusion

The benchmark demonstrates the potential performance benefits of introducing an in-memory caching layer between the Express backend and PostgreSQL.

By serving repeated requests directly from memory, the system reduces database access and improves API response latency.

**Note:** Actual performance depends on network conditions, database latency, dataset size, and system resources. These results represent the tested local environment and should not be generalized to all deployment environments.
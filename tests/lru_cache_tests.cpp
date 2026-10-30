#include <iostream>
#include <stdexcept>
#include <cassert>

#include "../src/lru-cache/lru_cache.h"

using namespace std;

void testLRUCache()
{

    LRUCache c1(3);

    c1.put("GET:/api/users",
           "{\"id\":1,\"name\":\"Ammar\"}");

    c1.put("GET:/api/products",
           "{\"id\":101,\"name\":\"Keyboard\",\"price\":1200}");

    c1.put("GET:/api/orders",
           "{\"id\":501,\"status\":\"delivered\"}");

    c1.printList();
    c1.printCache();

    assert(c1.get("GET:/api/users") == "{\"id\":1,\"name\":\"Ammar\"}");
    // key "GET:/api/users" should be mru

    c1.printList();
    c1.printCache();

    c1.put("GET:/api/products?page=2",
           "{\"id\":102,\"name\":\"Mouse\",\"price\":500}");
    // key "GET:/api/products" should be evicted

    c1.printList();
    c1.printCache();

    assert(c1.get("GET:/api/products") == nullopt);
    assert(c1.get("GET:/api/orders") == "{\"id\":501,\"status\":\"delivered\"}");
    assert(c1.get("GET:/api/products?page=2") == "{\"id\":102,\"name\":\"Mouse\",\"price\":500}");

    cout << "\ntest passed!\n";
}

void testUpdatingExistingKey()
{

    LRUCache c1(2);

    c1.put("GET:/api/users",
           "{\"id\":1,\"name\":\"Ammar\"}");

    c1.put("GET:/api/products",
           "{\"id\":101,\"name\":\"Keyboard\",\"price\":1200}");

    c1.printList();
    c1.printCache();

    c1.put("GET:/api/users",
           "{\"id\":1,\"name\":\"Rizvi\"}");
    // value updated to "{\"id\":1,\"name\":\"Rizvi\"}" and becomes mru

    c1.printList();
    c1.printCache();

    c1.put("GET:/api/orders",
           "{\"id\":501,\"status\":\"delivered\"}");
    // key "GET:/api/products" removed

    c1.printList();
    c1.printCache();

    assert(c1.get("GET:/api/users") == "{\"id\":1,\"name\":\"Rizvi\"}");
    assert(c1.get("GET:/api/products") == nullopt);
    assert(c1.get("GET:/api/orders") == "{\"id\":501,\"status\":\"delivered\"}");

    cout << "\ntest passed!\n";
}

void testCapacityOne()
{

    LRUCache c1(1);

    c1.put("GET:/api/users",
           "{\"id\":1,\"name\":\"Ammar\"}");

    c1.printList();
    c1.printCache();

    c1.put("GET:/api/products",
           "{\"id\":101,\"name\":\"Keyboard\",\"price\":1200}");
    // key "GET:/api/users" removed

    c1.printList();
    c1.printCache();

    assert(c1.get("GET:/api/users") == nullopt);
    assert(c1.get("GET:/api/products") == "{\"id\":101,\"name\":\"Keyboard\",\"price\":1200}");

    cout << "\ntest passed!\n";
}

void testInvalidCapacity()
{

    bool zeroRejected = false;
    bool negativeRejected = false;

    try
    {
        LRUCache c1(0);
    }
    catch (const invalid_argument &e)
    {
        zeroRejected = true;
    }

    try
    {
        LRUCache c1(-2);
    }
    catch (const invalid_argument &e)
    {
        negativeRejected = true;
    }

    assert(zeroRejected);
    assert(negativeRejected);

    cout << "\n"
         << zeroRejected << "\n";
    cout << "\n"
         << negativeRejected << "\n";
    cout << "\ntest passed!\n";
}

void testEmptyStringValue()
{
    LRUCache c1(2);

    c1.put("GET:/api/empty", "");

    auto result = c1.get("GET:/api/empty");

    assert(result.has_value());
    assert(result.value() == "");

    cout << "\ntest passed!\n";
}

void testMissingKey()
{
    LRUCache c1(2);

    c1.put("GET:/api/users", "{\"id\":1}");

    auto result = c1.get("GET:/api/products");

    assert(!result.has_value());

    cout << "\ntest passed!\n";
}

void testRepeatedAccessChangesEviction()
{
    LRUCache c1(3);

    c1.put("A", "{\"value\":1}");
    c1.put("B", "{\"value\":2}");
    c1.put("C", "{\"value\":3}");
    // Current order: C -> B -> A

    c1.printList();
    c1.printCache();

    c1.get("A");
    c1.get("B");
    // Current order: B -> A -> C

    c1.printList();
    c1.printCache();

    c1.put("D", "{\"value\":4}");
    // C should be evicted.

    c1.printList();
    c1.printCache();

    assert(!c1.get("C").has_value());
    assert(c1.get("A").has_value());
    assert(c1.get("B").has_value());
    assert(c1.get("D").has_value());

    cout << "\ntest passed!\n";
}

int main()
{
    testLRUCache();
    testUpdatingExistingKey();
    testCapacityOne();
    testInvalidCapacity();
    testEmptyStringValue();
    testMissingKey();
    testRepeatedAccessChangesEviction();

    cout << "\nALL LRU CACHE TESTS PASSED!\n";

    return 0;
}

#include <iostream>
#include <stdexcept>
#include <cassert>

#include "../src/lru-cache/lru_cache.h"

using namespace std;

void testLRUCache()
{

    LRUCache c1(3);

    c1.put(1, 100);
    c1.put(2, 200);
    c1.put(3, 300);

    c1.printList();
    c1.printCache();

    assert(c1.get(1) == 100); // key 1 should be mru

    c1.printList();
    c1.printCache();

    c1.put(4, 400); // key 2 should be evicted

    c1.printList();
    c1.printCache();

    assert(c1.get(2) == -1);
    assert(c1.get(3) == 300);
    assert(c1.get(4) == 400);

    cout << "\ntest passed!\n";
}

void testUpdatingExistingKey()
{

    LRUCache c1(2);

    c1.put(1, 100);
    c1.put(2, 200);

    c1.printList();
    c1.printCache();

    c1.put(1, 999); // value updated to 999 and becomes mru

    c1.printList();
    c1.printCache();

    c1.put(3, 300); // key 2 removed

    c1.printList();
    c1.printCache();

    assert(c1.get(1) == 999);
    assert(c1.get(2) == -1);
    assert(c1.get(3) == 300);

    cout << "\ntest passed!\n";
}

void testCapacityOne()
{

    LRUCache c1(1);

    c1.put(1, 100);

    c1.printList();
    c1.printCache();

    c1.put(2, 200); // key 1 removed

    c1.printList();
    c1.printCache();

    assert(c1.get(1) == -1);
    assert(c1.get(2) == 200);

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
    cout << "test passed!\n";
}

int main()
{
    testLRUCache();
    // testUpdatingExistingKey();
    // testCapacityOne();
    // testInvalidCapacity();

    // cout << "\nALL LRU CACHE TESTS PASSED!\n";

    return 0;
}

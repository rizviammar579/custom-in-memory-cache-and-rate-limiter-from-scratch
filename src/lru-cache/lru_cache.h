#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <unordered_map>
using namespace std;

struct Node
{
    int key;
    int value;

    Node *prev;
    Node *next;

    Node(int k = 0, int v = 0) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache
{
private:
    Node *head;
    Node *tail;

    int capacity;

    unordered_map<int, Node *> cache;

    // used to remove/detach node from DLL used for both eviction as well as updating mru
    void removeNode(Node *node);

    // insert after head used for updating mru
    void insertAfterHead(Node *node);


public:
    // constructor to initialise LRUCache objects with a fixed capacity
    LRUCache(int capacity);

    // destructor for LRUCache objects
    ~LRUCache();

    // prevents copying two LRUCache objects
    //  eg : LRUCache c1(3);
    //       LRUCache c2 = c1;
    // problem here is that both contains the pointers to same nodes due to copying. When one object is deleted the pointers of other object points to nowhere. Hence problem comes when deleting the other object.
    LRUCache(const LRUCache &) = delete;
    LRUCache &operator=(const LRUCache &) = delete;

    // finds for the key in cache/map
    // if key found -> update mru and return value
    // if not found -> return -1;
    int get(int key);

    // checks the cache/map for the key
    // if key found -> updates the value and mru
    // if key not found -> checks for capacity
    // if capacity full -> remove lru from DLL and cache and deletes the node
    // then add a new node in DLL, updates mru and add a new (key, value) pair in cache/map
    void put(int key, int value);

    void printCache();
    void printList();

};

#endif
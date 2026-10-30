#include <iostream>
#include <stdexcept>
#include "lru_cache.h"

using namespace std;

LRUCache::LRUCache(int capacity)
{
    if (capacity <= 0)
        throw invalid_argument("Capacity must be positive");

    this->capacity = capacity;

    head = new Node();
    tail = new Node();

    // most recently used will be at head
    head->next = tail;

    // least recently used will be at tail
    tail->prev = head;
}

LRUCache::~LRUCache()
{
    Node *current = head;

    while (current != nullptr)
    {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

// DLL structure :
// HEAD ⇄ 10 ⇄ 30 ⇄ 20 ⇄ TAIL

void LRUCache::printCache()
{
    cout << "Cache (key -> value):\n";

    for (const auto &it : cache)
    {
        cout << it.first << " -> "
             << it.second->value << '\n';
    }
}

void LRUCache::printList()
{

    cout << "DLL (MRU -> LRU): ";

    Node *current = head->next;

    while (current != tail)
    {
        cout << "[" << current->key
             << ":" << current->value << "]";

        current = current->next;

        if (current != tail)
        {
            cout << " <-> ";
        }
    }

    cout << '\n';
}

// used to remove node from DLL used for both eviction as well as updating mru
void LRUCache::removeNode(Node *node)
{

    Node *previousNode = node->prev;
    Node *nextNode = node->next;

    previousNode->next = nextNode;
    nextNode->prev = previousNode;
}

// insert after head used for updating mru
void LRUCache::insertAfterHead(Node *node)
{

    Node *headNext = head->next;

    head->next = node;
    node->prev = head;
    node->next = headNext;
    headNext->prev = node;
}

// finds for the key in cache/map
// if key found -> update mru and return value
// if not found -> return nullopt;
optional<string> LRUCache::get(const string &key)
{
    auto it = cache.find(key);

    if (it != cache.end())
    {
        Node *targetNode = it->second;

        removeNode(targetNode);
        insertAfterHead(targetNode);

        return targetNode->value;
    }
    else
        return nullopt;
}

// checks the cache/map for the key
// if key found -> updates the value and mru
// if key not found -> checks for capacity
// if capacity full -> remove lru from DLL and cache and deletes the node
// then add a new node in DLL, updates mru and add a new (key, value) pair in cache/map

void LRUCache::put(const string &key, const string &value)
{

    auto it = cache.find(key);

    if (it != cache.end())
    {
        Node *targetNode = it->second;

        targetNode->value = value;

        removeNode(targetNode);
        insertAfterHead(targetNode);

        return;
    }
    else
    {

        Node *newNode = new Node(key, value);

        if (cache.size() == capacity)
        {
            Node *lruNode = tail->prev;

            removeNode(lruNode);
            cache.erase(lruNode->key);
            delete lruNode;
        }

        insertAfterHead(newNode);
        cache.emplace(key, newNode);
    }
}
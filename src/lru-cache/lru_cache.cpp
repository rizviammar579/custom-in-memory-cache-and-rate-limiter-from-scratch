#include <lru_cache.h>

LRUCache::LRUCache(int capacity){

    this->capacity = capacity;

    head = new Node();
    tail = new Node();

    //most recently used will be at head
    head->next = tail;
    head->prev = nullptr;

    //least recently used will be at tail
    tail->next = nullptr;
    tail->prev = head;

}
#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <unordered_map>
using namespace std;

class LRUCache {
    private:

    struct Node
    {
        int key;
        int value;

        Node* prev;
        Node* next;

        Node(int k = 0, int v = 0): key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;

    int capacity;

    unordered_map<int,Node*> cache;


    public:

    LRUCache(int capacity);
        

};

#endif
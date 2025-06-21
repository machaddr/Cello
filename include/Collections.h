#ifndef CELLO_STD_COLLECTIONS_H
#define CELLO_STD_COLLECTIONS_H

#include "Cello.h"

// Additional collection types
extern var Stack;
extern var Queue;
extern var Deque;
extern var Set;
extern var HashMap;
extern var LinkedList;
extern var BinaryTree;
extern var Heap;

// Stack structure
struct Stack {
    var* items;
    size_t size;
    size_t capacity;
};

// Queue structure
struct Queue {
    var* items;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
};

// Deque (Double-ended queue) structure
struct Deque {
    var* items;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
};

// Set structure (hash-based)
struct Set {
    var* buckets;
    size_t size;
    size_t capacity;
    size_t num_items;
};

// HashMap structure
struct HashMap {
    var* keys;
    var* values;
    bool* occupied;
    size_t size;
    size_t capacity;
};

// LinkedList node
struct ListNode {
    var data;
    struct ListNode* next;
    struct ListNode* prev;
};

// LinkedList structure
struct LinkedList {
    struct ListNode* head;
    struct ListNode* tail;
    size_t size;
};

// Binary tree node
struct TreeNode {
    var data;
    struct TreeNode* left;
    struct TreeNode* right;
};

// BinaryTree structure
struct BinaryTree {
    struct TreeNode* root;
    size_t size;
    int (*compare)(var, var);
};

// Heap structure (min-heap by default)
struct Heap {
    var* items;
    size_t size;
    size_t capacity;
    int (*compare)(var, var);
};

// Stack functions
var stack_new(void);
void stack_push(var self, var item);
var stack_pop(var self);
var stack_peek(var self);
bool stack_is_empty(var self);
size_t stack_size(var self);

// Queue functions
var queue_new(void);
void queue_enqueue(var self, var item);
var queue_dequeue(var self);
var queue_front(var self);
bool queue_is_empty(var self);
size_t queue_size(var self);

// Deque functions
var deque_new(void);
void deque_push_front(var self, var item);
void deque_push_back(var self, var item);
var deque_pop_front(var self);
var deque_pop_back(var self);
var deque_front(var self);
var deque_back(var self);
bool deque_is_empty(var self);
size_t deque_size(var self);

// Set functions
var set_new(void);
void set_add(var self, var item);
void set_remove(var self, var item);
bool set_contains(var self, var item);
size_t set_size(var self);
var set_union(var a, var b);
var set_intersection(var a, var b);
var set_difference(var a, var b);

// HashMap functions
var hashmap_new(void);
void hashmap_put(var self, var key, var value);
var hashmap_get(var self, var key);
void hashmap_remove(var self, var key);
bool hashmap_contains_key(var self, var key);
size_t hashmap_size(var self);
var hashmap_keys(var self);
var hashmap_values(var self);

// LinkedList functions
var linkedlist_new(void);
void linkedlist_add_first(var self, var item);
void linkedlist_add_last(var self, var item);
void linkedlist_add_at(var self, size_t index, var item);
var linkedlist_remove_first(var self);
var linkedlist_remove_last(var self);
var linkedlist_remove_at(var self, size_t index);
var linkedlist_get(var self, size_t index);
void linkedlist_set(var self, size_t index, var item);
size_t linkedlist_size(var self);

// BinaryTree functions
var binarytree_new(int (*compare)(var, var));
void binarytree_insert(var self, var item);
void binarytree_remove(var self, var item);
bool binarytree_contains(var self, var item);
var binarytree_find_min(var self);
var binarytree_find_max(var self);
size_t binarytree_size(var self);
void binarytree_inorder(var self, void (*callback)(var));
void binarytree_preorder(var self, void (*callback)(var));
void binarytree_postorder(var self, void (*callback)(var));

// Heap functions
var heap_new(int (*compare)(var, var));
void heap_push(var self, var item);
var heap_pop(var self);
var heap_peek(var self);
size_t heap_size(var self);
bool heap_is_empty(var self);

// Utility functions
var collections_default_compare(var a, var b);
uint64_t collections_hash(var item);

// Initialization function
void __cello_std_collections_init(void);

#endif
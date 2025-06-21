#include "Cello.h"
#include "Collections.h"
#include <stdlib.h>
#include <string.h>

// Collection types
var Stack;
var Queue;
var Deque;
var Set;
var HashMap;
var LinkedList;
var BinaryTree;  
var Heap;

static bool collections_initialized = false;

#define DEFAULT_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75

// Stack implementation
static void Stack_New(var self, var args) {
    struct Stack* s = cast(self, Stack);
    s->capacity = DEFAULT_CAPACITY;
    s->size = 0;
    s->items = malloc(s->capacity * sizeof(var));
}

static void Stack_Del(var self) {
    struct Stack* s = cast(self, Stack);
    if (s->items) {
        free(s->items);
        s->items = NULL;
    }
}

static int Stack_Show(var self, var output, int pos) {
    struct Stack* s = cast(self, Stack);
    int written = 0;
    written += print_to(output, pos + written, "Stack[");
    for (size_t i = 0; i < s->size; i++) {
        written += show_to(s->items[i], output, pos + written);
        if (i < s->size - 1) written += print_to(output, pos + written, ", ");
    }
    written += print_to(output, pos + written, "]");
    return written;
}

var Stack = Cello(Stack,
    Instance(New, Stack_New, Stack_Del),
    Instance(Show, Stack_Show));

// Queue implementation
static void Queue_New(var self, var args) {
    struct Queue* q = cast(self, Queue);
    q->capacity = DEFAULT_CAPACITY;
    q->size = 0;
    q->front = 0;
    q->rear = 0;
    q->items = malloc(q->capacity * sizeof(var));
}

static void Queue_Del(var self) {
    struct Queue* q = cast(self, Queue);
    if (q->items) {
        free(q->items);
        q->items = NULL;
    }
}

static int Queue_Show(var self, var output, int pos) {
    struct Queue* q = cast(self, Queue);
    int written = 0;
    written += print_to(output, pos + written, "Queue[");
    for (size_t i = 0; i < q->size; i++) {
        size_t idx = (q->front + i) % q->capacity;
        written += show_to(q->items[idx], output, pos + written);
        if (i < q->size - 1) written += print_to(output, pos + written, ", ");
    }
    written += print_to(output, pos + written, "]");
    return written;
}

var Queue = Cello(Queue,
    Instance(New, Queue_New, Queue_Del),
    Instance(Show, Queue_Show));

// Deque implementation
static void Deque_New(var self, var args) {
    struct Deque* d = cast(self, Deque);
    d->capacity = DEFAULT_CAPACITY;
    d->size = 0;
    d->front = 0;
    d->rear = 0;
    d->items = malloc(d->capacity * sizeof(var));
}

static void Deque_Del(var self) {
    struct Deque* d = cast(self, Deque);
    if (d->items) {
        free(d->items);
        d->items = NULL;
    }
}

static int Deque_Show(var self, var output, int pos) {
    struct Deque* d = cast(self, Deque);
    int written = 0;
    written += print_to(output, pos + written, "Deque[");
    for (size_t i = 0; i < d->size; i++) {
        size_t idx = (d->front + i) % d->capacity;
        written += show_to(d->items[idx], output, pos + written);
        if (i < d->size - 1) written += print_to(output, pos + written, ", ");
    }
    written += print_to(output, pos + written, "]");
    return written;
}

var Deque = Cello(Deque,
    Instance(New, Deque_New, Deque_Del),  
    Instance(Show, Deque_Show));

// Set implementation
static void Set_New(var self, var args) {
    struct Set* s = cast(self, Set);
    s->capacity = DEFAULT_CAPACITY;
    s->size = 0;
    s->num_items = 0;
    s->buckets = calloc(s->capacity, sizeof(var));
}

static void Set_Del(var self) {
    struct Set* s = cast(self, Set);
    if (s->buckets) {
        free(s->buckets);
        s->buckets = NULL;
    }
}

static int Set_Show(var self, var output, int pos) {
    struct Set* s = cast(self, Set);
    int written = 0;
    written += print_to(output, pos + written, "Set{");
    bool first = true;
    for (size_t i = 0; i < s->capacity; i++) {
        if (s->buckets[i]) {
            if (!first) written += print_to(output, pos + written, ", ");
            written += show_to(s->buckets[i], output, pos + written);
            first = false;
        }
    }
    written += print_to(output, pos + written, "}");
    return written;
}

var Set = Cello(Set,
    Instance(New, Set_New, Set_Del),
    Instance(Show, Set_Show));

// HashMap implementation
static void HashMap_New(var self, var args) {
    struct HashMap* h = cast(self, HashMap);
    h->capacity = DEFAULT_CAPACITY;
    h->size = 0;
    h->keys = calloc(h->capacity, sizeof(var));
    h->values = calloc(h->capacity, sizeof(var));
    h->occupied = calloc(h->capacity, sizeof(bool));
}

static void HashMap_Del(var self) {
    struct HashMap* h = cast(self, HashMap);
    if (h->keys) {
        free(h->keys);
        h->keys = NULL;
    }
    if (h->values) {
        free(h->values);
        h->values = NULL;
    }
    if (h->occupied) {
        free(h->occupied);
        h->occupied = NULL;
    }
}

static int HashMap_Show(var self, var output, int pos) {
    struct HashMap* h = cast(self, HashMap);
    int written = 0;
    written += print_to(output, pos + written, "HashMap{");
    bool first = true;
    for (size_t i = 0; i < h->capacity; i++) {
        if (h->occupied[i]) {
            if (!first) written += print_to(output, pos + written, ", ");
            written += show_to(h->keys[i], output, pos + written);
            written += print_to(output, pos + written, ": ");
            written += show_to(h->values[i], output, pos + written);
            first = false;
        }
    }
    written += print_to(output, pos + written, "}");
    return written;
}

var HashMap = Cello(HashMap,
    Instance(New, HashMap_New, HashMap_Del),
    Instance(Show, HashMap_Show));

// LinkedList implementation
static void LinkedList_New(var self, var args) {
    struct LinkedList* l = cast(self, LinkedList);
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

static void LinkedList_Del(var self) {
    struct LinkedList* l = cast(self, LinkedList);
    struct ListNode* current = l->head;
    while (current) {
        struct ListNode* next = current->next;
        free(current);
        current = next;
    }
}

static int LinkedList_Show(var self, var output, int pos) {
    struct LinkedList* l = cast(self, LinkedList);
    int written = 0;
    written += print_to(output, pos + written, "LinkedList[");
    struct ListNode* current = l->head;
    bool first = true;
    while (current) {
        if (!first) written += print_to(output, pos + written, ", ");
        written += show_to(current->data, output, pos + written);
        current = current->next;
        first = false;
    }
    written += print_to(output, pos + written, "]");
    return written;
}

var LinkedList = Cello(LinkedList,
    Instance(New, LinkedList_New, LinkedList_Del),
    Instance(Show, LinkedList_Show));

// BinaryTree implementation
static void BinaryTree_New(var self, var args) {
    struct BinaryTree* t = cast(self, BinaryTree);
    t->root = NULL;
    t->size = 0;
    t->compare = NULL;
    
    if (len(args) > 0) {
        // First argument should be comparison function
        // For now, use default comparison
        t->compare = (int(*)(var,var))collections_default_compare;
    }
}

static void BinaryTree_Del(var self) {
    struct BinaryTree* t = cast(self, BinaryTree);
    // TODO: Implement tree cleanup
}

static int BinaryTree_Show(var self, var output, int pos) {
    struct BinaryTree* t = cast(self, BinaryTree);
    return print_to(output, pos, "BinaryTree(size=%lu)", $I(t->size));
}

var BinaryTree = Cello(BinaryTree,
    Instance(New, BinaryTree_New, BinaryTree_Del),
    Instance(Show, BinaryTree_Show));

// Heap implementation  
static void Heap_New(var self, var args) {
    struct Heap* h = cast(self, Heap);
    h->capacity = DEFAULT_CAPACITY;
    h->size = 0;
    h->items = malloc(h->capacity * sizeof(var));
    h->compare = (int(*)(var,var))collections_default_compare;
}

static void Heap_Del(var self) {
    struct Heap* h = cast(self, Heap);
    if (h->items) {
        free(h->items);
        h->items = NULL;
    }
}

static int Heap_Show(var self, var output, int pos) {
    struct Heap* h = cast(self, Heap);
    int written = 0;
    written += print_to(output, pos + written, "Heap[");
    for (size_t i = 0; i < h->size; i++) {
        written += show_to(h->items[i], output, pos + written);
        if (i < h->size - 1) written += print_to(output, pos + written, ", ");
    }
    written += print_to(output, pos + written, "]");
    return written;
}

var Heap = Cello(Heap,
    Instance(New, Heap_New, Heap_Del),
    Instance(Show, Heap_Show));

// Stack functions
var stack_new(void) {
    return new(Stack);
}

void stack_push(var self, var item) {
    struct Stack* s = cast(self, Stack);
    if (s->size >= s->capacity) {
        s->capacity *= 2;
        s->items = realloc(s->items, s->capacity * sizeof(var));
    }
    s->items[s->size++] = item;
}

var stack_pop(var self) {
    struct Stack* s = cast(self, Stack);
    if (s->size == 0) return NULL;
    return s->items[--s->size];
}

var stack_peek(var self) {
    struct Stack* s = cast(self, Stack);
    if (s->size == 0) return NULL;
    return s->items[s->size - 1];
}

bool stack_is_empty(var self) {
    struct Stack* s = cast(self, Stack);
    return s->size == 0;
}

size_t stack_size(var self) {
    struct Stack* s = cast(self, Stack);
    return s->size;
}

// Queue functions
var queue_new(void) {
    return new(Queue);
}

void queue_enqueue(var self, var item) {
    struct Queue* q = cast(self, Queue);
    if (q->size >= q->capacity) {
        q->capacity *= 2;
        var* new_items = malloc(q->capacity * sizeof(var));
        for (size_t i = 0; i < q->size; i++) {
            new_items[i] = q->items[(q->front + i) % (q->capacity / 2)];
        }
        free(q->items);
        q->items = new_items;
        q->front = 0;
        q->rear = q->size;
    }
    q->items[q->rear] = item;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
}

var queue_dequeue(var self) {
    struct Queue* q = cast(self, Queue);
    if (q->size == 0) return NULL;
    var item = q->items[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return item;
}

var queue_front(var self) {
    struct Queue* q = cast(self, Queue);
    if (q->size == 0) return NULL;
    return q->items[q->front];
}

bool queue_is_empty(var self) {
    struct Queue* q = cast(self, Queue);
    return q->size == 0;
}

size_t queue_size(var self) {
    struct Queue* q = cast(self, Queue);
    return q->size;
}

// Deque functions
var deque_new(void) {
    return new(Deque);
}

void deque_push_front(var self, var item) {
    struct Deque* d = cast(self, Deque);
    if (d->size >= d->capacity) {
        d->capacity *= 2;
        var* new_items = malloc(d->capacity * sizeof(var));
        for (size_t i = 0; i < d->size; i++) {
            new_items[i] = d->items[(d->front + i) % (d->capacity / 2)];
        }
        free(d->items);
        d->items = new_items;
        d->front = 0;
        d->rear = d->size;
    }
    d->front = (d->front - 1 + d->capacity) % d->capacity;
    d->items[d->front] = item;
    d->size++;
}

void deque_push_back(var self, var item) {
    struct Deque* d = cast(self, Deque);
    if (d->size >= d->capacity) {
        d->capacity *= 2;
        var* new_items = malloc(d->capacity * sizeof(var));
        for (size_t i = 0; i < d->size; i++) {
            new_items[i] = d->items[(d->front + i) % (d->capacity / 2)];
        }
        free(d->items);
        d->items = new_items;
        d->front = 0;
        d->rear = d->size;
    }
    d->items[d->rear] = item;
    d->rear = (d->rear + 1) % d->capacity;
    d->size++;
}

var deque_pop_front(var self) {
    struct Deque* d = cast(self, Deque);
    if (d->size == 0) return NULL;
    var item = d->items[d->front];
    d->front = (d->front + 1) % d->capacity;
    d->size--;
    return item;
}

var deque_pop_back(var self) {
    struct Deque* d = cast(self, Deque);
    if (d->size == 0) return NULL;
    d->rear = (d->rear - 1 + d->capacity) % d->capacity;
    d->size--;
    return d->items[d->rear];
}

var deque_front(var self) {
    struct Deque* d = cast(self, Deque);
    if (d->size == 0) return NULL;
    return d->items[d->front];
}

var deque_back(var self) {
    struct Deque* d = cast(self, Deque);
    if (d->size == 0) return NULL;
    size_t back_idx = (d->rear - 1 + d->capacity) % d->capacity;
    return d->items[back_idx];
}

bool deque_is_empty(var self) {
    struct Deque* d = cast(self, Deque);
    return d->size == 0;
}

size_t deque_size(var self) {
    struct Deque* d = cast(self, Deque);
    return d->size;
}

// Set functions
var set_new(void) {
    return new(Set);
}

static size_t set_hash_index(var self, var item) {
    struct Set* s = cast(self, Set);
    uint64_t h = hash(item);
    return h % s->capacity;
}

void set_add(var self, var item) {
    struct Set* s = cast(self, Set);
    size_t idx = set_hash_index(self, item);
    
    // Linear probing
    while (s->buckets[idx] && !eq(s->buckets[idx], item)) {
        idx = (idx + 1) % s->capacity;
    }
    
    if (!s->buckets[idx]) {
        s->buckets[idx] = item;
        s->num_items++;
        
        // Resize if load factor too high
        if ((double)s->num_items / s->capacity > LOAD_FACTOR_THRESHOLD) {
            // TODO: Implement resize
        }
    }
}

void set_remove(var self, var item) {
    struct Set* s = cast(self, Set);
    size_t idx = set_hash_index(self, item);
    
    while (s->buckets[idx]) {
        if (eq(s->buckets[idx], item)) {
            s->buckets[idx] = NULL;
            s->num_items--;
            return;
        }
        idx = (idx + 1) % s->capacity;
    }
}

bool set_contains(var self, var item) {
    struct Set* s = cast(self, Set);
    size_t idx = set_hash_index(self, item);
    
    while (s->buckets[idx]) {
        if (eq(s->buckets[idx], item)) {
            return true;
        }
        idx = (idx + 1) % s->capacity;
    }
    return false;
}

size_t set_size(var self) {
    struct Set* s = cast(self, Set);
    return s->num_items;
}

// HashMap functions
var hashmap_new(void) {
    return new(HashMap);
}

static size_t hashmap_hash_index(var self, var key) {
    struct HashMap* h = cast(self, HashMap);
    uint64_t hash_val = hash(key);
    return hash_val % h->capacity;
}

void hashmap_put(var self, var key, var value) {
    struct HashMap* h = cast(self, HashMap);
    size_t idx = hashmap_hash_index(self, key);
    
    // Linear probing
    while (h->occupied[idx] && !eq(h->keys[idx], key)) {
        idx = (idx + 1) % h->capacity;
    }
    
    if (!h->occupied[idx]) {
        h->size++;
    }
    
    h->keys[idx] = key;
    h->values[idx] = value;
    h->occupied[idx] = true;
}

var hashmap_get(var self, var key) {
    struct HashMap* h = cast(self, HashMap);
    size_t idx = hashmap_hash_index(self, key);
    
    while (h->occupied[idx]) {
        if (eq(h->keys[idx], key)) {
            return h->values[idx];
        }
        idx = (idx + 1) % h->capacity;
    }
    return NULL;
}

void hashmap_remove(var self, var key) {
    struct HashMap* h = cast(self, HashMap);
    size_t idx = hashmap_hash_index(self, key);
    
    while (h->occupied[idx]) {
        if (eq(h->keys[idx], key)) {
            h->occupied[idx] = false;
            h->size--;
            return;
        }
        idx = (idx + 1) % h->capacity;
    }
}

bool hashmap_contains_key(var self, var key) {
    return hashmap_get(self, key) != NULL;
}

size_t hashmap_size(var self) {
    struct HashMap* h = cast(self, HashMap);
    return h->size;
}

// LinkedList functions
var linkedlist_new(void) {
    return new(LinkedList);
}

void linkedlist_add_first(var self, var item) {
    struct LinkedList* l = cast(self, LinkedList);
    struct ListNode* node = malloc(sizeof(struct ListNode));
    node->data = item;
    node->next = l->head;
    node->prev = NULL;
    
    if (l->head) {
        l->head->prev = node;
    } else {
        l->tail = node;
    }
    
    l->head = node;
    l->size++;
}

void linkedlist_add_last(var self, var item) {
    struct LinkedList* l = cast(self, LinkedList);
    struct ListNode* node = malloc(sizeof(struct ListNode));
    node->data = item;
    node->next = NULL;
    node->prev = l->tail;
    
    if (l->tail) {
        l->tail->next = node;
    } else {
        l->head = node;
    }
    
    l->tail = node;
    l->size++;
}

var linkedlist_remove_first(var self) {
    struct LinkedList* l = cast(self, LinkedList);
    if (!l->head) return NULL;
    
    struct ListNode* node = l->head;
    var data = node->data;
    
    l->head = node->next;
    if (l->head) {
        l->head->prev = NULL;
    } else {
        l->tail = NULL;
    }
    
    free(node);
    l->size--;
    return data;
}

var linkedlist_remove_last(var self) {
    struct LinkedList* l = cast(self, LinkedList);
    if (!l->tail) return NULL;
    
    struct ListNode* node = l->tail;
    var data = node->data;
    
    l->tail = node->prev;
    if (l->tail) {
        l->tail->next = NULL;
    } else {
        l->head = NULL;
    }
    
    free(node);
    l->size--;
    return data;
}

size_t linkedlist_size(var self) {
    struct LinkedList* l = cast(self, LinkedList);
    return l->size;
}

// Heap functions
var heap_new(int (*compare)(var, var)) {
    var h = new(Heap);
    struct Heap* heap = cast(h, Heap);
    if (compare) {
        heap->compare = compare;
    }
    return h;
}

static void heap_swap(struct Heap* h, size_t i, size_t j) {
    var temp = h->items[i];
    h->items[i] = h->items[j];
    h->items[j] = temp;
}

static void heap_heapify_up(struct Heap* h, size_t idx) {
    if (idx == 0) return;
    
    size_t parent = (idx - 1) / 2;
    if (h->compare(h->items[idx], h->items[parent]) < 0) {
        heap_swap(h, idx, parent);
        heap_heapify_up(h, parent);
    }
}

static void heap_heapify_down(struct Heap* h, size_t idx) {
    size_t left = 2 * idx + 1;
    size_t right = 2 * idx + 2;
    size_t smallest = idx;
    
    if (left < h->size && h->compare(h->items[left], h->items[smallest]) < 0) {
        smallest = left;
    }
    
    if (right < h->size && h->compare(h->items[right], h->items[smallest]) < 0) {
        smallest = right;
    }
    
    if (smallest != idx) {
        heap_swap(h, idx, smallest);
        heap_heapify_down(h, smallest);
    }
}

void heap_push(var self, var item) {
    struct Heap* h = cast(self, Heap);
    if (h->size >= h->capacity) {
        h->capacity *= 2;
        h->items = realloc(h->items, h->capacity * sizeof(var));
    }
    
    h->items[h->size] = item;
    heap_heapify_up(h, h->size);
    h->size++;
}

var heap_pop(var self) {
    struct Heap* h = cast(self, Heap);
    if (h->size == 0) return NULL;
    
    var root = h->items[0];
    h->items[0] = h->items[h->size - 1];
    h->size--;
    heap_heapify_down(h, 0);
    
    return root;
}

var heap_peek(var self) {
    struct Heap* h = cast(self, Heap);
    if (h->size == 0) return NULL;
    return h->items[0];
}

size_t heap_size(var self) {
    struct Heap* h = cast(self, Heap);
    return h->size;
}

bool heap_is_empty(var self) {
    struct Heap* h = cast(self, Heap);
    return h->size == 0;
}

// Utility functions
var collections_default_compare(var a, var b) {
    return $I(cmp(a, b));
}

uint64_t collections_hash(var item) {
    return hash(item);
}

// Initialization function
void __cello_std_collections_init(void) {
    if (collections_initialized) return;
    collections_initialized = true;
}
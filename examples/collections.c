/*
 * Test Cello Collections Standard Library Module
 * Using pure Cello syntax and data structures
 */
#include <Cello.h>

int main(int argc, char** argv) {
    println("=== Testing Cello Collections Standard Library ===");
    
    // Test Stack using Cello objects
    println("\n--- Stack Tests ---");
    var stack = stack_new();
    var val1 = $I(10);
    var val2 = $I(20);
    var val3 = $I(30);
    var str1 = $S("hello");
    var str2 = $S("world");
    
    print("Stack is empty: "); 
    show($I(stack_is_empty(stack) ? 1 : 0)); println("");
    
    stack_push(stack, val1);
    stack_push(stack, val2);
    stack_push(stack, val3);
    stack_push(stack, str1);
    
    var stack_size_val = $I(stack_size(stack));
    print("Stack size: "); show(stack_size_val); println("");
    
    var peek_val = stack_peek(stack);
    print("Peek: "); show(peek_val); println("");
    
    var pop_val = stack_pop(stack);
    print("Popped: "); show(pop_val); println("");
    
    var new_size_val = $I(stack_size(stack));
    print("Stack size after pop: "); show(new_size_val); println("");
    
    // Test Queue using Cello objects
    println("\n--- Queue Tests ---");
    var queue = queue_new();
    var item1 = $S("first");
    var item2 = $S("second");
    var item3 = $S("third");
    var num1 = $I(100);
    var num2 = $I(200);
    
    print("Queue is empty: ");
    show($I(queue_is_empty(queue) ? 1 : 0)); println("");
    
    queue_enqueue(queue, item1);
    queue_enqueue(queue, item2);
    queue_enqueue(queue, item3);
    queue_enqueue(queue, num1);
    
    var queue_size_val = $I(queue_size(queue));
    print("Queue size: "); show(queue_size_val); println("");
    
    var front_val = queue_front(queue);
    print("Front: "); show(front_val); println("");
    
    var dequeue_val = queue_dequeue(queue);
    print("Dequeued: "); show(dequeue_val); println("");
    
    var new_queue_size = $I(queue_size(queue));
    print("Queue size after dequeue: "); show(new_queue_size); println("");
    
    // Test Deque using Cello objects
    println("\n--- Deque Tests ---");
    var deque = deque_new();
    var deque_val1 = $I(1);
    var deque_val2 = $I(2);
    var deque_val0 = $I(0);
    var deque_str = $S("middle");
    
    print("Deque is empty: ");
    show($I(deque_is_empty(deque) ? 1 : 0)); println("");
    
    deque_push_front(deque, deque_val1);
    deque_push_back(deque, deque_val2);
    deque_push_front(deque, deque_val0);
    deque_push_back(deque, deque_str);
    
    var deque_size_val = $I(deque_size(deque));
    print("Deque size: "); show(deque_size_val); println("");
    
    var front_deque = deque_front(deque);
    var back_deque = deque_back(deque);
    print("Front: "); show(front_deque); 
    print(", Back: "); show(back_deque); println("");
    
    var pop_front_val = deque_pop_front(deque);
    var pop_back_val = deque_pop_back(deque);
    print("Popped front: "); show(pop_front_val);
    print(", Popped back: "); show(pop_back_val); println("");
    
    // Test Set using Cello objects
    println("\n--- Set Tests ---");
    var my_set = set_new();
    var set_val1 = $I(5);
    var set_val2 = $I(10);
    var set_val3 = $I(15);
    var set_str1 = $S("apple");
    var set_str2 = $S("banana");
    
    set_add(my_set, set_val1);
    set_add(my_set, set_val2);
    set_add(my_set, set_val1); // Duplicate
    set_add(my_set, set_str1);
    
    var set_size_val = $I(set_size(my_set));
    print("Set size: "); show(set_size_val); println("");
    
    print("Contains 5: ");
    show($I(set_contains(my_set, set_val1) ? 1 : 0)); println("");
    print("Contains 15: ");
    show($I(set_contains(my_set, set_val3) ? 1 : 0)); println("");
    
    set_remove(my_set, set_val1);
    var set_size_after = $I(set_size(my_set));
    print("Set size after removing 5: "); show(set_size_after); println("");
    
    // Test HashMap using Cello objects
    println("\n--- HashMap Tests ---");
    var hashmap = hashmap_new();
    var key1 = $S("name");
    var key2 = $S("age");
    var key3 = $S("city");
    var value1 = $S("John");
    var value2 = $I(30);
    var value3 = $S("NYC");
    
    hashmap_put(hashmap, key1, value1);
    hashmap_put(hashmap, key2, value2);
    hashmap_put(hashmap, key3, value3);
    
    var hashmap_size_val = $I(hashmap_size(hashmap));
    print("HashMap size: "); show(hashmap_size_val); println("");
    
    var name_value = hashmap_get(hashmap, key1);
    print("Name: "); 
    if (name_value) show(name_value);
    else print("null");
    println("");
    
    print("Contains key 'age': ");
    show($I(hashmap_contains_key(hashmap, key2) ? 1 : 0)); println("");
    
    hashmap_remove(hashmap, key3);
    var hashmap_final_size = $I(hashmap_size(hashmap));
    print("HashMap size after removing 'city': "); 
    show(hashmap_final_size); println("");
    
    // Test LinkedList using Cello objects (only implemented functions)
    println("\n--- LinkedList Tests ---");
    var linkedlist = linkedlist_new();
    var list_val1 = $S("first");
    var list_val2 = $S("second");
    var list_val3 = $S("third");
    var list_val4 = $S("fourth");
    
    linkedlist_add_first(linkedlist, list_val1);
    linkedlist_add_last(linkedlist, list_val2);
    linkedlist_add_last(linkedlist, list_val3);
    
    var linkedlist_size_val = $I(linkedlist_size(linkedlist));
    print("LinkedList size: "); show(linkedlist_size_val); println("");
    
    // Note: linkedlist_get() is not implemented, so we skip index access
    print("LinkedList created successfully with add_first/add_last"); println("");
    
    var removed_first = linkedlist_remove_first(linkedlist);
    print("Removed first: "); show(removed_first); println("");
    
    var final_linkedlist_size = $I(linkedlist_size(linkedlist));
    print("LinkedList size after removal: "); 
    show(final_linkedlist_size); println("");
    
    // Test storing collections in Cello List
    println("\n--- Testing Collections Storage in Cello Structures ---");
    var collections_list = new(List, String);
    
    // Store collection names
    push(collections_list, $S("Stack"));
    push(collections_list, $S("Queue"));
    push(collections_list, $S("Deque"));
    push(collections_list, $S("Set"));
    push(collections_list, $S("HashMap"));
    push(collections_list, $S("LinkedList"));
    
    print("Tested collections: "); show(collections_list); println("");
    
    // Store collection sizes in Table
    var sizes_table = new(Table, String, Int);
    set(sizes_table, $S("stack_final"), new_size_val);
    set(sizes_table, $S("queue_final"), new_queue_size);
    set(sizes_table, $S("set_final"), set_size_after);
    set(sizes_table, $S("hashmap_final"), hashmap_final_size);
    
    print("Collection sizes: "); show(sizes_table); println("");
    
    println("\n=== Collections Standard Library Test Complete ===");
    return 0;
}
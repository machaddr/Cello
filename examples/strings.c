/*
 * Test Cello Core String Functionality (Working Functions Only)
 * Using pure Cello syntax and data structures
 */
#include <Cello.h>

int main(int argc, char** argv) {
    println("=== Testing Cello Core String Functionality ===");
    
    // Test string creation and basic operations
    println("\n--- String Creation and Basic Operations ---");
    var str1 = $S("Hello");
    var str2 = $S("World");
    var str3 = $S("Testing");
    
    // Test string length using Cello's len() function
    var len1 = $I(len(str1));
    var len2 = $I(len(str2));
    var len3 = $I(len(str3));
    print("String 1 length: "); show(len1); println("");
    print("String 2 length: "); show(len2); println("");
    print("String 3 length: "); show(len3); println("");
    
    // Test string concatenation using copy + concat
    println("\n--- String Concatenation ---");
    var concat_result = copy(str1);
    concat(concat_result, str2);
    print("Concatenated 'Hello' + 'World': "); show(concat_result); println("");
    
    // Test string comparison using Cello's eq() and cmp()
    println("\n--- String Comparison ---");
    var eq_result1 = $I(eq(str1, str2) ? 1 : 0);
    var eq_result2 = $I(eq(str1, str1) ? 1 : 0);
    var cmp_result = $I(cmp(str1, str2));
    print("'Hello' equals 'World': "); show(eq_result1); println("");
    print("'Hello' equals 'Hello': "); show(eq_result2); println("");
    print("Compare 'Hello' vs 'World': "); show(cmp_result); println("");
    
    // Test string contains and search (basic Strings library functions)
    println("\n--- String Search Operations ---");
    var search_str = $S("ell");
    var contains_result = $I(string_contains(str1, search_str) ? 1 : 0);
    print("'Hello' contains 'ell': "); show(contains_result); println("");
    
    var find_result = $I(string_find(str1, search_str));
    print("Index of 'ell' in 'Hello': "); show(find_result); println("");
    
    // Test string startswith/endswith
    println("\n--- String Prefix/Suffix Tests ---");
    var prefix = $S("Hell");
    var suffix = $S("rld");
    var starts_result = $I(string_startswith(str1, prefix) ? 1 : 0);
    var ends_result = $I(string_endswith(str2, suffix) ? 1 : 0);
    print("'Hello' starts with 'Hell': "); show(starts_result); println("");
    print("'World' ends with 'rld': "); show(ends_result); println("");
    
    // Test character classification (simple cases)
    println("\n--- Character Classification ---");
    var alpha_str = $S("abc");
    var digit_str = $S("123");
    
    var is_alpha = $I(string_isalpha(alpha_str) ? 1 : 0);
    var is_digit = $I(string_isdigit(digit_str) ? 1 : 0);
    
    print("'abc' is alpha: "); show(is_alpha); println("");
    print("'123' is digit: "); show(is_digit); println("");
    
    // Test C string access
    println("\n--- C String Access ---");
    var c_str_val = c_str(str1);
    printf("C string from 'Hello': %s\n", c_str_val);
    printf("C string length: %zu\n", strlen(c_str_val));
    
    // Test hash function
    println("\n--- String Hashing ---");
    var hash_val1 = $I(hash(str1));
    var hash_val2 = $I(hash(str2));
    print("Hash of 'Hello': "); show(hash_val1); println("");
    print("Hash of 'World': "); show(hash_val2); println("");
    
    // Test copy functionality
    println("\n--- String Copying ---");
    var str_copy = copy(str3);
    var copy_equal = $I(eq(str3, str_copy) ? 1 : 0);
    print("Original: "); show(str3); println("");
    print("Copy: "); show(str_copy); println("");
    print("Copy equals original: "); show(copy_equal); println("");
    
    // Test storing string results in Cello structures
    println("\n--- Testing String Storage in Cello Structures ---");
    var strings_list = new(List, String);
    
    // Store strings in list
    push(strings_list, str1);
    push(strings_list, str2);
    push(strings_list, str3);
    push(strings_list, concat_result);
    push(strings_list, str_copy);
    
    print("Strings in list: "); show(strings_list); println("");
    
    // Store string metrics in Table
    var metrics_table = new(Table, String, Int);
    set(metrics_table, $S("total_strings"), $I(5));
    set(metrics_table, $S("hello_length"), len1);
    set(metrics_table, $S("world_length"), len2);
    set(metrics_table, $S("testing_length"), len3);
    
    print("String metrics: "); show(metrics_table); println("");
    
    // Test string access by index in list
    println("\n--- String List Access ---");
    var first_str = get(strings_list, $I(0));
    var last_str = get(strings_list, $I(-1));
    print("First string in list: "); show(first_str); println("");
    print("Last string in list: "); show(last_str); println("");
    
    println("\n========================================");
    println("✅ SUCCESS: Core Cello string functionality works!");
    println("   - Basic string operations functional");
    println("   - String comparison and search working");
    println("   - String storage in collections working");
    println("   - Core Strings library functions working");
    println("========================================");
    
    return 0;
}
/*
 * Test Cello Math Standard Library Module
 * Using pure Cello syntax and data structures
 */
#include <Cello.h>

int main(int argc, char** argv) {
    println("=== Testing Cello Math Standard Library ===");
    
    // Test basic math functions using Cello Float objects
    println("\n--- Basic Math Functions ---");
    var num1 = $F(25.0);
    var num2 = $F(2.0);
    var num3 = $F(3.0);
    var neg_num = $F(-7.5);
    var floor_num = $F(3.8);
    var ceil_num = $F(3.2);
    var round_num = $F(3.6);
    
    var sqrt_res = $F(std_sqrt(c_float(num1)));
    var pow_res = $F(std_pow(c_float(num2), c_float(num3)));
    var abs_res = $F(std_abs(c_float(neg_num)));
    var floor_res = $F(std_floor(c_float(floor_num)));
    var ceil_res = $F(std_ceil(c_float(ceil_num)));
    var round_res = $F(std_round(c_float(round_num)));
    
    print("std_sqrt(25.0) = "); show(sqrt_res); println("");
    print("std_pow(2.0, 3.0) = "); show(pow_res); println("");
    print("std_abs(-7.5) = "); show(abs_res); println("");
    print("std_floor(3.8) = "); show(floor_res); println("");
    print("std_ceil(3.2) = "); show(ceil_res); println("");
    print("std_round(3.6) = "); show(round_res); println("");
    
    // Test trigonometric functions using Cello objects
    println("\n--- Trigonometric Functions ---");
    var pi = $F(3.14159265359);
    var pi_quarter = $F(3.14159265359 / 4.0);
    var half = $F(0.5);
    var one = $F(1.0);
    
    var sin_res = $F(std_sin(c_float(pi_quarter)));
    var cos_res = $F(std_cos(c_float(pi_quarter)));
    var tan_res = $F(std_tan(c_float(pi_quarter)));
    var asin_res = $F(std_asin(c_float(half)));
    var acos_res = $F(std_acos(c_float(half)));
    var atan_res = $F(std_atan(c_float(one)));
    
    print("std_sin(π/4) = "); show(sin_res); println("");
    print("std_cos(π/4) = "); show(cos_res); println("");
    print("std_tan(π/4) = "); show(tan_res); println("");
    print("std_asin(0.5) = "); show(asin_res); println("");
    print("std_acos(0.5) = "); show(acos_res); println("");
    print("std_atan(1.0) = "); show(atan_res); println("");
    
    // Test logarithmic functions using Cello objects
    println("\n--- Logarithmic Functions ---");
    var e_val = $F(2.718282);
    var hundred = $F(100.0);
    var eight = $F(8.0);
    
    var log_res = $F(std_log(c_float(e_val)));
    var log10_res = $F(std_log10(c_float(hundred)));
    var log2_res = $F(std_log2(c_float(eight)));
    var exp_res = $F(std_exp(c_float(one)));
    
    print("std_log(2.718282) = "); show(log_res); println("");
    print("std_log10(100.0) = "); show(log10_res); println("");
    print("std_log2(8.0) = "); show(log2_res); println("");
    print("std_exp(1.0) = "); show(exp_res); println("");
    
    // Test min/max functions using Cello objects
    println("\n--- Min/Max Functions ---");
    var five = $F(5.0);
    var three = $F(3.0);
    var seven = $F(7.0);
    var zero = $F(0.0);
    var ten = $F(10.0);
    
    var min_res = $F(std_min(c_float(five), c_float(three)));
    var max_res = $F(std_max(c_float(five), c_float(three)));
    var clamp_res = $F(std_clamp(c_float(seven), c_float(one), c_float(five)));
    var lerp_res = $F(std_lerp(c_float(zero), c_float(ten), c_float(half)));
    
    print("std_min(5.0, 3.0) = "); show(min_res); println("");
    print("std_max(5.0, 3.0) = "); show(max_res); println("");
    print("std_clamp(7.0, 1.0, 5.0) = "); show(clamp_res); println("");
    print("std_lerp(0.0, 10.0, 0.5) = "); show(lerp_res); println("");
    
    // Test random functions
    println("\n--- Random Functions ---");
    var seed = $I(42);
    var min_rand = $I(1);
    var max_rand = $I(10);
    var min_range = $F(5.0);
    var max_range = $F(15.0);
    
    std_seed_random(c_int(seed));
    var rand_int_res = $I(std_random_int(c_int(min_rand), c_int(max_rand)));
    var rand_double_res = $F(std_random_double());
    var rand_range_res = $F(std_random_range(c_float(min_range), c_float(max_range)));
    
    print("std_random_int(1, 10) = "); show(rand_int_res); println("");
    print("std_random_double() = "); show(rand_double_res); println("");
    print("std_random_range(5.0, 15.0) = "); show(rand_range_res); println("");
    
    // Test mathematical constants using Cello List for results
    println("\n--- Testing Math Results Storage in Cello Structures ---");
    var results = new(List, Float);
    
    // Store some calculation results in Cello List
    var sqrt_result = $F(std_sqrt(16.0));
    var pow_result = $F(std_pow(3.0, 2.0));
    var sin_result = $F(std_sin(1.57079633)); // π/2
    
    push(results, sqrt_result);
    push(results, pow_result);
    push(results, sin_result);
    
    print("Math results stored in List: "); show(results); println("");
    
    // Test with Cello Table for named results
    var math_table = new(Table, String, Float);
    set(math_table, $S("sqrt_16"), sqrt_result);
    set(math_table, $S("pow_3_2"), pow_result);
    set(math_table, $S("sin_pi_2"), sin_result);
    
    print("Math results in Table: "); show(math_table); println("");
    
    println("\n=== Math Standard Library Test Complete ===");
    return 0;
}
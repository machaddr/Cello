/*
 * Test Cello Time Standard Library Module
 * Using pure Cello syntax and data structures
 */
#include <Cello.h>

int main(int argc, char** argv) {
    println("=== Testing Cello Time Standard Library ===");
    
    // Test current time functions
    println("\n--- Current Time Functions ---");
    var now = datetime_now();
    var utc_now = datetime_utcnow();
    
    var now_str = datetime_to_string(now);
    var utc_str = datetime_to_string(utc_now);
    
    print("Current local time: "); show(now_str); println("");
    print("Current UTC time: "); show(utc_str); println("");
    
    // Test datetime creation with specific values
    println("\n--- DateTime Creation ---");
    var birthday = datetime_new(1990, 5, 15, 14, 30, 0);
    var birthday_str = datetime_to_string(birthday);
    print("Created birthday: "); show(birthday_str); println("");
    
    // Test datetime component access
    println("\n--- DateTime Component Access ---");
    var year_val = $I(datetime_year(now));
    var month_val = $I(datetime_month(now));
    var day_val = $I(datetime_day(now));
    var hour_val = $I(datetime_hour(now));
    var minute_val = $I(datetime_minute(now));
    var second_val = $I(datetime_second(now));
    var weekday_val = $I(datetime_weekday(now));
    
    print("Year: "); show(year_val); println("");
    print("Month: "); show(month_val); println("");
    print("Day: "); show(day_val); println("");
    print("Hour: "); show(hour_val); println("");
    print("Minute: "); show(minute_val); println("");
    print("Second: "); show(second_val); println("");
    print("Weekday: "); show(weekday_val); println("");
    
    // Test timestamp functions
    println("\n--- Timestamp Functions ---");
    var current_timestamp = $I(time_now());
    var precise_time = $F(time_now_precise());
    print("Current timestamp: "); show(current_timestamp); println("");
    print("Precise time: "); show(precise_time); println("");
    
    // Test duration creation and operations
    println("\n--- Duration Creation and Operations ---");
    var one_hour = duration_from_seconds(3600.0);
    var two_minutes = duration_from_seconds(120.0);
    var one_day = duration_new(1, 0, 0, 0, 0);
    
    var one_hour_str = duration_to_string(one_hour);
    var two_minutes_str = duration_to_string(two_minutes);
    var one_day_str = duration_to_string(one_day);
    
    print("One hour duration: "); show(one_hour_str); println("");
    print("Two minutes duration: "); show(two_minutes_str); println("");
    print("One day duration: "); show(one_day_str); println("");
    
    // Test duration totals
    var hour_total_secs = $F(duration_total_seconds(one_hour));
    var day_total_secs = $F(duration_total_seconds(one_day));
    print("One hour total seconds: "); show(hour_total_secs); println("");
    print("One day total seconds: "); show(day_total_secs); println("");
    
    // Test datetime arithmetic
    println("\n--- DateTime Arithmetic ---");
    var future_time = datetime_add_duration(now, one_hour);
    var past_time = datetime_subtract_duration(now, two_minutes);
    
    var future_str = datetime_to_string(future_time);
    var past_str = datetime_to_string(past_time);
    
    print("Current + 1 hour: "); show(future_str); println("");
    print("Current - 2 minutes: "); show(past_str); println("");
    
    // Test datetime comparison
    println("\n--- DateTime Comparison ---");
    var is_before = $I(datetime_before(birthday, now) ? 1 : 0);
    var is_after = $I(datetime_after(future_time, now) ? 1 : 0);
    var is_equal = $I(datetime_equals(now, now) ? 1 : 0);
    
    print("Birthday before now: "); show(is_before); println("");
    print("Future time after now: "); show(is_after); println("");
    print("Now equals now: "); show(is_equal); println("");
    
    // Test time difference
    println("\n--- Time Difference ---");
    var time_diff = datetime_difference(future_time, now);
    var diff_str = duration_to_string(time_diff);
    var diff_seconds = $F(duration_total_seconds(time_diff));
    
    print("Difference between future and now: "); show(diff_str); println("");
    print("Difference in seconds: "); show(diff_seconds); println("");
    
    // Test calendar functions
    println("\n--- Calendar Functions ---");
    int current_year_int = datetime_year(now);
    int current_month_int = datetime_month(now);
    var current_year_val = $I(current_year_int);
    var is_leap = $I(is_leap_year(current_year_int) ? 1 : 0);
    var days_in_current_month = $I(days_in_month(current_year_int, current_month_int));
    
    print("Current year: "); show(current_year_val); println("");
    print("Is current year leap: "); show(is_leap); println("");
    print("Days in current month: "); show(days_in_current_month); println("");
    
    // Test storing time results in Cello structures
    println("\n--- Testing Time Results Storage in Cello Structures ---");
    var time_strings = new(List, String);
    
    // Store time-related strings
    push(time_strings, now_str);
    push(time_strings, utc_str);
    push(time_strings, birthday_str);
    push(time_strings, future_str);
    push(time_strings, past_str);
    
    print("Time strings: "); show(time_strings); println("");
    
    // Store time metrics in Table
    var time_metrics = new(Table, String, Int);
    set(time_metrics, $S("current_year"), year_val);
    set(time_metrics, $S("current_month"), month_val);
    set(time_metrics, $S("current_day"), day_val);
    set(time_metrics, $S("weekday"), weekday_val);
    set(time_metrics, $S("is_leap_year"), is_leap);
    
    print("Time metrics: "); show(time_metrics); println("");
    
    println("\n=== Time Standard Library Test Complete ===");
    return 0;
}
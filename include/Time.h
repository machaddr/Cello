#ifndef CELLO_STD_TIME_H
#define CELLO_STD_TIME_H

#include "Cello.h"

// Time types
extern var DateTime;
extern var Duration;
extern var TimeZone;

// DateTime structure
struct DateTime {
    int year;
    int month;      // 1-12
    int day;        // 1-31
    int hour;       // 0-23
    int minute;     // 0-59
    int second;     // 0-59
    int microsecond; // 0-999999
    int weekday;    // 0-6 (Monday=0, Sunday=6)
    int yearday;    // 1-366
    bool is_dst;    // Daylight saving time flag
    time_t timestamp; // Unix timestamp
};

// Duration structure (represents time differences)
struct Duration {
    int64_t total_seconds;
    int32_t microseconds;
};

// TimeZone structure
struct TimeZone {
    char* name;
    int offset_seconds; // UTC offset in seconds
    bool is_dst;       // Current DST status
};

// DateTime creation functions
var datetime_now(void);
var datetime_utcnow(void);
var datetime_new(int year, int month, int day, int hour, int minute, int second);
var datetime_from_timestamp(time_t timestamp);
var datetime_from_string(const char* date_string, const char* format);

// DateTime formatting
var datetime_to_string(var self);
var datetime_format(var self, const char* format);
var datetime_isoformat(var self);

// DateTime arithmetic
var datetime_add_duration(var self, var duration);
var datetime_subtract_duration(var self, var duration);
var datetime_difference(var self, var other);

// DateTime comparison
bool datetime_equals(var self, var other);
bool datetime_before(var self, var other);
bool datetime_after(var self, var other);

// DateTime component access
int datetime_year(var self);
int datetime_month(var self);
int datetime_day(var self);
int datetime_hour(var self);
int datetime_minute(var self);
int datetime_second(var self);
int datetime_microsecond(var self);
int datetime_weekday(var self);
int datetime_yearday(var self);

// DateTime modification
var datetime_replace(var self, int year, int month, int day, int hour, int minute, int second);
var datetime_replace_date(var self, int year, int month, int day);
var datetime_replace_time(var self, int hour, int minute, int second);

// DateTime timezone operations
var datetime_to_utc(var self);
var datetime_to_timezone(var self, var timezone);

// Duration creation functions
var duration_new(int days, int hours, int minutes, int seconds, int microseconds);
var duration_from_seconds(double seconds);
var duration_from_milliseconds(int64_t milliseconds);
var duration_from_microseconds(int64_t microseconds);

// Duration formatting
var duration_to_string(var self);
double duration_total_seconds(var self);
int64_t duration_total_milliseconds(var self);
int64_t duration_total_microseconds(var self);

// Duration arithmetic
var duration_add(var self, var other);
var duration_subtract(var self, var other);
var duration_multiply(var self, double factor);
var duration_divide(var self, double divisor);
var duration_abs(var self);

// Duration comparison
bool duration_equals(var self, var other);
bool duration_less_than(var self, var other);
bool duration_greater_than(var self, var other);

// Duration constants
extern var DURATION_ZERO;
extern var DURATION_SECOND;
extern var DURATION_MINUTE;
extern var DURATION_HOUR;
extern var DURATION_DAY;
extern var DURATION_WEEK;

// TimeZone functions
var timezone_new(const char* name, int offset_seconds);
var timezone_utc(void);
var timezone_local(void);
var timezone_from_name(const char* name);

// Utility functions
time_t time_now(void);
double time_now_precise(void);
void time_sleep(double seconds);
void time_sleep_milliseconds(int milliseconds);
void time_sleep_microseconds(int microseconds);

// Timing functions
var stopwatch_start(void);
double stopwatch_elapsed(var stopwatch);
void stopwatch_reset(var stopwatch);

// Calendar functions
bool is_leap_year(int year);
int days_in_month(int year, int month);
int day_of_year(int year, int month, int day);
var easter_date(int year);

// Date parsing and formatting
var parse_date(const char* date_string, const char* format);
var format_date(var datetime, const char* format);

// Common date formats
extern const char* ISO_DATE_FORMAT;
extern const char* ISO_DATETIME_FORMAT;
extern const char* RFC2822_FORMAT;
extern const char* HTTP_DATE_FORMAT;

// Weekday and month names
extern var WEEKDAY_NAMES;
extern var WEEKDAY_NAMES_SHORT;
extern var MONTH_NAMES;
extern var MONTH_NAMES_SHORT;

// Performance timing
typedef struct {
    double start_time;
    double end_time;
    bool running;
} Stopwatch;

var stopwatch_new(void);
void stopwatch_start_timing(var self);
void stopwatch_stop(var self);
double stopwatch_get_elapsed(var self);

// High-resolution timing
uint64_t time_get_ticks(void);
double time_ticks_to_seconds(uint64_t ticks);
uint64_t time_get_frequency(void);

// Initialization function
void __cello_std_time_init(void);

#endif
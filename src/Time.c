#include "Cello.h"
#include "Time.h"
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Time types
var DateTime;
var Duration;
var TimeZone;

// Duration constants
var DURATION_ZERO;
var DURATION_SECOND;
var DURATION_MINUTE;
var DURATION_HOUR;
var DURATION_DAY;
var DURATION_WEEK;

// Date format constants
const char* ISO_DATE_FORMAT = "%Y-%m-%d";
const char* ISO_DATETIME_FORMAT = "%Y-%m-%dT%H:%M:%S";
const char* RFC2822_FORMAT = "%a, %d %b %Y %H:%M:%S %z";
const char* HTTP_DATE_FORMAT = "%a, %d %b %Y %H:%M:%S GMT";

// Name arrays
var WEEKDAY_NAMES;
var WEEKDAY_NAMES_SHORT;
var MONTH_NAMES;
var MONTH_NAMES_SHORT;

static bool time_initialized = false;

// DateTime implementation
static void DateTime_New(var self, var args) {
    struct DateTime* dt = cast(self, DateTime);
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    dt->year = tm_info->tm_year + 1900;
    dt->month = tm_info->tm_mon + 1;
    dt->day = tm_info->tm_mday;
    dt->hour = tm_info->tm_hour;
    dt->minute = tm_info->tm_min;
    dt->second = tm_info->tm_sec;
    dt->microsecond = 0;
    dt->weekday = (tm_info->tm_wday + 6) % 7; // Convert to Monday=0
    dt->yearday = tm_info->tm_yday + 1;
    dt->is_dst = tm_info->tm_isdst > 0;
    dt->timestamp = now;
    
    // Override with arguments if provided
    if (len(args) >= 1) dt->year = c_int(get(args, $I(0)));
    if (len(args) >= 2) dt->month = c_int(get(args, $I(1)));
    if (len(args) >= 3) dt->day = c_int(get(args, $I(2)));
    if (len(args) >= 4) dt->hour = c_int(get(args, $I(3)));
    if (len(args) >= 5) dt->minute = c_int(get(args, $I(4)));
    if (len(args) >= 6) dt->second = c_int(get(args, $I(5)));
    
    // Recalculate timestamp if args were provided
    if (len(args) >= 3) {
        struct tm tm = {0};
        tm.tm_year = dt->year - 1900;
        tm.tm_mon = dt->month - 1;
        tm.tm_mday = dt->day;
        tm.tm_hour = dt->hour;
        tm.tm_min = dt->minute;
        tm.tm_sec = dt->second;
        tm.tm_isdst = -1;
        dt->timestamp = mktime(&tm);
        
        // Update derived fields
        dt->weekday = (tm.tm_wday + 6) % 7;
        dt->yearday = tm.tm_yday + 1;
        dt->is_dst = tm.tm_isdst > 0;
    }
}

static void DateTime_Del(var self) {
    // No dynamic allocation to clean up
}

static int DateTime_Show(var self, var output, int pos) {
    struct DateTime* dt = cast(self, DateTime);
    return print_to(output, pos, "DateTime(%04d-%02d-%02d %02d:%02d:%02d)", 
                   $I(dt->year), $I(dt->month), $I(dt->day), $I(dt->hour), $I(dt->minute), $I(dt->second));
}

var DateTime = Cello(DateTime,
    Instance(New, DateTime_New, DateTime_Del),
    Instance(Show, DateTime_Show));

// Duration implementation
static void Duration_New(var self, var args) {
    struct Duration* d = cast(self, Duration);
    d->total_seconds = 0;
    d->microseconds = 0;
    
    if (len(args) >= 1) {
        // First argument is total seconds
        double seconds = c_float(get(args, $I(0)));
        d->total_seconds = (int64_t)seconds;
        d->microseconds = (int32_t)((seconds - d->total_seconds) * 1000000);
    }
}

static void Duration_Del(var self) {
    // No dynamic allocation to clean up
}

static int Duration_Show(var self, var output, int pos) {
    struct Duration* d = cast(self, Duration);
    if (d->total_seconds == 0 && d->microseconds == 0) {
        return print_to(output, pos, "Duration(0)");
    }
    
    double total = d->total_seconds + d->microseconds / 1000000.0;
    return print_to(output, pos, "Duration(%.6f seconds)", $F(total));
}

var Duration = Cello(Duration,
    Instance(New, Duration_New, Duration_Del),
    Instance(Show, Duration_Show));

// TimeZone implementation
static void TimeZone_New(var self, var args) {
    struct TimeZone* tz = cast(self, TimeZone);
    tz->name = NULL;
    tz->offset_seconds = 0;
    tz->is_dst = false;
    
    if (len(args) >= 1) {
        const char* name = c_str(get(args, $I(0)));
        tz->name = malloc(strlen(name) + 1);
        strcpy(tz->name, name);
    }
    if (len(args) >= 2) {
        tz->offset_seconds = c_int(get(args, $I(1)));
    }
}

static void TimeZone_Del(var self) {
    struct TimeZone* tz = cast(self, TimeZone);
    if (tz->name) {
        free(tz->name);
        tz->name = NULL;
    }
}

static int TimeZone_Show(var self, var output, int pos) {
    struct TimeZone* tz = cast(self, TimeZone);
    int hours = tz->offset_seconds / 3600;
    int minutes = abs(tz->offset_seconds % 3600) / 60;
    return print_to(output, pos, "TimeZone(%s, %+03d:%02d)", 
                   $S(tz->name ? tz->name : "Unknown"), $I(hours), $I(minutes));
}

var TimeZone = Cello(TimeZone,
    Instance(New, TimeZone_New, TimeZone_Del),
    Instance(Show, TimeZone_Show));

// DateTime creation functions
var datetime_now(void) {
    return new(DateTime);
}

var datetime_utcnow(void) {
    time_t now = time(NULL);
    struct tm* tm_info = gmtime(&now);
    
    return new(DateTime, 
               $I(tm_info->tm_year + 1900),
               $I(tm_info->tm_mon + 1),
               $I(tm_info->tm_mday),
               $I(tm_info->tm_hour),
               $I(tm_info->tm_min),
               $I(tm_info->tm_sec));
}

var datetime_new(int year, int month, int day, int hour, int minute, int second) {
    return new(DateTime, $I(year), $I(month), $I(day), $I(hour), $I(minute), $I(second));
}

var datetime_from_timestamp(time_t timestamp) {
    struct tm* tm_info = localtime(&timestamp);
    var dt = new(DateTime,
                 $I(tm_info->tm_year + 1900),
                 $I(tm_info->tm_mon + 1),
                 $I(tm_info->tm_mday),
                 $I(tm_info->tm_hour),
                 $I(tm_info->tm_min),
                 $I(tm_info->tm_sec));
    
    struct DateTime* dt_struct = cast(dt, DateTime);
    dt_struct->timestamp = timestamp;
    return dt;
}

// DateTime formatting
var datetime_to_string(var self) {
    struct DateTime* dt = cast(self, DateTime);
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second);
    return new(String, $S(buffer));
}

var datetime_format(var self, const char* format) {
    struct DateTime* dt = cast(self, DateTime);
    struct tm tm = {0};
    
    tm.tm_year = dt->year - 1900;
    tm.tm_mon = dt->month - 1;
    tm.tm_mday = dt->day;
    tm.tm_hour = dt->hour;
    tm.tm_min = dt->minute;
    tm.tm_sec = dt->second;
    tm.tm_wday = (dt->weekday + 1) % 7; // Convert back to Sunday=0
    tm.tm_yday = dt->yearday - 1;
    tm.tm_isdst = dt->is_dst ? 1 : 0;
    
    char buffer[256];
    strftime(buffer, sizeof(buffer), format, &tm);
    return new(String, $S(buffer));
}

var datetime_isoformat(var self) {
    return datetime_format(self, ISO_DATETIME_FORMAT);
}

// DateTime arithmetic
var datetime_add_duration(var self, var duration) {
    struct DateTime* dt = cast(self, DateTime);
    struct Duration* dur = cast(duration, Duration);
    
    time_t new_timestamp = dt->timestamp + dur->total_seconds;
    var result = datetime_from_timestamp(new_timestamp);
    
    // Add microseconds
    struct DateTime* result_dt = cast(result, DateTime);
    result_dt->microsecond = dt->microsecond + dur->microseconds;
    if (result_dt->microsecond >= 1000000) {
        result_dt->microsecond -= 1000000;
        result_dt->timestamp += 1;
    }
    
    return result;
}

var datetime_subtract_duration(var self, var duration) {
    struct DateTime* dt = cast(self, DateTime);
    struct Duration* dur = cast(duration, Duration);
    
    time_t new_timestamp = dt->timestamp - dur->total_seconds;
    var result = datetime_from_timestamp(new_timestamp);
    
    // Subtract microseconds
    struct DateTime* result_dt = cast(result, DateTime);
    result_dt->microsecond = dt->microsecond - dur->microseconds;
    if (result_dt->microsecond < 0) {
        result_dt->microsecond += 1000000;
        result_dt->timestamp -= 1;
    }
    
    return result;
}

var datetime_difference(var self, var other) {
    struct DateTime* dt1 = cast(self, DateTime);
    struct DateTime* dt2 = cast(other, DateTime);
    
    int64_t diff_seconds = dt1->timestamp - dt2->timestamp;
    int32_t diff_microseconds = dt1->microsecond - dt2->microsecond;
    
    if (diff_microseconds < 0) {
        diff_microseconds += 1000000;
        diff_seconds -= 1;
    }
    
    var duration = new(Duration, $F((double)diff_seconds + diff_microseconds / 1000000.0));
    return duration;
}

// DateTime comparison
bool datetime_equals(var self, var other) {
    struct DateTime* dt1 = cast(self, DateTime);
    struct DateTime* dt2 = cast(other, DateTime);
    return dt1->timestamp == dt2->timestamp && dt1->microsecond == dt2->microsecond;
}

bool datetime_before(var self, var other) {
    struct DateTime* dt1 = cast(self, DateTime);
    struct DateTime* dt2 = cast(other, DateTime);
    return dt1->timestamp < dt2->timestamp || 
           (dt1->timestamp == dt2->timestamp && dt1->microsecond < dt2->microsecond);
}

bool datetime_after(var self, var other) {
    struct DateTime* dt1 = cast(self, DateTime);
    struct DateTime* dt2 = cast(other, DateTime);
    return dt1->timestamp > dt2->timestamp || 
           (dt1->timestamp == dt2->timestamp && dt1->microsecond > dt2->microsecond);
}

// DateTime component access
int datetime_year(var self) {
    struct DateTime* dt = cast(self, DateTime);
    return dt->year;
}

int datetime_month(var self) {
    struct DateTime* dt = cast(self, DateTime);
    return dt->month;
}

int datetime_day(var self) {
    struct DateTime* dt = cast(self, DateTime);
    return dt->day;
}

int datetime_hour(var self) {
    struct DateTime* dt = cast(self, DateTime);
    return dt->hour;
}

int datetime_minute(var self) {
    struct DateTime* dt = cast(self, DateTime);
    return dt->minute;
}

int datetime_second(var self) {
    struct DateTime* dt = cast(self, DateTime);
    return dt->second;
}

int datetime_weekday(var self) {
    struct DateTime* dt = cast(self, DateTime);
    return dt->weekday;
}

// Duration creation functions
var duration_new(int days, int hours, int minutes, int seconds, int microseconds) {
    int64_t total_seconds = days * 86400 + hours * 3600 + minutes * 60 + seconds;
    double total_with_micro = total_seconds + microseconds / 1000000.0;
    return new(Duration, $F(total_with_micro));
}

var duration_from_seconds(double seconds) {
    return new(Duration, $F(seconds));
}

var duration_from_milliseconds(int64_t milliseconds) {
    return new(Duration, $F(milliseconds / 1000.0));
}

var duration_from_microseconds(int64_t microseconds) {
    return new(Duration, $F(microseconds / 1000000.0));
}

// Duration formatting
var duration_to_string(var self) {
    struct Duration* d = cast(self, Duration);
    char buffer[64];
    
    if (d->total_seconds == 0 && d->microseconds == 0) {
        strcpy(buffer, "0:00:00");
    } else {
        int hours = abs(d->total_seconds) / 3600;
        int minutes = (abs(d->total_seconds) % 3600) / 60;
        int seconds = abs(d->total_seconds) % 60;
        
        if (d->total_seconds < 0) {
            snprintf(buffer, sizeof(buffer), "-%d:%02d:%02d", hours, minutes, seconds);
        } else {
            snprintf(buffer, sizeof(buffer), "%d:%02d:%02d", hours, minutes, seconds);
        }
    }
    
    return new(String, $S(buffer));
}

double duration_total_seconds(var self) {
    struct Duration* d = cast(self, Duration);
    return d->total_seconds + d->microseconds / 1000000.0;
}

int64_t duration_total_milliseconds(var self) {
    struct Duration* d = cast(self, Duration);
    return d->total_seconds * 1000 + d->microseconds / 1000;
}

int64_t duration_total_microseconds(var self) {
    struct Duration* d = cast(self, Duration);
    return d->total_seconds * 1000000 + d->microseconds;
}

// Duration arithmetic
var duration_add(var self, var other) {
    double total1 = duration_total_seconds(self);
    double total2 = duration_total_seconds(other);
    return duration_from_seconds(total1 + total2);
}

var duration_subtract(var self, var other) {
    double total1 = duration_total_seconds(self);
    double total2 = duration_total_seconds(other);
    return duration_from_seconds(total1 - total2);
}

var duration_multiply(var self, double factor) {
    double total = duration_total_seconds(self);
    return duration_from_seconds(total * factor);
}

var duration_abs(var self) {
    struct Duration* d = cast(self, Duration);
    if (d->total_seconds < 0 || (d->total_seconds == 0 && d->microseconds < 0)) {
        double total = duration_total_seconds(self);
        return duration_from_seconds(-total);
    }
    return copy(self);
}

// Utility functions
time_t time_now(void) {
    return time(NULL);
}

double time_now_precise(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void time_sleep(double seconds) {
    if (seconds <= 0) return;
    
    time_t sec = (time_t)seconds;
    long nsec = (long)((seconds - sec) * 1000000000);
    
    struct timespec ts = {sec, nsec};
    nanosleep(&ts, NULL);
}

void time_sleep_milliseconds(int milliseconds) {
    time_sleep(milliseconds / 1000.0);
}

void time_sleep_microseconds(int microseconds) {
    time_sleep(microseconds / 1000000.0);
}

// Calendar functions
bool is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int days_in_month(int year, int month) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && is_leap_year(year)) return 29;
    return days[month - 1];
}

// Stopwatch functions
var stopwatch_start(void) {
    var sw = new(Function);
    struct Function* f = cast(sw, Function);
    // This is a simplified implementation - in practice you'd want a proper stopwatch type
    return sw;
}

double stopwatch_elapsed(var stopwatch) {
    // Simplified implementation
    return time_now_precise();
}

// High-resolution timing
uint64_t time_get_ticks(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

double time_ticks_to_seconds(uint64_t ticks) {
    return ticks / 1000000.0;
}

uint64_t time_get_frequency(void) {
    return 1000000; // 1 MHz (microsecond resolution)
}

// Initialization function
void __cello_std_time_init(void) {
    if (time_initialized) return;
    
    // Initialize duration constants
    DURATION_ZERO = duration_from_seconds(0);
    DURATION_SECOND = duration_from_seconds(1);
    DURATION_MINUTE = duration_from_seconds(60);
    DURATION_HOUR = duration_from_seconds(3600);
    DURATION_DAY = duration_from_seconds(86400);
    DURATION_WEEK = duration_from_seconds(604800);
    
    // Initialize name arrays
    WEEKDAY_NAMES = new(List);
    push(WEEKDAY_NAMES, new(String, "Monday"));
    push(WEEKDAY_NAMES, new(String, "Tuesday"));
    push(WEEKDAY_NAMES, new(String, "Wednesday"));
    push(WEEKDAY_NAMES, new(String, "Thursday"));
    push(WEEKDAY_NAMES, new(String, "Friday"));
    push(WEEKDAY_NAMES, new(String, "Saturday"));
    push(WEEKDAY_NAMES, new(String, "Sunday"));
    
    WEEKDAY_NAMES_SHORT = new(List);
    push(WEEKDAY_NAMES_SHORT, new(String, "Mon"));
    push(WEEKDAY_NAMES_SHORT, new(String, "Tue"));
    push(WEEKDAY_NAMES_SHORT, new(String, "Wed"));
    push(WEEKDAY_NAMES_SHORT, new(String, "Thu"));
    push(WEEKDAY_NAMES_SHORT, new(String, "Fri"));
    push(WEEKDAY_NAMES_SHORT, new(String, "Sat"));
    push(WEEKDAY_NAMES_SHORT, new(String, "Sun"));
    
    MONTH_NAMES = new(List);
    push(MONTH_NAMES, new(String, "January"));
    push(MONTH_NAMES, new(String, "February"));
    push(MONTH_NAMES, new(String, "March"));
    push(MONTH_NAMES, new(String, "April"));
    push(MONTH_NAMES, new(String, "May"));
    push(MONTH_NAMES, new(String, "June"));
    push(MONTH_NAMES, new(String, "July"));
    push(MONTH_NAMES, new(String, "August"));
    push(MONTH_NAMES, new(String, "September"));
    push(MONTH_NAMES, new(String, "October"));
    push(MONTH_NAMES, new(String, "November"));
    push(MONTH_NAMES, new(String, "December"));
    
    MONTH_NAMES_SHORT = new(List);
    push(MONTH_NAMES_SHORT, new(String, "Jan"));
    push(MONTH_NAMES_SHORT, new(String, "Feb"));
    push(MONTH_NAMES_SHORT, new(String, "Mar"));
    push(MONTH_NAMES_SHORT, new(String, "Apr"));
    push(MONTH_NAMES_SHORT, new(String, "May"));
    push(MONTH_NAMES_SHORT, new(String, "Jun"));
    push(MONTH_NAMES_SHORT, new(String, "Jul"));
    push(MONTH_NAMES_SHORT, new(String, "Aug"));
    push(MONTH_NAMES_SHORT, new(String, "Sep"));
    push(MONTH_NAMES_SHORT, new(String, "Oct"));
    push(MONTH_NAMES_SHORT, new(String, "Nov"));
    push(MONTH_NAMES_SHORT, new(String, "Dec"));
    
    time_initialized = true;
}
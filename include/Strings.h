#ifndef CELLO_STD_STRING_H
#define CELLO_STD_STRING_H

#include "Cello.h"

// String manipulation functions
var string_upper(var self);
var string_lower(var self);
var string_capitalize(var self);
var string_title(var self);
var string_strip(var self);
var string_lstrip(var self);
var string_rstrip(var self);

// String searching and testing
bool string_startswith(var self, var prefix);
bool string_endswith(var self, var suffix);
bool string_contains(var self, var substring);
int string_find(var self, var substring);
int string_rfind(var self, var substring);
int string_count(var self, var substring);

// String splitting and joining
var string_split(var self, var delimiter);
var string_rsplit(var self, var delimiter, int maxsplit);
var string_splitlines(var self);
var string_join(var self, var iterable);

// String replacement
var string_replace(var self, var old, var new);
var string_replace_n(var self, var old, var new, int count);

// String padding and alignment
var string_ljust(var self, int width, char fillchar);
var string_rjust(var self, int width, char fillchar);
var string_center(var self, int width, char fillchar);
var string_zfill(var self, int width);

// String character testing
bool string_isalpha(var self);
bool string_isdigit(var self);
bool string_isalnum(var self);
bool string_islower(var self);
bool string_isupper(var self);
bool string_isspace(var self);
bool string_istitle(var self);

// String formatting
var string_format(const char* format, ...);
var string_format_with(const char* format, var args);

// String encoding/decoding
var string_encode_base64(var self);
var string_decode_base64(var self);
var string_encode_url(var self);
var string_decode_url(var self);

// String comparison
bool string_equals_ignore_case(var self, var other);
int string_compare_ignore_case(var self, var other);

// String utilities
var string_reverse(var self);
var string_repeat(var self, int count);
var string_substring(var self, int start, int length);
var string_slice(var self, int start, int end);

// Regular expressions (basic)
bool string_match_pattern(var self, const char* pattern);
var string_find_pattern(var self, const char* pattern);
var string_replace_pattern(var self, const char* pattern, var replacement);

// String builder
extern var StringBuilder;

struct StringBuilder {
    char* buffer;
    size_t length;
    size_t capacity;
};

var string_builder_new(void);
void string_builder_append(var self, var str);
void string_builder_append_char(var self, char c);
void string_builder_append_format(var self, const char* format, ...);
var string_builder_to_string(var self);
void string_builder_clear(var self);
size_t string_builder_length(var self);

// String constants and utilities
extern var EMPTY_STRING;
extern var WHITESPACE_CHARS;
extern var DIGITS;
extern var ASCII_LETTERS;
extern var ASCII_LOWERCASE;
extern var ASCII_UPPERCASE;

// Character classification functions
bool is_alpha(char c);
bool is_digit(char c);
bool is_alnum(char c);
bool is_space(char c);
bool is_upper(char c);
bool is_lower(char c);
char to_upper(char c);
char to_lower(char c);

// String conversion functions
var string_from_int(int64_t value);
var string_from_float(double value);
var string_from_bool(bool value);
int64_t string_to_int(var self);
double string_to_float(var self);
bool string_to_bool(var self);

// Unicode support (basic)
bool string_is_valid_utf8(var self);
size_t string_utf8_length(var self);
var string_utf8_substring(var self, size_t start, size_t length);

// Initialization function
void __cello_std_string_init(void);

#endif
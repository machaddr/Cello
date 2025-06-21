#include "Cello.h"
#include "Strings.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// String builder type
var StringBuilder;

// String constants
var EMPTY_STRING;
var WHITESPACE_CHARS;
var DIGITS;
var ASCII_LETTERS;
var ASCII_LOWERCASE;
var ASCII_UPPERCASE;

static bool string_initialized = false;

// StringBuilder implementation
static void StringBuilder_New(var self, var args) {
    struct StringBuilder* sb = cast(self, StringBuilder);
    sb->capacity = 256;
    sb->length = 0;
    sb->buffer = malloc(sb->capacity);
    sb->buffer[0] = '\0';
}

static void StringBuilder_Del(var self) {
    struct StringBuilder* sb = cast(self, StringBuilder);
    if (sb->buffer) {
        free(sb->buffer);
        sb->buffer = NULL;
    }
}

static int StringBuilder_Show(var self, var output, int pos) {
    struct StringBuilder* sb = cast(self, StringBuilder);
    return print_to(output, pos, "StringBuilder(length=%zu, capacity=%zu)", 
                   $I(sb->length), $I(sb->capacity));
}

var StringBuilder = Cello(StringBuilder,
    Instance(New, StringBuilder_New, StringBuilder_Del),
    Instance(Show, StringBuilder_Show));

// Helper function to ensure string builder capacity
static void sb_ensure_capacity(struct StringBuilder* sb, size_t needed) {
    if (sb->length + needed >= sb->capacity) {
        while (sb->length + needed >= sb->capacity) {
            sb->capacity *= 2;
        }
        sb->buffer = realloc(sb->buffer, sb->capacity);
    }
}

// String manipulation functions
var string_upper(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    char* upper_str = malloc(len + 1);
    
    for (size_t i = 0; i < len; i++) {
        upper_str[i] = toupper(str[i]);
    }
    upper_str[len] = '\0';
    
    var result = new(String, upper_str);
    free(upper_str);
    return result;
}

var string_lower(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    char* lower_str = malloc(len + 1);
    
    for (size_t i = 0; i < len; i++) {
        lower_str[i] = tolower(str[i]);
    }
    lower_str[len] = '\0';
    
    var result = new(String, lower_str);
    free(lower_str);
    return result;
}

var string_capitalize(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    if (len == 0) return copy(self);
    
    char* cap_str = malloc(len + 1);
    cap_str[0] = toupper(str[0]);
    
    for (size_t i = 1; i < len; i++) {
        cap_str[i] = tolower(str[i]);
    }
    cap_str[len] = '\0';
    
    var result = new(String, cap_str);
    free(cap_str);
    return result;
}

var string_title(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    char* title_str = malloc(len + 1);
    bool capitalize_next = true;
    
    for (size_t i = 0; i < len; i++) {
        if (isalpha(str[i])) {
            title_str[i] = capitalize_next ? toupper(str[i]) : tolower(str[i]);
            capitalize_next = false;
        } else {
            title_str[i] = str[i];
            capitalize_next = true;
        }
    }
    title_str[len] = '\0';
    
    var result = new(String, title_str);
    free(title_str);
    return result;
}

var string_strip(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    
    // Find start of non-whitespace
    size_t start = 0;
    while (start < len && isspace(str[start])) {
        start++;
    }
    
    // Find end of non-whitespace
    size_t end = len;
    while (end > start && isspace(str[end - 1])) {
        end--;
    }
    
    size_t new_len = end - start;
    char* stripped = malloc(new_len + 1);
    memcpy(stripped, str + start, new_len);
    stripped[new_len] = '\0';
    
    var result = new(String, stripped);
    free(stripped);
    return result;
}

var string_lstrip(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    
    size_t start = 0;
    while (start < len && isspace(str[start])) {
        start++;
    }
    
    return new(String, (char*)(str + start));
}

var string_rstrip(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    
    size_t end = len;
    while (end > 0 && isspace(str[end - 1])) {
        end--;
    }
    
    char* stripped = malloc(end + 1);
    memcpy(stripped, str, end);
    stripped[end] = '\0';
    
    var result = new(String, stripped);
    free(stripped);
    return result;
}

// String searching and testing
bool string_startswith(var self, var prefix) {
    const char* str = c_str(self);
    const char* pre = c_str(prefix);
    return strncmp(str, pre, strlen(pre)) == 0;
}

bool string_endswith(var self, var suffix) {
    const char* str = c_str(self);
    const char* suf = c_str(suffix);
    size_t str_len = strlen(str);
    size_t suf_len = strlen(suf);
    
    if (suf_len > str_len) return false;
    return strcmp(str + str_len - suf_len, suf) == 0;
}

bool string_contains(var self, var substring) {
    const char* str = c_str(self);
    const char* sub = c_str(substring);
    return strstr(str, sub) != NULL;
}

int string_find(var self, var substring) {
    const char* str = c_str(self);
    const char* sub = c_str(substring);
    char* found = strstr(str, sub);
    return found ? (int)(found - str) : -1;
}

int string_rfind(var self, var substring) {
    const char* str = c_str(self);
    const char* sub = c_str(substring);
    char* last_found = NULL;
    char* current = (char*)str;
    
    while ((current = strstr(current, sub)) != NULL) {
        last_found = current;
        current++;
    }
    
    return last_found ? (int)(last_found - str) : -1;
}

int string_count(var self, var substring) {
    const char* str = c_str(self);
    const char* sub = c_str(substring);
    int count = 0;
    char* current = (char*)str;
    
    while ((current = strstr(current, sub)) != NULL) {
        count++;
        current += strlen(sub);
    }
    
    return count;
}

// String splitting
var string_split(var self, var delimiter) {
    const char* str = c_str(self);
    const char* delim = c_str(delimiter);
    var result = new(List);
    
    char* str_copy = malloc(strlen(str) + 1);
    strcpy(str_copy, str);
    
    char* token = strtok(str_copy, delim);
    while (token) {
        push(result, new(String, token));
        token = strtok(NULL, delim);
    }
    
    free(str_copy);
    return result;
}

var string_splitlines(var self) {
    const char* str = c_str(self);
    var result = new(List);
    
    char* str_copy = malloc(strlen(str) + 1);
    strcpy(str_copy, str);
    
    char* line = strtok(str_copy, "\n\r");
    while (line) {
        push(result, new(String, line));
        line = strtok(NULL, "\n\r");
    }
    
    free(str_copy);
    return result;
}

var string_join(var self, var iterable) {
    const char* separator = c_str(self);
    var sb = string_builder_new();
    bool first = true;
    
    foreach (item in iterable) {
        if (!first) {
            string_builder_append(sb, $S((char*)separator));
        }
        string_builder_append(sb, item);
        first = false;
    }
    
    var result = string_builder_to_string(sb);
    del(sb);
    return result;
}

// String replacement
var string_replace(var self, var old, var new) {
    const char* str = c_str(self);
    const char* old_str = c_str(old);
    const char* new_str = c_str(new);
    
    size_t old_len = strlen(old_str);
    size_t new_len = strlen(new_str);
    
    if (old_len == 0) return copy(self);
    
    // Count occurrences
    int count = string_count(self, old);
    if (count == 0) return copy(self);
    
    // Calculate new string length
    size_t str_len = strlen(str);
    size_t result_len = str_len + count * (new_len - old_len);
    
    char* result = malloc(result_len + 1);
    char* dest = result;
    const char* src = str;
    
    while ((src = strstr(src, old_str)) != NULL) {
        // Copy everything before the match
        size_t prefix_len = src - str;
        memcpy(dest, str, prefix_len);
        dest += prefix_len;
        
        // Copy the replacement
        memcpy(dest, new_str, new_len);
        dest += new_len;
        
        // Move past the old string
        src += old_len;
        str = src;
    }
    
    // Copy remaining part
    strcpy(dest, str);
    
    var result_var = new(String, result);
    free(result);
    return result_var;
}

// String padding
var string_ljust(var self, int width, char fillchar) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    
    if ((int)len >= width) return copy(self);
    
    size_t padding = width - len;
    char* padded = malloc(width + 1);
    
    strcpy(padded, str);
    for (size_t i = len; i < (size_t)width; i++) {
        padded[i] = fillchar;
    }
    padded[width] = '\0';
    
    var result = new(String, padded);
    free(padded);
    return result;
}

var string_rjust(var self, int width, char fillchar) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    
    if ((int)len >= width) return copy(self);
    
    size_t padding = width - len;
    char* padded = malloc(width + 1);
    
    for (size_t i = 0; i < padding; i++) {
        padded[i] = fillchar;
    }
    strcpy(padded + padding, str);
    
    var result = new(String, padded);
    free(padded);
    return result;
}

var string_center(var self, int width, char fillchar) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    
    if ((int)len >= width) return copy(self);
    
    size_t total_padding = width - len;
    size_t left_padding = total_padding / 2;
    size_t right_padding = total_padding - left_padding;
    
    char* centered = malloc(width + 1);
    
    for (size_t i = 0; i < left_padding; i++) {
        centered[i] = fillchar;
    }
    
    strcpy(centered + left_padding, str);
    
    for (size_t i = left_padding + len; i < (size_t)width; i++) {
        centered[i] = fillchar;
    }
    centered[width] = '\0';
    
    var result = new(String, centered);
    free(centered);
    return result;
}

// String character testing
bool string_isalpha(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    if (len == 0) return false;
    
    for (size_t i = 0; i < len; i++) {
        if (!isalpha(str[i])) return false;
    }
    return true;
}

bool string_isdigit(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    if (len == 0) return false;
    
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool string_isalnum(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    if (len == 0) return false;
    
    for (size_t i = 0; i < len; i++) {
        if (!isalnum(str[i])) return false;
    }
    return true;
}

bool string_islower(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    if (len == 0) return false;
    
    bool has_cased = false;
    for (size_t i = 0; i < len; i++) {
        if (isupper(str[i])) return false;
        if (islower(str[i])) has_cased = true;
    }
    return has_cased;
}

bool string_isupper(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    if (len == 0) return false;
    
    bool has_cased = false;
    for (size_t i = 0; i < len; i++) {
        if (islower(str[i])) return false;
        if (isupper(str[i])) has_cased = true;
    }
    return has_cased;
}

bool string_isspace(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    if (len == 0) return false;
    
    for (size_t i = 0; i < len; i++) {
        if (!isspace(str[i])) return false;
    }
    return true;
}

// String comparison
bool string_equals_ignore_case(var self, var other) {
    const char* str1 = c_str(self);
    const char* str2 = c_str(other);
    return strcasecmp(str1, str2) == 0;
}

int string_compare_ignore_case(var self, var other) {
    const char* str1 = c_str(self);
    const char* str2 = c_str(other);
    return strcasecmp(str1, str2);
}

// String utilities
var string_reverse(var self) {
    const char* str = c_str(self);
    size_t len = strlen(str);
    char* reversed = malloc(len + 1);
    
    for (size_t i = 0; i < len; i++) {
        reversed[i] = str[len - 1 - i];
    }
    reversed[len] = '\0';
    
    var result = new(String, reversed);
    free(reversed);
    return result;
}

var string_repeat(var self, int count) {
    if (count <= 0) return new(String, "");
    
    const char* str = c_str(self);
    size_t len = strlen(str);
    size_t total_len = len * count;
    
    char* repeated = malloc(total_len + 1);
    for (int i = 0; i < count; i++) {
        strcpy(repeated + i * len, str);
    }
    
    var result = new(String, repeated);
    free(repeated);
    return result;
}

var string_substring(var self, int start, int length) {
    const char* str = c_str(self);
    size_t str_len = strlen(str);
    
    if (start < 0 || start >= (int)str_len || length <= 0) {
        return new(String, "");
    }
    
    if (start + length > (int)str_len) {
        length = str_len - start;
    }
    
    char* sub = malloc(length + 1);
    memcpy(sub, str + start, length);
    sub[length] = '\0';
    
    var result = new(String, sub);
    free(sub);
    return result;
}

// StringBuilder functions
var string_builder_new(void) {
    return new(StringBuilder);
}

void string_builder_append(var self, var str) {
    struct StringBuilder* sb = cast(self, StringBuilder);
    const char* s = c_str(str);
    size_t s_len = strlen(s);
    
    sb_ensure_capacity(sb, s_len);
    strcpy(sb->buffer + sb->length, s);
    sb->length += s_len;
}

void string_builder_append_char(var self, char c) {
    struct StringBuilder* sb = cast(self, StringBuilder);
    sb_ensure_capacity(sb, 1);
    sb->buffer[sb->length] = c;
    sb->buffer[sb->length + 1] = '\0';
    sb->length++;
}

void string_builder_append_format(var self, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    // Get required length
    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    
    if (len > 0) {
        struct StringBuilder* sb = cast(self, StringBuilder);
        sb_ensure_capacity(sb, len);
        vsnprintf(sb->buffer + sb->length, len + 1, format, args);
        sb->length += len;
    }
    
    va_end(args);
}

var string_builder_to_string(var self) {
    struct StringBuilder* sb = cast(self, StringBuilder);
    return new(String, sb->buffer);
}

void string_builder_clear(var self) {
    struct StringBuilder* sb = cast(self, StringBuilder);
    sb->length = 0;
    sb->buffer[0] = '\0';
}

size_t string_builder_length(var self) {
    struct StringBuilder* sb = cast(self, StringBuilder);
    return sb->length;
}

// Character classification functions
bool is_alpha(char c) { return isalpha(c); }
bool is_digit(char c) { return isdigit(c); }
bool is_alnum(char c) { return isalnum(c); }
bool is_space(char c) { return isspace(c); }
bool is_upper(char c) { return isupper(c); }
bool is_lower(char c) { return islower(c); }
char to_upper(char c) { return toupper(c); }
char to_lower(char c) { return tolower(c); }

// String conversion functions
var string_from_int(int64_t value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%ld", value);
    return new(String, buffer);
}

var string_from_float(double value) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.6f", value);
    return new(String, buffer);
}

var string_from_bool(bool value) {
    return new(String, value ? "true" : "false");
}

int64_t string_to_int(var self) {
    const char* str = c_str(self);
    return strtoll(str, NULL, 10);
}

double string_to_float(var self) {
    const char* str = c_str(self);
    return strtod(str, NULL);
}

bool string_to_bool(var self) {
    const char* str = c_str(self);
    return strcmp(str, "true") == 0 || strcmp(str, "1") == 0;
}

// Basic pattern matching (simplified)
bool string_match_pattern(var self, const char* pattern) {
    const char* str = c_str(self);
    
    // Very basic wildcard matching (* and ?)
    while (*pattern && *str) {
        if (*pattern == '*') {
            pattern++;
            if (!*pattern) return true;
            while (*str && *str != *pattern) str++;
        } else if (*pattern == '?' || *pattern == *str) {
            pattern++;
            str++;
        } else {
            return false;
        }
    }
    
    return !*pattern && !*str;
}

// Initialization function
void __cello_std_strings_init(void) {
    if (string_initialized) return;
    
    // Initialize string constants
    EMPTY_STRING = new(String, "");
    WHITESPACE_CHARS = new(String, " \t\n\r\f\v");
    DIGITS = new(String, "0123456789");
    ASCII_LETTERS = new(String, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    ASCII_LOWERCASE = new(String, "abcdefghijklmnopqrstuvwxyz");
    ASCII_UPPERCASE = new(String, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    
    string_initialized = true;
}
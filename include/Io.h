#ifndef CELLO_STD_IO_H
#define CELLO_STD_IO_H

#include "Cello.h"

// Enhanced I/O types
extern var FileReader;
extern var FileWriter;
extern var BufferedReader;
extern var BufferedWriter;
extern var StringReader;
extern var StringWriter;
extern var Path;

// File modes
typedef enum {
    STD_READ_ONLY,
    STD_WRITE_ONLY,
    STD_READ_WRITE,
    STD_APPEND,
    STD_BINARY_READ,
    STD_BINARY_WRITE,
    STD_BINARY_READ_WRITE
} FileMode;

// FileReader structure
struct FileReader {
    FILE* file;
    char* filename;
    bool owns_file;
};

// FileWriter structure
struct FileWriter {
    FILE* file;
    char* filename;
    bool owns_file;
};

// BufferedReader structure
struct BufferedReader {
    var reader;
    char* buffer;
    size_t buffer_size;
    size_t buffer_pos;
    size_t buffer_len;
};

// BufferedWriter structure
struct BufferedWriter {
    var writer;
    char* buffer;
    size_t buffer_size;
    size_t buffer_pos;
};

// StringReader structure
struct StringReader {
    char* data;
    size_t length;
    size_t position;
    bool owns_data;
};

// StringWriter structure
struct StringWriter {
    char* data;
    size_t length;
    size_t capacity;
};

// Path structure
struct Path {
    char* path;
    size_t length;
};

// File I/O functions
var file_open(const char* filename, FileMode mode);
void file_close(var self);
var file_read(var self, size_t size);
var file_read_line(var self);
var file_read_all(var self);
void file_write(var self, var data);
void file_write_line(var self, var data);
void file_flush(var self);
bool file_eof(var self);
int64_t file_tell(var self);
void file_seek(var self, int64_t offset, int origin);

// FileReader functions
var file_reader_new(const char* filename);
var file_reader_from_file(FILE* file);
var file_reader_read(var self, size_t size);
var file_reader_read_line(var self);
var file_reader_read_all(var self);
bool file_reader_eof(var self);

// FileWriter functions
var file_writer_new(const char* filename);
var file_writer_from_file(FILE* file);
void file_writer_write(var self, var data);
void file_writer_write_line(var self, var data);
void file_writer_flush(var self);

// BufferedReader functions
var buffered_reader_new(var reader, size_t buffer_size);
var buffered_reader_read(var self, size_t size);
var buffered_reader_read_line(var self);
bool buffered_reader_eof(var self);

// BufferedWriter functions
var buffered_writer_new(var writer, size_t buffer_size);
void buffered_writer_write(var self, var data);
void buffered_writer_write_line(var self, var data);
void buffered_writer_flush(var self);

// StringReader functions
var string_reader_new(const char* data);
var string_reader_from_string(var str);
var string_reader_read(var self, size_t size);
var string_reader_read_line(var self);
bool string_reader_eof(var self);
void string_reader_seek(var self, size_t position);

// StringWriter functions
var string_writer_new(void);
void string_writer_write(var self, var data);
void string_writer_write_line(var self, var data);
var string_writer_to_string(var self);
void string_writer_clear(var self);

// Path functions
var path_new(const char* path);
var path_join(var self, const char* component);
var path_parent(var self);
var path_filename(var self);
var path_extension(var self);
var path_stem(var self);
bool path_exists(var self);
bool path_is_file(var self);
bool path_is_directory(var self);
bool path_is_absolute(var self);
var path_absolute(var self);
var path_normalize(var self);

// Directory functions
var list_directory(const char* path);
bool create_directory(const char* path);
bool create_directories(const char* path);
bool remove_directory(const char* path);
bool remove_file(const char* path);
bool copy_file(const char* src, const char* dest);
bool move_file(const char* src, const char* dest);

// Utility functions
var read_file_to_string(const char* filename);
bool write_string_to_file(const char* filename, var content);
bool append_string_to_file(const char* filename, var content);

// Standard streams
extern var STDIN;
extern var STDOUT;
extern var STDERR;

// Convenience macros
#define println_to(writer, ...) do { \
    file_writer_write_line(writer, $S(format_string(__VA_ARGS__))); \
} while(0)

#define print_to_file(filename, ...) do { \
    var writer = file_writer_new(filename); \
    file_writer_write(writer, $S(format_string(__VA_ARGS__))); \
    file_close(writer); \
    del(writer); \
} while(0)

// Initialization function
void __cello_std_io_init(void);

#endif
#include "Cello.h"
#include "Io.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// I/O types
var FileReader;
var FileWriter;
var BufferedReader;
var BufferedWriter;
var StringReader;
var StringWriter;
var Path;

// Standard streams
var STDIN;
var STDOUT;
var STDERR;

static bool io_initialized = false;

#define DEFAULT_BUFFER_SIZE 8192

// FileReader implementation
static void FileReader_New(var self, var args) {
    struct FileReader* fr = cast(self, FileReader);
    fr->file = NULL;
    fr->filename = NULL;
    fr->owns_file = true;
    
    if (len(args) > 0) {
        const char* filename = c_str(get(args, $I(0)));
        fr->filename = malloc(strlen(filename) + 1);
        strcpy(fr->filename, filename);
        fr->file = fopen(filename, "r");
    }
}

static void FileReader_Del(var self) {
    struct FileReader* fr = cast(self, FileReader);
    if (fr->file && fr->owns_file) {
        fclose(fr->file);
    }
    if (fr->filename) {
        free(fr->filename);
    }
}

static int FileReader_Show(var self, var output, int pos) {
    struct FileReader* fr = cast(self, FileReader);
    return print_to(output, pos, "FileReader(\"%s\")", 
                   fr->filename ? fr->filename : "<unknown>");
}

var FileReader = Cello(FileReader,
    Instance(New, FileReader_New, FileReader_Del),
    Instance(Show, FileReader_Show));

// FileWriter implementation
static void FileWriter_New(var self, var args) {
    struct FileWriter* fw = cast(self, FileWriter);
    fw->file = NULL;
    fw->filename = NULL;
    fw->owns_file = true;
    
    if (len(args) > 0) {
        const char* filename = c_str(get(args, $I(0)));
        fw->filename = malloc(strlen(filename) + 1);
        strcpy(fw->filename, filename);
        fw->file = fopen(filename, "w");
    }
}

static void FileWriter_Del(var self) {
    struct FileWriter* fw = cast(self, FileWriter);
    if (fw->file && fw->owns_file) {
        fclose(fw->file);
    }
    if (fw->filename) {
        free(fw->filename);
    }
}

static int FileWriter_Show(var self, var output, int pos) {
    struct FileWriter* fw = cast(self, FileWriter);
    return print_to(output, pos, "FileWriter(\"%s\")", 
                   fw->filename ? fw->filename : "<unknown>");
}

var FileWriter = Cello(FileWriter,
    Instance(New, FileWriter_New, FileWriter_Del),
    Instance(Show, FileWriter_Show));

// BufferedReader implementation
static void BufferedReader_New(var self, var args) {
    struct BufferedReader* br = cast(self, BufferedReader);
    br->reader = NULL;
    br->buffer = NULL;
    br->buffer_size = DEFAULT_BUFFER_SIZE;
    br->buffer_pos = 0;
    br->buffer_len = 0;
    
    if (len(args) > 0) {
        br->reader = get(args, $I(0));
    }
    if (len(args) > 1) {
        br->buffer_size = c_int(get(args, $I(1)));
    }
    
    br->buffer = malloc(br->buffer_size);
}

static void BufferedReader_Del(var self) {
    struct BufferedReader* br = cast(self, BufferedReader);
    if (br->buffer) {
        free(br->buffer);
    }
}

static int BufferedReader_Show(var self, var output, int pos) {
    return print_to(output, pos, "BufferedReader(buffer_size=%zu)", 
                   $I(((struct BufferedReader*)self)->buffer_size));
}

var BufferedReader = Cello(BufferedReader,
    Instance(New, BufferedReader_New, BufferedReader_Del),
    Instance(Show, BufferedReader_Show));

// BufferedWriter implementation
static void BufferedWriter_New(var self, var args) {
    struct BufferedWriter* bw = cast(self, BufferedWriter);
    bw->writer = NULL;
    bw->buffer = NULL;
    bw->buffer_size = DEFAULT_BUFFER_SIZE;
    bw->buffer_pos = 0;
    
    if (len(args) > 0) {
        bw->writer = get(args, $I(0));
    }
    if (len(args) > 1) {
        bw->buffer_size = c_int(get(args, $I(1)));
    }
    
    bw->buffer = malloc(bw->buffer_size);
}

static void BufferedWriter_Del(var self) {
    struct BufferedWriter* bw = cast(self, BufferedWriter);
    if (bw->buffer) {
        free(bw->buffer);
    }
}

static int BufferedWriter_Show(var self, var output, int pos) {
    return print_to(output, pos, "BufferedWriter(buffer_size=%zu)", 
                   $I(((struct BufferedWriter*)self)->buffer_size));
}

var BufferedWriter = Cello(BufferedWriter,
    Instance(New, BufferedWriter_New, BufferedWriter_Del),
    Instance(Show, BufferedWriter_Show));

// StringReader implementation
static void StringReader_New(var self, var args) {
    struct StringReader* sr = cast(self, StringReader);
    sr->data = NULL;
    sr->length = 0;
    sr->position = 0;
    sr->owns_data = false;
    
    if (len(args) > 0) {
        const char* data = c_str(get(args, $I(0)));
        sr->length = strlen(data);
        sr->data = malloc(sr->length + 1);
        strcpy(sr->data, data);
        sr->owns_data = true;
    }
}

static void StringReader_Del(var self) {
    struct StringReader* sr = cast(self, StringReader);
    if (sr->data && sr->owns_data) {
        free(sr->data);
    }
}

static int StringReader_Show(var self, var output, int pos) {
    struct StringReader* sr = cast(self, StringReader);
    return print_to(output, pos, "StringReader(length=%zu, pos=%zu)", 
                   $I(sr->length), $I(sr->position));
}

var StringReader = Cello(StringReader,
    Instance(New, StringReader_New, StringReader_Del),
    Instance(Show, StringReader_Show));

// StringWriter implementation
static void StringWriter_New(var self, var args) {
    struct StringWriter* sw = cast(self, StringWriter);
    sw->capacity = 256;
    sw->length = 0;
    sw->data = malloc(sw->capacity);
    sw->data[0] = '\0';
}

static void StringWriter_Del(var self) {
    struct StringWriter* sw = cast(self, StringWriter);
    if (sw->data) {
        free(sw->data);
    }
}

static int StringWriter_Show(var self, var output, int pos) {
    struct StringWriter* sw = cast(self, StringWriter);
    return print_to(output, pos, "StringWriter(length=%zu, capacity=%zu)", 
                   $I(sw->length), $I(sw->capacity));
}

var StringWriter = Cello(StringWriter,
    Instance(New, StringWriter_New, StringWriter_Del),
    Instance(Show, StringWriter_Show));

// Path implementation
static void Path_New(var self, var args) {
    struct Path* p = cast(self, Path);
    p->path = NULL;
    p->length = 0;
    
    if (len(args) > 0) {
        const char* path = c_str(get(args, $I(0)));
        p->length = strlen(path);
        p->path = malloc(p->length + 1);
        strcpy(p->path, path);
    }
}

static void Path_Del(var self) {
    struct Path* p = cast(self, Path);
    if (p->path) {
        free(p->path);
    }
}

static int Path_Show(var self, var output, int pos) {
    struct Path* p = cast(self, Path);
    return print_to(output, pos, "Path(\"%s\")", p->path ? p->path : "");
}

var Path = Cello(Path,
    Instance(New, Path_New, Path_Del),
    Instance(Show, Path_Show));

// FileReader functions
var file_reader_new(const char* filename) {
    return new(FileReader, $S((char*)filename));
}

var file_reader_from_file(FILE* file) {
    var fr = new(FileReader);
    struct FileReader* reader = cast(fr, FileReader);
    reader->file = file;
    reader->owns_file = false;
    return fr;
}

var file_reader_read(var self, size_t size) {
    struct FileReader* fr = cast(self, FileReader);
    if (!fr->file) return NULL;
    
    char* buffer = malloc(size + 1);
    size_t bytes_read = fread(buffer, 1, size, fr->file);
    buffer[bytes_read] = '\0';
    
    var result = new(String, $S(buffer));
    free(buffer);
    return result;
}

var file_reader_read_line(var self) {
    struct FileReader* fr = cast(self, FileReader);
    if (!fr->file) return NULL;
    
    char* line = NULL;
    size_t len = 0;
    ssize_t read_len = getline(&line, &len, fr->file);
    
    if (read_len == -1) {
        if (line) free(line);
        return NULL;
    }
    
    // Remove trailing newline
    if (read_len > 0 && line[read_len - 1] == '\n') {
        line[read_len - 1] = '\0';
    }
    
    var result = new(String, $S(line));
    free(line);
    return result;
}

var file_reader_read_all(var self) {
    struct FileReader* fr = cast(self, FileReader);
    if (!fr->file) return NULL;
    
    fseek(fr->file, 0, SEEK_END);
    long file_size = ftell(fr->file);
    fseek(fr->file, 0, SEEK_SET);
    
    char* buffer = malloc(file_size + 1);
    if (fread(buffer, 1, file_size, fr->file) != file_size) {
        free(buffer);
        return NULL;
    }
    buffer[file_size] = '\0';
    
    var result = new(String, $S(buffer));
    free(buffer);
    return result;
}

bool file_reader_eof(var self) {
    struct FileReader* fr = cast(self, FileReader);
    return fr->file ? feof(fr->file) : true;
}

// FileWriter functions
var file_writer_new(const char* filename) {
    return new(FileWriter, $S((char*)filename));
}

var file_writer_from_file(FILE* file) {
    var fw = new(FileWriter);
    struct FileWriter* writer = cast(fw, FileWriter);
    writer->file = file;
    writer->owns_file = false;
    return fw;
}

void file_writer_write(var self, var data) {
    struct FileWriter* fw = cast(self, FileWriter);
    if (!fw->file) return;
    
    const char* str = c_str(data);
    fwrite(str, 1, strlen(str), fw->file);
}

void file_writer_write_line(var self, var data) {
    file_writer_write(self, data);
    struct FileWriter* fw = cast(self, FileWriter);
    if (fw->file) {
        fwrite("\n", 1, 1, fw->file);
    }
}

void file_writer_flush(var self) {
    struct FileWriter* fw = cast(self, FileWriter);
    if (fw->file) {
        fflush(fw->file);
    }
}

// StringReader functions
var string_reader_new(const char* data) {
    return new(StringReader, $S((char*)data));
}

var string_reader_from_string(var str) {
    return new(StringReader, str);
}

var string_reader_read(var self, size_t size) {
    struct StringReader* sr = cast(self, StringReader);
    if (!sr->data || sr->position >= sr->length) return NULL;
    
    size_t available = sr->length - sr->position;
    size_t to_read = (size < available) ? size : available;
    
    char* buffer = malloc(to_read + 1);
    memcpy(buffer, sr->data + sr->position, to_read);
    buffer[to_read] = '\0';
    
    sr->position += to_read;
    
    var result = new(String, $S(buffer));
    free(buffer);
    return result;
}

var string_reader_read_line(var self) {
    struct StringReader* sr = cast(self, StringReader);
    if (!sr->data || sr->position >= sr->length) return NULL;
    
    char* start = sr->data + sr->position;
    char* newline = strchr(start, '\n');
    
    size_t line_len;
    if (newline) {
        line_len = newline - start;
        sr->position += line_len + 1; // +1 for newline
    } else {
        line_len = sr->length - sr->position;
        sr->position = sr->length;
    }
    
    char* buffer = malloc(line_len + 1);
    memcpy(buffer, start, line_len);
    buffer[line_len] = '\0';
    
    var result = new(String, $S(buffer));
    free(buffer);
    return result;
}

bool string_reader_eof(var self) {
    struct StringReader* sr = cast(self, StringReader);
    return sr->position >= sr->length;
}

void string_reader_seek(var self, size_t position) {
    struct StringReader* sr = cast(self, StringReader);
    sr->position = (position > sr->length) ? sr->length : position;
}

// StringWriter functions
var string_writer_new(void) {
    return new(StringWriter);
}

static void string_writer_ensure_capacity(struct StringWriter* sw, size_t needed) {
    if (sw->length + needed >= sw->capacity) {
        while (sw->length + needed >= sw->capacity) {
            sw->capacity *= 2;
        }
        sw->data = realloc(sw->data, sw->capacity);
    }
}

void string_writer_write(var self, var data) {
    struct StringWriter* sw = cast(self, StringWriter);
    const char* str = c_str(data);
    size_t str_len = strlen(str);
    
    string_writer_ensure_capacity(sw, str_len);
    strcpy(sw->data + sw->length, str);
    sw->length += str_len;
}

void string_writer_write_line(var self, var data) {
    string_writer_write(self, data);
    struct StringWriter* sw = cast(self, StringWriter);
    string_writer_ensure_capacity(sw, 1);
    sw->data[sw->length] = '\n';
    sw->length++;
    sw->data[sw->length] = '\0';
}

var string_writer_to_string(var self) {
    struct StringWriter* sw = cast(self, StringWriter);
    
    // Ensure the data is null-terminated
    if (sw->length >= sw->capacity) {
        sw->capacity = sw->length + 1;
        sw->data = realloc(sw->data, sw->capacity);
    }
    sw->data[sw->length] = '\0';
    
    // Create a new String by copying the data using $S macro
    return new(String, $S(sw->data));
}

void string_writer_clear(var self) {
    struct StringWriter* sw = cast(self, StringWriter);
    sw->length = 0;
    sw->data[0] = '\0';
}

// Path functions
var path_new(const char* path) {
    return new(Path, $S((char*)path));
}

var path_join(var self, const char* component) {
    struct Path* p = cast(self, Path);
    if (!p->path || !component) return copy(self);
    
    size_t new_len = p->length + strlen(component) + 2; // +2 for '/' and null terminator
    char* new_path = malloc(new_len);
    
    strcpy(new_path, p->path);
    if (p->path[p->length - 1] != '/') {
        strcat(new_path, "/");
    }
    strcat(new_path, component);
    
    var result = new(Path, $S(new_path));
    free(new_path);
    return result;
}

var path_filename(var self) {
    struct Path* p = cast(self, Path);
    if (!p->path) return NULL;
    
    char* last_slash = strrchr(p->path, '/');
    if (!last_slash) {
        return new(String, $S(p->path));
    }
    
    return new(String, $S(last_slash + 1));
}

var path_extension(var self) {
    var filename = path_filename(self);
    if (!filename) return NULL;
    
    const char* fname = c_str(filename);
    char* last_dot = strrchr(fname, '.');
    
    if (!last_dot) {
        del(filename);
        return NULL;
    }
    
    var ext = new(String, $S(last_dot));
    del(filename);
    return ext;
}

bool path_exists(var self) {
    struct Path* p = cast(self, Path);
    if (!p->path) return false;
    
    return access(p->path, F_OK) == 0;
}

bool path_is_file(var self) {
    struct Path* p = cast(self, Path);
    if (!p->path) return false;
    
    struct stat st;
    if (stat(p->path, &st) != 0) return false;
    return S_ISREG(st.st_mode);
}

bool path_is_directory(var self) {
    struct Path* p = cast(self, Path);
    if (!p->path) return false;
    
    struct stat st;
    if (stat(p->path, &st) != 0) return false;
    return S_ISDIR(st.st_mode);
}

// Utility functions
var read_file_to_string(const char* filename) {
    var reader = file_reader_new(filename);
    if (!reader) return NULL;
    
    var content = file_reader_read_all(reader);
    del(reader);
    return content;
}

bool write_string_to_file(const char* filename, var content) {
    var writer = file_writer_new(filename);
    if (!writer) return false;
    
    file_writer_write(writer, content);
    del(writer);
    return true;
}

bool append_string_to_file(const char* filename, var content) {
    FILE* file = fopen(filename, "a");
    if (!file) return false;
    
    var writer = file_writer_from_file(file);
    file_writer_write(writer, content);
    del(writer);
    fclose(file);
    return true;
}

// Directory functions
var list_directory(const char* path) {
    DIR* dir = opendir(path);
    if (!dir) return NULL;
    
    var list = new(List);
    struct dirent* entry;
    
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            push(list, new(String, $S(entry->d_name)));
        }
    }
    
    closedir(dir);
    return list;
}

bool create_directory(const char* path) {
    return mkdir(path, 0755) == 0;
}

bool remove_file(const char* path) {
    return unlink(path) == 0;
}

bool copy_file(const char* src, const char* dest) {
    var content = read_file_to_string(src);
    if (!content) return false;
    
    bool result = write_string_to_file(dest, content);
    del(content);
    return result;
}

// Initialization function
void __cello_std_io_init(void) {
    if (io_initialized) return;
    
    // Initialize standard streams
    STDIN = file_reader_from_file(stdin);
    STDOUT = file_writer_from_file(stdout);
    STDERR = file_writer_from_file(stderr);
    
    io_initialized = true;
}
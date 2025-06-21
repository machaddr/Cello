#ifndef CELLO_STD_OS_H
#define CELLO_STD_OS_H

#include "Cello.h"

// Operating system interface types
extern var OSProcess;
extern var Environment;
extern var Path;

// Process structure
struct OSProcess {
    pid_t pid;
    int exit_code;
    bool running;
    char* command;
    var stdin_pipe;
    var stdout_pipe;
    var stderr_pipe;
};

// Environment structure
struct Environment {
    var env_vars; // HashMap of environment variables
};

// System information
typedef struct {
    char* os_name;
    char* os_version;
    char* architecture;
    char* hostname;
    char* username;
    size_t total_memory;
    size_t available_memory;
    int cpu_count;
} SystemInfo;

// Process management functions
var process_spawn(const char* command, var args);
var process_spawn_shell(const char* command);
int process_wait(var self);
bool process_is_running(var self);
void process_kill(var self);
void process_terminate(var self);
pid_t process_get_pid(var self);
int process_get_exit_code(var self);

// Process communication
void process_write_stdin(var self, var data);
var process_read_stdout(var self);
var process_read_stderr(var self);
var process_read_stdout_line(var self);
var process_read_stderr_line(var self);

// Current process functions
pid_t get_current_pid(void);
pid_t get_parent_pid(void);
var get_current_working_directory(void);
bool set_current_working_directory(const char* path);
var get_executable_path(void);
var get_command_line_args(void);

// Environment variable functions
var env_new(void);
var env_get(const char* name);
bool env_set(const char* name, const char* value);
bool env_unset(const char* name);
bool env_exists(const char* name);
var env_get_all(void);

// User and group functions
var get_username(void);
var get_user_home_directory(void);
var get_user_data_directory(void);
var get_user_config_directory(void);
var get_user_cache_directory(void);
var get_temp_directory(void);

// System information functions
SystemInfo* get_system_info(void);
void free_system_info(SystemInfo* info);
var get_hostname(void);
var get_os_name(void);
var get_os_version(void);
var get_architecture(void);
size_t get_total_memory(void);
size_t get_available_memory(void);
int get_cpu_count(void);

// File system functions
bool file_exists(const char* path);
bool directory_exists(const char* path);
bool is_file(const char* path);
bool is_directory(const char* path);
bool is_symlink(const char* path);
size_t file_size(const char* path);
time_t file_modified_time(const char* path);
time_t file_created_time(const char* path);
time_t file_accessed_time(const char* path);

// File permissions
bool is_readable(const char* path);
bool is_writable(const char* path);
bool is_executable(const char* path);
bool set_file_permissions(const char* path, int mode);
int get_file_permissions(const char* path);

// Directory operations
var list_directory_detailed(const char* path);
bool create_directory_recursive(const char* path);
bool remove_directory_recursive(const char* path);
var find_files(const char* directory, const char* pattern);
var walk_directory(const char* path);

// Path manipulation
var os_path_absolute(const char* path);
var os_path_canonical(const char* path);
var os_path_relative(const char* path, const char* base);
var os_path_join_multiple(var paths);
bool os_path_is_absolute(const char* path);
bool os_path_is_relative(const char* path);

// Process execution with pipes
var execute_command(const char* command);
var execute_command_with_input(const char* command, var input);
var execute_shell_command(const char* command);

// Signal handling
typedef void (*signal_handler_t)(int);
signal_handler_t signal_set_handler(int signum, signal_handler_t handler);
void signal_ignore(int signum);
void signal_default(int signum);
void signal_send(pid_t pid, int signum);

// Memory information
typedef struct {
    size_t total;
    size_t available;
    size_t used;
    size_t free;
    size_t cached;
    size_t buffered;
} MemoryInfo;

MemoryInfo* get_memory_info(void);
void free_memory_info(MemoryInfo* info);

// CPU information
typedef struct {
    int logical_cores;
    int physical_cores;
    double current_usage;
    var model_name;
    double frequency_mhz;
} CPUInfo;

CPUInfo* get_cpu_info(void);
void free_cpu_info(CPUInfo* info);

// Network information
typedef struct {
    char* interface_name;
    char* ip_address;
    char* mac_address;
    bool is_up;
    uint64_t bytes_sent;
    uint64_t bytes_received;
} NetworkInterface;

var get_network_interfaces(void);
void free_network_interfaces(var interfaces);

// System monitoring
double get_cpu_usage(void);
double get_memory_usage_percent(void);
var get_running_processes(void);

// Platform-specific functions
#ifdef CELLO_WINDOWS
var get_windows_version(void);
var get_registry_value(const char* key, const char* value);
bool set_registry_value(const char* key, const char* value, const char* data);
#endif

#ifdef CELLO_UNIX
var get_kernel_version(void);
var get_distribution_info(void);
var get_uptime(void);
var get_load_average(void);
#endif

// Error handling
extern var OSError;

struct OSError {
    int error_code;
    char* message;
    char* function_name;
};

// Utility functions
void os_sleep(double seconds);
void os_yield(void);
uint64_t os_get_tick_count(void);
double os_get_time(void);

// Constants
extern var OS_PATH_SEPARATOR;
extern var OS_LINE_SEPARATOR;
extern var OS_CURRENT_DIRECTORY;
extern var OS_PARENT_DIRECTORY;

// Initialization function
void __cello_std_os_init(void);

#endif
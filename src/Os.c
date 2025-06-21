#include "Cello.h"
#include "Os.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// OS types
var OSProcess;
var Environment;
var OSError;

// OS constants
var OS_PATH_SEPARATOR;
var OS_LINE_SEPARATOR;
var OS_CURRENT_DIRECTORY;
var OS_PARENT_DIRECTORY;

static bool os_initialized = false;

// Process implementation
static void OSProcess_New(var self, var args) {
    struct OSProcess* p = cast(self, OSProcess);
    p->pid = -1;
    p->exit_code = 0;
    p->running = false;
    p->command = NULL;
    p->stdin_pipe = NULL;
    p->stdout_pipe = NULL;
    p->stderr_pipe = NULL;
    
    if (len(args) > 0) {
        const char* command = c_str(get(args, $I(0)));
        p->command = malloc(strlen(command) + 1);
        strcpy(p->command, command);
    }
}

static void OSProcess_Del(var self) {
    struct OSProcess* p = cast(self, OSProcess);
    if (p->command) {
        free(p->command);
        p->command = NULL;
    }
    if (p->stdin_pipe) del(p->stdin_pipe);
    if (p->stdout_pipe) del(p->stdout_pipe);
    if (p->stderr_pipe) del(p->stderr_pipe);
}

static int OSProcess_Show(var self, var output, int pos) {
    struct OSProcess* p = cast(self, OSProcess);
    return print_to(output, pos, "Process(pid=%d, command=\"%s\", running=%s)", 
                   $I(p->pid), $S(p->command ? p->command : "<unknown>"), $S(p->running ? "true" : "false"));
}

var OSProcess = Cello(OSProcess,
    Instance(New, OSProcess_New, OSProcess_Del),
    Instance(Show, OSProcess_Show));

// Environment implementation
static void Environment_New(var self, var args) {
    struct Environment* env = cast(self, Environment);
    env->env_vars = new(Table, String, String);
    
    // Load current environment variables
    extern char **environ;
    for (char **env_ptr = environ; *env_ptr != NULL; env_ptr++) {
        char* env_var = *env_ptr;
        char* equals = strchr(env_var, '=');
        if (equals) {
            size_t name_len = equals - env_var;
            char* name = malloc(name_len + 1);
            strncpy(name, env_var, name_len);
            name[name_len] = '\0';
            
            set(env->env_vars, new(String, $S(name)), new(String, $S(equals + 1)));
            free(name);
        }
    }
}

static void Environment_Del(var self) {
    struct Environment* env = cast(self, Environment);
    if (env->env_vars) {
        del(env->env_vars);
    }
}

static int Environment_Show(var self, var output, int pos) {
    struct Environment* env = cast(self, Environment);
    return print_to(output, pos, "Environment(vars=%zu)", $I(len(env->env_vars)));
}

var Environment = Cello(Environment,
    Instance(New, Environment_New, Environment_Del),
    Instance(Show, Environment_Show));

// OSError implementation
static void OSError_New(var self, var args) {
    struct OSError* err = cast(self, OSError);
    err->error_code = 0;
    err->message = NULL;
    err->function_name = NULL;
    
    if (len(args) > 0) {
        err->error_code = c_int(get(args, $I(0)));
    }
    if (len(args) > 1) {
        const char* msg = c_str(get(args, $I(1)));
        err->message = malloc(strlen(msg) + 1);
        strcpy(err->message, msg);
    }
    if (len(args) > 2) {
        const char* func = c_str(get(args, $I(2)));
        err->function_name = malloc(strlen(func) + 1);
        strcpy(err->function_name, func);
    }
}

static void OSError_Del(var self) {
    struct OSError* err = cast(self, OSError);
    if (err->message) {
        free(err->message);
        err->message = NULL;
    }
    if (err->function_name) {
        free(err->function_name);
        err->function_name = NULL;
    }
}

static int OSError_Show(var self, var output, int pos) {
    struct OSError* err = cast(self, OSError);
    return print_to(output, pos, "OSError(%d: %s)", 
                   $I(err->error_code), $S(err->message ? err->message : "Unknown error"));
}

var OSError = Cello(OSError,
    Instance(New, OSError_New, OSError_Del),
    Instance(Show, OSError_Show));

// Process management functions
var process_spawn(const char* command, var args) {
    var proc = new(OSProcess, $S((char*)command));
    struct OSProcess* p = cast(proc, Process);
    
    // Simple implementation using system() for now
    // In a full implementation, you'd use fork/exec with proper pipe handling
    p->pid = fork();
    if (p->pid == 0) {
        // Child process
        execlp("/bin/sh", "sh", "-c", command, NULL);
        exit(1);
    } else if (p->pid > 0) {
        // Parent process
        p->running = true;
    }
    
    return proc;
}

var process_spawn_shell(const char* command) {
    return process_spawn(command, NULL);
}

int process_wait(var self) {
    struct OSProcess* p = cast(self, OSProcess);
    if (!p->running || p->pid <= 0) return p->exit_code;
    
    int status;
    waitpid(p->pid, &status, 0);
    p->running = false;
    
    if (WIFEXITED(status)) {
        p->exit_code = WEXITSTATUS(status);
    } else {
        p->exit_code = -1;
    }
    
    return p->exit_code;
}

bool process_is_running(var self) {
    struct OSProcess* p = cast(self, OSProcess);
    if (!p->running || p->pid <= 0) return false;
    
    int status;
    pid_t result = waitpid(p->pid, &status, WNOHANG);
    
    if (result == 0) {
        return true; // Still running
    } else if (result == p->pid) {
        p->running = false;
        if (WIFEXITED(status)) {
            p->exit_code = WEXITSTATUS(status);
        }
        return false;
    }
    
    return false;
}

void process_kill(var self) {
    struct OSProcess* p = cast(self, OSProcess);
    if (p->running && p->pid > 0) {
        kill(p->pid, SIGKILL);
        p->running = false;
    }
}

void process_terminate(var self) {
    struct OSProcess* p = cast(self, OSProcess);
    if (p->running && p->pid > 0) {
        kill(p->pid, SIGTERM);
        p->running = false;
    }
}

pid_t process_get_pid(var self) {
    struct OSProcess* p = cast(self, OSProcess);
    return p->pid;
}

int process_get_exit_code(var self) {
    struct OSProcess* p = cast(self, OSProcess);
    return p->exit_code;
}

// Current process functions
pid_t get_current_pid(void) {
    return getpid();
}

pid_t get_parent_pid(void) {
    return getppid();
}

var get_current_working_directory(void) {
    char* cwd = getcwd(NULL, 0);
    if (!cwd) return NULL;
    
    var result = new(String, $S(cwd));
    free(cwd);
    return result;
}

bool set_current_working_directory(const char* path) {
    return chdir(path) == 0;
}

var get_executable_path(void) {
    char path[1024];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len != -1) {
        path[len] = '\0';
        return new(String, $S(path));
    }
    return NULL;
}

// Environment variable functions
var env_new(void) {
    return new(Environment);
}

var env_get(const char* name) {
    char* value = getenv(name);
    return value ? new(String, $S(value)) : NULL;
}

bool env_set(const char* name, const char* value) {
    return setenv(name, value, 1) == 0;
}

bool env_unset(const char* name) {
    return unsetenv(name) == 0;
}

bool env_exists(const char* name) {
    return getenv(name) != NULL;
}

var env_get_all(void) {
    return new(Environment);
}

// User and group functions
var get_username(void) {
    struct passwd* pw = getpwuid(getuid());
    return pw ? new(String, $S(pw->pw_name)) : NULL;
}

var get_user_home_directory(void) {
    const char* home = getenv("HOME");
    if (!home) {
        struct passwd* pw = getpwuid(getuid());
        home = pw ? pw->pw_dir : NULL;
    }
    return home ? new(String, $S((char*)home)) : NULL;
}

var get_user_data_directory(void) {
    const char* xdg_data = getenv("XDG_DATA_HOME");
    if (xdg_data) {
        return new(String, $S((char*)xdg_data));
    }
    
    var home = get_user_home_directory();
    if (home) {
        const char* home_str = c_str(home);
        size_t len = strlen(home_str) + 14; // "/.local/share" + null
        char* data_dir = malloc(len);
        snprintf(data_dir, len, "%s/.local/share", home_str);
        
        var result = new(String, $S(data_dir));
        free(data_dir);
        del(home);
        return result;
    }
    
    return NULL;
}

var get_user_config_directory(void) {
    const char* xdg_config = getenv("XDG_CONFIG_HOME");
    if (xdg_config) {
        return new(String, $S((char*)xdg_config));
    }
    
    var home = get_user_home_directory();
    if (home) {
        const char* home_str = c_str(home);
        size_t len = strlen(home_str) + 10; // "/.config" + null
        char* config_dir = malloc(len);
        snprintf(config_dir, len, "%s/.config", home_str);
        
        var result = new(String, config_dir);
        free(config_dir);
        del(home);
        return result;
    }
    
    return NULL;
}

var get_user_cache_directory(void) {
    const char* xdg_cache = getenv("XDG_CACHE_HOME");
    if (xdg_cache) {
        return new(String, $S((char*)xdg_cache));
    }
    
    var home = get_user_home_directory();
    if (home) {
        const char* home_str = c_str(home);
        size_t len = strlen(home_str) + 8; // "/.cache" + null
        char* cache_dir = malloc(len);
        snprintf(cache_dir, len, "%s/.cache", home_str);
        
        var result = new(String, cache_dir);
        free(cache_dir);
        del(home);
        return result;
    }
    
    return NULL;
}

var get_temp_directory(void) {
    const char* tmpdir = getenv("TMPDIR");
    if (!tmpdir) tmpdir = getenv("TMP");
    if (!tmpdir) tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "/tmp";
    
    return new(String, $S((char*)tmpdir));
}

// System information functions
SystemInfo* get_system_info(void) {
    SystemInfo* info = malloc(sizeof(SystemInfo));
    memset(info, 0, sizeof(SystemInfo));
    
    // Get OS name
    info->os_name = malloc(32);
    strcpy(info->os_name, "Linux"); // Simplified
    
    // Get hostname
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        info->hostname = malloc(strlen(hostname) + 1);
        strcpy(info->hostname, hostname);
    }
    
    // Get username
    var username = get_username();
    if (username) {
        const char* user_str = c_str(username);
        info->username = malloc(strlen(user_str) + 1);
        strcpy(info->username, user_str);
        del(username);
    }
    
    // Get CPU count
    info->cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
    
    return info;
}

void free_system_info(SystemInfo* info) {
    if (!info) return;
    
    if (info->os_name) free(info->os_name);
    if (info->os_version) free(info->os_version);
    if (info->architecture) free(info->architecture);
    if (info->hostname) free(info->hostname);
    if (info->username) free(info->username);
    
    free(info);
}

var get_hostname(void) {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return new(String, hostname);
    }
    return NULL;
}

var get_os_name(void) {
    return new(String, "Linux"); // Simplified
}

int get_cpu_count(void) {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

// File system functions
bool file_exists(const char* path) {
    return access(path, F_OK) == 0;
}

bool directory_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

bool is_file(const char* path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

bool is_directory(const char* path) {
    return directory_exists(path);
}

bool is_symlink(const char* path) {
    struct stat st;
    return lstat(path, &st) == 0 && S_ISLNK(st.st_mode);
}

size_t file_size(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return st.st_size;
    }
    return 0;
}

time_t file_modified_time(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return st.st_mtime;
    }
    return 0;
}

// File permissions
bool is_readable(const char* path) {
    return access(path, R_OK) == 0;
}

bool is_writable(const char* path) {
    return access(path, W_OK) == 0;
}

bool is_executable(const char* path) {
    return access(path, X_OK) == 0;
}

bool set_file_permissions(const char* path, int mode) {
    return chmod(path, mode) == 0;
}

int get_file_permissions(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return st.st_mode & 0777;
    }
    return -1;
}

// Directory operations
var list_directory_detailed(const char* path) {
    DIR* dir = opendir(path);
    if (!dir) return NULL;
    
    var list = new(List);
    struct dirent* entry;
    
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            var file_info = new(Table, String, String);
            set(file_info, new(String, "name"), new(String, entry->d_name));
            
            // Get file stats
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
            
            struct stat st;
            if (stat(full_path, &st) == 0) {
                if (S_ISREG(st.st_mode)) {
                    set(file_info, new(String, "type"), new(String, "file"));
                } else if (S_ISDIR(st.st_mode)) {
                    set(file_info, new(String, "type"), new(String, "directory"));
                } else if (S_ISLNK(st.st_mode)) {
                    set(file_info, new(String, "type"), new(String, "symlink"));
                } else {
                    set(file_info, new(String, "type"), new(String, "other"));
                }
                
                char size_str[32];
                snprintf(size_str, sizeof(size_str), "%ld", (long)st.st_size);
                set(file_info, new(String, "size"), new(String, $S(size_str)));
            }
            
            push(list, file_info);
        }
    }
    
    closedir(dir);
    return list;
}

bool create_directory_recursive(const char* path) {
    char* path_copy = malloc(strlen(path) + 1);
    strcpy(path_copy, path);
    
    char* p = path_copy;
    if (*p == '/') p++; // Skip leading slash
    
    bool success = true;
    while ((p = strchr(p, '/'))) {
        *p = '\0';
        if (mkdir(path_copy, 0755) != 0 && errno != EEXIST) {
            success = false;
            break;
        }
        *p = '/';
        p++;
    }
    
    if (success) {
        success = (mkdir(path_copy, 0755) == 0 || errno == EEXIST);
    }
    
    free(path_copy);
    return success;
}

// Process execution
var execute_command(const char* command) {
    FILE* pipe = popen(command, "r");
    if (!pipe) return NULL;
    
    var output = new(String, "");
    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), pipe)) {
        var line = new(String, buffer);
        concat(output, line);
        del(line);
    }
    
    pclose(pipe);
    return output;
}

var execute_shell_command(const char* command) {
    return execute_command(command);
}

// Signal handling
signal_handler_t signal_set_handler(int signum, signal_handler_t handler) {
    return signal(signum, handler);
}

void signal_ignore(int signum) {
    signal(signum, SIG_IGN);
}

void signal_default(int signum) {
    signal(signum, SIG_DFL);
}

void signal_send(pid_t pid, int signum) {
    kill(pid, signum);
}

// Utility functions
void os_sleep(double seconds) {
    if (seconds <= 0) return;
    
    time_t sec = (time_t)seconds;
    long nsec = (long)((seconds - sec) * 1000000000);
    
    struct timespec ts = {sec, nsec};
    nanosleep(&ts, NULL);
}

void os_yield(void) {
    sched_yield();
}

double os_get_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Unix-specific functions
#ifdef CELLO_UNIX
var get_uptime(void) {
    FILE* file = fopen("/proc/uptime", "r");
    if (!file) return NULL;
    
    double uptime;
    if (fscanf(file, "%lf", &uptime) == 1) {
        fclose(file);
        return new(Float, $F(uptime));
    }
    
    fclose(file);
    return NULL;
}

var get_load_average(void) {
    double loads[3];
    if (getloadavg(loads, 3) == 3) {
        var result = new(List);
        push(result, new(Float, $F(loads[0])));
        push(result, new(Float, $F(loads[1])));
        push(result, new(Float, $F(loads[2])));
        return result;
    }
    return NULL;
}
#endif

// Initialization function
void __cello_std_os_init(void) {
    if (os_initialized) return;
    
    // Initialize OS constants
    OS_PATH_SEPARATOR = new(String, "/");
    OS_LINE_SEPARATOR = new(String, "\n");
    OS_CURRENT_DIRECTORY = new(String, ".");
    OS_PARENT_DIRECTORY = new(String, "..");
    
    os_initialized = true;
}
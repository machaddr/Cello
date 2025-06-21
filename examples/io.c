/*
 * Test Cello Io Standard Library Module
 * Using pure Cello syntax and data structures
 */
#include <Cello.h>

int main(int argc, char** argv) {
    println("=== Testing Cello Io Standard Library ===");
    
    // Test string I/O operations
    println("\n--- String I/O Operations ---");
    
    // Test StringWriter
    var writer = string_writer_new();
    string_writer_write(writer, $S("Hello "));
    string_writer_write(writer, $S("World"));
    string_writer_write_line(writer, $S("!"));
    string_writer_write_line(writer, $S("This is a test."));
    
    var written_content = string_writer_to_string(writer);
    print("String writer content: "); show(written_content); println("");
    
    // Test StringReader  
    var reader = string_reader_from_string(written_content);
    var first_line = string_reader_read_line(reader);
    var second_line = string_reader_read_line(reader);
    
    print("First line read: "); show(first_line); println("");
    print("Second line read: "); show(second_line); println("");
    
    var is_eof = $I(string_reader_eof(reader) ? 1 : 0);
    print("Reader EOF: "); show(is_eof); println("");
    
    // Test file I/O operations
    println("\n--- File I/O Operations ---");
    
    // Create a test file
    var test_filename = $S("/tmp/cello_test.txt");
    var test_content = $S("This is a test file for Cello Io library.\nLine 2\nLine 3");
    
    var write_success = $I(write_string_to_file(c_str(test_filename), test_content) ? 1 : 0);
    print("File write success: "); show(write_success); println("");
    
    // Read the file back
    var read_content = read_file_to_string(c_str(test_filename));
    if (read_content) {
        print("File content: "); show(read_content); println("");
        
        // Test file reading with FileReader
        var file_reader = file_reader_new(c_str(test_filename));
        if (file_reader) {
            var file_line1 = file_reader_read_line(file_reader);
            var file_line2 = file_reader_read_line(file_reader);
            
            print("File line 1: "); show(file_line1); println("");
            print("File line 2: "); show(file_line2); println("");
            
            var file_eof = $I(file_reader_eof(file_reader) ? 1 : 0);
            print("File reader EOF: "); show(file_eof); println("");
        } else {
            println("Failed to create file reader");
        }
    } else {
        println("Failed to read file");
    }
    
    // Test path operations
    println("\n--- Path Operations ---");
    
    var path = path_new(c_str(test_filename));
    
    if (path) {
        var path_exists_result = $I(path_exists(path) ? 1 : 0);
        var path_is_file_result = $I(path_is_file(path) ? 1 : 0);
        var path_is_dir_result = $I(path_is_directory(path) ? 1 : 0);
        
        print("Path exists: "); show(path_exists_result); println("");
        print("Path is file: "); show(path_is_file_result); println("");
        print("Path is directory: "); show(path_is_dir_result); println("");
        
        var filename = path_filename(path);
        if (filename) {
            print("Filename: "); show(filename); println("");
        }
        
        var extension = path_extension(path);
        if (extension) {
            print("Extension: "); show(extension); println("");
        }
    } else {
        println("Failed to create path object");
    }
    
    // Test append operation
    println("\n--- File Append Operations ---");
    
    var append_content = $S("\nAppended line");
    var append_success = $I(append_string_to_file(c_str(test_filename), append_content) ? 1 : 0);
    print("Append success: "); show(append_success); println("");
    
    // Read file after append
    var final_content = read_file_to_string(c_str(test_filename));
    if (final_content) {
        print("Final file content: "); show(final_content); println("");
    }
    
    // Test storing I/O results in Cello structures
    println("\n--- Testing I/O Results Storage in Cello Structures ---");
    
    var io_results = new(List, String);
    
    // Store I/O operation results
    if (written_content) push(io_results, written_content);
    if (first_line) push(io_results, first_line);
    if (second_line) push(io_results, second_line);
    if (read_content) push(io_results, read_content);
    if (final_content) push(io_results, final_content);
    
    print("I/O operation results: "); show(io_results); println("");
    
    // Store I/O metrics in Table using pure Cello syntax
    var io_metrics = new(Table, String, Int);
    set(io_metrics, $S("operations_count"), $I(5));
    set(io_metrics, $S("write_success"), write_success);
    set(io_metrics, $S("append_success"), append_success);
    
    // Add path metrics directly with new Int objects
    if (path) {
        set(io_metrics, $S("path_exists"), $I(path_exists(path) ? 1 : 0));
        set(io_metrics, $S("is_file"), $I(path_is_file(path) ? 1 : 0));
    } else {
        set(io_metrics, $S("path_exists"), $I(0));
        set(io_metrics, $S("is_file"), $I(0));
    }
    
    print("I/O metrics: "); show(io_metrics); println("");
    
    // Clean up - remove test file
    println("\n--- Cleanup ---");
    var remove_success = $I(remove_file(c_str(test_filename)) ? 1 : 0);
    print("File removal success: "); show(remove_success); println("");
    
    println("\n=== Io Standard Library Test Complete ===");
    return 0;
}
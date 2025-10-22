#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PATH_LENGTH 4096
#define MAX_SEARCH_TEXT 1024
#define BUFFER_SIZE 8192

// Text file extensions to search
const char* TEXT_EXTENSIONS[] = {
    ".txt", ".md", ".log", ".cfg", ".conf", ".ini",
    ".sh", ".bash", ".py", ".js", ".html", ".css",
    ".xml", ".json", ".csv", ".yaml", ".yml", 
    ".c", ".h", ".cpp", ".hpp",
    NULL
};

// Binary file extensions to skip
const char* BINARY_EXTENSIONS[] = {
    ".exe", ".dll", ".bin", ".zip", ".rar", ".7z",
    ".tar", ".gz", ".jpg", ".jpeg", ".png", ".gif",
    ".bmp", ".pdf", ".doc", ".docx", ".xls", ".xlsx",
    ".ppt", ".pptx", ".msi", ".iso", ".img", ".pyc",
    NULL
};

// Global variables
int files_found = 0;
int files_searched = 0;
char** found_files_list = NULL;

// Function declarations
int is_text_file(const char* filename);
int is_binary_file(const char* filename);
int file_contains_text(const char* filename, const char* search_text);
void search_directory(const char* path, const char* search_text);
void print_usage(const char* program_name);
char* to_lower_case(const char* str);
char* get_extension(const char* filename);
void show_move_warning();
int copy_files(char** found_files, int file_count, const char* dest_dir);
int move_files(char** found_files, int file_count, const char* dest_dir);
char* get_unique_filename(const char* dest_dir, const char* filename);

int main(int argc, char* argv[]) {
    char search_path[MAX_PATH_LENGTH] = {0};
    char search_text[MAX_SEARCH_TEXT] = {0};
    int i;

    printf("=== File Search Tool for Windows ===\n\n");

    // Check command line arguments
    if (argc == 3) {
        strncpy(search_path, argv[1], MAX_PATH_LENGTH - 1);
        strncpy(search_text, argv[2], MAX_SEARCH_TEXT - 1);
        printf("Using command line parameters:\n");
        printf("Path: %s\n", search_path);
        printf("Search text: %s\n\n", search_text);
    } else {
        // Interactive mode
        printf("Enter search path: ");
        fgets(search_path, MAX_PATH_LENGTH, stdin);
        search_path[strcspn(search_path, "\n")] = 0;

        printf("Enter text to search for: ");
        fgets(search_text, MAX_SEARCH_TEXT, stdin);
        search_text[strcspn(search_text, "\n")] = 0;
        printf("\n");
    }

    // Validate path
    if (GetFileAttributesA(search_path) == INVALID_FILE_ATTRIBUTES) {
        fprintf(stderr, "Error: Path '%s' does not exist\n", search_path);
        return 1;
    }

    if (strlen(search_text) == 0) {
        fprintf(stderr, "Error: No search text provided\n");
        return 1;
    }

    printf("Searching for: '%s'\n", search_text);
    printf("Starting in: %s\n", search_path);
    printf("This may take a while for large directories...\n\n");

    // Allocate space for found files list
    found_files_list = malloc(1000 * sizeof(char*));

    search_directory(search_path, search_text);

    printf("\n=== Search Complete ===\n");
    printf("Files searched: %d\n", files_searched);
    printf("Files containing text: %d\n", files_found);

    if (files_found > 0) {
        printf("\nWhat would you like to do?\n");
        printf("1) Copy files to another directory\n");
        printf("2) Move files to another directory\n");
        printf("3) Just show results (do nothing)\n");
        printf("Enter your choice [1-3]: ");
        
        char choice[10];
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;
        
        if (strcmp(choice, "1") == 0 || strcmp(choice, "2") == 0) {
            char dest_dir[MAX_PATH_LENGTH];
            printf("Enter destination directory: ");
            fgets(dest_dir, sizeof(dest_dir), stdin);
            dest_dir[strcspn(dest_dir, "\n")] = 0;
            
            // Create directory if it doesn't exist
            if (GetFileAttributesA(dest_dir) == INVALID_FILE_ATTRIBUTES) {
                printf("Creating directory: %s\n", dest_dir);
                CreateDirectoryA(dest_dir, NULL);
            }
            
            if (strcmp(choice, "1") == 0) {
                copy_files(found_files_list, files_found, dest_dir);
            } else {
                show_move_warning();
                printf("Are you sure? (yes/no): ");
                char confirm[10];
                fgets(confirm, sizeof(confirm), stdin);
                confirm[strcspn(confirm, "\n")] = 0;
                if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0) {
                    move_files(found_files_list, files_found, dest_dir);
                } else {
                    printf("Move cancelled.\n");
                }
            }
        }
        
        // Free allocated memory
        for (i = 0; i < files_found; i++) {
            free(found_files_list[i]);
        }
        free(found_files_list);
    } else {
        printf("\nNo files found containing '%s'\n", search_text);
    }

    printf("\nPress Enter to exit...");
    getchar();
    return 0;
}

void search_directory(const char* path, const char* search_text) {
    char search_pattern[MAX_PATH_LENGTH];
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle;

    // Create search pattern
    _snprintf(search_pattern, MAX_PATH_LENGTH, "%s\\*", path);

    find_handle = FindFirstFileA(search_pattern, &find_data);
    if (find_handle == INVALID_HANDLE_VALUE) return;

    do {
        if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) {
            continue;
        }

        char full_path[MAX_PATH_LENGTH];
        _snprintf(full_path, MAX_PATH_LENGTH, "%s\\%s", path, find_data.cFileName);

        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            search_directory(full_path, search_text);
        } else {
            if (is_text_file(find_data.cFileName) && !is_binary_file(find_data.cFileName)) {
                files_searched++;
                
                if (files_searched % 100 == 0) {
                    printf("Searched %d files...\n", files_searched);
                }
                
                if (file_contains_text(full_path, search_text)) {
                    found_files_list[files_found] = _strdup(full_path);
                    if (found_files_list[files_found] != NULL) {
                        files_found++;
                        printf("FOUND: %s\n", full_path);
                    }
                }
            }
        }
    } while (FindNextFileA(find_handle, &find_data) != 0);

    FindClose(find_handle);
}

void show_move_warning() {
    printf("\nWARNING: Moving files is PERMANENT!\n");
    printf("Files will be removed from original locations.\n");
    printf("Make sure you have backups if needed.\n\n");
}

int copy_files(char** found_files, int file_count, const char* dest_dir) {
    int success_count = 0;
    int i;
    printf("\nCopying %d files to: %s\n", file_count, dest_dir);
    
    for (i = 0; i < file_count; i++) {
        char* source_file = found_files[i];
        char* filename = strrchr(source_file, '\\');
        if (filename == NULL) filename = source_file;
        else filename++;
        
        char dest_file[MAX_PATH_LENGTH];
        _snprintf(dest_file, MAX_PATH_LENGTH, "%s\\%s", dest_dir, filename);
        
        char* unique_dest = get_unique_filename(dest_dir, filename);
        
        if (CopyFileA(source_file, unique_dest, FALSE)) {
            printf("Copied: %s\n", filename);
            success_count++;
        } else {
            printf("Failed: %s\n", filename);
        }
        
        if (unique_dest != dest_file) free(unique_dest);
    }
    
    printf("\nCopied: %d/%d files\n", success_count, file_count);
    return success_count;
}

int move_files(char** found_files, int file_count, const char* dest_dir) {
    int success_count = 0;
    int i;
    printf("\nMoving %d files to: %s\n", file_count, dest_dir);
    
    for (i = 0; i < file_count; i++) {
        char* source_file = found_files[i];
        char* filename = strrchr(source_file, '\\');
        if (filename == NULL) filename = source_file;
        else filename++;
        
        char dest_file[MAX_PATH_LENGTH];
        _snprintf(dest_file, MAX_PATH_LENGTH, "%s\\%s", dest_dir, filename);
        
        char* unique_dest = get_unique_filename(dest_dir, filename);
        
        if (CopyFileA(source_file, unique_dest, FALSE)) {
            if (DeleteFileA(source_file)) {
                printf("Moved: %s\n", filename);
                success_count++;
            } else {
                printf("Copied but failed to remove: %s\n", filename);
                DeleteFileA(unique_dest);
            }
        } else {
            printf("Failed: %s\n", filename);
        }
        
        if (unique_dest != dest_file) free(unique_dest);
    }
    
    printf("\nMoved: %d/%d files\n", success_count, file_count);
    return success_count;
}

char* get_unique_filename(const char* dest_dir, const char* filename) {
    char base_path[MAX_PATH_LENGTH];
    char name[256];
    char ext[64];
    char* dot;
    int i;
    
    _snprintf(base_path, MAX_PATH_LENGTH, "%s\\%s", dest_dir, filename);
    
    // Check if file exists
    if (GetFileAttributesA(base_path) == INVALID_FILE_ATTRIBUTES) {
        return _strdup(base_path);
    }
    
    // File exists, find unique name
    dot = strrchr(filename, '.');
    if (dot != NULL) {
        strncpy(name, filename, dot - filename);
        name[dot - filename] = '\0';
        strcpy(ext, dot);
    } else {
        strcpy(name, filename);
        strcpy(ext, "");
    }
    
    for (i = 1; i < 1000; i++) {
        char new_name[MAX_PATH_LENGTH];
        _snprintf(new_name, MAX_PATH_LENGTH, "%s\\%s_%d%s", dest_dir, name, i, ext);
        
        if (GetFileAttributesA(new_name) == INVALID_FILE_ATTRIBUTES) {
            return _strdup(new_name);
        }
    }
    
    return _strdup(base_path);
}

int is_text_file(const char* filename) {
    char* ext = get_extension(filename);
    char* lower_ext;
    int i, result = 0;
    
    if (ext == NULL) return 0;
    
    lower_ext = to_lower_case(ext);
    
    for (i = 0; TEXT_EXTENSIONS[i] != NULL; i++) {
        if (strcmp(lower_ext, TEXT_EXTENSIONS[i]) == 0) {
            result = 1;
            break;
        }
    }
    
    free(lower_ext);
    free(ext);
    return result;
}

int is_binary_file(const char* filename) {
    char* ext = get_extension(filename);
    char* lower_ext;
    int i, result = 0;
    
    if (ext == NULL) return 0;
    
    lower_ext = to_lower_case(ext);
    
    for (i = 0; BINARY_EXTENSIONS[i] != NULL; i++) {
        if (strcmp(lower_ext, BINARY_EXTENSIONS[i]) == 0) {
            result = 1;
            break;
        }
    }
    
    free(lower_ext);
    free(ext);
    return result;
}

int file_contains_text(const char* filename, const char* search_text) {
    FILE* file = fopen(filename, "r");
    char* lower_search;
    char buffer[BUFFER_SIZE];
    int found = 0;
    
    if (file == NULL) return 0;
    
    lower_search = to_lower_case(search_text);
    
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char* lower_line = to_lower_case(buffer);
        if (strstr(lower_line, lower_search) != NULL) {
            found = 1;
            free(lower_line);
            break;
        }
        free(lower_line);
    }
    
    fclose(file);
    free(lower_search);
    return found;
}

char* get_extension(const char* filename) {
    const char* dot = strrchr(filename, '.');
    if (dot == NULL || dot == filename) return NULL;
    return _strdup(dot);
}

char* to_lower_case(const char* str) {
    size_t len;
    char* result;
    size_t i;
    
    if (str == NULL) return NULL;
    
    len = strlen(str);
    result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    
    return result;
}

void print_usage(const char* program_name) {
    printf("Usage: %s [directory] \"search text\"\n", program_name);
    printf("Example: %s c:\\myfolder \"search text\"\n", program_name);
}
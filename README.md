File Search Tool (C Windows Version)
A high-performance, native Windows file search utility written in C that allows you to search for text content within files and perform batch operations on the results. This version is optimized for Windows systems and provides fast, efficient searching without external dependencies.

https://img.shields.io/badge/C-17-blue.svg
https://img.shields.io/badge/Platform-Windows-lightgrey.svg
https://img.shields.io/badge/License-MIT-green.svg

Features
⚡ Native Performance: Written in C for maximum speed and efficiency

🔍 Fast Text Search: Optimized file searching with case-insensitive matching

📁 Recursive Directory Search: Automatically searches through subdirectories

🎯 Smart File Filtering: Focuses on text files while excluding binaries (EXE, ZIP, etc.)

📋 Batch File Operations: Copy or move multiple files based on search results

🛡️ Safety Features: Clear warnings and confirmations for destructive operations

📊 Progress Updates: Real-time progress during large searches

💾 No Dependencies: Single executable - runs on any Windows system

Supported File Types
Text Files Searched:
Documents: .txt, .md, .log, .cfg, .conf, .ini

Source Code: .c, .h, .cpp, .hpp, .py, .js, .html, .css, .sh, .bash

Data Files: .xml, .json, .csv, .yaml, .yml

Binary Files Excluded:
Archives: .zip, .rar, .7z, .tar, .gz

Executables: .exe, .dll, .bin, .msi

Media: .jpg, .jpeg, .png, .gif, .bmp, .pdf

Documents: .doc, .docx, .xls, .xlsx, .ppt, .pptx

System: .iso, .img, .pyc

Download
Pre-built Executable
Go to the Releases page

Download the latest FileSearchTool.exe

Run directly - no installation required

Build from Source
Requirements: Microsoft Visual Studio or MinGW compiler

bash
# Using GCC (MinGW)
gcc -o FileSearchTool.exe filesearchtool.c -lshlwapi

# Using Microsoft CL
cl filesearchtool.c
Usage
Command Line Mode
cmd
FileSearchTool.exe "C:\Search\Path" "search text"
Interactive Mode
cmd
FileSearchTool.exe
Then follow the prompts to enter search path and text.

Examples
Search for configuration settings:

cmd
FileSearchTool.exe "C:\Program Files\MyApp" "database_host"
Find TODO comments in source code:

cmd
FileSearchTool.exe "D:\Projects" "TODO:"
Search documents for specific content:

cmd
FileSearchTool.exe "%USERPROFILE%\Documents" "important note"
How It Works
Search Process:
Recursive Directory Traversal: Uses Windows API to efficiently walk through directories

File Type Filtering: Automatically skips binary files and focuses on text-based formats

Content Search: Performs case-insensitive text matching within files

Progress Tracking: Shows search progress every 100 files

Results Collection: Stores found file paths for batch operations

File Operations:
Copy Files: Safe duplication to destination directory

Move Files: Relocation with safety warnings and confirmations

Unique Naming: Automatically handles filename conflicts

Directory Creation: Creates destination directories if needed

Performance
Memory Efficient: Minimal memory footprint during operation

Fast Search: Optimized file I/O and string matching

Large Directory Support: Handles directories with thousands of files

Progress Feedback: Regular updates during long operations

Safety Features
✅ Move Operation Warnings: Clear warning messages before file relocation

✅ User Confirmation: Additional "yes/no" prompt for destructive operations

✅ File Existence Checks: Validates paths and file accessibility

✅ Error Handling: Graceful handling of permission issues and I/O errors

✅ Unique Filenames: Prevents overwriting existing files during copy/move

Technical Details
Architecture:
Pure Windows API: Uses FindFirstFile, FindNextFile for directory traversal

Efficient Memory Management: Manual memory allocation with proper cleanup

Case-Insensitive Matching: Converts both search text and file content to lowercase

Buffer Management: Optimized file reading with 8KB buffers

Compilation:
c
// Required libraries
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Compile with:
gcc -O2 -o FileSearchTool.exe filesearchtool.c
Comparison with Other Versions
Feature	C Version	Python Version
Performance	⚡ Very Fast	🐍 Moderate
Dependencies	None	Python 3.6+
Executable Size	~50KB	~5-10MB
Cross-Platform	Windows Only	Windows/Linux/Mac
Ease of Modification	Requires C knowledge	Easy Python scripting
Troubleshooting
Common Issues:
"Path does not exist" error

Ensure the path is spelled correctly

Use full paths (e.g., C:\Users\Name\Documents)

Permission errors

Run as Administrator if accessing protected directories

Check file/folder permissions

No files found

Verify search text is correct

Check if file types are in supported list

Try different casing (search is case-insensitive)

Build errors

Ensure Windows SDK is installed

Use compatible C compiler (MSVC or MinGW)

Contributing
Fork the repository

Create a feature branch

Make your changes

Test thoroughly

Submit a pull request

Building for Development:
bash
# Debug build
gcc -g -o FileSearchTool.exe filesearchtool.c

# Release build
gcc -O2 -o FileSearchTool.exe filesearchtool.c
License
This project is licensed under the MIT License - see the LICENSE file for details.

Security Note
This tool only reads text files and performs basic file operations. It does not:

Execute any file content

Modify file contents

Access network resources

Collect or transmit user data

Warning: Always backup important files before performing move operations. The tool includes safety features, but users should exercise caution when moving files.

*Optimized for Windows 7/8/10/11. Requires no installation or dependencies.*


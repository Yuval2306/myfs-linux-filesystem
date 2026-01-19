# MyFS - Simple Filesystem Simulator

## Project Purpose
MyFS is a simple filesystem implementation that simulates the three layers of a traditional filesystem:
1. Virtual File System (VFS) - Interfaces with the user through a command-line interface
2. Filesystem Layer - Manages files and directories (MyFS)
3. Block Device Layer - Simulates physical storage (BlockDeviceSimulator)

This project provides a simple implementation of a filesystem with basic operations like creating files, reading and writing content, listing directories, and removing files. Additionally, it supports directory operations including creating directories, removing directories, and moving/renaming files and directories.

## How to Build and Run
To build the project, use the provided Makefile or CMake:

### Using Make
```bash
make
```

### Using CMake
```bash
mkdir build
cd build
cmake ..
make
```

### Running the Simulator
```bash
./bin/myfs <filename>
```
Where `<filename>` is the path to the file that will be used to store the filesystem data.

Example:
```bash
./bin/myfs /tmp/myfs_storage
```

## Usage Example
```
$ ./bin/myfs /tmp/myfs_storage
Did not find myfs instance on blkdev
Creating...
Finished!
Welcome to myfs
To get help, please type 'help' on the prompt below.

myfs$ touch file_a
myfs$ edit file_a
Enter new file content
This is some file content
myfs$ ls
file_a 26
myfs$ cat file_a
This is some file content
myfs$ mkdir docs
myfs$ touch docs/note.txt
myfs$ edit docs/note.txt
Enter new file content
This is a note in the docs directory
myfs$ ls
file_a 26
docs 0
myfs$ exit
```

## Implemented Files
- **myfs.h**: Defines the filesystem structures and interfaces
- **myfs.cpp**: Implements the filesystem operations including file and directory management
- **vfs.cpp**: Implements the command-line interface for the filesystem
- **blkdev.h/cpp**: Implements the block device simulator layer that handles storage
- **myfs_main.cpp**: Main program entry point that initializes the system

## Filesystem Structure
The filesystem is organized as follows:
1. **Header**: Located at the beginning of the block device, contains a magic number and version
2. **Metadata**: Contains information about the filesystem, such as the number of files
3. **File Table**: An array of file entries, each containing file metadata
4. **Data Blocks**: Where the actual file content is stored

### File Entry Structure
Each file is represented by a FileEntry structure with the following information:
- **name**: The name of the file (max 20 characters)
- **size**: The size of the file in bytes
- **block_start**: The starting block address of the file content
- **is_directory**: A flag indicating whether this is a directory

## Data Storage and File Management
The filesystem stores files and directories as entries in a file table with metadata about each entry. The actual file content is stored in data blocks starting after the file table. Each file keeps track of its start block and size, allowing for efficient storage and retrieval.

When files are created or modified, the system ensures proper allocation of space and updates the metadata to maintain consistency. Data persistence is achieved by writing all changes to the block device, allowing the filesystem to be preserved between runs.

## Supported Commands

### Basic Operations (Part A)
- **ls [directory]**: Lists files in the specified directory (defaults to root)
- **cat <path>**: Displays the content of a file
- **touch <path>**: Creates a new empty file
- **touch <path>**: Creates a new empty file
- **edit <path>**: Edits the content of a file
- **rm <path>**: Removes a file
- **help**: Shows the available commands
- **exit**: Exits the simulator

### Directory Operations (Part B - Bonus)
- **mkdir <name>**: Creates a new directory
- **rmdir <name>**: Removes an empty directory
- **mv <src> <dst>**: Renames or moves a file or directory

## Directory Support Implementation
The implementation includes full support for directories (Part B). Directories are implemented using the same FileEntry structure as files, but with the is_directory flag set to true. The system correctly handles:

1. Creating directories with the `mkdir` command
2. Removing empty directories with the `rmdir` command 
3. Moving/renaming files and directories with the `mv` command

The directory support allows for better organization of files within the filesystem while maintaining the same underlying storage structure.
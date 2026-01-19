# 🐧 MyFS – User-Space File System (C++)

MyFS is a user-space file system implementation written in C++, simulating core file system functionality on top of a block device abstraction.

The project implements a minimal virtual file system that supports directories, files, and basic file operations.

---

## 🧠 Overview

The file system is implemented entirely in user space and is backed by a virtual block device stored in a regular file.

The implementation is organized into clear layers:
- 🧱 Block device abstraction
- 📁 Virtual File System (VFS)
- 📂 File system logic and metadata management
- 💻 Command-line interface for user interaction

---

## 📁 Supported Operations

The file system supports the following operations:

- Create and remove files
- Create and remove directories
- List directory contents
- Read from and write to files
- Navigate the directory hierarchy
- Format and initialize a new file system

All operations are executed through a command-line interface.

---

## 🧩 Architecture

The project is structured into the following main components:

- **Block Device (`blkdev`)** 🧱  
  Provides a low-level block-based storage abstraction backed by a file.

- **Virtual File System (`vfs`)** 📁  
  Handles core file system structures such as directories, path resolution, and in-memory representations.

- **MyFS (`myfs`)** 📂  
  Implements file system logic, metadata management, and file operations.

- **Main Program (`myfs_main`)** 💻  
  Provides a command-line interface for interacting with the file system.

---

## 📂 Files

```
.
├── blkdev.h / blkdev.cpp     # Block device abstraction
├── vfs.h / vfs.cpp           # Virtual file system layer
├── myfs.h / myfs.cpp         # File system implementation
├── myfs_main.cpp             # Command-line interface
├── Makefile
└── README.md
```

---

## 🛠️ Build

The project is built using `make`.

```bash
make
```

This produces an executable that can be used to create and interact with the file system.

---

## 🔧 Technologies

- C++
- Linux
- User-space file system design
- Block-based storage abstraction
- Makefile-based build system

---

## Author

Yuval Boker

#ifndef __MYFS_H__
#define __MYFS_H__

#include <memory>
#include <vector>
#include <stdint.h>
#include <string>
#include "blkdev.h"

class MyFs {
public:
    // File entry structure
    struct FileEntry {
        char name[20];          // Name of the file (max 20 chars)
        uint32_t size;          // Size of the file in bytes
        uint32_t block_start;   // Starting block of the file
        bool is_directory;      // Is this a directory
    };

    MyFs(BlockDeviceSimulator *blkdevsim_);

    /**
     * Initializes a new empty filesystem (format).
     */
    void format();

    /**
     * Creates a new file or directory.
     * @param path_str name of the file/directory
     * @param directory true for directory, false for regular file
     */
    void create_file(std::string path_str, bool directory);

    /**
     * Reads the full content of a file.
     * @param path_str name of the file
     * @return content as string
     */
    std::string get_content(std::string path_str);

    /**
     * Writes (overwrites) content to a file.
     * @param path_str name of the file
     * @param content the content to write
     */
    void set_content(std::string path_str, std::string content);

    /**
     * Lists all files (in root or a flat path).
     * @param path_str optional path (only "/" supported in Part A)
     * @return vector of FileEntry
     */
    std::vector<FileEntry> list_dir(std::string path_str);

    /**
     * Deletes a file or directory entry.
     * @param path_str the name of the file/directory
     */
    void remove_file(std::string path_str);

    /**
     * Finds a file/directory by name (used internally or by VFS).
     * @param filename the name to search
     * @return pointer to FileEntry if exists, else nullptr
     */
    FileEntry* find_file(std::string filename);

    /**
     * Saves all file entries and metadata to disk.
     */
    void save_file_entries();

private:
    struct myfs_header {
        char magic[4];
        uint8_t version;
    };

    struct fs_metadata {
        uint32_t num_files;
    };

    void load_file_entries(); // only used internally

    // Filesystem layout constants
    static const uint32_t HEADER_SIZE = sizeof(myfs_header);
    static const uint32_t METADATA_OFFSET = HEADER_SIZE;
    static const uint32_t METADATA_SIZE = sizeof(fs_metadata);
    static const uint32_t FILE_ENTRY_OFFSET = METADATA_OFFSET + METADATA_SIZE;
    static const uint32_t MAX_FILES = 100;
    static const uint32_t FILE_ENTRY_SIZE = sizeof(FileEntry);
    static const uint32_t FILE_TABLE_SIZE = MAX_FILES * FILE_ENTRY_SIZE;
    static const uint32_t DATA_START = FILE_ENTRY_OFFSET + FILE_TABLE_SIZE;

    BlockDeviceSimulator *blkdevsim;
    std::vector<FileEntry> file_entries;
    fs_metadata metadata;

    static const uint8_t CURR_VERSION;
    static const char *MYFS_MAGIC;
};

#endif // __MYFS_H__

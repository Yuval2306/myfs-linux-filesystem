#include "myfs.h"
#include <string.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <algorithm>

const char *MyFs::MYFS_MAGIC = "MYFS";
const uint8_t MyFs::CURR_VERSION = 1;

/**
 * Constructor for the MyFs class
 * Initializes the filesystem and checks if a valid filesystem exists on the block device
 * If not, formats the device to create a new filesystem
 */
MyFs::MyFs(BlockDeviceSimulator *blkdevsim_):blkdevsim(blkdevsim_) {
    myfs_header header;
    blkdevsim->read(0, sizeof(header), (char *)&header);

    if (strncmp(header.magic, MYFS_MAGIC, sizeof(header.magic)) != 0 ||
        (header.version != CURR_VERSION)) {
        std::cout << "Did not find myfs instance on blkdev" << std::endl;
        std::cout << "Creating..." << std::endl;
        format();
        std::cout << "Finished!" << std::endl;
    } else {
        load_file_entries();
    }
}
/**
 * Format method
 * Initializes a new filesystem on the block device
 * Sets up the header, metadata, and empty file table
 */
void MyFs::format() {
    myfs_header header;
    strncpy(header.magic, MYFS_MAGIC, sizeof(header.magic));
    header.version = CURR_VERSION;
    blkdevsim->write(0, sizeof(header), (const char*)&header);

    metadata.num_files = 0;
    blkdevsim->write(METADATA_OFFSET, sizeof(metadata), (const char*)&metadata);

    file_entries.clear();

    save_file_entries();
}
/**
 * Create file method
 * Creates a new file or directory in the filesystem
 * @param path_str The file path
 * @param directory Whether this is a directory (true) or file (false)
 * @throws std::runtime_error if file already exists
 */
void MyFs::create_file(std::string path_str, bool directory) {
    if (find_file(path_str) != nullptr) {
        throw std::runtime_error("File already exists: " + path_str);
    }

    FileEntry entry;
    strncpy(entry.name, path_str.c_str(), sizeof(entry.name) - 1);
    entry.name[sizeof(entry.name) - 1] = '\0'; // ודא סיום מחרוזת
    entry.size = 0;
    entry.is_directory = directory;

    uint32_t next_block_start = DATA_START;
    for (const auto& e : file_entries) {
        uint32_t end = e.block_start + e.size;
        if (end > next_block_start) {
            next_block_start = end;
        }
    }

    if (next_block_start % 4 != 0) {
        next_block_start += (4 - (next_block_start % 4));
    }

    entry.block_start = next_block_start;

    file_entries.push_back(entry);
    metadata.num_files = file_entries.size();

    save_file_entries();
}

/**
 * Get content method
 * Retrieves the content of a file
 * @param path_str The file path
 * @return The content of the file as a string
 * @throws std::runtime_error if file not found or is a directory
 */
std::string MyFs::get_content(std::string path_str) {
    FileEntry* entry = find_file(path_str);
    if (entry == nullptr) {
        throw std::runtime_error("File not found: " + path_str);
    }

    if (entry->is_directory) {
        throw std::runtime_error("Cannot get content of a directory: " + path_str);
    }

    if (entry->size == 0) {
        return "";
    }

    char* buffer = new char[entry->size + 1];
    blkdevsim->read(entry->block_start, entry->size, buffer);
    buffer[entry->size] = '\0'; // Ensure null termination

    std::string content(buffer);
    delete[] buffer;

    return content;
}
/**
 * Set content method
 * Updates the content of a file
 * @param path_str The file path
 * @param content The new content to write
 * @throws std::runtime_error if file not found or is a directory
 */
void MyFs::set_content(std::string path_str, std::string content) {
    FileEntry* entry = find_file(path_str);
    if (entry == nullptr) {
        throw std::runtime_error("File not found: " + path_str);
    }

    if (entry->is_directory) {
        throw std::runtime_error("Cannot set content of a directory: " + path_str);
    }

    blkdevsim->write(entry->block_start, content.size(), content.c_str());

    entry->size = content.size();

    save_file_entries();
}
/**
 * List directory method
 * Returns a list of files in a directory
 * @param path_str The directory path
 * @return Vector of FileEntry objects for files in the directory
 * @throws std::runtime_error if path is not root (in basic implementation)
 */
std::vector<MyFs::FileEntry> MyFs::list_dir(std::string path_str) {
    if (path_str != "/" && path_str != "") {
        throw std::runtime_error("Only root directory is supported in this version");
    }

    return file_entries;
}
/**
 * Remove file method
 * Deletes a file from the filesystem
 * @param path_str The file path to remove
 * @throws std::runtime_error if file not found
 */
void MyFs::remove_file(std::string path_str) {
    auto it = std::find_if(file_entries.begin(), file_entries.end(),
                           [&path_str](const FileEntry& entry) {
                               return strcmp(entry.name, path_str.c_str()) == 0;
                           });

    if (it == file_entries.end()) {
        throw std::runtime_error("File not found: " + path_str);
    }

    file_entries.erase(it);
    metadata.num_files = file_entries.size();

    save_file_entries();
}

// Helper methods
MyFs::FileEntry* MyFs::find_file(std::string filename) {
    for (auto& entry : file_entries) {
        if (strcmp(entry.name, filename.c_str()) == 0) {
            return &entry;
        }
    }
    return nullptr;
}
/**
 * Save file entries helper method
 * Writes the metadata and file entries to the block device
 */
void MyFs::save_file_entries() {
    blkdevsim->write(METADATA_OFFSET, sizeof(metadata), (const char*)&metadata);

    if (!file_entries.empty()) {
        blkdevsim->write(FILE_ENTRY_OFFSET,
                         file_entries.size() * sizeof(FileEntry),
                         (const char*)file_entries.data());
    }
}
/**
 * Load file entries helper method
 * Reads the metadata and file entries from the block device
 */
void MyFs::load_file_entries() {
    blkdevsim->read(METADATA_OFFSET, sizeof(metadata), (char*)&metadata);

    if (metadata.num_files > 0) {
        file_entries.resize(metadata.num_files);
        blkdevsim->read(FILE_ENTRY_OFFSET,
                        metadata.num_files * sizeof(FileEntry),
                        (char*)file_entries.data());
    } else {
        file_entries.clear();
    }
}
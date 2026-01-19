#include "vfs.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>

const std::string FS_NAME = "myfs";
// Command constants
const std::string LIST_CMD = "ls";
const std::string CONTENT_CMD = "cat";
const std::string CREATE_FILE_CMD = "touch";
const std::string EDIT_CMD = "edit";
const std::string REMOVE_CMD = "rm";
const std::string HELP_CMD = "help";
const std::string EXIT_CMD = "exit";
const std::string MKDIR_CMD = "mkdir";
const std::string RMDIR_CMD = "rmdir";
const std::string MV_CMD = "mv";

// Help text showing available commands
const std::string HELP_STRING = "The following commands are supported: \n"
                                + LIST_CMD + " [<directory>] - list directory content. \n"
                                + CONTENT_CMD + " <path> - show file content. \n"
                                + CREATE_FILE_CMD + " <path> - create empty file. \n"
                                + EDIT_CMD + " <path> - re-set file content. \n"
                                + REMOVE_CMD + " <path> - remove file. \n"
                                + MKDIR_CMD + " <name> - create a directory. \n"
                                + RMDIR_CMD + " <name> - remove empty directory. \n"
                                + MV_CMD + " <src> <dst> - rename or move a file or directory. \n"
                                + HELP_CMD + " - show this help message. \n"
                                + EXIT_CMD + " - gracefully exit. \n";

std::vector<std::string> split_cmd(std::string cmd) {
    std::stringstream ss(cmd);
    std::string part;
    std::vector<std::string> ans;

    while (std::getline(ss, part, ' '))
        ans.push_back(part);

    return ans;
}

void run_vfs(MyFs &fs) {
    std::cout << "Welcome to " << FS_NAME << std::endl;
    std::cout << "To get help, please type 'help' on the prompt below." << std::endl;
    std::cout << std::endl;

    bool exit = false;
    while (!exit) {
        try {
            std::string cmdline;
            std::cout << FS_NAME << "$ ";
            std::getline(std::cin, cmdline, '\n');
            if (cmdline.empty())
                continue;

            std::vector<std::string> cmd = split_cmd(cmdline);

            if (cmd[0] == EXIT_CMD) {
                exit = true;
            } else if (cmd[0] == HELP_CMD) {
                std::cout << HELP_STRING;
            } else if (cmd[0] == LIST_CMD) {
                std::string path = "/";
                if (cmd.size() > 1) {
                    path = cmd[1];
                }

                auto files = fs.list_dir(path);
                for (const auto& file : files) {
                    std::cout << file.name << " " << file.size << std::endl;
                }
            } else if (cmd[0] == CREATE_FILE_CMD) {
                if (cmd.size() < 2) {
                    std::cout << "Usage: " << CREATE_FILE_CMD << " <filename>" << std::endl;
                    continue;
                }
                fs.create_file(cmd[1], false);
            } else if (cmd[0] == CONTENT_CMD) {
                if (cmd.size() < 2) {
                    std::cout << "Usage: " << CONTENT_CMD << " <filename>" << std::endl;
                    continue;
                }
                std::string content = fs.get_content(cmd[1]);
                std::cout << content << std::endl;
            } else if (cmd[0] == EDIT_CMD) {
                if (cmd.size() < 2) {
                    std::cout << "Usage: " << EDIT_CMD << " <filename>" << std::endl;
                    continue;
                }

                std::cout << "Enter new file content" << std::endl;
                std::string content;
                std::getline(std::cin, content, '\n');
                fs.set_content(cmd[1], content);
            } else if (cmd[0] == REMOVE_CMD) {
                if (cmd.size() < 2) {
                    std::cout << "Usage: " << REMOVE_CMD << " <filename>" << std::endl;
                    continue;
                }
                fs.remove_file(cmd[1]);
            } else if (cmd[0] == MKDIR_CMD) {
                if (cmd.size() < 2) {
                    std::cout << "Usage: mkdir <dirname>" << std::endl;
                    continue;
                }
                fs.create_file(cmd[1], true);
            } else if (cmd[0] == RMDIR_CMD) {
                if (cmd.size() < 2) {
                    std::cout << "Usage: rmdir <dirname>" << std::endl;
                    continue;
                }

                std::string dirname = cmd[1];
                auto files = fs.list_dir("/");
                bool is_empty = true;
                for (const auto& file : files) {
                    std::string fname(file.name);
                    if (fname != dirname && fname.rfind(dirname + "/", 0) == 0) {
                        is_empty = false;
                        break;
                    }
                }

                if (!is_empty) {
                    std::cout << "Directory is not empty: " << dirname << std::endl;
                } else {
                    fs.remove_file(dirname);
                }
            } else if (cmd[0] == MV_CMD) {
                if (cmd.size() < 3) {
                    std::cout << "Usage: mv <src> <dst>" << std::endl;
                    continue;
                }

                MyFs::FileEntry* entry = fs.find_file(cmd[1]);
                if (entry == nullptr) {
                    std::cout << "File not found: " << cmd[1] << std::endl;
                    continue;
                }

                strncpy(entry->name, cmd[2].c_str(), sizeof(entry->name) - 1);
                entry->name[sizeof(entry->name) - 1] = '\0';

                fs.save_file_entries();
            } else {
                std::cout << "unknown command: " << cmd[0] << std::endl;
            }
        } catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
        }
    }
}

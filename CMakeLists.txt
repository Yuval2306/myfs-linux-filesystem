cmake_minimum_required(VERSION 3.10)
project(myfs)

set(MYFS_HEADERS fs/blkdev.h fs/myfs.h fs/vfs.h)
set(MYFS_SRC_FILES fs/blkdev.cpp fs/myfs.cpp fs/vfs.cpp)
set(MYFS_MAIN_SRC ${MYFS_SRC_FILES} fs/myfs_main.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(myfs ${MYFS_MAIN_SRC})

target_compile_options(myfs PRIVATE -g -Wall)

install(TARGETS myfs DESTINATION bin)
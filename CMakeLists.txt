cmake_minimum_required(VERSION 3.15)
project(EightBall CXX)

file (GLOB_RECURSE EIGHTBALL_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE EIGHTBALL_HEADERS CONFIGURE_DEPENDS "src/*.h")

set (EIGHTBALL_INCLUDE_DIRS "")
foreach (_headerFile ${EIGHTBALL_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND EIGHTBALL_INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES EIGHTBALL_INCLUDE_DIRS)

find_package(ZLIB REQUIRED)

add_executable(${PROJECT_NAME} ${EIGHTBALL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${EIGHTBALL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)

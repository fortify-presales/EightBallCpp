cmake_minimum_required(VERSION 3.15)
project(EightBall CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ZLIB REQUIRED)
find_package(pugixml REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(http_parser REQUIRED)
find_package(json-c REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
    src/Answer.hpp
    src/AnswerXML.hpp
    src/AnswerXML.cpp
    src/AnswerSQL.hpp
    src/AnswerSQL.cpp
    src/Shell.hpp
    src/Shell.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE ${EIGHTBALL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)
target_link_libraries(${PROJECT_NAME} pugixml::pugixml)
target_link_libraries(${PROJECT_NAME} SQLite::SQLite3)
target_link_libraries(${PROJECT_NAME} http_parser::http_parser)
target_link_libraries(${PROJECT_NAME} json-c::json-c)
target_link_libraries(${PROJECT_NAME} openssl::openssl)

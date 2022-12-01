cmake_minimum_required(VERSION 3.11)
set(CMAKE_C_STANDARD 11)
set(
  CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${PROJECT_SOURCE_DIR}/lib CACHE
  PATH "Directory where all the .lib files are dumped." FORCE
)
set(
  CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR} CACHE
  PATH "Directory where .exe and .dll files are dumped." FORCE
)

project(ASCC C)

add_executable(${PROJECT_NAME} src/main.c src/lexer.c)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE)

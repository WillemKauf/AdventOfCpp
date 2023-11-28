cmake_minimum_required (VERSION 3.13)

#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(AdventOfCpp)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE common)

find_package(OpenMP REQUIRED)

target_link_libraries(${PROJECT_NAME}
  crypto
  OpenMP::OpenMP_CXX
  #-fsanitize=address
  ssl)


#Clang-tidy
#find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
#set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*,-std=c++23")
#set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

#target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -g)
target_compile_options(${PROJECT_NAME} PRIVATE -O3)

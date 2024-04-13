cmake_minimum_required(VERSION 3.12)
project(CPPThround)

set(SOURCE_DIR "src")
set(INCLU_DIR "include")

file(GLOB HEADE_FILES ${INCLU_DIR}/*.h)
file(GLOB SOURC_FILES ${SOURCE_DIR}/*.cpp)

add_executable(CPPThround ${TEST_SOURCE_FILES} ${SOURC_FILES})

target_include_directories(CPPThround PUBLIC ${INCLU_DIR})
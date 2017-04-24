cmake_minimum_required(VERSION 3.0)

project(yoba-perl CXX)



### Set up CMake ###
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_VERBOSE_MAKEFILE true)
#set(CMAKE_BUILD_TYPE Debug)

### Build options ###
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

option(MULTIPLICITY "Enable multiple Perl instances support" OFF)
option(TESTS "Build tests" ON)

message("Build type: ${CMAKE_BUILD_TYPE} ( -DCMAKE_BUILD_TYPE=Release or -DCMAKE_BUILD_TYPE=Debug )")
message("Multiple Perl instances: ${MULTIPLICITY} ( -DMULTIPLICITY=ON or -DMULTIPLICITY=OFF )")
message("Building tests: ${TESTS} ( -DTESTS=ON or -DTESTS=OFF )")

if(MULTIPLICITY)
   add_definitions(-DYOBAPERL_MULTIPLICITY)
endif()

if(TESTS)
   subdirs(${CMAKE_SOURCE_DIR}/tests)
endif()

if(CMAKE_BUILD_TYPE EQUAL "Debug")
   add_definitions(-DYOBAPERL_DEBUG -DDEBUGGING)
endif()

# Disable caching
unset(MULTIPLICITY CACHE)
unset(TESTS CACHE)



### Check C++11 support ###
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
CHECK_CXX_COMPILER_FLAG("-Wall" COMPILER_SUPPORTS_WALL)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

if(COMPILER_SUPPORTS_WALL)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()



### Check Perl headers and libs ###
find_package(Perl REQUIRED)
find_package(PerlLibs REQUIRED)
find_package(PerlModules COMPONENTS ExtUtils::Embed REQUIRED)



### Compiler flags ###
execute_process(COMMAND ${PERL_EXECUTABLE} -MExtUtils::Embed -e ccopts OUTPUT_VARIABLE PERL_COMPILER_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${PERL_EXECUTABLE} -MExtUtils::Embed -e ldopts OUTPUT_VARIABLE PERL_LINKER_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PERL_COMPILER_FLAGS} -Wno-literal-suffix")
#set(CMAKE_SHARED_LINKER_FLAGS "${PERL_LINKER_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PERL_LINKER_FLAGS}")



### Build main shared library ###
message("Generating ppport.h")
execute_process(COMMAND ${PERL_EXECUTABLE} -mDevel::PPPort -e Devel::PPPort::WriteFile WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src)
message("Generating clear_namespace.hpp")
execute_process(COMMAND ${PERL_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/clear_namespace.pl WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${PERL_LINKER_FLAGS})

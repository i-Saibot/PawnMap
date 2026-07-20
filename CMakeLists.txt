cmake_minimum_required(VERSION 3.10)
project(PawnMap)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT lto_error)
if(lto_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -fPIC -O3 -w -DLINUX -fno-plt -fpermissive")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32 -static-libgcc -static-libstdc++")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/samp-sdk
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/samp-sdk/amx
)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE SDK_FILES "lib/samp-sdk/*.cpp")

set(SOURCES ${SRC_FILES} ${SDK_FILES})

message(STATUS "--- Found sources: ${SOURCES}")

if(NOT SOURCES)
    message(FATAL_ERROR "SOURCES list is empty! Check src/ and lib/samp-sdk/ folders...")
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Release"
)
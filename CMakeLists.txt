cmake_minimum_required(VERSION 3.10)
project(YADA VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Add debug flag
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

# Add source files
set(SOURCES
    src/main.cpp
    src/user/user.cpp
    src/food/food.cpp
    src/food/basic_food.cpp
    src/food/composite_food.cpp
    src/database/database.cpp
    src/logger/logger.cpp
    src/utils/utils.cpp
)

# Add header files
set(HEADERS
    include/user/user.h
    include/food/food.h
    include/food/basic_food.h
    include/food/composite_food.h
    include/database/database.h
    include/logger/logger.h
    include/utils/utils.h
)

# Create executable
add_executable(yada ${SOURCES} ${HEADERS})

# Add include directories
target_include_directories(yada PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Link against OpenSSL libraries
target_link_libraries(yada PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Add compiler warnings
if(MSVC)
    target_compile_options(yada PRIVATE /W4)
else()
    target_compile_options(yada PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Enable debug mode by default
target_compile_definitions(yada PRIVATE DEBUG) 
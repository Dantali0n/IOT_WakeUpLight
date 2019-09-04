CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(GamHaTests)

IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    MESSAGE(FATAL_ERROR "Please create a dedicated build directory for this load balancer. (You may need remove the CmakeCache.txt and the cache)")
ENDIF()

# Add custom cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(macros)

# Enable c++11 even when cmake < 3.1
use_cxx11()

# Enable permissive c++
#use_cxx_permissive()
use_cxx_warning_pedantic()

# Download thirdparty dependencies & include them
message("Downloading thirdparty dependencies")
file(DOWNLOAD https://github.com/catchorg/Catch2/releases/download/v2.6.1/catch.hpp ${CMAKE_SOURCE_DIR}/thirdparty/catch.hpp
	SHOW_PROGRESS TLS_VERIFY on
	EXPECTED_HASH SHA512=d0dbbdaf817c43e756d3442961f7972ca984776adab33654a4056bd4f8188ee9dbc37c9a5c3db82b9b7f7e9e93fca94d4768ecf901687db873ecc17f57786ade
)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty)

# Add test directory & enable tests
ENABLE_TESTING()
add_subdirectory(test)
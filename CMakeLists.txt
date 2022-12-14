cmake_minimum_required(VERSION 3.23)
project(Bargain_Hunting C)

add_definitions( -DCURL_STATICLIB )

set(CMAKE_C_STANDARD 99)

#link curl with Bargain_Hunting
set( CURL_LIBRARY ${CMAKE_SOURCE_DIR}/lib )
set( CURL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include )
include_directories( ${CMAKE_SOURCE_DIR}/include )
link_directories(${CMAKE_SOURCE_DIR}/lib/libcurl.dll.a)
FIND_PACKAGE(CURL REQUIRED)

add_executable(Bargain_Hunting main.c scraper.c scraper.h)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
target_link_libraries(Bargain_Hunting ${CMAKE_SOURCE_DIR}/lib/libcurl.dll.a)

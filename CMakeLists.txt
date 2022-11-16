cmake_minimum_required(VERSION 3.23)
project(Bargain_Hunting C)


FIND_PACKAGE(CURL)
#FIND_PACKAGE(CURL)
set(CMAKE_C_STANDARD 99)

#link curl with Bargain_Hunting
add_executable(Bargain_Hunting scraper.c scraper.h)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
target_link_libraries(Bargain_Hunting ${CURL_LIBRARIES})




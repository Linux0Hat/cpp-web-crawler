cmake_minimum_required(VERSION 3.0.0)

project(web-crawler VERSION 0.1.0 LANGUAGES C CXX)

find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
add_executable(web-crawler ${SOURCES})

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/SQLiteCpp)

target_include_directories(web-crawler PRIVATE ${PROJECT_SOURCE_DIR}/libs/SQLiteCpp/include)
target_link_libraries(web-crawler SQLiteCpp sqlite3 dl ${CURL_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

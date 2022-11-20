cmake_minimum_required(VERSION 3.11.3)
project(CapStone VERSION 0.1.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

include(CTest)
enable_testing()

#set(OpenCV_DIR "/mnt/c/c/dev/vcpkg/installed/x64-windows/share/opencv4")
#find_package(OpenCV CONFIG REQUIRED)
find_package(OpenCV 4.1 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

find_package(unofficial-curlpp CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

file(GLOB project_SRCS src/*.cpp) 
#file(GLOB project_SRCS src/*.h)
add_executable(CapStone ${project_SRCS})
target_link_libraries(CapStone ${OpenCV_LIBRARIES})
target_link_libraries(CapStone unofficial::curlpp::curlpp)
target_link_libraries(CapStone jsoncpp_object jsoncpp_static JsonCpp::JsonCpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

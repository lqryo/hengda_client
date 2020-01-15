cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(Border)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/;${CMAKE_MODULE_PATH}")   
 
set(CMAKE_CXX_STANDARD 14)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /wd4819 /wd4828 /utf-8")   
	add_definitions(-DWIN32_LEAN_AND_MEAN)
 	add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES) 
	# Use the static C library for all build types
    foreach(var 
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO 
      )
      if(${var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
      endif()
    endforeach()  
endif(MSVC) 

################################################################   
## OpenCV
#find_package(OpenCV REQUIRED) 
#include_directories(${OpenCV_INCLUDE_DIRS})

################################################################      
## glog
# find_package(Glog REQUIRED) 
# set(GLOG_INCLUDE_DIRS "D:/develop/glog/Libs/include")
# set(GLOG_LIBRARIES "D:/develop/glog/Libs/lib/glog.lib;D:/develop/glog/Libs/lib/glogd.lib")
# include_directories(${GLOG_INCLUDE_DIRS})

################################################################      
## Qt 
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)  

find_package(Qt5 COMPONENTS Core Gui Widgets Network CONFIG REQUIRED)

################################################################  
## mysql
find_package(MySQL REQUIRED)
include_directories(${MySQL_INCLUDE_DIRS})

################################################################  

#################################################################
## co
include_directories("D:/develop/co/include")  

#################################################################

include_directories(3rdparty)

# Source files
file(GLOB_RECURSE GUI_FILES ./gui/*.h ./gui/*.cpp ./gui/widgets/*.h ./gui/widgets/*.cpp) 
include_directories(gui)
include_directories(gui/widgets)

file(GLOB_RECURSE MODEL_FILES ./model/*.h ./model/*.cpp)  
include_directories(model) 

file(GLOB_RECURSE DB_FILES ./db/*.h  ./db/*.cpp)  
include_directories(db)

file(GLOB_RECURSE NETWORK_FILES ./network/*.h ./network/*.cpp) 
include_directories(network)

add_executable(${PROJECT_NAME} main.cpp ${GUI_FILES} ${MODEL_FILES} ${DB_FILES} ${NETWORK_FILES} border.qrc system.h)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)

target_link_libraries(${PROJECT_NAME} ${MySQL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} "D:/develop/co/lib/base.lib")
target_link_libraries(${PROJECT_NAME} "iphlpapi.lib")
target_link_libraries(${PROJECT_NAME} "WS2_32.lib")

file(COPY ${CMAKE_SOURCE_DIR}/setting.json DESTINATION ${CMAKE_BINARY_DIR})  
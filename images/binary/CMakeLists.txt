cmake_minimum_required(VERSION 2.8)

set ( PROJECT_NAME binary )
set ( SRC_LIST binary.cpp )

INCLUDE_DIRECTORIES(/usr/include/opencv)
INCLUDE_DIRECTORIES(/usr/include/opencv2)
INCLUDE_DIRECTORIES(/usr/include)
INCLUDE_DIRECTORIES(/usr/local/include)

LINK_DIRECTORIES(/usr/lib)
LINK_DIRECTORIES(/usr/lib64)
LINK_DIRECTORIES(/usr/local/lib)

add_executable( ${PROJECT_NAME} ${SRC_LIST} )

target_link_libraries( ${PROJECT_NAME} boost_system )
target_link_libraries( ${PROJECT_NAME} boost_filesystem )
target_link_libraries( ${PROJECT_NAME} boost_program_options )
target_link_libraries( ${PROJECT_NAME} opencv_core )
target_link_libraries( ${PROJECT_NAME} opencv_highgui )
target_link_libraries( ${PROJECT_NAME} opencv_imgproc )

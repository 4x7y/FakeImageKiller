# Edited by Xue Yuechuan @ 2015.5.15

PROJECT( FakeImageKiller )
CMAKE_MINIMUM_REQUIRED( VERSION 2.6)
AUX_SOURCE_DIRECTORY( ./src DIR_SRCS)
INCLUDE_DIRECTORIES(./include)
FIND_PACKAGE ( OpenCV REQUIRED )
# ADD_SUBDIRECTORY ( src )
ADD_EXECUTABLE( FakeImageKiller ${DIR_SRCS} include/tempered_image_parser.h)
TARGET_LINK_LIBRARIES( FakeImageKiller ${OpenCV_LIBS} )


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


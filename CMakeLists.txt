cmake_minimum_required( VERSION 3.14 )
project( QtGLTest )

find_package( Qt5 COMPONENTS Core Widgets OpenGL REQUIRED )
set( CMAKE_AUTOMOC ON )

set(
	PROJECT_FILES
	src/main.cc
	src/QtGLTestMainWindow.cc
	src/GLTestWidget.cc
)

set( CMAKE_CXX_STANDARD 17 )

add_executable( QtGLTest ${PROJECT_FILES} )
target_link_libraries( QtGLTest Qt5::Core Qt5::Widgets Qt5::OpenGL )

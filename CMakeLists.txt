project(style)

cmake_minimum_required (VERSION 3.8.0)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set (QT_MIN_VERSION $ENV{REZ_QT_VERSION})
set (BUILD_SHARED_LIBS ON)

set(protobuf_VERBOSE ON)
set(protobuf_MODULE_COMPATIBLE ON)
set(Boost_NO_BOOST_CMAKE ON)

find_package (Boost ${REZ_BOOST_VERSION} EXACT COMPONENTS program_options filesystem date_time system chrono)
find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
  Core
  Widgets
  Xml
  XmlPatterns
  )

include_directories (
  ${Boost_INCLUDE_DIR}
  )

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

set (SERVER_UI_FILES mainwindow.ui)

add_definitions("-DSTYLESHEET_FILE=\"${CMAKE_SOURCE_DIR}/stylesheet.css\"")
add_executable (style main.cc mainwindow.cc)


target_link_libraries (style
  Boost::filesystem
  Boost::chrono
  Boost::system
  Boost::date_time
  Boost::program_options
  Qt5::Widgets Qt5::Xml Qt5::XmlPatterns
  )


cmake_minimum_required(VERSION 3.5)
project(pricerightnlp)

include_directories(/usr/local/include src/matplotlib-cpp src/json)

set(SOURCE_FILES src/main.cpp src/csv/csv.h src/traincrf.cpp src/traincrf.h src/testcrf.cpp src/VerticalProfiles.cpp src/VerticalProfiles.h src/testcrf.h src/visualize.cpp src/visualize.h src/json/json.h src/matplotlib-cpp/matplotlibcpp.h)
add_executable(pricerightnlp ${SOURCE_FILES})

target_link_libraries(pricerightnlp python2.7 lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

set_property(TARGET pricerightnlp PROPERTY CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow")


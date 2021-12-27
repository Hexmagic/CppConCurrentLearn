project(learn)
cmake_minimum_required(VERSION 3.17)

find_package(OpenCV REQUIRED)
include_directories(
    深度网络/include
)
add_executable(doc project/doc.cpp)
target_link_libraries(doc ${OpenCV_LIBS})

add_executable(tmp project/tmp.cpp)
target_link_libraries(tmp ${OpenCV_LIBS})

add_executable(yolov3-test project/yolov3-test.cpp)
target_link_libraries(yolov3-test ${OpenCV_LIBS})

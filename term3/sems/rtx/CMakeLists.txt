# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(MyProject)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем пути к исходникам и заголовкам
include_directories(include)
include_directories(src)

#собираем геометрию
add_library(geometry STATIC src/vec3.cpp src/ray.cpp)
#визуал тоже, че нам

add_executable(RTX src/main.cpp src/camera.cpp src/ray.cpp src/scene.cpp src/vec3.cpp)





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
add_executable(RTX src/main.cpp)
target_sources(RTX PUBLIC src/vec3.cpp)

# Либо используйте команду для указания нескольких файлов:
# add_executable(MyProject src/main.cpp src/other.cpp)
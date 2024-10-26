cmake_minimum_required(VERSION 3.20)    # Проверка версии CMake.
									    # Если версия установленой программы
									    # старее указаной, произойдёт аварийный выход.

add_executable(main main.cpp)		    # Создает исполняемый файл с именем main
									    # из исходника main.cpp
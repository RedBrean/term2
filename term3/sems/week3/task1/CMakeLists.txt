cmake_minimum_required(VERSION 3.20)	 # Проверка версии CMake.
										# Если версия установленой программы
										# старее указаной, произайдёт аварийный выход.

project(Cheta)			# Название проекта

set(SOURCE_EXE main.cpp utils.cpp)		# Установка переменной со списком исходников для исполняемого файла

add_executable(Cheta ${SOURCE_EXE})	# Создает исполняемый файл с именем main

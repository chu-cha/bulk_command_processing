cmake_minimum_required(VERSION 3.12)
project(bulk)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка выходных каталогов ДО определения целей
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Windows-специфичные настройки
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Опция для типа библиотеки
option(BUILD_ASYNC_AS_SHARED "Build async as shared library" ON)

# Исходники библиотеки
file(GLOB ASYNC_SOURCES "src/*.cpp")
file(GLOB ASYNC_HEADERS "include/*.h")

# Создание библиотеки
if(BUILD_ASYNC_AS_SHARED)
    add_library(async SHARED ${ASYNC_HEADERS} ${ASYNC_SOURCES})
else()
    add_library(async STATIC ${ASYNC_HEADERS} ${ASYNC_SOURCES})
endif()

# Заголовки
target_include_directories(async PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Явное указание выходных каталогов для каждой цели
set_target_properties(async PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Исполняемый файл
add_executable(bulk main.cpp generate_tests.h generate_tests.cpp)
set_target_properties(bulk PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Связывание
target_link_libraries(bulk PRIVATE async)

# Установка
install(TARGETS async
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/async
)

install(TARGETS bulk RUNTIME DESTINATION bin)
install(FILES ${ASYNC_HEADERS} DESTINATION include/async)

# Настройки CPack
set(CPACK_GENERATOR "DEB") 
set(CPACK_PACKAGE_NAME "bulk")
set(CPACK_PACKAGE_FILE_NAME "bulk")  
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "t.o.nikolaeva@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Bulk command processor")

# Отключаем ненужные генераторы
set(CPACK_SOURCE_GENERATOR "")  # Отключаем генерацию исходников
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)

include(CPack)
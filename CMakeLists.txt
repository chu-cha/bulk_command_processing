
cmake_minimum_required(VERSION 3.5)

project(bulk VERSION 0.1.1) 

add_executable(bulk 
	main.cpp
	command_processor.cpp
    	command_processor.h
	generate_tests.h
	generate_tests.cpp
	thread_safe_queue.h
	thread_safe_queue.cpp )

set (ALL_SOURCES bulk)

set_target_properties(${ALL_SOURCES} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

foreach(s ${ALL_SOURCES})
	if (MSVC)
		target_compile_options(${s} PRIVATE
			/W4
		)
		
	else ()
		target_compile_options(${s} PRIVATE
			-Wall 
		)
	endif()
endforeach()

install(TARGETS bulk RUNTIME DESTINATION bin)

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



cmake_minimum_required(VERSION 3.5)

project(bulk VERSION 0.1.1) 

add_executable(bulk 
	main.cpp
	command_processor.cpp
    	command_processor.h
	generate_tests.h
	generate_tests.cpp )

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
set(CPACK_PACKAGE_FILE_NAME "bulk.deb")  
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "TK")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Bulk command processor")

# Для DEB пакетов (если нужно)
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")  # Использует только имя без версии

include(CPack)


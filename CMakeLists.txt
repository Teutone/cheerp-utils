cmake_minimum_required (VERSION 2.6)
project (Cheerp_Utils)

set (CHEERP_PREFIX ${CMAKE_INSTALL_PREFIX} CACHE FILEPATH "set cheerp installation prefix")

# scripts
configure_file (
	"scripts/cheerpwrap.in"
	"${PROJECT_BINARY_DIR}/cheerpwrap"
	)

install (PROGRAMS "${PROJECT_BINARY_DIR}/cheerpwrap"
	DESTINATION bin)
install (PROGRAMS
		scripts/cheerp-unknown-none-ar
		scripts/cheerp-unknown-none-g++
		scripts/cheerp-unknown-none-gcc
		scripts/cheerp-unknown-none-ranlib
	DESTINATION libexec )
	
# tools
SET(toolchain_files
	"tools/CheerpToolchainCommon.cmake.in"
	"tools/CheerpToolchain.cmake.in"
	"tools/CheerpWasmToolchain.cmake.in"
	)
foreach(toolchain_file ${toolchain_files})
	get_filename_component(base ${toolchain_file} NAME_WE)
	configure_file (
		${toolchain_file}
		"${PROJECT_BINARY_DIR}/${base}.cmake"
		)
	install (FILES "${PROJECT_BINARY_DIR}/${base}.cmake"
			DESTINATION share/cmake/Modules/)
endforeach()
SET(platform_files
	"tools/CheerpCommon.cmake.in"
	"tools/Cheerp.cmake.in"
	"tools/CheerpWasm.cmake.in"
	)
foreach(platform_file ${platform_files})
	get_filename_component(base ${platform_file} NAME_WE)
	configure_file (
		${platform_file}
		"${PROJECT_BINARY_DIR}/${base}.cmake"
		)
	install (FILES "${PROJECT_BINARY_DIR}/${base}.cmake"
			DESTINATION share/cmake/Modules/Platform)
endforeach()

# tests
install (FILES tests/dom/dom1.cpp tests/dom/test1.html
	DESTINATION share/cheerp/examples/dom )

install (FILES tests/webgl/webgl.cpp tests/webgl/webgl.html
	DESTINATION share/cheerp/examples/webgl )

# include
install (DIRECTORY include/client/
	DESTINATION include/client/cheerp )

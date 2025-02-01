cmake_minimum_required(VERSION 3.24)

project(md_prsr VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option (BUILD_TESTS "build tests" ON)
option (BUILD_TOOLS "build tools" OFF)
option (BUILD_NASDAQ_ITCH_V5_0 "build nasdaq/itch_v5.0" ON)
option (BUILD_NASDAQ_MOLDUDP64_V1_0 "build nasdaq/moldudp64_v1.0" ON)

if(BUILD_NASDAQ_ITCH_V5_0)
    add_subdirectory(nasdaq/itch_v5.0)
endif (BUILD_NASDAQ_ITCH_V5_0)

if(BUILD_NASDAQ_MOLDUDP64_V1_0)
    add_subdirectory(nasdaq/moldudp64_v1.0)
endif (BUILD_NASDAQ_MOLDUDP64_V1_0)

if(BUILD_TESTS)
  include(CTest)
  enable_testing()
  if(BUILD_NASDAQ_ITCH_V5_0) 
    add_subdirectory(test/nasdaq_itch_v5.0)
  endif (BUILD_NASDAQ_ITCH_V5_0)    
endif (BUILD_TESTS)

if(BUILD_TOOLS)
  if(BUILD_NASDAQ_ITCH_V5_0) 
	add_subdirectory(tools/nasdaq_itch_v5.0)
   endif (BUILD_NASDAQ_ITCH_V5_0)
endif (BUILD_TOOLS)


include(CMakePackageConfigHelpers)

write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
		VERSION ${CMAKE_PROJECT_VERSION}
		COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/config.cmake.in
							${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
							INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(GNUInstallDirs)
install(FILES 
			${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
#			${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
		DESTINATION
            ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
		    #${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)


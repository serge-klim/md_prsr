find_package(Boost REQUIRED )

add_library(test_helpers INTERFACE)

target_sources(test_helpers PUBLIC FILE_SET HEADERS
				FILES
				cmp.hpp
			   )

target_include_directories(test_helpers INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		)


target_link_libraries(test_helpers
		INTERFACE 
		Boost::boost
)


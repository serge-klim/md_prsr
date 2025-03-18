find_package(Boost REQUIRED )
find_package(transcoder CONFIG REQUIRED)

add_library(nasdaq_moldudp64_v1_0 INTERFACE)

add_library (md_prsr::nasdaq_moldudp64_v1_0 ALIAS nasdaq_moldudp64_v1_0)

target_sources(nasdaq_moldudp64_v1_0 PUBLIC FILE_SET HEADERS
				FILES
				messages.hpp
				describe.hpp
			   )

target_link_libraries(nasdaq_moldudp64_v1_0
		INTERFACE 
		transcoder::transcoder
		Boost::boost
)

target_include_directories(nasdaq_moldudp64_v1_0 INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)

include(GNUInstallDirs)

install(TARGETS nasdaq_moldudp64_v1_0
        EXPORT nasdaq_moldudp64_v1_0-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/md_prsr/nasdaq/moldudp64_v1.0
)

install(EXPORT nasdaq_moldudp64_v1_0-targets
		FILE nasdaq_moldudp64_v1_0-targets.cmake
		NAMESPACE md_prsr::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)





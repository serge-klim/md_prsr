find_package(Boost REQUIRED )
find_package(transcoder CONFIG REQUIRED)

add_library(eobi_v13_1 INTERFACE)

add_library(md_prsr::eobi_v13_1 ALIAS eobi_v13_1)

target_sources(eobi_v13_1 PUBLIC FILE_SET HEADERS
				FILES
				messages.hpp
				describe.hpp
			   )

target_link_libraries(eobi_v13_1
		INTERFACE 
		transcoder::transcoder
		Boost::boost
)

target_include_directories(eobi_v13_1 INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)

include(GNUInstallDirs)

install(TARGETS eobi_v13_1
        EXPORT eobi_v13_1-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/md_prsr/v13_1
)

install(EXPORT eobi_v13_1-targets
		FILE eobi_v13_1-targets.cmake
		NAMESPACE md_prsr::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)





find_package(Boost REQUIRED )
find_package(transcoder CONFIG REQUIRED)

add_library(mdp3_techical_headers INTERFACE)

add_library (md_prsr::mdp3_techical_headers ALIAS mdp3_techical_headers)

target_sources(mdp3_techical_headers PUBLIC FILE_SET HEADERS
				FILES
				messages.hpp
				describe.hpp
			   )

target_link_libraries(mdp3_techical_headers
		INTERFACE 
		transcoder::transcoder
		Boost::boost
)

target_include_directories(mdp3_techical_headers INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)

include(GNUInstallDirs)

install(TARGETS mdp3_techical_headers
        EXPORT mdp3_techical_headers-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/md_prsr/cme/mdp3/techical_headers
)

install(EXPORT mdp3_techical_headers-targets
		FILE mdp3_techical_headers-targets.cmake
		NAMESPACE md_prsr::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)





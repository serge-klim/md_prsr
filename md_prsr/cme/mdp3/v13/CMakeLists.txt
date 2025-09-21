find_package(Boost REQUIRED )
find_package(transcoder CONFIG REQUIRED)

add_library(cme_mdp3_v13 INTERFACE)

add_library (md_prsr::cme_mdp3_v13 ALIAS cme_mdp3_v13)

target_sources(cme_mdp3_v13 PUBLIC FILE_SET HEADERS
				FILES
				messages.hpp
				describe.hpp
			   )

target_link_libraries(cme_mdp3_v13
		INTERFACE 
		md_prsr::cme_mdp3_sbe
		transcoder::transcoder
		Boost::boost
)

target_include_directories(cme_mdp3_v13 INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)

include(GNUInstallDirs)

install(TARGETS cme_mdp3_v13
        EXPORT cme_mdp3_v13-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/md_prsr/cme/mdp3/v13
)

install(EXPORT cme_mdp3_v13-targets
		FILE cme_mdp3_v13-targets.cmake
		NAMESPACE md_prsr::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)





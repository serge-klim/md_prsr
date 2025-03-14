find_package(Boost REQUIRED )
find_package(transcoder CONFIG REQUIRED)

add_library(cme_mdp3_v9 INTERFACE)

add_library (md_prsr::cme_mdp3_v9 ALIAS cme_mdp3_v9)

target_sources(cme_mdp3_v9 PUBLIC FILE_SET HEADERS
				FILES
				v9/messages.hpp
				v9/describe.hpp
				techical_header/messages.hpp
				techical_header/describe.hpp
				sbe.hpp
			   )

target_link_libraries(cme_mdp3_v9
		INTERFACE 
		transcoder::transcoder
		Boost::boost
)

target_include_directories(cme_mdp3_v9 INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)

include(GNUInstallDirs)

install(TARGETS cme_mdp3_v9
        EXPORT cme_mdp3_v9-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/md_prsr/cme/mdp3/v9
)

install(EXPORT cme_mdp3_v9-targets
		FILE cme_mdp3_v9-targets.cmake
		NAMESPACE md_prsr::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)





find_package(Boost REQUIRED )
find_package(transcoder CONFIG REQUIRED)

add_library(nasdaq_itch_v5_0 INTERFACE)

add_library (md_prsr::nasdaq_itch_v5_0 ALIAS nasdaq_itch_v5_0)

target_sources(nasdaq_itch_v5_0 PUBLIC FILE_SET HEADERS
				FILES
				transcoder/timestamp.hpp
				messages.hpp
				describe.hpp
				message_id.hpp
				transcoder.hpp
				timestamp.hpp
				traits.hpp
				io.hpp
			   )

target_link_libraries(nasdaq_itch_v5_0
		INTERFACE 
		transcoder::transcoder
		Boost::boost
)

target_include_directories(nasdaq_itch_v5_0 INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)

include(GNUInstallDirs)

install(TARGETS nasdaq_itch_v5_0
        EXPORT nasdaq_itch_v5_0-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/md_prsr/nasdaq/itch_v5_0
)

install(EXPORT nasdaq_itch_v5_0-targets
		FILE nasdaq_itch_v5_0-targets.cmake
		NAMESPACE md_prsr::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)





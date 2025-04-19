find_package(Boost COMPONENTS unit_test_framework REQUIRED )
find_package(transcoder CONFIG REQUIRED)

add_executable (nasdaq_itch_v5_0_test
		nasdaq_itch_v5.0_test.cpp
)

target_link_libraries(nasdaq_itch_v5_0_test
		md_prsr::nasdaq_itch_v5_0
		transcoder::transcoder        
		test_helpers
	    Boost::disable_autolinking
        Boost::boost
		Boost::unit_test_framework
)

target_compile_options(nasdaq_itch_v5_0_test PRIVATE
     $<$<CXX_COMPILER_ID:MSVC>:/bigobj>
)

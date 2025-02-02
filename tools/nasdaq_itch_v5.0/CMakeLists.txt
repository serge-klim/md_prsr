#set(Boost_USE_STATIC_LIBS ON)
include(FetchContent)

FetchContent_Declare(
  fb_utils
  GIT_REPOSITORY https://github.com/serge-klim/fb_utils.git
  GIT_TAG        f86e57f3dad01f6f6b2e6f533a53c8f8efc17d83
)

FetchContent_MakeAvailable(fb_utils)

if (WIN32)
   find_package(zstd REQUIRED CONFIG)
   add_library(zstd ALIAS zstd::libzstd_static)
   find_package(liblzma REQUIRED CONFIG)
   add_library(lzma ALIAS liblzma::liblzma)
endif (WIN32)

find_package(Boost COMPONENTS program_options iostreams log log_setup REQUIRED )
find_package(transcoder CONFIG REQUIRED)
find_package(hdf5_blosc CONFIG)
find_package(h5x CONFIG REQUIRED)

add_executable (nasdaq_itch_v5_0_tool
                main.cpp
                reader.cpp
                message_counter.cpp
                message_counter.hpp
                h5x/timestamp.hpp
                hdf5_writer.cpp
                hdf5_writer.hpp
                loggers.hpp
                loggers.cpp
                message_dump.hpp
                message_dump.cpp
                )

target_link_libraries(nasdaq_itch_v5_0_tool
		transcoder::transcoder
        md_prsr::nasdaq_itch_v5_0
        unofficial::hdf5_blosc
        h5x::h5x
        fb::fb_utils::program_options
        fb::fb_utils::program_options::log
        Boost::boost
        Boost::program_options
        Boost::iostreams
        Boost::log
        Boost::log_setup
)


target_compile_options(nasdaq_itch_v5_0_tool PRIVATE
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /EHsc /D_CRT_SECURE_NO_WARNINGS>
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-string-conversion -Wno-unsafe-buffer-usage>
)


cmake_minimum_required(VERSION 4.0)

add_library(MediaSession
        importer.cpp
        importer.hpp
        exporter.cpp
        exporter.hpp
        movie_reader.cpp
        movie_reader.hpp
        movie_writer.cpp
        movie_writer.hpp
        ffmpeg_helpers.cpp
        ffmpeg_helpers.hpp
        freelist.hpp
        sample_cache.hpp
)

target_include_directories(MediaSession
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

find_package(FFMPEG REQUIRED)
target_link_libraries(MediaSession
    PUBLIC
        ${FFMPEG_LIBRARIES}
    PRIVATE
        BaseCodec
        HapCodec
	# need this for ffmpeg on windows; TODO: get rid of it
        $<$<PLATFORM_ID:Windows>:bcrypt>
)

cmake_minimum_required(VERSION 3.15)

project(simplewebm)

add_library(simplewebm)

target_sources(simplewebm
    PRIVATE
        OpusVorbisDecoder.cpp
        WebMDemuxer.cpp
        VPXDecoder.cpp
)

find_library(WEBM_LIBRARY NAMES webm)
# HACK: Hardcoded path to webm include directory
set(WEBM_INCLUDE_DIR /usr/include/webm /usr/local/include/webm /opt/local/include/webm)

target_include_directories(simplewebm
    PUBLIC
    ${WEBM_INCLUDE_DIR}
)

target_link_directories(simplewebm
    PUBLIC
    ${WEBM_LIBRARY}
)

target_link_libraries(simplewebm
    PUBLIC
        opus
        vorbis
        vpx
        webm
)

option(SIMPLEWEBM_EXAMPLES "Build examples" OFF)
if(SIMPLEWEBM_EXAMPLES)
    add_executable(simplewebm-example example.cpp)
    target_link_libraries(simplewebm-example
        PRIVATE
            simplewebm
    )
endif()

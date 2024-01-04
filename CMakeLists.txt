cmake_minimum_required(VERSION 3.27)
project(midi_bench)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(midifile)
add_subdirectory(nanobench)
add_subdirectory(symusic)
include_directories(symusic/include symusic/3rdparty/pdqsort)

add_executable(midi_bench main.cpp)
target_link_libraries(midi_bench midifile nanobench symusic)

# add LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_target_properties(midi_bench PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
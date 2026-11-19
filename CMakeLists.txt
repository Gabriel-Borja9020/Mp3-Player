
cmake_minimum_required(VERSION 4.4)
project(Musica_CPP LANGUAGES CXX)

include_directories(. ${CMAKE_CURRENT_SOURCE_DIR}/miniaudio-master)

add_executable(Musica_CPP
        Graficos.c++
        Graficos.hpp
        "Reproductor de musica.cpp"
)

target_compile_features(Musica_CPP PRIVATE cxx_std_17)

cmake_minimum_required(VERSION 3.15)
project(BlackboxLoader C)

add_library(BlackboxLoader SHARED 
    src/dllmain.c
    version.def
)

set_target_properties(BlackboxLoader PROPERTIES
    OUTPUT_NAME "version"
    PREFIX ""
)
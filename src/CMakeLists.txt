cmake_minimum_required(VERSION 3.16)
project(CMolecule LANGUAGES CXX)

find_package(Arcane REQUIRED)

add_executable(CMolecule 
main.cc 
CMModule.cc 
CM_axl.h)

arcane_generate_axl(CM)
arcane_add_arcane_libraries_to_target(CMolecule)
target_include_directories(CMolecule PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
configure_file(CMolecule.config ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

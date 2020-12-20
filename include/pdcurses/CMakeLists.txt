cmake_minimum_required(VERSION 3.0.0)

include_directories(./common)
include_directories(./pdcurses)
include_directories(./wincon)
include_directories(.)

aux_source_directory(./common COMMON_SRCS)
aux_source_directory(./pdcurses PDCURSES_SRCS)
aux_source_directory(./wincon WINCON_SRCS)

add_library(PDcurses ${COMMON_SRCS} ${PDCURSES_SRCS} ${WINCON_SRCS})

target_include_directories(PDcurses
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
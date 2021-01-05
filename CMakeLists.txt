cmake_minimum_required(VERSION 3.0.0)
project(Notepad VERSION 0.1.0)

include(CTest)
enable_testing()

# set directories of .h files
include_directories(./include)

aux_source_directory(./src DIR_SRCS)

add_executable(Notepad main.c ${DIR_SRCS})

if(WIN32 OR MSVC)
        add_definitions(-DPDC_WIDE)
        add_subdirectory(./include/pdcurses)
        target_link_libraries(Notepad
                PRIVATE
                PDcurses
        )
else(UNIX)
        find_package(Curses REQUIRED)
        include_directories(${CURSES_INCLUDE_DIRS})
        target_link_libraries(Notepad ${CURSES_LIBRARIES})
        # find ncurses and use that
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

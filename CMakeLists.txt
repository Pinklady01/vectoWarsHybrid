# CMakeList.txt : CMake project for hybrid_ggpo, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("VectorWarHybrid")

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message("FOUND GIT")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)

        #add_subdirectory(ggpo)
        add_subdirectory(hybrid-GGPO)

        #add_library (hybrid_ggpo "hybrid_ggpo.cpp" "hybrid_ggpo.h")
        add_executable(VectorWarHybrid "gamestate.cpp" "gamestate.h" "gdi_renderer.cpp" "gdi_renderer.h"
                "ggpo_perfmon.cpp" "ggpo_perfmon.h" "main.cpp" "nongamestate.h" "renderer.h" "Resource.h" "targetver.h"
                "vectorwar.cpp" "vectorwar.h")
        target_link_libraries(VectorWarHybrid hybrid_ggpo)
        #target_link_libraries(VectorWarHybrid GGPO)

        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()
# Add source to this project's executable.
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET VectorWarHybrid PROPERTY CXX_STANDARD 20)
endif()
#if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/repo/CMakeLists.txt")
#    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
#endif()

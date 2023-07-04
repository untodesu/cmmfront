cmake_minimum_required(VERSION 3.10)
project(cmmfront LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(eigen)
add_subdirectory(glad)
add_subdirectory(glfw)

add_executable(cmmfront
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_impl_opengl3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_stdlib.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/cmd_comment.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/cmd_move_at.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/cmd_plane.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/cmd_point.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/cmd_report.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/cmm_stub.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/cmm_wrap.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/globals.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/gui.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/math_plane.cpp")
target_include_directories(cmmfront PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/imgui"
    "${CMAKE_CURRENT_LIST_DIR}/src")
target_link_libraries(cmmfront PRIVATE eigen glad glfw)

cmake_minimum_required(VERSION 3.10)
project(sandboxing)
add_executable(sandbx main.cpp)

include(FetchContent)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.4
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3)

target_link_libraries(sandbx PRIVATE SDL3::SDL3)

add_custom_command(TARGET sandbx POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:sandbx>
)
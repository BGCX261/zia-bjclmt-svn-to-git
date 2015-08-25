#CMakeLists.txt
cmake_minimum_required(VERSION 2.8.2)

set(project_name "Zia")
project(${project_name})
set(EXECUTABLE_OUTPUT_PATH ${${project_name}_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

include_directories(${${project_name}_SOURCE_DIR}/inc)

#OpenSsl
include_directories(${openssl_include_dir})
link_directories(${openssl_lib_dir})

file(
    GLOB
    source_files
    src/*.cpp
)

file(
    GLOB
    include_files
    inc/*.h*
)

add_executable(
    zia
    ${source_files}
    ${include_files}
)

target_link_libraries(
   zia
   ${libs_name}
)

target_link_libraries(
   zia
   libeay32
   ssleay32
)
cmake_minimum_required(VERSION 3.6)

project(Loki)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(compiler_c_flags "-Wno-error=format-security")
set(compiler_c_flags "-Wno-error=error")

##################################### CHIBI ###################################
add_library(chibi_scheme SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/bignum.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/eval.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/gc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/gc_heap.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/opcodes.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/sexp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/simplify.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/vm.c
)

target_compile_definitions(chibi_scheme PRIVATE
    SEXP_USE_WARN_UNDEFS=1 SEXP_USE_DL=0 SEXP_USE_STATIC_LIBS=1 SEXP_USE_STATIC_LIBS_NO_INCLUDE=0)

target_include_directories(chibi_scheme PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/include
)

target_link_libraries(chibi_scheme PRIVATE
)


##################################### Loki ####################################
add_library(${CMAKE_PROJECT_NAME} SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/loki_main.cpp
  ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/
  ${ANDROID_NDK}/sources/android/native_app_glue
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/chibi-scheme-static/include
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  android
  EGL
  GLESv3
  log
  chibi_scheme
)



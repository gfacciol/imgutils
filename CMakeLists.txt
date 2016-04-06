cmake_minimum_required(VERSION 2.8)
project(imgutils)

# The build type "Release" adds some optimizations
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif ()

# Are we using gcc?
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # GCC on MacOs needs this option to use the clang assembler
  if (APPLE)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wa,-q")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-q")
  endif ()
  # Optimize to the current CPU and enable warnings
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -Wall -Wextra")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra")
endif ()

# Enable C99
if (CMAKE_VERSION VERSION_LESS "3.1")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
else ()
  set (CMAKE_C_STANDARD 99)
endif ()

# Enable C++11
if (CMAKE_VERSION VERSION_LESS "3.1")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else ()
  set (CMAKE_CXX_STANDARD 11)
endif ()

# Enable OpenMP
find_package (OpenMP)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")

# Enable Eigen
set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package (Eigen3 REQUIRED)
include_directories (PUBLIC ${EIGEN3_INCLUDE_DIR})

# Enable LibFFTW
find_path (FFTW_INCLUDE_DIR fftw3.h)
find_library (FFTWF_LIBRARIES NAMES fftw3f)
include_directories (PUBLIC ${FFTW_INCLUDE_DIR})
link_libraries (${FFTWF_LIBRARIES})

# Link LibTIFF, LibJPEG, LibPNG
find_package (TIFF REQUIRED)
find_package (JPEG REQUIRED)
find_package (PNG REQUIRED)
include_directories (PUBLIC ${TIFF_INCLUDE_DIR} PUBLIC ${JPEG_INCLUDE_DIR} PUBLIC ${PNG_INCLUDE_DIRS})
link_libraries (${TIFF_LIBRARIES} ${JPEG_LIBRARIES} ${PNG_LIBRARIES})

set_property(SOURCE iio.c PROPERTY COMPILE_FLAGS "-Wno-unused-variable -Wno-unused-parameter -Wno-pointer-sign -Wno-parentheses -Wno-deprecated-declarations -Wno-unused-function")
set_property(SOURCE smooth_detector.cpp PROPERTY COMPILE_FLAGS "-Wno-deprecated-declarations")

add_library(imgutils OBJECT iio.c iio.h utils.cpp utils.hpp Image.hpp EigenImage.hpp)
foreach(program readRGB num_pixels dct anti_dead_pixel 4to3 vst apply_histogram cfa_split cfa_rebuild cfa_colorize smooth_detector fpsnr)
  add_executable(${program} ${program}.cpp $<TARGET_OBJECTS:imgutils>)
endforeach(program)

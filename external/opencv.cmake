include_guard()
include(FetchContent)

find_package(OpenCV COMPONENTS imgproc imgcodecs videoio) # channeltx/modatv

if(OpenCV_FOUND)
foreach(the_module ${OpenCV_LIBS})
  if(TARGET ${the_module})
    set_target_properties(
      ${the_module}
      PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES
          ${OpenCV_INCLUDE_DIRS}
        IMPORTED_GLOBAL
          TRUE
    )
    string(REPLACE "opencv_" "" module_name ${the_module})
    add_library(OpenCV::${module_name} ALIAS ${the_module})
  endif()
endforeach()
else()
  FetchContent_Declare(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv.git
    GIT_TAG 4.5.2
  )

  FetchContent_GetProperties(opencv)
  if(NOT opencv_POPULATED)
    FetchContent_Populate(opencv)

    set(BUILD_WITH_STATIC_CRT OFF CACHE BOOL "Enables use of statically linked CRT for statically linked OpenCV")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)")
    set(BUILD_TESTS OFF CACHE BOOL "Build accuracy & regression tests")
    set(BUILD_PERF_TESTS OFF CACHE BOOL "Build performance tests")
    set(BUILD_opencv_apps OFF CACHE BOOL "Build utility applications (used for example to train classifiers)")
    set(BUILD_LIST "core,imgcodecs,imgproc,videoio" CACHE STRING "Build only listed modules (comma-separated, e.g. 'videoio,dnn,ts')")

    if(HAS_AVX512F)
      set(CPU_BASELINE AVX_512F)
    elseif(HAS_AVX2)
      set(CPU_BASELINE AVX2)
    elseif(HAS_AVX)
      set(CPU_BASELINE AVX)
    elseif(HAS_SSE4_2)
      set(CPU_BASELINE SSE4_2)
    elseif(HAS_SSE4_1)
      set(CPU_BASELINE SSE4_1)
    elseif(HAS_SSSE3)
      set(CPU_BASELINE SSSE3)
    elseif(HAS_SSE3)
      set(CPU_BASELINE SSE3)
    endif()

    add_subdirectory(${opencv_SOURCE_DIR} ${opencv_BINARY_DIR} EXCLUDE_FROM_ALL)

    foreach(the_module ${OPENCV_MODULES_BUILD})
      if(TARGET ${the_module})
        set_target_properties(
          ${the_module}
          PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES
              $<TARGET_PROPERTY:${the_module},INCLUDE_DIRECTORIES>
        )
        string(REPLACE "opencv_" "" module_name ${the_module})
        add_library(OpenCV::${module_name} ALIAS ${the_module})
      endif()
    endforeach()
  endif()
endif()

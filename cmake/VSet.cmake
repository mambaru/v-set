INCLUDE(ConfigureLibrary)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(UNIX)
  IF("${CMAKE_COMPILER_IS_GNUCXX}" MATCHES "1")
    exec_program(
      ${CMAKE_CXX_COMPILER}
      ARGS -dumpversion
      OUTPUT_VARIABLE gcc_compiler_version
    )        

    STRING(REGEX REPLACE "^([0-9]+)\\..*" "\\1" gcc_major_version "${gcc_compiler_version}")
    STRING(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1" gcc_minor_version "${gcc_compiler_version}")
    MATH(EXPR gcc_version_number "${gcc_major_version} * 1000 + ${gcc_minor_version}" )

    message(STATUS "C++ compiler version: ${gcc_compiler_version} major: ${gcc_major_version} minor: ${gcc_minor_version} number: ${gcc_version_number} [${CMAKE_CXX_COMPILER}]")

    SET(VSET_CXX_STANDARD  "")
    if ( NOT ${gcc_version_number} LESS 4006 )
      SET(VSET_CXX_STANDARD    "${VSET_CXX_STANDARD} -std=c++0x")
    endif()

    if ( NOT ${gcc_version_number} LESS 4008 )
      SET(VSET_CXX_STANDARD    "${VSET_CXX_STANDARD} -ftemplate-backtrace-limit=0")
    endif()
    SET(CMAKE_CXX_WARN_FLAGS  "-W -Wall -pedantic")
    SET(CMAKE_CXX_FLAGS       "${CXX_FLAGS} ${VSET_CXX_STANDARD} ${CMAKE_CXX_WARN_FLAGS}")
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb ${CMAKE_CXX_FLAGS}")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -ggdb -DNDEBUG ${CMAKE_CXX_FLAGS}")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -ggdb -DNDEBUG  ${CMAKE_CXX_FLAGS}")
  ENDIF("${CMAKE_COMPILER_IS_GNUCXX}" MATCHES "1")
elseif(MSVC)
  SET(CMAKE_CXX_FLAGS "/GR- /W3 /wd4503 /wd4307 /EHsc /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS")
  SET(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
  SET(CMAKE_CXX_FLAGS_DEBUG  "/Od /D_DEBUG")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "/Yd /O2 /DNDEBUG")
else(UNIX)

endif(UNIX)

SET(FASLIB_DIR "${CMAKE_SOURCE_DIR}/../faslib")
SET(FAS_TESTING_CPP "${FASLIB_DIR}/fas/testing/testing.cpp")




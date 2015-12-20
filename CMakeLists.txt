cmake_minimum_required(VERSION 3.2)
project(OpenGLApp CXX)

if("${CMAKE_SIZEOF_VOID_P}" EQUAL "4")
    set(WIN_BIT_VER32 ON)
elseif("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    set(WIN_BIT_VER64 ON)
endif()

set(COMPILER_TYPE 0)


if(WIN_BIT_VER32)
    set( CMAKE_EXE_LINKER_FLAGS "-L/mingw32/lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system")
elseif(WIN_BIT_VER64)
    set( CMAKE_EXE_LINKER_FLAGS "-L/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system")
endif()

if(${COMPILER_TYPE} EQUAL 0)
    set(EXTRA_COMPILER_FLAGS "-std=c++14 -Wall -flto")
elseif(${COMPILER_TYPE} EQUAL 1)
    set(EXTRA_COMPILER_FLAGS "-std=c++14 -Wall")
endif()

if(WIN_BIT_VER32)
    set(CCOMPILER_VERSION "-m32")
elseif(WIN_BIT_VER64)
    set(CCOMPILER_VERSION "-m64 -msse4 -mavx")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILER_FLAGS} ${CCOMPILER_VERSION} ${SFML_FLAGS}")

set(CMAKE_VERBOSE_MAKEFILE OFF)

if(WIN_BIT_VER32)
    set(COMPILER_DIR "E:\\Programs\\Dev\\MSYS2\\mingw32" CACHE FILEPATH "Path to mingw-w64-x32 toolset" FORCE)
    set(EXCEPT_LIB libgcc_s_dw2-1.dll)
elseif(WIN_BIT_VER64)
    set(COMPILER_DIR "E:\\Programs\\Dev\\MSYS2\\mingw64" CACHE FILEPATH "Path to mingw-w64-x32 toolset" FORCE)
    set(EXCEPT_LIB libgcc_s_seh-1.dll)
endif()

if(${COMPILER_TYPE} EQUAL 0)
    set(CMAKE_CXX_COMPILER "${COMPILER_DIR}\\bin\\g++.exe" CACHE FILEPATH "Path to c++ compiler." FORCE)
    set(CMAKE_C_COMPILER "${COMPILER_DIR}\\bin\\gcc.exe" CACHE FILEPATH "Path to c compiler." FORCE)
    set(CMAKE_AR "${COMPILER_DIR}\\bin\\gcc-ar.exe" CACHE FILEPATH "Path to ar executable." FORCE)
elseif(${COMPILER_TYPE} EQUAL 1)
    set(CMAKE_CXX_COMPILER "${COMPILER_DIR}\\bin\\clang++.exe" CACHE FILEPATH "Path to c++ compiler." FORCE)
    set(CMAKE_C_COMPILER "${COMPILER_DIR}\\bin\\clang.exe" CACHE FILEPATH "Path to c compiler." FORCE)
    set(CMAKE_AR "${COMPILER_DIR}\\bin\\llvm-ar.exe" CACHE FILEPATH "Path to ar executable." FORCE)
endif()


set(SOURCE_FILES
            stdafx.h
            Main.cpp
            Main.h
            MainImpl.cpp
            OScriptInterpreter/OScriptInterpreter.cpp
            OScriptInterpreter/OScriptInterpreter.h
            OScriptInterpreter/UDS.cpp
            OScriptInterpreter/UDS.h
            ShaderManagerRealization.cpp
            ShaderManagerRealization.h
            ShaderWrapper.cpp
            ShaderWrapper.h
            Settings.h
            SFML/SFMLWindow.cpp
            SFML/SFMLWindow.h
            SFML/Shape.cpp
            SFML/Shape.h
            Utilities.h
        )

add_executable(OpenGLApp ${SOURCE_FILES})

if(${COMPILER_TYPE} EQUAL 0)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    else()
        message(WARNING "WARNING! Current gcc version is not supporting OpenMP")
    endif()
endif()

set(OpenGLApp_Bin_Dir "${CMAKE_SOURCE_DIR}\\bin\\Debug")
foreach(file ${OpenGLApp_Bin_Dir})
    if (EXISTS ${file})
        file(REMOVE_RECURSE ${file})
    endif()
endforeach(file)

set(BOOST_ROOT ${COMPILER_DIR})
set(BOOST_INCLUDEDIR ${COMPILER_DIR}\\include)
find_package(Boost 1.57.0 COMPONENTS system filesystem iostreams REQUIRED)
IF (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  set(Boost_USE_MULTITHREADED      ON)
  set(Boost_USE_STATIC_LIBS       OFF)
  set(Boost_USE_STATIC_RUNTIME    OFF)
  if(Boost_USE_STATIC_LIBS)
      target_link_libraries(OpenGLApp ${Boost_LIBRARIES})
  else()
      file(COPY ${COMPILER_DIR}\\bin\\libboost_filesystem-mt.dll DESTINATION ${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE} USE_SOURCE_PERMISSIONS)
      file(COPY ${COMPILER_DIR}\\bin\\libboost_system-mt.dll DESTINATION ${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE} USE_SOURCE_PERMISSIONS)
      file(COPY ${COMPILER_DIR}\\bin\\libboost_iostreams-mt.dll DESTINATION ${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE} USE_SOURCE_PERMISSIONS)
      target_link_libraries(OpenGLApp ${Boost_LIBRARIES})
  endif()
else()
    message(FATAL_ERROR "Error! Boost not found")
endif()
file(COPY ${COMPILER_DIR}\\bin\\${EXCEPT_LIB} DESTINATION ${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE} USE_SOURCE_PERMISSIONS)

set(GLEW_INCLUDE_DIR ${COMPILER_DIR}\\include)
set(GLUT_INCLUDE_DIR ${COMPILER_DIR}\\include)
set(GLUT_STAIC OFF CACHE BOOL "Use or not freeglut in static variant.")

find_package(OpenGL REQUIRED)
find_package (GLEW REQUIRED)
find_package(GLUT REQUIRED)
set(GLEW_USE_STATIC_LIBS ON)
set(GLUT_STATIC OFF)
target_link_libraries(OpenGLApp ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARIES})

if(GLUT_STATIC)
    target_link_libraries(OpenGLApp ${GLUT_LIBRARIES})
else()
    target_link_libraries(OpenGLApp ${GLUT_LIBRARIES})
    file(COPY ${COMPILER_DIR}\\bin\\libfreeglut.dll DESTINATION ${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE} USE_SOURCE_PERMISSIONS)
endif()

if(GLEW_USE_STATIC_LIBS)
    file(COPY ${COMPILER_DIR}\\bin\\glew32.dll DESTINATION ${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE} USE_SOURCE_PERMISSIONS)
endif()

file(COPY ${COMPILER_DIR}\\bin\\libwinpthread-1.dll DESTINATION ${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE} USE_SOURCE_PERMISSIONS)
file(COPY ${COMPILER_DIR}\\bin\\libgomp-1.dll DESTINATION ${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE} USE_SOURCE_PERMISSIONS)
file(COPY "${COMPILER_DIR}\\bin\\libstdc++-6.dll" DESTINATION "${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE}" USE_SOURCE_PERMISSIONS)

#sfml
file(COPY "${COMPILER_DIR}\\bin\\sfml-system-2.dll" DESTINATION "${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE}" USE_SOURCE_PERMISSIONS)
file(COPY "${COMPILER_DIR}\\bin\\sfml-window-2.dll" DESTINATION "${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE}" USE_SOURCE_PERMISSIONS)
file(COPY "${COMPILER_DIR}\\bin\\sfml-graphics-2.dll" DESTINATION "${CMAKE_SOURCE_DIR}\\bin\\${CMAKE_BUILD_TYPE}" USE_SOURCE_PERMISSIONS)
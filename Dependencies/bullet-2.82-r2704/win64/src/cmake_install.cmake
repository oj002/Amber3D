# Install script for directory: C:/dev/projects/games_graphics/Amber3D/Dependencies/bullet-2.82-r2704/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/BULLET_PHYSICS")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/dev/projects/games_graphics/Amber3D/Dependencies/bullet-2.82-r2704/win64/src/BulletSoftBody/cmake_install.cmake")
  include("C:/dev/projects/games_graphics/Amber3D/Dependencies/bullet-2.82-r2704/win64/src/BulletCollision/cmake_install.cmake")
  include("C:/dev/projects/games_graphics/Amber3D/Dependencies/bullet-2.82-r2704/win64/src/BulletDynamics/cmake_install.cmake")
  include("C:/dev/projects/games_graphics/Amber3D/Dependencies/bullet-2.82-r2704/win64/src/LinearMath/cmake_install.cmake")
  include("C:/dev/projects/games_graphics/Amber3D/Dependencies/bullet-2.82-r2704/win64/src/MiniCL/cmake_install.cmake")
  include("C:/dev/projects/games_graphics/Amber3D/Dependencies/bullet-2.82-r2704/win64/src/BulletMultiThreaded/cmake_install.cmake")

endif()


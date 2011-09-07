CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(%MODEL_NAME%_%SUBMODEL_NAME%)

#
# Do setup in case of ros package, If ROS_ROOT is set, it is
# recommended to use RTT/OCL through the ros packages.
#
set (ROS_ROOT $ENV{ROS_ROOT} )
if (ROS_ROOT)
  include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)
  rosbuild_init()
  rosbuild_find_ros_package( rtt )
  set( RTT_HINTS HINTS ${rtt_PACKAGE_PATH}/install )
endif()
 
# Set the CMAKE_PREFIX_PATH in case you're not using Orocos through ROS
# for helping these find commands find RTT.
find_package(OROCOS-RTT REQUIRED ${RTT_HINTS})
 
# Defines the orocos_* cmake macros. See that file for additional
# documentation.
include(${OROCOS-RTT_USE_FILE_PATH}/UseOROCOS-RTT.cmake)


#OROCOS setup
set(OROCOS_TARGET $ENV{OROCOS_TARGET})

#Add include directories
INCLUDE_DIRECTORIES(include)

IF(${OROCOS_TARGET} MATCHES xenomai)
    INCLUDE_DIRECTORIES(/usr/xenomai/include)
ENDIF(${OROCOS_TARGET} MATCHES xenomai)

#Add link directories for libraries
IF(${OROCOS_TARGET} MATCHES xenomai)
    LINK_DIRECTORIES(/usr/xenomai/lib)
    LINK_LIBRARIES(native)
ENDIF(${OROCOS_TARGET} MATCHES xenomai)

#Add necessary libraries
orocos_use_package(ocl-taskbrowser)
orocos_use_package(rtt)

find_package(Orocos-RTT REQUIRED rtt-marshalling)
if ( OROCOS-RTT_RTT-MARSHALLING_FOUND )
  link_libraries( ${OROCOS-RTT_RTT-MARSHALLING_LIBRARY} )
else( OROCOS-RTT_RTT-MARSHALLING_FOUND )
  message(SEND_ERROR "'rtt-marshalling' not found !")
endif( OROCOS-RTT_RTT-MARSHALLING_FOUND )

#Header and Source files used in the component
SET(COMMON_HEADERS  include/xxfuncs.h   include/xxinteg.h   include/tinyxml.h   include/xxmatrix.h
            include/xxinverse.h include/xxmodel.h   include/MotionProfiles.h    include/tinystr.h
            include/EulerAngles.h  include/xxtypes.h include/%SUBMODEL_NAME%.h include/Port20Sim.h)

SET(COMMON_SOURCE  src/xxfuncs.cpp src/xxinteg.cpp src/xxinverse.cpp   src/tinyxml.cpp src/tinystr.cpp
            src/xxmatrix.cpp src/EulerAngles.cpp src/MotionProfiles.cpp src/tinyxmlerror.cpp
            src/tinyxmlparser.cpp src/Port20Sim.cpp)

#Executable component and can also be a library usable for orocos deployment
orocos_component(%SUBMODEL_NAME% src/%SUBMODEL_NAME%.cpp ${COMMON_SOURCE} ${COMMON_HEADERS})

orocos_executable(test_%SUBMODEL_NAME% test_%SUBMODEL_NAME%.cpp ${COMMON_HEADERS})
target_link_libraries(test_%SUBMODEL_NAME% %SUBMODEL_NAME%)

file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_BINARY_DIR})

#
# Generates and installs our package. Must be the last statement such
# that it can pick up all above settings.
#
orocos_generate_package()

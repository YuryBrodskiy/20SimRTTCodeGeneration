CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(%MODEL_NAME%_%SUBMODEL_NAME%)

#OROCOS setup
SET(Target gnulinux)
SET(OROCOS_INSTALL_DIR /usr/local)
ADD_DEFINITIONS(-DOROCOS_TARGET=${Target})

#Add include directories
INCLUDE_DIRECTORIES(include
                    ${OROCOS_INSTALL_DIR}/include
                    ${OROCOS_INSTALL_DIR}/include/orocos)

#Add link directories for libraries
LINK_DIRECTORIES(${OROCOS_INSTALL_DIR}/lib)

#Add necessary libraries
#Other libraries can be orocos-ocl-taskbrowser-${Target}
LINK_LIBRARIES(orocos-rtt-${Target})


#Header and Source files used in the component
SET(HEADER  include/xxfuncs.h include/xxinteg.h   include/xxmatrix.h
            include/xxinverse.h include/xxmodel.h include/MotionProfiles.h
            include/EulerAngles.h include/%SUBMODEL_NAME%.h include/xxtypes.h)
SET(SOURCE  src/xxfuncs.cpp src/xxinteg.cpp src/xxinverse.cpp
            src/xxmatrix.cpp src/EulerAngles.cpp src/MotionProfiles.cpp
            src/%SUBMODEL_NAME%.cpp test_%SUBMODEL_NAME%.cpp)

#Executable component and can also be a library usable for orocos deployment
# use ADD_LIBRARY(%SUBMODEL_NAME% SHARED %SUBMODEL_NAME%.cpp)
ADD_EXECUTABLE(%SUBMODEL_NAME% ${SOURCE} ${HEADER})

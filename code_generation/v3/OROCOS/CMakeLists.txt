CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(%MODEL_NAME%_%SUBMODEL_NAME%)

INCLUDE_DIRECTORIES(include)

SET(HEADER 	include/xxfuncs.h include/xxinteg.h   include/xxmatrix.h
		include/xxinverse.h include/xxmodel.h include/MotionProfiles.h
		include/EulerAngles.h include/%SUBMODEL_NAME%.h include\xxtypes.h)
SET(SOURCE  src/xxfuncs.cpp src/xxinteg.cpp src/xxinverse.cpp
		src/xxmatrix.cpp src/EulerAngles.cpp src/MotionProfiles.cpp
		src/%SUBMODEL_NAME%.cpp test_%SUBMODEL_NAME%.cpp)

ADD_EXECUTABLE(%SUBMODEL_NAME% ${SOURCE} ${HEADER})
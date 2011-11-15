# This Makefile is here for 'rosmake' like systems. In case you don't use
# ROS, it will create a build directory for you and build the package with
# default settings. It will install it at the same location as the RTT is installed.
ifdef ROS_ROOT
include $(shell rospack find mk)/cmake.mk
else
all:
	mkdir -p build
	cd build ; cmake .. -DINSTALL_PATH=orocos && make
	echo -e "\n Now do 'make install' to install this package.\n"
install: all
	cd build ; make install
endif

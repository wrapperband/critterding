#!/bin/bash 
rm CMakeCache.txt -fr && rm CMakeFiles -fr
CXXFLAGS="-march=native -Ofast -w -pipe" cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ./
# CXXFLAGS="-Ofast -Wall -Wextra" cmake -DCMAKE_BUILD_TYPE=Release ./
if [ -z "$1" ]
	then
		time make
	else
		time make -j${1}
fi

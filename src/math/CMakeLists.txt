
#include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(math_STAT_SRCS vector3f.cpp vector2i.cpp vector2f.cpp)

add_library(math STATIC ${math_STAT_SRCS})


########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#noinst_LTLIBRARIES = libmath.la
#noinst_HEADERS =	vector3f.h \
#			vector2i.h \
#			vector2f.h
#libmath_la_SOURCES =	vector3f.cpp \
#			vector2i.cpp \
#			vector2f.cpp

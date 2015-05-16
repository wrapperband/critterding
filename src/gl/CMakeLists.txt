include_directories( ${bengine_INCLUDES} )

#include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(gl_STAT_SRCS glwindow.cpp)

add_library(gl STATIC ${gl_STAT_SRCS})
TARGET_LINK_LIBRARIES ( gl utils kernel )

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes) -I../utils/bullet 
#METASOURCES = AUTO
#libgl_la_LDFLAGS = -avoid-version -no-undefined
#noinst_LTLIBRARIES = libgl.la
#noinst_HEADERS = glwindow.h glscene.h
#libgl_la_SOURCES = glwindow.cpp glscene.cpp
#
#libgl_la_LIBADD = -L/usr/lib

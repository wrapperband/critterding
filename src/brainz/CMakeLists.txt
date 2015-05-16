
#include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(brainz_STAT_SRCS
    archneuronz.cpp
    archsynapse.cpp
    brainzarch.cpp
    brainz.cpp
    neuroninterz.cpp
    synapse.cpp)

add_library(brainz STATIC ${brainz_STAT_SRCS})


########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#noinst_LTLIBRARIES = libbrainz.la
#libbrainz_la_LDFLAGS = -avoid-version -no-undefined
#noinst_HEADERS =	archneuronz.h \
#			archsynapse.h \
#			brainzarch.h \
#			brainz.h \
#			neuroninterz.h \
#			synapse.h
#libbrainz_la_SOURCES =	archneuronz.cpp \
#			archsynapse.cpp \
#			brainzarch.cpp \
#			brainz.cpp \
#			neuroninterz.cpp \
#			synapse.cpp
#libbrainz_la_LIBADD = $(top_builddir)/src/utils/libutils.la

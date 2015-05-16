
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(modes_STAT_SRCS
    race.cpp
#     testworld1.cpp
#     concavefloor.cpp
    roundworld.cpp
)

add_library(modes STATIC ${modes_STAT_SRCS})
target_link_libraries( modes cdentities )


########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes) -I../../../utils/bullet -fopenmp
#METASOURCES = AUTO
#noinst_LTLIBRARIES = libmodes.la
#noinst_HEADERS =	race.h \
#			testworld1.h \
#			concavefloor.h \
#			roundworld.h
#libmodes_la_SOURCES =	race.cpp \
#			testworld1.cpp \
#			concavefloor.cpp \
#			roundworld.cpp
#

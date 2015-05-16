
# include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

add_subdirectory(gui)
add_subdirectory(entities)
add_subdirectory(modes)


########### next target ###############

set(sccritterding_STAT_SRCS evolution.cpp)

# TARGET_LINK_LIBRARIES ( sccritterding cdentities cdgui modes ) 
add_library(sccritterding STATIC ${sccritterding_STAT_SRCS})

target_link_libraries( sccritterding graphics filesystem cdentities cdgui modes common brainz )

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes) -I../../utils/bullet -fopenmp
#METASOURCES = AUTO
#noinst_LTLIBRARIES = libcritterding.la
#libcritterding_la_SOURCES = evolution.cpp
#libcritterding_la_LIBADD =	$(top_builddir)/src/brainz/libbrainz.la \
#			$(top_builddir)/src/gl/libgl.la \
#			$(top_builddir)/src/gui/libgui.la \
#			$(top_builddir)/src/utils/libutils.la \
#			$(top_builddir)/src/utils/bullet/libbulletmath.la \
#			$(top_builddir)/src/scenes/critterding/gui/libcdgui.la \
#			$(top_builddir)/src/scenes/critterding/entities/libentities.la \
#			$(top_builddir)/src/scenes/critterding/modes/libmodes.la
## 			$(top_builddir)/src/scenes/basescene/libbasescene.la
#
#
#SUBDIRS = gui entities modes
#

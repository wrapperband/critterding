
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(cdgui_STAT_SRCS
    critterview.cpp
    brainview.cpp
    helpinfo.cpp
    infostats.cpp
    infobar.cpp
    textverbosemessage.cpp
    statsgraph.cpp
    globalsettingspanel.cpp
    mutationpanel.cpp
    settingsbodypanel.cpp
    settingsbrainpanel.cpp
    hud.cpp
    speciesview.cpp
    populationcontrolpanel.cpp
    settingslightpanel.cpp
)

add_library(cdgui STATIC ${cdgui_STAT_SRCS})
TARGET_LINK_LIBRARIES ( cdgui gui utils )


########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes) -I../../../utils/bullet -fopenmp
#METASOURCES = AUTO
#noinst_LTLIBRARIES = libcdgui.la
#noinst_HEADERS =	critterview.h \
#			brainview.h \
#			helpinfo.h \
#			infostats.h \
#			infobar.h \
#			textverbosemessage.h \
#			statsgraph.h \
#			globalsettingspanel.h \
#			mutationpanel.h \
#			settingsbodypanel.h \
#			settingsbrainpanel.h \
#			hud.h \
#			speciesview.h
#
#libcdgui_la_SOURCES =	critterview.cpp \
#			brainview.cpp \
#			helpinfo.cpp \
#			infostats.cpp \
#			infobar.cpp \
#			textverbosemessage.cpp \
#			statsgraph.cpp \
#			globalsettingspanel.cpp \
#			mutationpanel.cpp \
#			settingsbodypanel.cpp \
#			settingsbrainpanel.cpp \
#			hud.cpp \
#			speciesview.cpp
#
#libcdgui_la_LIBADD =	$(top_builddir)/src/gui/libgui.la \
#			$(top_builddir)/src/utils/libutils.la \
#			$(top_builddir)/src/math/libmath.la

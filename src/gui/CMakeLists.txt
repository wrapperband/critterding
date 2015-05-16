include_directories( ${bengine_INCLUDES} )

########### next target ###############

set(gui_STAT_SRCS
    widget.cpp
    container.cpp
    canvas.cpp
    text.cpp
    text_uintp.cpp
    panel.cpp
    settingspanel.cpp
    enginesettingspanel.cpp
    button.cpp
    checkbox.cpp
    textprinter.cpp
    exitpanel.cpp
    logbuffermessage.cpp
    textinput.cpp
)

add_library(gui STATIC ${gui_STAT_SRCS})

TARGET_LINK_LIBRARIES ( gui filesystem ftgl math )
########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes) -I../utils/bullet 
#METASOURCES = AUTO
#noinst_LTLIBRARIES = libgui.la
#noinst_HEADERS =	widget.h \
#			container.h \
#			canvas.h \
#			text.h \
#			text_uintp.h \
#			panel.h \
#			settingspanel.h \
#			enginesettingspanel.h \
#			button.h \
#			checkbox.h \
#			textprinter.h \
#			exitpanel.h \
#			logbuffermessage.h
#
#libgui_la_SOURCES = 	widget.cpp \
#			container.cpp \
#			canvas.cpp \
#			text.cpp \
#			text_uintp.cpp \
#			panel.cpp \
#			settingspanel.cpp \
#			enginesettingspanel.cpp \
#			button.cpp \
#			checkbox.cpp \
#			textprinter.cpp \
#			exitpanel.cpp \
#			logbuffermessage.cpp
#
#libgui_la_LIBADD =	$(top_builddir)/src/utils/libutils.la \
#			$(top_builddir)/src/math/libmath.la

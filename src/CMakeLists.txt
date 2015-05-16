# include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )
include_directories( ${bengine_INCLUDES} )
# TARGET_LINK_LIBRARIES( critterding utils )

add_subdirectory(math)
add_subdirectory(gl)
add_subdirectory(scenes/critterding)
add_subdirectory(utils)
add_subdirectory(gui)
add_subdirectory(brainz)
# add_subdirectory(scenes/stuntcoureur)
add_subdirectory(graphics)
add_subdirectory(filesystem)
add_subdirectory(common)
add_subdirectory(net)
add_subdirectory(physics)
# add_subdirectory(thread)
add_subdirectory(kernel)
# add_subdirectory(input)
# add_subdirectory(virtual_machine)
# add_subdirectory(webcam)


set(critterding_SRCS scenes/critterding/critterding.cpp)
add_executable(critterding ${critterding_SRCS})
TARGET_LINK_LIBRARIES ( critterding sccritterding ${bengine_LIBS} ${bengine_WIN32LIBS})


# set(stuntcoureur_SRCS scenes/stuntcoureur/stuntcoureur.cpp)
# add_executable(stuntcoureur ${stuntcoureur_SRCS})
# TARGET_LINK_LIBRARIES ( stuntcoureur stscene collada14dom ${bengine_LIBS} ${bengine_WIN32LIBS})

install(TARGETS critterding DESTINATION bin)
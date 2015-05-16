include_directories( ${bengine_INCLUDES} )

#include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

# add_subdirectory(ftgl)
# add_subdirectory(bullet)


########### next target ###############

set(common_STAT_SRCS
    be_image.cpp
    be_obj_loader.cpp
    be_parser.cpp
    be_physics_debug_renderer.cpp
    be_mouse_picker.cpp
    be_rand.cpp
    be_model_system.cpp
    be_event_system.cpp
    be_command_system.cpp
    be_scene_node.cpp
    be_xml_helper.h
)

add_library(common STATIC ${common_STAT_SRCS})
target_link_libraries( common physics graphics )

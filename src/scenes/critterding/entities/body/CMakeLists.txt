include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

set(body_STAT_SRCS
    bodypart_rigid.cpp
    bodypart_rigid_box.cpp
    bodypart_rigid_sphere.cpp
    bodypart_rigid_cylinder.cpp
    bodypart_rigid_trianglemesh.cpp
    bodyphysics.cpp
)

add_library(body STATIC ${body_STAT_SRCS})
target_link_libraries( body physics )

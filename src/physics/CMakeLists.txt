include_directories( ${bengine_INCLUDES} )

set(physics_STAT_SRCS 
	be_physics_system.cpp
	be_raycast.cpp
# 	be_physics_material_loader.cpp
)

add_library(physics STATIC ${physics_STAT_SRCS})
target_link_libraries( physics graphics BulletSoftBody BulletDynamics BulletCollision LinearMath )
#  LinearMath BulletDynamics BulletCollision BulletSoftBody BulletMultiThreaded

# bulletmultithreaded, bulletsoftbody, bulletdynamics, bulletcollision, linearmath
# BulletMultiThreaded BulletSoftBody BulletDynamics BulletCollision LinearMath
include_directories( ${bengine_INCLUDES} )

set(webcam_STAT_SRCS 
	img.cpp
	videocap.cpp
# 	videograb.cpp
)

add_library(webcam STATIC ${webcam_STAT_SRCS})
target_link_libraries( webcam )

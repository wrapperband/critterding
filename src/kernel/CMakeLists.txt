include_directories( ${bengine_INCLUDES} )

set(kernel_STAT_SRCS
    be_log.cpp
)

add_library(kernel STATIC ${kernel_STAT_SRCS})
target_link_libraries( kernel )

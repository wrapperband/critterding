include_directories( ${bengine_INCLUDES} )

set(filesystem_STAT_SRCS
    be_file.cpp
    be_filesystem.cpp
    be_rootpaths.cpp
    be_dir.cpp
)

add_library(filesystem STATIC ${filesystem_STAT_SRCS})
target_link_libraries( filesystem boost_filesystem )
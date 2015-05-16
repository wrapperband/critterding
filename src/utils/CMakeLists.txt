include_directories( ${bengine_INCLUDES} )

add_subdirectory(ftgl)

set(utils_STAT_SRCS
    settings.cpp
    texturestore.cpp
    be_settings_loader.cpp
)

add_library(utils STATIC ${utils_STAT_SRCS})
target_link_libraries( utils common filesystem gui tinyxml )

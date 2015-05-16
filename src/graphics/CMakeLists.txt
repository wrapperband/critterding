include_directories( ${bengine_INCLUDES} )

set(gl_STAT_SRCS 
	be_graphics_kernel.cpp
	be_texture.cpp
	be_framebuffer.cpp
	be_buffer.cpp
	be_vertex_array.cpp
	be_shader.cpp
	be_mesh.cpp
	be_graphics_system.cpp
	be_camera.cpp
	be_color.cpp
	sc_graphics.cpp
)

add_library(graphics STATIC ${gl_STAT_SRCS})
target_link_libraries( graphics gl glew utils common math )

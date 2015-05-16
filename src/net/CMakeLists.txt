include_directories( ${bengine_INCLUDES} )

set(net_STAT_SRCS 
# 	be_net_system.cpp
# 	be_socket_udp.cpp
# 	be_socket_udp_client.cpp
# 	be_socket_udp_server.cpp
# 	be_socket_tcp.cpp
# 	be_socket_tcp_client.cpp
# 	be_socket_tcp_server.cpp
	be_net_message.cpp
# 	be_socket_local.cpp
)

# enable_unity_build(be_net, net_STAT_SRCS)
add_library(be_net STATIC ${net_STAT_SRCS})
target_link_libraries( be_net kernel )

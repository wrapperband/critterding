#ifndef BE_MESSAGE_CLIENT_H_INCLUDED
#define BE_MESSAGE_CLIENT_H_INCLUDED

// #include "be_socket_udp_client.h"
// #include "be_socket_tcp.h"
// #include "be_socket_local.h"
// #include "scenes/stuntcoureur/entities/sc_netmessages.h"
// #include "boost/circular_buffer.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <list>
// #include <vector>
// #include <iostream>
// #include <cstdint>
#include <boost/scoped_ptr.hpp>



class BeMessageClient
{
public:
		BeMessageClient(BeMessageManager& messageManager) : m_messageManager(messageManager), m_socket(m_ioService), m_resolver(m_ioService) {};
		~BeMessageClient() {};

		bool connectTCP( const char* hostip, const std::uint16_t port ) 
		{ 
			/*IPaddress ipAddress;
			if ( SDLNet_ResolveHost(&ipAddress, hostip, port) == -1 )
			{
				fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
				exit(-1);
			}	*/
// 			resizeMessageBuffer(4000000);
			resizeMessageBuffer(sizeof(std::uint32_t));
			char portstr[256];
			sprintf(portstr, "%u", port);
			boost::asio::ip::tcp::resolver resolver(m_ioService);
			boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), hostip, portstr);
			//boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
			//m_socket.connect(*iterator);

			m_resolver.async_resolve(query, boost::bind(&BeMessageClient::onResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator) );
			return true;
			//return m_socket.open(ipAddress);
		}

		void onResolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		{
			if (!err)
			{
				// Attempt a connection to the first endpoint in the list. Each endpoint
				// will be tried until we successfully establish a connection.
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
				m_socket.async_connect(endpoint, boost::bind(&BeMessageClient::onConnect, this, boost::asio::placeholders::error, ++endpoint_iterator));
			}
			else
			{
				std::cout << "Error: " << err.message() << "\n";
			}
		}

		void onConnect(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		{
			if (!err)
			{
				// The connection was successful
				m_connectHandler();

				boost::asio::async_read(m_socket, boost::asio::buffer(&m_messageBuffer[0], 4), boost::asio::transfer_at_least(4), 
					boost::bind(&BeMessageClient::onReadHead, this,
					boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));

			}
			else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
			{
				// The connection failed. Try the next endpoint in the list.
				m_socket.close();
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
				m_socket.async_connect(endpoint, boost::bind(&BeMessageClient::onConnect, this,
					boost::asio::placeholders::error, ++endpoint_iterator));
			}
			else
			{
				std::cout << "Error: " << err.message() << "\n";
			}
		}

// 		void onReadHead(const boost::system::error_code& err)
// 		{
// 			if(!err)
// 			{
// 				size_t size=*(std::uint32_t*)(&m_messageBuffer[0]);
// 				if ( size > 1000000000 )
// 				{
// 				  
// 					std::cout << "BeMessageClient error: message size > 1000Mb : " << size << std::endl;
// 					exit(1);
// // 					return;
// 				}
// 				resizeMessageBuffer(size);
// 				
// 				boost::asio::async_read(m_socket, boost::asio::buffer(&m_messageBuffer[0], size), boost::asio::transfer_at_least(size), 
// 					boost::bind(&BeMessageClient::onReadBody, this, size, boost::asio::placeholders::error));
// 
// 				if(!err)
// 				{
// // 					net_message_t* message=(net_message_t*)&m_messageBuffer[0];
// // 					const unsigned int additional_size(message->getHistory().size());
// // 					if ( /* false && */ additional_size > 0 )
// // 					{
// // 						message->m_entity_history.m_history_string = new char[additional_size];
// // 						memcpy( message->m_entity_history.m_history_string, &m_messageBuffer[0]+size-additional_size, additional_size );
// // // 						std::cout << "client receive 2  " << additional_size << "   '" << message->getHistory().getStdString() << "'" << std::endl;
// // 						
// // // 						delete[] message->m_entity_history.m_history_string;
// // 					}
// 				}
// 				else
// 					std::cout << "Error: " << err.message() << "\n";
// 
// 			}
// 		}
// 
// 		void onReadBody(const size_t size, const boost::system::error_code& err)
// 		{
// 			if(!err)
// 			{
// 				net_message_t* message=(net_message_t*)&m_messageBuffer[0];
// 				
// // 				static unsigned int counter( 0 );
// // 				std::cout << ++counter << " CLIENT " << message->type() << std::endl;
// 				
// 				
// 				const unsigned int additional_size(message->getHistory().size());
// 				if ( /*false &&*/ additional_size > 0 )
// 				{
// 					message->m_entity_history.m_history_string = new char[additional_size];
// 					memcpy( message->m_entity_history.m_history_string, &m_messageBuffer[0]+size-additional_size, additional_size);
// 				}
// 				
// 				if(m_messageManager.handle(*message)==false)
// 				{
// 					std::cout << "::CLIENT unknown message from client" << " size: " << m_messageBuffer.size() << std::endl;
// 					return;
// 				}
// 				
// 				// free additional
// 					if ( /* false && */ additional_size > 0 )
// 						delete[] message->m_entity_history.m_history_string;
// 
// 				resizeMessageBuffer(4);
// 				boost::asio::async_read(m_socket, boost::asio::buffer(&m_messageBuffer[0], 4), boost::asio::transfer_at_least(4), 
// 					boost::bind(&BeMessageClient::onReadHead, this,
// 					boost::asio::placeholders::error));
// 			}
// 			else
// 			{
// 				std::cout << "Error: " << err.message() << "\n";
// 			}
// 		}


		void onReadHead(const std::size_t bytes_transferred, const boost::system::error_code& err)
		{
// 			if ( m_clientSockets[id] != 0 )
			{
				if( bytes_transferred != m_messageBuffer.size() )
				{
					std::cout << "::BeMessageClient::onReadHead: Error: partial read" << std::endl;
					// FIXME do disconnect/connect or something
				}
				else 
				{
					if(!err)
					{
						size_t size=*(std::uint32_t*)(&m_messageBuffer[0]);
						resizeMessageBuffer(size);
						
						
						boost::asio::async_read(
							m_socket,
							boost::asio::buffer(&m_messageBuffer[0], size),
							boost::asio::transfer_at_least(size),  
							boost::bind(&BeMessageClient::onReadBody, this, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)
						);
						
						if(!err)
						{
							net_message_t* message=(net_message_t*)&m_messageBuffer[0];
							const unsigned int additional_size(message->getHistory().size());
							if ( /* false && */ additional_size > 0 )
							{
		// 						size_t size=*(std::uint32_t*)(&m_messageBuffer[0]);
								
								message->m_entity_history.setHistoryString( new char[additional_size] );
								memcpy( message->m_entity_history.getHistoryString(), &m_messageBuffer[0]+size-additional_size, additional_size );
		// 						std::cout << "server receive 2  " << additional_size << "   '" << message->getHistory().getStdString() << "'" << std::endl;
		// 						delete[] message->m_entity_history.m_history_string;
							}
						}
						else
						{
							std::cout << "::BeMessageClient::onReadHead: Error 1: " << err.message() << "\n";
							// FIXME do disconnect/connect or something
						}
					}
					else
					{
						std::cout << "::BeMessageClient::onReadHead: Error: 2 " << err.message() << "\n";
						// FIXME do disconnect/connect or something
					}
				}
			}
// 			else
// 				std::cout << "::BeEntityNetSocketTcpServer::onReadHead: socket: " << id << " , still receiving: " << "\n";
			
		}

		void onReadBody(const std::size_t bytes_transferred, const boost::system::error_code& err)
		{
// 			if ( m_clientSockets[id] != 0 )
			{
				if(bytes_transferred!=m_messageBuffer.size())
				{
					std::cout << "::BeMessageClient::onReadBody: Error: partial read" << std::endl;
					// FIXME do disconnect/connect or something
				}
				else if(!err)
				{
					// FIXME DO ADDITIONAL RIGHT HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					
					net_message_t* message=(net_message_t*)&m_messageBuffer[0];
					
					if ( message->type() != 0 )
					{
						if(m_messageManager.handle(*message)==false)
						{
							std::cout << "::BeMessageClient::onReadBody: Error: unknown message  1: " << message->type() << " of size: " << m_messageBuffer.size() << std::endl;
							for(size_t i=0;i<m_messageBuffer.size();++i)
							{
								printf( "%x: %x %c\n", i, m_messageBuffer[i], isprint(m_messageBuffer[i]) ? m_messageBuffer[i] : ' ' );
							}
						}
					}
					else
					{
						std::cout << "::BeMessageClient::onReadBody: Error: unknown message 2: " << message->type() << " of size: " << m_messageBuffer.size() << std::endl;
					}
					
					// free additional
						const unsigned int additional_size(message->getHistory().size());
						if ( /* false && */ additional_size > 0 )
							delete[] message->m_entity_history.getHistoryString();

					resizeMessageBuffer(4);
					boost::asio::async_read(
						m_socket,
						boost::asio::buffer(&m_messageBuffer[0], 4),
						boost::asio::transfer_at_least(4), 
						boost::bind(&BeMessageClient::onReadHead, this, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
				}
				else
				{
					std::cout << "::BeMessageClient::onReadBody: Error:: " << err.message() << "\n";
					// FIXME do disconnect/connect or something
				}
			}
// 			else
// 				std::cout << "::BeEntityNetSocketTcpServer::onReadBody: socket: " << id << " , still receiving: " << "\n";
		}

private:
		/*bool send( const void* const data, const std::uint32_t size )
		{
			boost::asio::write(m_socket, boost::asio::buffer(&size, sizeof(size)));
			boost::asio::write(m_socket, boost::asio::buffer(data, size));
            return false;
		}*/

		bool send( const void* const data, std::uint32_t size )
		{
			if(!m_socket.is_open())
			{
				return false;
			}
			std::uint8_t* buf=(std::uint8_t*)malloc(size+4);
			std::uint32_t *s=(std::uint32_t*)buf;
			*s=size;
			memcpy(&buf[4],data,size);
			//printf( "sending %u bytes\n", size);
			//for(size_t i=0;i<size;++i)
			//{
// 				printf("%u %x %c\n", buf[4+i], buf[4+i], buf[4+i]);
// 			}
// 			if(m_writeBuffer.size()==0)
// 			{
// 				boost::asio::async_write(m_socket, boost::asio::buffer(buf, size+4), boost::bind(&BeMessageClient::onWrite, this, buf, boost::asio::placeholders::error) );
// 			}
// 			else
// 			{
				
				boost::asio::async_write(m_socket, boost::asio::buffer(buf, size+4),  boost::asio::transfer_at_least(size+4), boost::bind(&BeMessageClient::onWrite, this, buf, boost::asio::placeholders::error) );
				
				
				
				
				
// 				m_writeBuffer.push_back(buf);
// 				boost::system::error_code err;
// 				m_ioService.poll(err);
// 				if(err)
// 				{
// 					std::cout << "Client io_service::poll Error: " << err.message() << std::endl;
// 				}
// 			}
			return true;
		}

		void onWrite( const void* const buf, const boost::system::error_code& err)
		{
			free(const_cast<void*>(buf));
			if (!err)
			{	
// 				if(m_writeBuffer.size())
// 				{
// 					const void* const buf=m_writeBuffer.front();
// 					m_writeBuffer.pop_front();
// 					std::uint32_t size=*(std::uint32_t*)(buf);
// 					boost::asio::async_write(m_socket, boost::asio::buffer(buf, size+4), boost::bind(&BeMessageClient::onWrite, this, buf, boost::asio::placeholders::error) );
// 				}
			}
			else
			{
				//m_clientSockets[socketid].reset();
				std::cout << "BeMessageClient Error: " << err.message() << "\n";
			}
		}

public:
        template<typename T>
        bool send(const T& data)
        {
			
			
			// base size
				const std::uint32_t base_size( sizeof(T) );

			// additional size
				const std::uint32_t additional_size(data.sizeAddition());

			if ( /* false && */ additional_size > 0 )
			{
				// full size
					const std::uint32_t full_size( base_size + additional_size );
				
				// create data_full
					char data_full[full_size];
	// 				char* data_full = new char[full_size];
					
				// copy base
					memcpy( data_full, (void*)&data, base_size );

				// get additional data
					const char* data_additional = data.getHistory().getString();
				
				// copy additional
					memcpy( data_full + base_size, data_additional, additional_size );
				
				// send
					return send(&data_full, full_size );
				
	// 			// clear data_full;
	// 				delete[] data_full;
			}
			else
			{
			// send
				return send(&data, sizeof(T) );
			}
			
			
			
			
//             return send( &data, sizeof(T) );
        }
	public:

		template<typename T>
		BeMessageClient& operator<<(const T&data)
		{
			send(data);
			return *this;
		}

		void resizeMessageBuffer(const size_t size)
		{
	// 		if(size>m_messageBuffer.size())
			{
				m_messageBuffer.resize(size);
			}
		}

		void handleMessages()
		{
			boost::system::error_code err;
			
			m_ioService.poll(err);
// 			bool polled(m_ioService.poll(err));
// 			while ( polled && !err )
// 			{
// 				polled = m_ioService.poll(err);
// 			}
// 			
			if(err)
			{
				std::cout << "Client io_service::poll Error: " << err.message() << std::endl;
			}
		}

		void setConnectHandler(boost::function<void()> connectHandler)
		{
			m_connectHandler=connectHandler;
		}

	private:
		//BeSocketTcp m_socket;
		BeMessageManager& m_messageManager;
		std::vector<char> m_messageBuffer;
// 		std::list<std::uint8_t*> m_writeBuffer;

		boost::asio::io_service m_ioService;
		boost::asio::ip::tcp::socket m_socket;
		boost::asio::ip::tcp::resolver m_resolver;
		boost::function<void()> m_connectHandler;

};

#endif

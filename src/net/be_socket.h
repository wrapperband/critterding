#ifndef BE_SOCKET_H_INCLUDED
#define BE_SOCKET_H_INCLUDED

#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>
#include <iostream>

// #include "boost/circular_buffer.hpp"
#include "boost/circular_buffer/space_optimized.hpp"

class BeSocket
{
public:
		BeSocket() {};
		virtual ~BeSocket() {}
		virtual size_t recieve(boost::circular_buffer_space_optimized<char>& messages) = 0;
		virtual size_t send(const void* const message, size_t size) = 0;
		virtual bool isReady()=0;
		virtual void close()=0;
};

#endif

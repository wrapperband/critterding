#ifndef BE_NET_MESSAGE_H_INCLUDED
#define BE_NET_MESSAGE_H_INCLUDED

#include <iostream>
#include <string.h>
#include <cstdint>
#include <unordered_map>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
// #include "boost/circular_buffer.hpp"
// #include "boost/circular_buffer/space_optimized.hpp"
#include <boost/function.hpp>

typedef float BEFloat;
typedef double BEDouble;
// typedef std::float_t BEFloat;
// typedef std::double_t BEDouble;


typedef std::uint8_t BEUInt8;
typedef std::uint16_t BEUInt16;
typedef std::uint32_t BEUInt32;
typedef std::uint64_t BEUInt64;
typedef BEUInt32 BEUInt;

typedef std::int8_t BEInt8;
typedef std::int16_t BEInt16;
typedef std::int32_t BEInt32;
typedef std::int64_t BEInt64;
typedef BEInt32 BEInt;

// namespace BeNetTypes
// {
	class be_net_type_String
	{
	public:
		be_net_type_String() { memset( m_value, 0, sizeof(m_value) ); }
		be_net_type_String(const char* const value)
		{
			memset( m_value, 0, sizeof(m_value) );
			if(value)
			{
				size_t length=strlen(value);
				if(length>127)
				{
					length=127;
				}
				if(length>0)
				{
					memcpy( m_value, value, length );
				}
			}
		}
		operator const char*() const
		{
			return m_value;
		}
	private:
		char m_value[128];
	};

	class btVector3;
	class be_net_type_Vector3
	{
	public:
		be_net_type_Vector3() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {};
		be_net_type_Vector3(const be_net_type_Vector3& vector) : m_x(vector.x()), m_y(vector.y()), m_z(vector.z()) { };
		be_net_type_Vector3(const btVector3& vector);
		be_net_type_Vector3(BEFloat x, BEFloat y, BEFloat z) : m_x(x), m_y(y), m_z(z) { };
		operator btVector3() const;
		bool operator!=(const be_net_type_Vector3& rhs) const;
		bool operator==(const be_net_type_Vector3& rhs) const;
		be_net_type_Vector3 operator+=(const be_net_type_Vector3& rhs) const;
		be_net_type_Vector3 operator-=(const be_net_type_Vector3& rhs) const;
		
		BEFloat x() const { return m_x; }
		BEFloat y() const { return m_y; }
		BEFloat z() const { return m_z; }
		void setX( const BEFloat value ) { m_x=value; }
		void setY( const BEFloat value ) { m_y=value; }
		void setZ( const BEFloat value ) { m_z=value; }
	private:
		BEFloat m_x;
		BEFloat m_y;
		BEFloat m_z;
	};

	class btVector4;
	class be_net_type_Vector4
	{
	public:
		be_net_type_Vector4() : m_x(0.0f), m_y(0.0f), m_z(0.0f), m_w(0.0f) {};
		be_net_type_Vector4(const be_net_type_Vector4& vector) : m_x(vector.x()), m_y(vector.y()), m_z(vector.z()), m_w(vector.w()) { };
		be_net_type_Vector4(const btVector4& vector);
		be_net_type_Vector4(BEFloat x, BEFloat y, BEFloat z, BEFloat w) : m_x(x), m_y(y), m_z(z), m_w(w) { };
		bool operator!=(const be_net_type_Vector4& rhs) const;
		bool operator==(const be_net_type_Vector4& rhs) const;
		BEFloat x() const { return m_x; }
		BEFloat y() const { return m_y; }
		BEFloat z() const { return m_z; }
		BEFloat w() const { return m_w; }
		virtual void setX( const BEFloat value ) { m_x=value; }
		virtual void setY( const BEFloat value ) { m_y=value; }
		virtual void setZ( const BEFloat value ) { m_z=value; }
		virtual void setW( const BEFloat value ) { m_w=value; }
	protected:
		BEFloat m_x;
		BEFloat m_y;
		BEFloat m_z;
		BEFloat m_w;
	};

	class btQuaternion;
	class be_net_type_Quaternion : public be_net_type_Vector4
	{
	public:
		
// 		be_net_type_Quaternion() : be_net_type_Vector4( 0.0f, 0.0f, 0.0f, 1.0f ) {};
// 		be_net_type_Quaternion(BEFloat x, BEFloat y, BEFloat z, BEFloat w) : be_net_type_Vector4( x, y, z, w ) { };
// 		be_net_type_Quaternion(const btQuaternion& q);
// 		be_net_type_Quaternion(const be_net_type_Quaternion& q);


		be_net_type_Quaternion() : be_net_type_Vector4( 0.0f, 0.0f, 0.0f, 1.0f ) {};
		be_net_type_Quaternion(BEFloat x, BEFloat y, BEFloat z, BEFloat w) : be_net_type_Vector4( x, y, z, w ) { };
		be_net_type_Quaternion(const btQuaternion& q);
		be_net_type_Quaternion( const be_net_type_Vector4& q) : be_net_type_Vector4( q ) { };
		operator btQuaternion() const;
	};

	class btTransform;

	class be_net_type_Transform
	{
	public:
		be_net_type_Transform() {};
		be_net_type_Transform(const be_net_type_Transform& transform)
		{
			m_position = transform.getPosition();
			m_rotation = transform.getRotation();
		}
		be_net_type_Transform(const btTransform& transform);
		operator btTransform() const;
		bool operator!=(const be_net_type_Transform& rhs) const;

		const be_net_type_Vector3& getOrigin() const { return m_position ; }
		const be_net_type_Vector3& getPosition() const { return m_position ; }
		void setPosition( const be_net_type_Vector3& position ) { m_position = position; }
		void setOrigin( const be_net_type_Vector3& position ) { m_position = position; }

		const be_net_type_Quaternion& getRotation() const { return m_rotation ; }
		void setRotation( const be_net_type_Quaternion& rotation ) { m_rotation = rotation; }
		void setIdentity();
		
		bool isValid() const
		{
			if ( this != this )
				return false;
			return true;

// 			if (   m_position.x() != m_position.x()
// 				|| m_position.y() != m_position.y()
// 				|| m_position.z() != m_position.z()
// 			) return false;
// 
// 			if (   m_rotation.x() != m_rotation.x()
// 				|| m_rotation.y() != m_rotation.y()
// 				|| m_rotation.z() != m_rotation.z()
// 				|| m_rotation.w() != m_rotation.w()
// 			) return false;
// 
// 			return true;
		}
		
		void postInvalid( const std::string& header, const bool die ) const
		{
// 			std::cout << " testing: " << header << std::endl;
			if ( !isValid() )
			{
				std::cout << " " << header << ", x: " << getOrigin().x() << ", y: " << getOrigin().y() << ", z: " << getOrigin().z() << std::endl;
				std::cout << " " << header << ", x: " << getRotation().x() << ", y: " << getRotation().y() << ", z: " << getRotation().z() << ", w: " << getRotation().w() << std::endl;
				if ( die )
				{
// 					char* n = (char*)malloc(1);
// 					int a;
// 					memcpy(&n[0],&a,100000);
					exit(1);
				}
			}
		}

		
	private:
		be_net_type_Vector3 m_position;
		be_net_type_Quaternion m_rotation;
	};

// }

typedef be_net_type_String BEString;
typedef be_net_type_Vector3 BEVector3;
typedef be_net_type_Vector4 BEVector4;
typedef be_net_type_Transform BETransform;
typedef be_net_type_Quaternion BEQuaternion;
















class BeSignalHistory_String;
class BeSignalHistory
{
	public:
		BeSignalHistory() {};
		BeSignalHistory( const BeSignalHistory_String& signal_history );

		virtual ~BeSignalHistory(){};
		void addID( const unsigned int new_id )
		{
			m_entity_history[new_id]=true;
		}
		bool hasID( const unsigned int find_id ) const;
		bool hasIDorAdd( const unsigned int find_id, const unsigned int add_id );
		const std::unordered_map<unsigned int, bool>& getList() const { return m_entity_history; }

	private:
		std::unordered_map<unsigned int, bool> m_entity_history;
};

class BeSignalHistory_String
{
	public:
		BeSignalHistory_String() : m_size(0), m_history_string(0) {};
		BeSignalHistory_String( const BeSignalHistory& signal_history );
		virtual ~BeSignalHistory_String();
		char* getString() const { return m_history_string; }
		unsigned int size() const { return m_size; }
		void initString();
		char* getHistoryString() const { return m_history_string; }
		void setHistoryString( char* history_string ) { m_history_string = history_string; }
		
		void free() const
		{
			if ( size() > 0 )
				delete[] getHistoryString();
		}
		
		
		
// 		const std::string getStdString() const
// 		{
// 			std::string n(m_history_string, m_history_string+m_size);
// // 			for ( unsigned int i(0); i < m_size; ++i )
// // 				n.append(m_history_string[i]);
// 			return n;
// 			
// // 			std::string str(buf, buf + l);
// 			
// 		}

	private:
		unsigned int m_size;
		char* m_history_string; // FIXME TO PRIVATE

// 		std::string m_history_string;
// 		char* m_value[128];
};







struct net_message_t
{
public:
	net_message_t() : m_type(0) {};
	~net_message_t( ){};
	int type() const { return m_type; }
	const BeSignalHistory_String& getHistory() const { return m_entity_history; }
	
	unsigned int sizeAddition() const
	{
		return m_entity_history.size();
	}
	BeSignalHistory_String m_entity_history;
	
protected:
	net_message_t(const int type) : m_type(type) {};
	net_message_t(const int type, const BeSignalHistory& entity_history) : m_entity_history(entity_history), m_type(type)
	{
// 		std::cout << "net_message_t: MESSAGE NEWS" << std::endl;
// 		std::cout << "net_message_t:  size: " << sizeof(m_entity_history) << std::endl;
// 		std::cout << "net_message_t:  items: " << m_entity_history.getList().size() << std::endl;
// 		m_entity_history = entity_history;
// 		std::cout << "net_message_t:  post size: " << sizeof(m_entity_history) << std::endl;
// 		std::cout << "net_message_t:  post items: " << m_entity_history.getList().size() << std::endl;

// 		std::cout << "net_message_t: MESSAGE NEWS" << std::endl;
// 		std::cout << "net_message_t:  size: " << sizeof(m_entity_history) << std::endl;
// 		std::cout << "net_message_t:  items: " << m_entity_history.size() << std::endl;
	};
private:
	int m_type;
};

class BeMessageHandlerI
{
public:
	virtual void handle(const net_message_t& message) = 0;
};

template<typename T>
class BeMessageHandler : public BeMessageHandlerI
{
	typedef const T& Argument;
	typedef boost::function<void (Argument)> Function;
public:
	BeMessageHandler(Function function) : m_function(function)
	{
	}
	virtual void handle(const net_message_t& message)
	{
		Argument argument(*static_cast<const T*>(&message));
		m_function(argument);
	}
	Function m_function;
};

class BeMessageManager
{
	typedef std::unordered_map< int, boost::shared_ptr<BeMessageHandlerI> > Map;
public:
	template<typename T>
	void setHandler(boost::function<void (const T&)> function)
	{
		T t;
		const Map::iterator it=m_map.find( t.type() );
		if(it!=m_map.end())
		{
			m_map.erase(it);
		}
		m_map[ t.type() ]=boost::make_shared< BeMessageHandler<T> >(function);
	}
	bool handle(const net_message_t& message)
	{
		const Map::const_iterator iterator=m_map.find(message.type());
		if(iterator!=m_map.end())
		{
			iterator->second->handle(message);
			return true;
		}
		else
		{
			//Unhandled message
			std::cout << "unable to find message type : " << message.type() << std::endl;
			return false;
		}
	}
	Map m_map;
};




template<typename T>
class BeMessageHandlerI1
{
public:
	virtual void handle(T t, const net_message_t& message) = 0;
};

template<typename T, typename U>
class BeMessageHandler1 : public BeMessageHandlerI1<T>
{
	typedef const U& Argument;
	typedef boost::function<void (T, Argument)> Function;
public:
	BeMessageHandler1(Function function) : m_function(function)
	{
	}
	virtual void handle(T t, const net_message_t& message)
	{
		Argument argument(*static_cast<const U*>(&message));
		m_function(t, argument);
	}
	Function m_function;
};

template<typename T>
class BeMessageManager1
{
	typedef std::unordered_map< int, boost::shared_ptr<BeMessageHandlerI1<T> > > Map;
public:
	template<typename U>
	void setHandler(boost::function<void (T, const U&)> function)
	{
		U t;
		const typename Map::iterator it=m_map.find( t.type() );
		if(it!=m_map.end())
		{
			m_map.erase(it);
		}
		m_map[ t.type() ]=boost::make_shared< BeMessageHandler1<T, U> >(function);
	}
	bool handle(T t, const net_message_t& message)
	{
		const typename Map::const_iterator iterator=m_map.find(message.type());
		if(iterator!=m_map.end())
		{
			iterator->second->handle(t, message);
			return true;
		}
		else
		{
			//Unhandled message
			return false;
		}
	}
	bool handle(const net_message_t& message)
	{
		const typename Map::const_iterator iterator=m_map.find(message.type());
		if(iterator!=m_map.end())
		{
			iterator->second->handle(0, message);
			return true;
		}
		else
		{
			//Unhandled message
			return false;
		}
	}
	Map m_map;
};

#endif
































// #ifndef BE_NET_MESSAGE_H_INCLUDED
// #define BE_NET_MESSAGE_H_INCLUDED
// 
// #include <iostream>
// #include <string.h>
// #include <cstdint>
// #include <unordered_map>
// 
// #include <boost/shared_ptr.hpp>
// #include <boost/make_shared.hpp>
// // #include "boost/circular_buffer.hpp"
// // #include "boost/circular_buffer/space_optimized.hpp"
// #include <boost/function.hpp>
// 
// typedef std::float_t BEFloat;
// typedef std::double_t BEDouble;
// typedef std::uint32_t BEUInt;
// typedef std::int32_t BEInt;
// 
// // namespace BeNetTypes
// // {
// 	class be_net_type_String
// 	{
// 	public:
// 		be_net_type_String() { memset( m_value, 0, sizeof(m_value) ); }
// 		be_net_type_String(const char* const value)
// 		{
// 			memset( m_value, 0, sizeof(m_value) );
// 			if(value)
// 			{
// 				size_t length=strlen(value);
// 				if(length>127)
// 				{
// 					length=127;
// 				}
// 				if(length>0)
// 				{
// 					memcpy( m_value, value, length );
// 				}
// 			}
// 		}
// 		operator const char*() const
// 		{
// 			return m_value;
// 		}
// 	private:
// 		char m_value[128];
// 	};
// 
// 	class btVector3;
// 	class be_net_type_Vector3
// 	{
// 	public:
// 		be_net_type_Vector3() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {};
// 		be_net_type_Vector3(const be_net_type_Vector3& vector) : m_x(vector.x()), m_y(vector.y()), m_z(vector.z()) { };
// 		be_net_type_Vector3(const btVector3& vector);
// 		be_net_type_Vector3(BEFloat x, BEFloat y, BEFloat z) : m_x(x), m_y(y), m_z(z) { };
// 		operator btVector3() const;
// 		bool operator!=(const be_net_type_Vector3& rhs) const;
// 		bool operator==(const be_net_type_Vector3& rhs) const;
// 		
// 		const float x() const { return m_x; }
// 		const float y() const { return m_y; }
// 		const float z() const { return m_z; }
// 		void setX( const BEFloat value ) { m_x=value; }
// 		void setY( const BEFloat value ) { m_y=value; }
// 		void setZ( const BEFloat value ) { m_z=value; }
// 	private:
// 		BEFloat m_x;
// 		BEFloat m_y;
// 		BEFloat m_z;
// 	};
// 
// 	class btVector4;
// 	class be_net_type_Vector4
// 	{
// 	public:
// 		be_net_type_Vector4() : m_x(0.0f), m_y(0.0f), m_z(0.0f), m_w(1.0f) {};
// 		be_net_type_Vector4(const be_net_type_Vector4& vector) : m_x(vector.x()), m_y(vector.y()), m_z(vector.z()), m_w(vector.w()) { };
// 		be_net_type_Vector4(const btVector4& vector);
// 		be_net_type_Vector4(BEFloat x, BEFloat y, BEFloat z, BEFloat w) : m_x(x), m_y(y), m_z(z), m_w(w) { };
// 		bool operator!=(const be_net_type_Vector4& rhs) const;
// 		bool operator==(const be_net_type_Vector4& rhs) const;
// // 		operator btVector4() const
// // 		{
// // 			btVector4 v((float)m_x, (float)m_y, (float)m_z, (float)m_w);
// // 			return v;
// // 		}
// 		const float x() const { return m_x; }
// 		const float y() const { return m_y; }
// 		const float z() const { return m_z; }
// 		const float w() const { return m_w; }
// 		void setX( const BEFloat value ) { m_x=value; }
// 		void setY( const BEFloat value ) { m_y=value; }
// 		void setZ( const BEFloat value ) { m_z=value; }
// 		void setW( const BEFloat value ) { m_w=value; }
// 	protected:
// 		BEFloat m_x;
// 		BEFloat m_y;
// 		BEFloat m_z;
// 		BEFloat m_w;
// 	};
// 
// 	class btQuaternion;
// 	
// 	class be_net_type_Quaternion : public be_net_type_Vector4
// 	{
// 	public:
// 		be_net_type_Quaternion() : be_net_type_Vector4() {};
// 		be_net_type_Quaternion(BEFloat x, BEFloat y, BEFloat z, BEFloat w) : be_net_type_Vector4( x, y, z, w ) { };
// 		be_net_type_Quaternion( const be_net_type_Vector4& q) : be_net_type_Vector4( q ) { };
// 		be_net_type_Quaternion(const btQuaternion& q);
// 		operator btQuaternion() const;
// 	};
// 
// 	class btTransform;
// 
// 	class be_net_type_Transform
// 	{
// 	public:
// 		be_net_type_Transform() {};
// 		be_net_type_Transform(const be_net_type_Transform& transform)
// 		{
// 			m_position = transform.getPosition();
// 			m_rotation = transform.getRotation();
// 		}
// 		be_net_type_Transform(const btTransform& transform);
// 		operator btTransform() const;
// 		bool operator!=(const be_net_type_Transform& rhs) const;
// 
// 		const be_net_type_Vector3& getOrigin() const { return m_position ; }
// 		const be_net_type_Vector3& getPosition() const { return m_position ; }
// 		void setPosition( const be_net_type_Vector3& position ) { m_position = position; }
// 		void setOrigin( const be_net_type_Vector3& position ) { m_position = position; }
// 
// 		const be_net_type_Quaternion& getRotation() const { return m_rotation ; }
// 		void setRotation( const be_net_type_Quaternion& rotation ) { m_rotation = rotation; }
// 		void setIdentity();
// 		
// 		
// 		
// 		const bool isValid() const
// 		{
// 			if ( this != this )
// 				return false;
// 			return true;
// 
// // 			if (   m_position.x() != m_position.x()
// // 				|| m_position.y() != m_position.y()
// // 				|| m_position.z() != m_position.z()
// // 			) return false;
// // 
// // 			if (   m_rotation.x() != m_rotation.x()
// // 				|| m_rotation.y() != m_rotation.y()
// // 				|| m_rotation.z() != m_rotation.z()
// // 				|| m_rotation.w() != m_rotation.w()
// // 			) return false;
// // 
// // 			return true;
// 		}
// 		
// 		void postInvalid( const std::string& header, const bool die ) const
// 		{
// // 			std::cout << " testing: " << header << std::endl;
// 			if ( !isValid() )
// 			{
// 				std::cout << " " << header << ", x: " << getOrigin().x() << ", y: " << getOrigin().y() << ", z: " << getOrigin().z() << std::endl;
// 				std::cout << " " << header << ", x: " << getRotation().x() << ", y: " << getRotation().y() << ", z: " << getRotation().z() << ", w: " << getRotation().w() << std::endl;
// 				if ( die )
// 				{
// // 					char* n = (char*)malloc(1);
// // 					int a;
// // 					memcpy(&n[0],&a,100000);
// 					exit(1);
// 				}
// 			}
// 		}
// 		
// 	private:
// 		be_net_type_Vector3 m_position;
// 		be_net_type_Quaternion m_rotation;
// 	};
// 
// // }
// 
// typedef be_net_type_String BEString;
// typedef be_net_type_Vector3 BEVector3;
// typedef be_net_type_Vector4 BEVector4;
// typedef be_net_type_Transform BETransform;
// typedef be_net_type_Quaternion BEQuaternion;
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// class BeSignalHistory_String;
// class BeSignalHistory
// {
// 	public:
// 		BeSignalHistory() {};
// 		BeSignalHistory( const BeSignalHistory_String& signal_history );
// 
// 		virtual ~BeSignalHistory(){};
// 		void addID( const unsigned int new_id )
// 		{
// 			m_entity_history[new_id]=true;
// 		}
// 		const bool hasID( const unsigned int find_id ) const;
// 		const bool hasIDorAdd( const unsigned int find_id, const unsigned int add_id );
// 		const std::unordered_map<unsigned int, bool>& getList() const { return m_entity_history; }
// 
// 	private:
// 		std::unordered_map<unsigned int, bool> m_entity_history;
// };
// 
// class BeSignalHistory_String
// {
// 	public:
// 		BeSignalHistory_String() : m_size(0), m_history_string(0) {};
// 		BeSignalHistory_String( const BeSignalHistory& signal_history );
// 		virtual ~BeSignalHistory_String();
// 		char* getString() const { return m_history_string; }
// 		const unsigned int size() const { return m_size; }
// 		void initString();
// 		const char* getHistoryString() const { return m_history_string; }
// 		void setHistoryString( char* history_string ) { m_history_string = history_string; }
// 		
// 		void free() const
// 		{
// 			if ( size() > 0 )
// 				delete[] m_history_string;
// 		}
// 		
// 		
// 		
// // 		const std::string getStdString() const
// // 		{
// // 			std::string n(m_history_string, m_history_string+m_size);
// // // 			for ( unsigned int i(0); i < m_size; ++i )
// // // 				n.append(m_history_string[i]);
// // 			return n;
// // 			
// // // 			std::string str(buf, buf + l);
// // 			
// // 		}
// 
// 	private:
// 		char* m_history_string; // FIXME TO PRIVATE
// 		unsigned int m_size;
// 
// // 		std::string m_history_string;
// // 		char* m_value[128];
// };
// 
// 
// 
// 
// 
// 
// 
// struct net_message_t
// {
// public:
// 	net_message_t(){};
// 	~net_message_t(){};
// 	int type() const { return m_type; }
// 	const BeSignalHistory_String& getHistory() const { return m_entity_history; }
// 	
// 	const unsigned int sizeAddition() const
// 	{
// 		return m_entity_history.size();
// 	}
// 	BeSignalHistory_String m_entity_history;
// 	
// protected:
// 	net_message_t(const int type) : m_type(type) {};
// 	net_message_t(const int type, const BeSignalHistory& entity_history) : m_type(type), m_entity_history(entity_history)
// 	{
// // 		std::cout << "net_message_t: MESSAGE NEWS" << std::endl;
// // 		std::cout << "net_message_t:  size: " << sizeof(m_entity_history) << std::endl;
// // 		std::cout << "net_message_t:  items: " << m_entity_history.getList().size() << std::endl;
// // 		m_entity_history = entity_history;
// // 		std::cout << "net_message_t:  post size: " << sizeof(m_entity_history) << std::endl;
// // 		std::cout << "net_message_t:  post items: " << m_entity_history.getList().size() << std::endl;
// 
// // 		std::cout << "net_message_t: MESSAGE NEWS" << std::endl;
// // 		std::cout << "net_message_t:  size: " << sizeof(m_entity_history) << std::endl;
// // 		std::cout << "net_message_t:  items: " << m_entity_history.size() << std::endl;
// 	};
// private:
// 	int m_type;
// };
// 
// class BeMessageHandlerI
// {
// public:
// 	virtual void handle(const net_message_t& message) = 0;
// };
// 
// template<typename T>
// class BeMessageHandler : public BeMessageHandlerI
// {
// 	typedef const T& Argument;
// 	typedef boost::function<void (Argument)> Function;
// public:
// 	BeMessageHandler(Function function) : m_function(function)
// 	{
// 	}
// 	virtual void handle(const net_message_t& message)
// 	{
// 		Argument argument(*static_cast<const T*>(&message));
// 		m_function(argument);
// 	}
// 	Function m_function;
// };
// 
// class BeMessageManager
// {
// 	typedef std::unordered_map< int, boost::shared_ptr<BeMessageHandlerI> > Map;
// public:
// 	template<typename T>
// 	void setHandler(boost::function<void (const T&)> function)
// 	{
// 		T t;
// 		const Map::iterator it=m_map.find( t.type() );
// 		if(it!=m_map.end())
// 		{
// 			m_map.erase(it);
// 		}
// 		m_map[ t.type() ]=boost::make_shared< BeMessageHandler<T> >(function);
// 	}
// 	bool handle(const net_message_t& message)
// 	{
// 		const Map::const_iterator iterator=m_map.find(message.type());
// 		if(iterator!=m_map.end())
// 		{
// 			iterator->second->handle(message);
// 			return true;
// 		}
// 		else
// 		{
// 			//Unhandled message
// 			std::cout << "unable to find message type : " << message.type() << std::endl;
// 			return false;
// 		}
// 	}
// 	Map m_map;
// };
// 
// 
// 
// 
// template<typename T>
// class BeMessageHandlerI1
// {
// public:
// 	virtual void handle(T t, const net_message_t& message) = 0;
// };
// 
// template<typename T, typename U>
// class BeMessageHandler1 : public BeMessageHandlerI1<T>
// {
// 	typedef const U& Argument;
// 	typedef boost::function<void (T, Argument)> Function;
// public:
// 	BeMessageHandler1(Function function) : m_function(function)
// 	{
// 	}
// 	virtual void handle(T t, const net_message_t& message)
// 	{
// 		Argument argument(*static_cast<const U*>(&message));
// 		m_function(t, argument);
// 	}
// 	Function m_function;
// };
// 
// template<typename T>
// class BeMessageManager1
// {
// 	typedef std::unordered_map< int, boost::shared_ptr<BeMessageHandlerI1<T> > > Map;
// public:
// 	template<typename U>
// 	void setHandler(boost::function<void (T, const U&)> function)
// 	{
// 		U t;
// 		const typename Map::iterator it=m_map.find( t.type() );
// 		if(it!=m_map.end())
// 		{
// 			m_map.erase(it);
// 		}
// 		m_map[ t.type() ]=boost::make_shared< BeMessageHandler1<T, U> >(function);
// 	}
// 	bool handle(T t, const net_message_t& message)
// 	{
// 		const typename Map::const_iterator iterator=m_map.find(message.type());
// 		if(iterator!=m_map.end())
// 		{
// 			iterator->second->handle(t, message);
// 			return true;
// 		}
// 		else
// 		{
// 			//Unhandled message
// 			return false;
// 		}
// 	}
// 	bool handle(const net_message_t& message)
// 	{
// 		const typename Map::const_iterator iterator=m_map.find(message.type());
// 		if(iterator!=m_map.end())
// 		{
// 			iterator->second->handle(0, message);
// 			return true;
// 		}
// 		else
// 		{
// 			//Unhandled message
// 			return false;
// 		}
// 	}
// 	Map m_map;
// };
// 
// #endif

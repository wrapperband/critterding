#include "LinearMath/btTransform.h"
#include "LinearMath/btQuaternion.h"
#include <sstream>

#include "be_net_message.h"


// BeSignalHistory

	BeSignalHistory::BeSignalHistory( const BeSignalHistory_String& signal_history )
	{
		// set size
			const unsigned int size = signal_history.size();

		if ( size > 0 )
		{
			// write id's
				for ( unsigned int count(0); count < size; count+=sizeof(unsigned int) )
				{
					unsigned int getid;
					memcpy( &getid, &signal_history.getString()[count], sizeof(unsigned int) );

// 					std::cout << " adding " << getid << std::endl;
					addID( getid );
				}
		}
	}
	bool BeSignalHistory::hasID( const unsigned int find_id ) const
	{
		const auto& result(m_entity_history.find(find_id));
		if ( result != m_entity_history.end() )
			return true;
		return false;
	}
	bool BeSignalHistory::hasIDorAdd( const unsigned int find_id, const unsigned int add_id )
	{
		const bool has( hasID(find_id) );
		if ( !has )
			addID(add_id);
// 		else
// 			std::cout << "found, not adding " << find_id << std::endl;
// 		std::cout << "has: " << find_id << " : " << has << std::endl;
		return has;
	}
	

// BeSignalHistory_String

	BeSignalHistory_String::BeSignalHistory_String( const BeSignalHistory& signal_history )
		: m_size(signal_history.getList().size()*sizeof(unsigned int))
		, m_history_string(0)
	{
		if ( m_size > 0 )
		{
		// set memory
			m_history_string = new char[m_size];

		// write id's
			unsigned int count(0);
			for ( auto it(signal_history.getList().begin()); it != signal_history.getList().end(); ++it )
			{
				const unsigned int id(it->first);
				memcpy( &m_history_string[count], &id, sizeof(unsigned int) );
				count+=sizeof(unsigned int);
			}
		}
	}

// 	void BeSignalHistory_String::initString()
// 	{
// 			if ( m_size > 0 )
// 				m_history_string = new char[m_size];
// 	}

	BeSignalHistory_String::~BeSignalHistory_String()
	{
		// delete memory
			if ( m_size > 0 )
			{
				delete[] m_history_string;
// 				std::cout << "deleted size " << m_size << std::endl;
			}
	}


// // be_net_type_String
// 	be_net_type_String::operator std::string() const
// 	{
// // 		btVector3 v((float)m_x, (float)m_y, (float)m_z);
// 		return m_value;
// 	}
	
// be_net_type_Vector3

	be_net_type_Vector3::be_net_type_Vector3(const btVector3& vector) : m_x(vector.getX()), m_y(vector.getY()), m_z(vector.getZ()) {};
	be_net_type_Vector3::operator btVector3() const
	{
		btVector3 v((float)m_x, (float)m_y, (float)m_z);
		return v;
	}
	bool be_net_type_Vector3::operator!=(const be_net_type_Vector3& rhs) const
	{
		if ( x() == rhs.x() )
			if ( y() == rhs.y() )
				if ( z() == rhs.z() )
					return false;
		return true;
	}
	bool be_net_type_Vector3::operator==(const be_net_type_Vector3& rhs) const
	{
		if ( x() == rhs.x() )
			if ( y() == rhs.y() )
				if ( z() == rhs.z() )
					return true;
		return false;
	}
	be_net_type_Vector3 be_net_type_Vector3::operator+=(const be_net_type_Vector3& rhs) const
	{
		return be_net_type_Vector3
		(
			  x() + rhs.x()
			, y() + rhs.y()
			, z() + rhs.z()
		);
	}
	be_net_type_Vector3 be_net_type_Vector3::operator-=(const be_net_type_Vector3& rhs) const
	{
		return be_net_type_Vector3
		(
			  x() - rhs.x()
			, y() - rhs.y()
			, z() - rhs.z()
		);
	}

// be_net_type_Vector4
	be_net_type_Vector4::be_net_type_Vector4(const btVector4& vector) : m_x(vector.getX()), m_y(vector.getY()), m_z(vector.getZ()), m_w(vector.getW()) {};
	bool be_net_type_Vector4::operator!=(const be_net_type_Vector4& rhs) const
	{
		if ( x() == rhs.x() )
			if ( y() == rhs.y() )
				if ( z() == rhs.z() )
					if ( w() == rhs.w() )
						return false;
		return true;
	}
	bool be_net_type_Vector4::operator==(const be_net_type_Vector4& rhs) const
	{
		if ( x() == rhs.x() )
			if ( y() == rhs.y() )
				if ( z() == rhs.z() )
					if ( w() == rhs.w() )
						return true;
		return false;
	}


// be_net_type_Quaternion

	be_net_type_Quaternion::be_net_type_Quaternion(const btQuaternion& q) : be_net_type_Vector4(q.x(), q.y(), q.z(), q.w()) { };
	be_net_type_Quaternion::operator btQuaternion() const
	{
		// FIXME DON'T THINK THIS IS NEEDED ANYMORE
		btTransform NEWtransform;
		NEWtransform.setIdentity();
		NEWtransform.setRotation( btQuaternion(m_x, m_y, m_z, m_w) );
		return NEWtransform.getRotation();
		
// 		btQuaternion q(m_x, m_y, m_z, m_w);
// 		return q;
	}

// be_net_type_Transform
	be_net_type_Transform::be_net_type_Transform(const btTransform& transform)
	{
		setIdentity();
		m_position = transform.getOrigin();
		m_rotation = transform.getRotation();

// 		std::stringstream to_post;
// 		to_post << "be_net_type_Transform::be_net_type_Transform(const btTransform& transform)";
// 		postInvalid( to_post.str(), true );

// 		if ( !isValid() )
// 			setIdentity();
	}
	be_net_type_Transform::operator btTransform() const
	{
		btTransform t;
		t.setIdentity();
		t.setOrigin(m_position);
		t.setRotation(btQuaternion(m_rotation));
		
// 		std::stringstream to_post;
// 		to_post << "be_net_type_Transform::operator btTransform";
// 		postInvalid( to_post.str(), true );
		
		return t;
	}
	void be_net_type_Transform::setIdentity()
	{
		m_position = be_net_type_Vector3(0.0f, 0.0f, 0.0f);
		m_rotation = be_net_type_Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	}
	bool be_net_type_Transform::operator!=(const be_net_type_Transform& rhs) const
	{
		if ( m_position == rhs.getOrigin() )
			if ( m_rotation == rhs.getRotation() )
				return false;
		return true;
	}
	





















// #include "LinearMath/btTransform.h"
// #include "LinearMath/btQuaternion.h"
// #include <sstream>
// 
// #include "be_net_message.h"
// 
// 
// // BeSignalHistory
// 
// 	BeSignalHistory::BeSignalHistory( const BeSignalHistory_String& signal_history )
// 	{
// 		// set size
// 			const unsigned int size = signal_history.size();
// 
// 		if ( size > 0 )
// 		{
// 			// write id's
// 				for ( unsigned int count(0); count < size; count+=sizeof(unsigned int) )
// 				{
// 					unsigned int getid;
// 					memcpy( &getid, &signal_history.getString()[count], sizeof(unsigned int) );
// 
// // 					std::cout << " adding " << getid << std::endl;
// 					addID( getid );
// 				}
// 		}
// 	}
// 	const bool BeSignalHistory::hasID( const unsigned int find_id ) const
// 	{
// 		const auto& result(m_entity_history.find(find_id));
// 		if ( result != m_entity_history.end() )
// 			return true;
// 		return false;
// 	}
// 	const bool BeSignalHistory::hasIDorAdd( const unsigned int find_id, const unsigned int add_id )
// 	{
// 		const bool has( hasID(find_id) );
// 		if ( !has )
// 			addID(add_id);
// // 		else
// // 			std::cout << "found, not adding " << find_id << std::endl;
// // 		std::cout << "has: " << find_id << " : " << has << std::endl;
// 		return has;
// 	}
// 	
// 
// // BeSignalHistory_String
// 
// 	BeSignalHistory_String::BeSignalHistory_String( const BeSignalHistory& signal_history )
// 		: m_size(signal_history.getList().size()*sizeof(unsigned int))
// 		, m_history_string(0)
// 	{
// 		if ( m_size > 0 )
// 		{
// 		// set memory
// 			m_history_string = new char[m_size];
// 
// 		// write id's
// 			unsigned int count(0);
// 			for ( auto it(signal_history.getList().begin()); it != signal_history.getList().end(); ++it )
// 			{
// 				const unsigned int id(it->first);
// 				memcpy( &m_history_string[count], &id, sizeof(unsigned int) );
// 				count+=sizeof(unsigned int);
// 			}
// 		}
// 	}
// 
// // 	void BeSignalHistory_String::initString()
// // 	{
// // 			if ( m_size > 0 )
// // 				m_history_string = new char[m_size];
// // 	}
// 
// 	BeSignalHistory_String::~BeSignalHistory_String()
// 	{
// 		// delete memory
// 			if ( m_size > 0 )
// 			{
// 				delete[] m_history_string;
// // 				std::cout << "deleted size " << m_size << std::endl;
// 			}
// 	}
// 
// 
// // // be_net_type_String
// // 	be_net_type_String::operator std::string() const
// // 	{
// // // 		btVector3 v((float)m_x, (float)m_y, (float)m_z);
// // 		return m_value;
// // 	}
// 	
// // be_net_type_Vector3
// 
// 	be_net_type_Vector3::be_net_type_Vector3(const btVector3& vector) : m_x(vector.getX()), m_y(vector.getY()), m_z(vector.getZ()) {};
// 	be_net_type_Vector3::operator btVector3() const
// 	{
// 		btVector3 v((float)m_x, (float)m_y, (float)m_z);
// 		return v;
// 	}
// 	bool be_net_type_Vector3::operator!=(const be_net_type_Vector3& rhs) const
// 	{
// 		if ( x() == rhs.x() )
// 			if ( y() == rhs.y() )
// 				if ( z() == rhs.z() )
// 					return false;
// 		return true;
// 	}
// 	bool be_net_type_Vector3::operator==(const be_net_type_Vector3& rhs) const
// 	{
// 		if ( x() == rhs.x() )
// 			if ( y() == rhs.y() )
// 				if ( z() == rhs.z() )
// 					return true;
// 		return false;
// 	}
// 
// // be_net_type_Vector4
// 	be_net_type_Vector4::be_net_type_Vector4(const btVector4& vector) : m_x(vector.getX()), m_y(vector.getY()), m_z(vector.getZ()), m_w(vector.getW()) {};
// 	bool be_net_type_Vector4::operator!=(const be_net_type_Vector4& rhs) const
// 	{
// 		if ( x() == rhs.x() )
// 			if ( y() == rhs.y() )
// 				if ( z() == rhs.z() )
// 					if ( w() == rhs.w() )
// 						return false;
// 		return true;
// 	}
// 	bool be_net_type_Vector4::operator==(const be_net_type_Vector4& rhs) const
// 	{
// 		if ( x() == rhs.x() )
// 			if ( y() == rhs.y() )
// 				if ( z() == rhs.z() )
// 					if ( w() == rhs.w() )
// 						return true;
// 		return false;
// 	}
// 
// 
// // be_net_type_Quaternion
// 
// 	be_net_type_Quaternion::be_net_type_Quaternion(const btQuaternion& q) : be_net_type_Vector4(q.x(), q.y(), q.z(), q.w()) { };
// 	be_net_type_Quaternion::operator btQuaternion() const
// 	{
// 		btQuaternion q((float)m_x, (float)m_y, (float)m_z, (float)m_w);
// 		return q;
// 	}
// 	
// 	
// // be_net_type_Transform
// 	be_net_type_Transform::be_net_type_Transform(const btTransform& transform)
// 	{
// 		m_position = transform.getOrigin();
// 		m_rotation = transform.getRotation();
// 	}
// 	be_net_type_Transform::operator btTransform() const
// 	{
// 		btTransform t;
// 		t.setIdentity();
// 		t.setOrigin(m_position);
// 		t.setRotation(m_rotation);
// 		return t;
// 	}
// 	void be_net_type_Transform::setIdentity()
// 	{
// 		m_position = be_net_type_Vector3(0.0f, 0.0f, 0.0f);
// 		m_rotation = be_net_type_Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
// 	}
// 	bool be_net_type_Transform::operator!=(const be_net_type_Transform& rhs) const
// 	{
// 		if ( m_position == rhs.getOrigin() )
// 			if ( m_rotation == rhs.getRotation() )
// 				return false;
// 		return true;
// 	}
// 	
#include "archbodypart.h"
#include <iostream>

archBodypart::archBodypart()
{
	type = 0;
	id = 0;
	x = 0;
	y = 0;
	z = 0;
	materialID = 0;
}

// void archBodypart::markTakenFace( const unsigned int face )
// {
// // 	std::cout << "marking face: " << face << std::endl;
// // 	std::cout << " freefaces pre: " << 5 - m_takenFaces.size() << std::endl;
// 	m_takenFaces[face] = true;
// // 	std::cout << " freefaces post: " << 5 - m_takenFaces.size() << std::endl;
// // 	std::cout << std::endl;
// }

// void archBodypart::unmarkTakenFace( const unsigned int face )
// {
// 	std::cout << "unmarking face: " << face << std::endl;
// 	std::cout << " taken faces pre: " << m_takenFaces.size() << std::endl;
// 	
// 	for ( auto it = m_takenFaces.begin(); it != m_takenFaces.end(); ++it )
// 	{
// 		if ( it->first == face )
// 		{
// 			m_takenFaces.erase(it);
// 			return;
// 		}
// 	}
// 	
// // 	const auto& it( m_takenFaces.find(face) );
// // 	if ( it != m_takenFaces.end() )
// // 		m_takenFaces.erase(it);
// 
// 	std::cout << " freefaces post: " << 5 - m_takenFaces.size() << std::endl;
// 	std::cout << std::endl;
// }

// const bool archBodypart::faceIsTaken( const unsigned int face ) const
// {
// 	const auto& it( m_takenFaces.find(face) );
// 	if ( it != m_takenFaces.end() )
// 		return true;
// 	
// 	return false;
// }

// const unsigned int archBodypart::numFreeFaces() const
// {
// // 	if ( m_takenFaces.size() > 0 )
// // 		std::cout << "number free faces: " << 5 - m_takenFaces.size() << std::endl;
// // 	std::cout << std::endl;
// 
// 	return 5 - m_takenFaces.size();
// }

archBodypart::~archBodypart()
{
}



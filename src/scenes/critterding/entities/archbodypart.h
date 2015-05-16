#ifndef ARCHBODYPART_H_INCLUDED
#define ARCHBODYPART_H_INCLUDED

#include <map>

class archBodypart
{
	public:
		archBodypart();
		~archBodypart();

		unsigned int type;
		unsigned int id;
		float x;
		float y;
		float z;
		unsigned int materialID;
// 		void markTakenFace( const unsigned int face );
// 		void unmarkTakenFace( const unsigned int face );
// 		const bool faceIsTaken( const unsigned int face ) const;
// 		const unsigned int numFreeFaces() const;

// 	private:
// 		std::map<unsigned int, bool> m_takenFaces;
};

#endif

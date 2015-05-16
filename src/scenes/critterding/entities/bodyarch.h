#ifndef BODYARCH_H
#define BODYARCH_H

#include "src/common/be_rand.h"
#include "src/common/be_parser.h"
#include "src/utils/settings.h"
#include "src/graphics/be_color.h"
#include "archbodypart.h"
#include "archconstraint.h"
#include "archmouth.h"
#include <vector>

class BodyArch
{
	public:
		BodyArch();
		~BodyArch();

		// load save architecture (serialize)
		void			buildArch();
		void			setArch(std::string* content);
		std::string*			getArch();
		void			copyFrom(const BodyArch* otherBody);
		void			mutate(const unsigned int runs);

		std::vector<archBodypart>	archBodyparts;
		std::vector<archConstraint>	archConstraints;
		std::vector<archMouth>	archMouths;
		BeColor			color;
		unsigned int		retinasize;

		unsigned int m_minenergyproc;
		unsigned int m_sightrange;
		unsigned int m_maxlifetime;
		unsigned int m_maxmutations;
		unsigned int m_mutationrate;

		float			totalWeight;

		int			findBodypart(const unsigned int id);
		int			findMouth( const unsigned int id );
	private:
		BeParser		parseH;
		BeParser		contentParser;
		BeRand			*randgen;
		Settings		*settings;

		float			bodypartspacer;
		void			repositiontoConstraints( archBodypart* bp );
		void			repositiontoConstraints( archMouth* bp );
		void			repositionConstraint( archConstraint* co, const unsigned int oneOrTwo, const btVector3& position );
		void			repositionConstraintAngle( archConstraint* co );
		// mutation helpers
		void			addRandomMouth();
		void			addRandomBodypart();
		void			addRandomConstraint(/*unsigned int connID1, */const unsigned int connID2, bool isMouth);
		void			removeBodypart(const unsigned int id);
		void			removeMouth(const unsigned int id);
		void			randomConstraintPosition(archConstraint* co, const unsigned int OneOrTwo, const unsigned int connID);

		unsigned int		getUniqueBodypartID();
		unsigned int		getUniqueConstraintID();

		std::string			archBuffer;
		const float		m_size_Factor;
		
		bool faceIsTaken( const archBodypart& archbodypart, const unsigned int face ) const;
		bool findOpenFace( unsigned int& bodypart, unsigned int& face ) const;

};
#endif

#include "bodyarch.h"
#include "LinearMath/btTransform.h"
#include <iostream>
#include <sstream>
// #include <iostream>
using namespace std;

BodyArch::BodyArch()
 : m_size_Factor(200.0f)
{
	settings = Settings::Instance();
	totalWeight = 0.0f;
	bodypartspacer = 2.05f;

	m_minenergyproc					= settings->getCVar("critter_minenergyproc");
	m_sightrange					= settings->getCVar("critter_sightrange");
	m_maxlifetime					= settings->getCVar("critter_maxlifetime");
	m_maxmutations					= settings->getCVar("body_maxmutations");
	m_mutationrate					= settings->getCVar("body_mutationrate");
}

void BodyArch::buildArch()
{
// 	string original;
// 	original.append("b 99999 box 0 75 75 200\n");
// 	original.append("b 99998 box 0 75 75 200\n");
// 	original.append("b 99997 box 0 75 75 200\n");
// 	original.append("m 99999 50, 50, 100\n");
// 	original.append("c 99999 99998 99999 1.5707 0 0 0 0 -0.25 -0.7853 99998 1.5707 0 0 0 0 0.25 0.7853\n");
// 	original.append("c 99997 99996 99998 1.5707 0 0 0 0 -0.25 -0.7853 99997 1.5707 0 0 0 0 0.25 0.7853\n");
// 	original.append("cm 99995 99994 99997 0 1.5707 0 0 0 -0.15 -0.7853 99999 0 1.5707 0 0 0 0.25 0.7853\n");
// 	setArch(&original);

// 	string fourlegged;
// 	// body
// 	fourlegged.append("b 99999 box 0 200 100 400\n");
// 	// 4 long legs
// 	fourlegged.append("b 99998 box 0 150 40 40\n");
// 	fourlegged.append("b 99997 box 0 150 40 40\n");
// 	fourlegged.append("b 99996 box 0 150 40 40\n");
// 	fourlegged.append("b 99995 box 0 150 40 40\n");
// 	// mouth
// 	fourlegged.append("m 99999 50, 50, 100\n");
// 	// connections
// 	fourlegged.append("c 99999 99998 99999 0 0 0.7853 -0.2 0 -0.325 -0.7853 99998 0 0 0 0.25 0 0 0.7853\n");
// 	fourlegged.append("c 99997 99996 99999 0 0 0.7853 -0.2 0 0.325 -0.7853 99997 0 0 0 0.25 0 0 0.7853\n");
// 	fourlegged.append("c 99995 99994 99999 0 0 -0.7853 0.2 0 -0.325 -0.7853 99996 0 0 0 -0.25 0 0 0.7853\n");
// 	fourlegged.append("c 99993 99992 99999 0 0 -0.7853 0.2 0 0.325 -0.7853 99995 0 0 0 -0.25 0 0 0.7853\n");
// 	fourlegged.append("cm 99991 99990 99999 1.5707 0 0 0 0 -0.15 -0.3926 99999 1.5707 0 0 0 0 0.45 0.3926\n");
// 	setArch(&fourlegged);



// 	string original;
// 	original.append("b 1000 box 0 200 200 200\n");
// 	original.append("b 1001 box 0 100 100 100\n");
// 	original.append("b 1002 box 0 100 100 100\n");
// 	original.append("b 1003 box 0 100 100 100\n");
// 	original.append("b 1004 box 0 100 100 100\n");
// 	original.append("m 99999 50, 50, 100\n");
// 
// 	-x
// 	original.append("c 1000 1001 1000 0.5707 1.3707 0.7707 -0.3 0 0 -0.7853 1001 1.1707 0.0707 0.5707 0.15 0 0 0.7853\n");
// 	-z
// 	original.append("c 1002 1003 1000 1.5707 1.5707 1.5707 0 0 -0.3 -0.7853 1002 1.5707 1.5707 1.5707 0 0 0.15 0.7853\n");
// 	+z
// 	original.append("c 1004 1005 1000 1.5707 0 0 0 0 0.3 -0.7853 1003 1.5707 0 0 0 0 -0.15 0.7853\n");
// 	+x
// 	original.append("c 1006 1007 1000 1.5707 0 0 0.3 0 0 -0.7853 1004 1.5707 0 0 -0.15 0 0 0.7853\n");
// 	setArch(&original);

	// Create a central body
// 		archBodyparts.push_back( archBodypart() );
// 		archBodypart *bp = &archBodyparts[archBodyparts.size()-1];
// 		bp->id		= 1000;
// 		bp->type	= 0;
// 		bp->materialID	= 0;
// 		bp->x		= randgen->Instance()->get( settings->getCVar("body_minbodypartsize"), settings->getCVar("body_maxbodypartsize") );
// 		bp->y		= randgen->Instance()->get( settings->getCVar("body_minbodypartsize"), settings->getCVar("body_maxbodypartsize") );
// 		bp->z		= randgen->Instance()->get( settings->getCVar("body_minbodypartsize"), settings->getCVar("body_maxbodypartsize") );

	// FIXME MAKE PERCENTAGES
	m_minenergyproc					= settings->getCVar("critter_minenergyproc");
	m_sightrange					= settings->getCVar("critter_sightrange");
	m_maxlifetime					= settings->getCVar("critter_maxlifetime");
	m_maxmutations					= settings->getCVar("body_maxmutations");
	m_mutationrate					= settings->getCVar("body_mutationrate");
	
	// add random more
		const unsigned int runs = randgen->Instance()->get( settings->getCVar("body_minbodypartsatbuildtime"), settings->getCVar("body_maxbodypartsatbuildtime") ); // -1 -> central bodypart
		for ( unsigned int i=0; i < runs; ++i )
			addRandomBodypart();

		addRandomMouth();
}

bool BodyArch::faceIsTaken( const archBodypart& archbodypart, const unsigned int face ) const
{
	
	for ( unsigned int i = 0; i < archConstraints.size(); ++i )
	{
		const archConstraint& c = archConstraints[i];
		if ( c.id_1 == archbodypart.id )
		{
			if ( c.XYZ1 == face )
				return true;
		}
	}
	return false;
}

bool BodyArch::findOpenFace( unsigned int& bodypart, unsigned int& face ) const
{
	const unsigned int last_bodypart( archBodyparts.size()-2 );

	bool isTaken(true);
	while ( isTaken )
	{
		bodypart = randgen->Instance()->get( 0, last_bodypart );
// 		face = randgen->Instance()->get( 0, 5 );
		face = randgen->Instance()->get( 0, 4 );

		isTaken = faceIsTaken( archBodyparts[bodypart], face );
	}

	return (!isTaken);
}

void BodyArch::removeBodypart(unsigned int id)
{
	// find constraints where this bodypart is id1, in order to remove connected bodyparts
	for ( unsigned int i = 0; i < archConstraints.size(); ++i )
	{
		archConstraint* c = &archConstraints[i];
		if ( c->id_1 == id )
		{
			if (  c->id_1 == c->id_2 )
			{
				cout << "WARNING: removeBodypart:: c->id_1 == c->id_2" << endl;
				exit(1);
			}
			
			if ( c->isMouthConstraint )
			{
				removeMouth( findMouth(c->id_2) );
			}
			else
			{
				removeBodypart( c->id_2 );
			}
		}
	}

	archBodyparts.erase( archBodyparts.begin() + findBodypart(id) );
}

void BodyArch::removeMouth(unsigned int id)
{
	archMouths.erase(archMouths.begin()+id);
}

void BodyArch::addRandomMouth()
{
	// Throw in a mouth
		archMouths.push_back( archMouth() );
		archMouth* mo = &archMouths[archMouths.size()-1];
		mo->id		= 1000;
		mo->x		= randgen->Instance()->get( settings->getCVar("body_minheadsize"), settings->getCVar("body_maxheadsize") );
		mo->y		= randgen->Instance()->get( settings->getCVar("body_minheadsize"), settings->getCVar("body_maxheadsize") );
		mo->z		= randgen->Instance()->get( settings->getCVar("body_minheadsize"), settings->getCVar("body_maxheadsize") );

	// Get it connected somehow

		const unsigned int connID2 = archMouths.size()-1;

		addRandomConstraint(/*connID1, */connID2, true);
}

void BodyArch::addRandomBodypart()
{
	// Throw in a bodypart
		archBodyparts.push_back( archBodypart() );
		archBodypart *bp = &archBodyparts[archBodyparts.size()-1];
		bp->id		= 0; // to avoid uninitialized id
		bp->id		= getUniqueBodypartID();
		bp->type	= 0;
		bp->materialID	= 0;
		bp->x		= randgen->Instance()->get( settings->getCVar("body_minbodypartsize"), settings->getCVar("body_maxbodypartsize") );
		bp->y		= randgen->Instance()->get( settings->getCVar("body_minbodypartsize"), settings->getCVar("body_maxbodypartsize") );
		bp->z		= randgen->Instance()->get( settings->getCVar("body_minbodypartsize"), settings->getCVar("body_maxbodypartsize") );

	// Get it connected to another bodypart, IF there are any
	if ( archBodyparts.size() > 1 )
	{
		const unsigned int connID2 = archBodyparts.size()-1;
		addRandomConstraint(connID2, false);
	}
}

void BodyArch::addRandomConstraint(/*unsigned int connID1, */unsigned int connID2, bool isMouth)
{
	
		unsigned int connect_to_bodypart(-1);
		unsigned int connect_to_face(-1);

		const bool found_open_face( findOpenFace( connect_to_bodypart, connect_to_face ) );
	
		if ( found_open_face )
		{
	// 			unsigned int connID1 = randgen->Instance()->get( 0, archBodyparts.size()-1 );
	// 			unsigned int attempt(0);
	// 			while ( (connID1 == connID2 || archBodyparts[connID1].numFreeFaces() == 0) && ++attempt < 300 )
	// 				connID1 = randgen->Instance()->get( 0, archBodyparts.size()-1 );
		
		
			archConstraints.push_back( archConstraint() );
			archConstraint *co = &archConstraints[archConstraints.size()-1];

			co->isMouthConstraint	= isMouth;
			// first initialize constraint id's
			co->constraint_id1	= 0;
			co->constraint_id2	= 0;
			co->constraint_id1	= getUniqueConstraintID();
			co->constraint_id2	= getUniqueConstraintID();
			co->id_1		= archBodyparts[ connect_to_bodypart ].id;
			if ( isMouth )
				co->id_2		= archMouths[ connID2 ].id;
			else
				co->id_2		= archBodyparts[ connID2 ].id;

			co->XYZ1 = connect_to_face;
			
	// 		co->rot_x_1		= ((float)randgen->Instance()->get( 0, 1571 ) - 0) / 1000;
	// 		co->rot_y_1		= ((float)randgen->Instance()->get( 0, 1571 ) - 0) / 1000;
	// 		co->rot_z_1		= ((float)randgen->Instance()->get( 0, 1571 ) - 0) / 1000;
	// 
	// 		co->rot_x_2		= ((float)randgen->Instance()->get( 0, 1571 ) - 0) / 1000;
	// 		co->rot_y_2		= ((float)randgen->Instance()->get( 0, 1571 ) - 0) / 1000;
	// 		co->rot_z_2		= ((float)randgen->Instance()->get( 0, 1571 ) - 0) / 1000;
	// 
	// 		co->limit_1		= (float)randgen->Instance()->get( 0, 7853 ) / -10000;
	// 		co->limit_2		= -1.0f * co->limit_1;

			co->pos_x_1		= 0.0f;
			co->pos_y_1		= 0.0f;
			co->pos_z_1		= 0.0f;
			co->pos_x_2		= 0.0f;
			co->pos_y_2		= 0.0f;
			co->pos_z_2		= 0.0f;
			
			co->rot_x_1		= 0.0f;
			co->rot_y_1		= 0.0f;
			co->rot_z_1		= 0.0f;

			co->rot_x_2		= 0.0f;
			co->rot_y_2		= 0.0f;
			co->rot_z_2		= 0.0f;

			co->limit_1		= -0.0001f * randgen->Instance()->get( 0, settings->getCVar("body_maxconstraintlimit") );
			co->limit_2		= 0.0001f * randgen->Instance()->get( 0, settings->getCVar("body_maxconstraintlimit") );
// 			co->limit_2		= -1.0f * co->limit_1;
			
			randomConstraintPosition(co, 1, connect_to_bodypart);
			randomConstraintPosition(co, 2, connID2);
		}
}

// void BodyArch::randomConstraintPositionOnPlane(archConstraint* co, const unsigned int OneOrTwo, btVector3& vector)
// {
// 	btVector3
// 	if ( OneOrTwo == 1 )
// 	{
// 		if ( co->XYZ1 == 0 ) // X
// 		{
// 			co->pos_y_1 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
// 			co->pos_z_1 = ((float)randgen->Instance()->get( 0, (bp.z)*2) - bp.z) / 100;
// 	}
// }

void BodyArch::randomConstraintPosition(archConstraint* co, const unsigned int OneOrTwo, const unsigned int connID)
{
	// FIXME rewrite this around oneortwo, just pass it through?
	if ( OneOrTwo == 1 )
	{
		archBodypart& bp( archBodyparts[connID] );

		repositionConstraint(co, 1, btVector3(bp.x, bp.y, bp.z));
		repositionConstraintAngle( co );
	}
	else
	{
		if ( !co->isMouthConstraint )
		{
			const archBodypart& bp( archBodyparts[connID] );
			repositionConstraint(co, 2, btVector3( bp.x, bp.y, bp.z ));
		}
		else
		{
			const archMouth& bp( archMouths[connID] );
			repositionConstraint(co, 2, btVector3( bp.x, bp.y, bp.z ));
		}
	}
}

void BodyArch::repositionConstraintAngle( archConstraint* co )
{
	co->rot_x_1 = 0.0f;
	co->rot_y_1 = 0.0f;
	co->rot_z_1 = 0.0f;
	
	const int max_angle( settings->getCVar("body_maxconstraintangle") );
	
// 	const float angle( 0.0001f * randgen->Instance()->get( -1571, 1571 ) );
// 	const float angle( 0.0001f * randgen->Instance()->get( -3141, 3141 ) );
	const float angle( 0.0001f * randgen->Instance()->get( -max_angle, max_angle ) );
// 	const float angle( 0.0f );

// 	std::cout << angle << std::endl;
	
	if ( co->XYZ1 == 0 )
		co->rot_x_1 = angle;
	else if ( co->XYZ1 == 1 )
		co->rot_y_1 = angle;
	else if ( co->XYZ1 == 2 )
		co->rot_y_1 = angle;
	else if ( co->XYZ1 == 3 )
		co->rot_z_1 = angle;
	else if ( co->XYZ1 == 4 )
		co->rot_z_1 = angle;
}

void BodyArch::repositionConstraint( archConstraint* co, const unsigned int OneOrTwo, const btVector3& position )
{
	const float ratio( 0.0001f *  settings->getCVar("body_constraintpositionrange"));
	
	if ( OneOrTwo == 1 )
	{
		co->pos_x_1 = 0.0f;
		co->pos_y_1 = 0.0f;
		co->pos_z_1 = 0.0f;
		co->rot_x_1 = 0.0f;
		co->rot_y_1 = 0.0f;
		co->rot_z_1 = 0.0f;
		co->rot_x_2 = 0.0f;
		co->rot_y_2 = 0.0f;
		co->rot_z_2 = 0.0f;

		const archBodypart& bp( archBodyparts[findBodypart(co->id_1)] );

		
		if ( co->XYZ1 == 0 ) // X
		{
			co->pos_x_1 = (position.x() / m_size_Factor) * bodypartspacer;

// 			co->pos_y_1 = (0.0001f * randgen->Instance()->get( 0, (bp.y)*2) - bp.y) * body_constraintpositionrange;
// 			co->pos_z_1 = (0.0001f * randgen->Instance()->get( 0, (bp.z)*2) - bp.z) * body_constraintpositionrange;

			// 			co->pos_y_1 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
// 			co->pos_z_1 = ((float)randgen->Instance()->get( 0, (bp.z)*2) - bp.z) / 100;

			co->pos_y_1 = ratio * (randgen->Instance()->get( 0, (bp.y)*2) - bp.y);
			co->pos_z_1 = ratio * (randgen->Instance()->get( 0, (bp.z)*2) - bp.z);
		}
		else if ( co->XYZ1 == 1 ) // Y
		{
			co->pos_y_1 = (position.y() / m_size_Factor) * bodypartspacer;
			co->rot_z_2 = -SIMD_HALF_PI;

// 			co->pos_x_1 = ((float)randgen->Instance()->get( 0, (bp.x)*2) - bp.x) / 100;
// 			co->pos_z_1 = ((float)randgen->Instance()->get( 0, (bp.z)*2) - bp.z) / 100;

			co->pos_x_1 = ratio * (randgen->Instance()->get( 0, (bp.x)*2) - bp.x);
			co->pos_z_1 = ratio * (randgen->Instance()->get( 0, (bp.z)*2) - bp.z);
		}
		else if ( co->XYZ1 == 2 ) // Y
		{
			co->pos_y_1 = (position.y() / m_size_Factor) * bodypartspacer * -1;
			co->rot_z_2 = SIMD_HALF_PI;

// 			co->pos_x_1 = ((float)randgen->Instance()->get( 0, (bp.x)*2) - bp.x) / 100;
// 			co->pos_z_1 = ((float)randgen->Instance()->get( 0, (bp.z)*2) - bp.z) / 100;
			co->pos_x_1 = ratio * (randgen->Instance()->get( 0, (bp.x)*2) - bp.x);
			co->pos_z_1 = ratio * (randgen->Instance()->get( 0, (bp.z)*2) - bp.z);
		}
		else if ( co->XYZ1 == 3 ) // Z
		{
			co->pos_z_1 = (position.z() / m_size_Factor) * bodypartspacer;
			co->rot_y_2 = SIMD_HALF_PI;

// 			co->pos_x_1 = ((float)randgen->Instance()->get( 0, (bp.x)*2) - bp.x) / 100;
// 			co->pos_y_1 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
			co->pos_x_1 = ratio * (randgen->Instance()->get( 0, (bp.x)*2) - bp.x);
			co->pos_y_1 = ratio * (randgen->Instance()->get( 0, (bp.y)*2) - bp.y);
		}
		else if ( co->XYZ1 == 4 ) // Z
		{
			co->pos_z_1 = (position.z() / m_size_Factor) * bodypartspacer * -1;
			co->rot_y_2 = -SIMD_HALF_PI;

// 			co->pos_x_1 = ((float)randgen->Instance()->get( 0, (bp.x)*2) - bp.x) / 100;
// 			co->pos_y_1 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
			co->pos_x_1 = ratio * (randgen->Instance()->get( 0, (bp.x)*2) - bp.x);
			co->pos_y_1 = ratio * (randgen->Instance()->get( 0, (bp.y)*2) - bp.y);
		}
	}
	else
	{
// 		co->pos_x_2 = 0.0f;
// 		co->pos_y_2 = 0.0f;
// 		co->pos_z_2 = 0.0f;

		if ( co->isMouthConstraint )
		{
			const archMouth& bp( archMouths[findMouth(co->id_2)] );
			co->pos_x_2 = (position.x() / m_size_Factor) * bodypartspacer;
			
// 			co->pos_y_2 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
// 			co->pos_z_2 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
			co->pos_y_2 = ratio * (randgen->Instance()->get( 0, (bp.y)*2) - bp.y);
			co->pos_z_2 = ratio * (randgen->Instance()->get( 0, (bp.y)*2) - bp.y);
		}
		else
		{
			const archBodypart& bp( archBodyparts[findBodypart(co->id_2)] );
			co->pos_x_2 = (position.x() / m_size_Factor) * bodypartspacer;

// 			co->pos_y_2 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
// 			co->pos_z_2 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
			co->pos_y_2 = ratio * (randgen->Instance()->get( 0, (bp.y)*2) - bp.y);
			co->pos_z_2 = ratio * (randgen->Instance()->get( 0, (bp.y)*2) - bp.y);
		}
			
			
// 		const int othersign(-1 * co->sign);
		co->pos_x_2 = (position.x() / m_size_Factor) * bodypartspacer * -1;

// 		const int othersign(-1 * co->sign);
// 		if ( co->XYZ1 == 0 ) // X
// 			co->pos_x_2 = (position.x() / m_size_Factor) * othersign * bodypartspacer;
// 		else if ( co->XYZ1 == 1 ) // Y
// 			co->pos_y_2 = (position.y() / m_size_Factor) * othersign * bodypartspacer;
// 		else if ( co->XYZ1 == 2 ) // Z
// 			co->pos_z_2 = (position.z() / m_size_Factor) * othersign * bodypartspacer;
	}
		
}


// void BodyArch::randomConstraintPosition(archConstraint* co, unsigned int OneOrTwo, unsigned int connID)
// {
// 	if ( OneOrTwo == 1 )
// 	{
// 		co->XYZ1 = randgen->Instance()->get( 0, 2 );
// // 		co->XYZ2 = randgen->Instance()->get( 0, 2 );
// // 		co->XYZ1 = 0;
// // 		co->XYZ2 = 0;
// // 		if ( co->XYZ1 == 1 )
// // 			co->XYZ1 = 2;
// 		
// 		co->sign = randgen->Instance()->get( 0, 1 );
// // 		co->sign = 0;
// 		if ( co->sign == 0 )
// 			co->sign = -1;
// 		
// 		const archBodypart& bp( archBodyparts[connID] );
// 
// 	// now we know the plane to connect to, determine positions
// 		if ( co->XYZ1 == 0 ) // X
// 		{
// 			// ((x / 1000.0f)  / 2)  * 1.5f * co->sign = 
// // 			co->pos_y_1 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
// // 			co->pos_z_1 = ((float)randgen->Instance()->get( 0, (bp.z)*2) - bp.z) / 100;
// 			co->pos_x_1 = (bp.x / m_size_Factor) * co->sign * bodypartspacer;
// 
// // 			// ROTARY
// 				co->rot_x_1 = 0.0001f * randgen->Instance()->get( -3141, 3141 );
// 		}
// 		else if ( co->XYZ1 == 1 ) // Y
// 		{
// // 			co->pos_x_1 = ((float)randgen->Instance()->get( 0, (bp.x)*2) - bp.x) / 100;
// // 			co->pos_z_1 = ((float)randgen->Instance()->get( 0, (bp.z)*2) - bp.z) / 100;
// 			co->pos_y_1 = (bp.y / m_size_Factor) * co->sign * bodypartspacer;
// 
// // 			// ROTARY
// 				co->rot_y_1 = 0.0001f * randgen->Instance()->get( -3141, 3141 );
// 		}
// 		else // Z
// 		{
// // 			co->pos_x_1 = ((float)randgen->Instance()->get( 0, (bp.x)*2) - bp.x) / 100;
// // 			co->pos_y_1 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
// 			co->pos_z_1 = (bp.z / m_size_Factor) * co->sign * bodypartspacer;
// 			
// // 			// ROTARY
// 				co->rot_z_1 = 0.0001f * randgen->Instance()->get( -3141, 3141 );
// 		}
// 	}
// 	else
// 	{
// 		repositionConstraintAngle( co );
// 
// 		const int othersign = -1 * co->sign;
// 		if ( !co->isMouthConstraint )
// 		{
// 			const archBodypart& bp( archBodyparts[connID] );
// 			if ( co->XYZ1 == 0 ) // X
// 			{
// // 				co->pos_y_2 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
// // 				co->pos_z_2 = ((float)randgen->Instance()->get( 0, (bp.z)*2) - bp.z) / 100;
// 				co->pos_x_2 = (bp.x / m_size_Factor) * othersign * bodypartspacer;
// 			}
// 			else if ( co->XYZ1 == 1 ) // Y
// 			{
// // 				co->pos_x_2 = ((float)randgen->Instance()->get( 0, (bp.x)*2) - bp.x) / 100;
// // 				co->pos_z_2 = ((float)randgen->Instance()->get( 0, (bp.z)*2) - bp.z) / 100;
// 				co->pos_y_2 = (bp.y / m_size_Factor) * othersign * bodypartspacer;
// 			}
// 			else // Z
// 			{
// // 				co->pos_x_2 = ((float)randgen->Instance()->get( 0, (bp.x)*2) - bp.x) / 100;
// // 				co->pos_y_2 = ((float)randgen->Instance()->get( 0, (bp.y)*2) - bp.y) / 100;
// // 				co->pos_z_2 = (bp.z / m_size_Factor) * othersign * bodypartspacer;
// 				co->pos_z_2 = (bp.z / m_size_Factor) * othersign * bodypartspacer;
// 			}
// 		}
// 		else
// 		{
// 			const archMouth& mouth( archMouths[connID] );
// 			if ( co->XYZ1 == 0 ) // X
// 			{
// // 				co->pos_y_2 = ((float)randgen->Instance()->get( 0, (mouth.y)*2) - mouth.y) / 100;
// // 				co->pos_z_2 = ((float)randgen->Instance()->get( 0, (mouth.z)*2) - mouth.z) / 100;
// 				co->pos_x_2 = (mouth.x / m_size_Factor) * othersign * bodypartspacer;
// 			}
// 			else if ( co->XYZ1 == 1 ) // Y
// 			{
// // 				co->pos_x_2 = ((float)randgen->Instance()->get( 0, (mouth.x)*2) - mouth.x) / 100;
// // 				co->pos_z_2 = ((float)randgen->Instance()->get( 0, (mouth.z)*2) - mouth.z) / 100;
// 				co->pos_y_2 = (mouth.y / m_size_Factor) * othersign * bodypartspacer;
// 			}
// 			else // Z
// 			{
// // 				co->pos_x_2 = ((float)randgen->Instance()->get( 0, (mouth.x)*2) - mouth.x) / 100;
// // 				co->pos_y_2 = ((float)randgen->Instance()->get( 0, (mouth.y)*2) - mouth.y) / 100;
// 				co->pos_z_2 = (mouth.z / m_size_Factor) * othersign * bodypartspacer;
// 			}
// 		}
// 	}
// }

void BodyArch::repositiontoConstraints( archBodypart* bp )
{
	// reposition the constraints back to the resized bodypart / mouth
	for ( int i = 0; i < (int)archConstraints.size(); ++i )
	{
		archConstraint* co = &archConstraints[i];
		if ( co->id_1 == bp->id )
// 		if ( findBodypart( co->id_1 ) == (int)bp->id )
		{
			repositionConstraint(co, 1, btVector3(bp->x, bp->y, bp->z));
// 			if ( co->XYZ1 == 0 ) // X
// 				co->pos_x_1 = (bp->x / m_size_Factor) * co->sign * bodypartspacer;
// 			else if ( co->XYZ1 == 1 ) // Y
// 				co->pos_y_1 = (bp->y / m_size_Factor) * co->sign * bodypartspacer;
// 			else if ( co->XYZ1 == 2 ) // Z
// 				co->pos_z_1 = (bp->z / m_size_Factor) * co->sign * bodypartspacer;
		}
		else if ( !co->isMouthConstraint && co->id_2 == bp->id )
// 		else if ( !co->isMouthConstraint && findBodypart( co->id_2 ) == (int)bp->id )
		{
			repositionConstraint(co, 2, btVector3(bp->x, bp->y, bp->z));
// 			const int othersign = -1 * co->sign;
// 			if ( co->XYZ1 == 0 ) // X
// 				co->pos_x_2 = (bp->x / m_size_Factor) * othersign * bodypartspacer;
// 			else if ( co->XYZ1 == 1 ) // Y
// 				co->pos_y_2 = (bp->y / m_size_Factor) * othersign * bodypartspacer;
// 			else if ( co->XYZ1 == 2 ) // Z
// 				co->pos_z_2 = (bp->z / m_size_Factor) * othersign * bodypartspacer;
		}
	}
}

void BodyArch::repositiontoConstraints( archMouth* bp )
{
	// reposition the constraints back to the resized bodypart / mouth
	for ( int i = 0; i < (int)archConstraints.size(); ++i )
	{
		archConstraint* co = &archConstraints[i];
		if ( co->isMouthConstraint && co->id_2 == bp->id )
		{
// 			const int othersign = -1 * co->sign;
			
			repositionConstraint(co, 2, btVector3(bp->x, bp->y, bp->z));
			
// 			if ( co->XYZ1 == 0 ) // X
// 				co->pos_x_2 = (bp->x / m_size_Factor) * othersign * bodypartspacer;
// 			else if ( co->XYZ1 == 1 ) // Y
// 				co->pos_y_2 = (bp->y / m_size_Factor) * othersign * bodypartspacer;
// 			else if ( co->XYZ1 == 2 ) // Z
// 				co->pos_z_2 = (bp->z / m_size_Factor) * othersign * bodypartspacer;
		}
	}
}



void BodyArch::mutate(unsigned int runs)
{
	for ( unsigned int i=0; i < runs; ++i )
	{
		unsigned int tsum = 	settings->getCVar("body_percentmutateeffectchangecolor")
					+ settings->getCVar("body_percentmutateeffectchangecolor_slightly")
					+ settings->getCVar("body_percentmutateeffectaddbodypart")
					+ settings->getCVar("body_percentmutateeffectremovebodypart")
					+ settings->getCVar("body_percentmutateeffectresizebodypart")
					+ settings->getCVar("body_percentmutateeffectresizebodypart_slightly")
					+ settings->getCVar("body_percentmutateeffectresizehead")
					+ settings->getCVar("body_percentmutateeffectresizehead_slightly")
					+ settings->getCVar("body_percentmutateeffectchangeconstraintlimits")
					+ settings->getCVar("body_percentmutateeffectchangeconstraintlimits_slightly")
					+ settings->getCVar("body_percentmutateeffectchangeconstraintangles")
					+ settings->getCVar("body_percentmutateeffectchangeconstraintangles_slightly")
					+ settings->getCVar("body_percentmutateeffectchangeconstraintposition")
					+ settings->getCVar("body_percentmutateeffectchangeconstraintposition_slightly")
					+ settings->getCVar("body_percentmutateeffectrepositionhead")
					+ settings->getCVar("critter_mutate_minenergyproc")
// 					+ settings->getCVar("critter_mutate_sightrange")
					+ settings->getCVar("critter_mutate_maxlifetime")
// 					+ settings->getCVar("body_percentmutateeffectchangemutationrate")
// 					+ settings->getCVar("body_percentmutateeffectchangemaxmutations")
				;

		unsigned int mode = randgen->Instance()->get(1,tsum);

		// CHANGE COLOR
			unsigned int modesum = settings->getCVar("body_percentmutateeffectchangecolor");
			if ( mode <= modesum )
			{
				// mutate color
				unsigned int ncolor = randgen->Instance()->get(0,2);

				if ( ncolor == 0 )
					color.setR((float)BeRand::Instance()->get(0,1000)/1000.0f);
				else if ( ncolor == 1 )
					color.setG((float)BeRand::Instance()->get(0,1000)/1000.0f);
				else if ( ncolor == 2 )
					color.setB((float)BeRand::Instance()->get(00,100)/1000.0f);

				continue;
			}

		// CHANGE COLOR SLIGHTLY
			modesum += settings->getCVar("body_percentmutateeffectchangecolor_slightly");
			if ( mode <= modesum )
			{
				// mutate color
				const int ncolor = randgen->Instance()->get(0,2);
				const int nweight = randgen->Instance()->get(1,50);
				float amount = 0.001 * nweight;

				if ( randgen->Instance()->get(0,1) == 0 )
					amount *= -1.0f;

				if ( ncolor == 0 )
					color.setR(color.r()+amount);
				else if ( ncolor == 1 )
					color.setG(color.g()+amount);
				else if ( ncolor == 2 )
					color.setB(color.b()+amount);
				
// 				if ( color.r() < 0.1f )
// 				{
// 					float diff = 0.1f - color.r();
// 					color.setR(color.r()+diff);
// 					color.setG(color.g()+diff);
// 					color.setB(color.b()+diff);
// 					color.setA(color.a()+diff);
// 				}
// 				if ( color.g() < 0.1f )
// 				{
// 					float diff = 0.1f - color.g();
// 					color.setR(color.r()+diff);
// 					color.setG(color.g()+diff);
// 					color.setB(color.b()+diff);
// 					color.setA(color.a()+diff);
// 				}
// 				if ( color.b() < 0.1f )
// 				{
// 					float diff = 0.1f - color.b();
// 					color.setR(color.r()+diff);
// 					color.setG(color.g()+diff);
// 					color.setB(color.b()+diff);
// 					color.setA(color.a()+diff);
// 				}
				if ( color.r() < 0.1f )
				{
					color.setR(0.1f);
				}
				if ( color.g() < 0.1f )
				{
					color.setG(0.1f);
				}
				if ( color.b() < 0.1f )
				{
					color.setB(0.1f);
				}
			
// 				if ( color.r() > 1.0f || color.g() > 1.0f || color.b() > 1.0f )
// 					color.normalize();

				if ( color.r() > 1.0f )
				{
					color.setR(1.0f);
				}
				if ( color.g() > 1.0f )
				{
					color.setG(1.0f);
				}
				if ( color.b() > 1.0f )
				{
					color.setB(1.0f);
				}

				continue;
			}

		// ADD BODYPART
			modesum += settings->getCVar("body_percentmutateeffectaddbodypart");
			if ( mode <= modesum )
			{
				if ( archBodyparts.size() < (unsigned int)settings->getCVar("body_maxbodyparts") )
				{
// 					cerr << "adding bodypart" << endl;
					addRandomBodypart();
// 					cerr << "done adding bodypart" << endl;
				}
				else
					runs++;
				continue;
			}

		// REMOVE BODYPART
			modesum += settings->getCVar("body_percentmutateeffectremovebodypart");
			if ( mode <= modesum )
			{
				if ( archBodyparts.size() > (unsigned int)settings->getCVar("body_minbodyparts") )
				{
					// pick a random bodypart
					const unsigned int bid = randgen->Instance()->get( 0, archBodyparts.size()-1 );

					// if not main body, remove it
					if ( archBodyparts[bid].id != 1000 )
					{
// 						cerr << "removing bodypart " << bid << " id " << archBodyparts[bid].id  << endl;

						removeBodypart( archBodyparts[bid].id );

						
// 						cerr << "removing obsolete constraints, expected errors:" << endl;
						for ( int i = 0; i < (int)archConstraints.size(); ++i )
						{
							archConstraint* c = &archConstraints[i];
							const int bodypart1( findBodypart( c->id_1 ) );
							if ( bodypart1 == -1 )
							{
								archConstraints.erase(archConstraints.begin()+i);
								i--;
							}
							else
							{
								if ( c->isMouthConstraint && findMouth( c->id_2 ) == -1 )
								{
									archConstraints.erase(archConstraints.begin()+i);
									i--;
								}
								else if ( !c->isMouthConstraint && findBodypart( c->id_2 ) == -1 )
								{
									archConstraints.erase(archConstraints.begin()+i);
									i--;
								}
							}
						}
// 						cerr << "done removing obsolete constraints" << endl << endl;

// 						cerr << "done removing bodypart" << endl;

						// re add mouth if needed
						if ( archMouths.size() == 0 )
								addRandomMouth();
					}
					else
						runs++;

				}
				else
					runs++;
				continue;
			}

		// RESIZE BODYPART
			modesum += settings->getCVar("body_percentmutateeffectresizebodypart");
			if ( mode <= modesum )
			{
// 				cerr << "resize bodypart" << endl;

					// pick a random bodypart
					const unsigned int bid = randgen->Instance()->get( 0, archBodyparts.size()-1 );
					archBodypart *bp = &archBodyparts[bid];

					unsigned int axismode = randgen->Instance()->get(0,2);
					if ( axismode == 0 )
						bp->x = randgen->Instance()->get( settings->getCVar("body_minbodypartsize"), settings->getCVar("body_maxbodypartsize") );
					else if ( axismode == 1 )
						bp->y = randgen->Instance()->get( settings->getCVar("body_minbodypartsize"), settings->getCVar("body_maxbodypartsize") );
					else
						bp->z = randgen->Instance()->get( settings->getCVar("body_minbodypartsize"), settings->getCVar("body_maxbodypartsize") );

					// reposition the constraints back to the resized bodypart
					repositiontoConstraints(bp);

// 				cerr << "done resize bodypart" << endl;
				continue;
			}

		// RESIZE BODYPART SLIGHTLY
			modesum += settings->getCVar("body_percentmutateeffectresizebodypart_slightly");
			if ( mode <= modesum )
			{
// 				cerr << "resize bodypart slightly" << endl;

					// pick a random bodypart
					const unsigned int bid = randgen->Instance()->get( 0, archBodyparts.size()-1 );
					archBodypart* bp = &archBodyparts[bid];

					unsigned int axismode = randgen->Instance()->get(0,2);
					unsigned int direction = randgen->Instance()->get(0,1);
					if ( axismode == 0 )
					{
						if ( direction == 0 )
							bp->x += randgen->Instance()->get( 1, settings->getCVar("body_maxbodypartsize") / 10 );
						else
							bp->x -= randgen->Instance()->get( 1, settings->getCVar("body_maxbodypartsize") / 10 );
					}
					else if ( axismode == 1 )
					{
						if ( direction == 0 )
							bp->y += randgen->Instance()->get( 1, settings->getCVar("body_maxbodypartsize") / 10 );
						else
							bp->y -= randgen->Instance()->get( 1, settings->getCVar("body_maxbodypartsize") / 10 );
					}
					else
					{
						if ( direction == 0 )
							bp->z += randgen->Instance()->get( 1, settings->getCVar("body_maxbodypartsize") / 10 );
						else
							bp->z -= randgen->Instance()->get( 1, settings->getCVar("body_maxbodypartsize") / 10 );
					}

					// see that they didn't go over their limits
					if ( bp->x < settings->getCVar("body_minbodypartsize") )
						bp->x = settings->getCVar("body_minbodypartsize");
					else if ( bp->x > settings->getCVar("body_maxbodypartsize") )
						bp->x = settings->getCVar("body_minbodypartsize");

					if ( bp->y < settings->getCVar("body_minbodypartsize") )
						bp->y = settings->getCVar("body_minbodypartsize");
					else if ( bp->y > settings->getCVar("body_maxbodypartsize") )
						bp->y = settings->getCVar("body_minbodypartsize");

					if ( bp->z < settings->getCVar("body_minbodypartsize") )
						bp->z = settings->getCVar("body_minbodypartsize");
					else if ( bp->z > settings->getCVar("body_maxbodypartsize") )
						bp->z = settings->getCVar("body_minbodypartsize");

					// reposition the constraints back to the resized bodypart
					repositiontoConstraints(bp);

// 				cerr << "done resize bodypart" << endl;
				continue;
			}

		// RESIZE HEAD
			modesum += settings->getCVar("body_percentmutateeffectresizehead");
			if ( mode <= modesum )
			{
// 				cerr << "resize mouth" << endl;

					// pick a random head
					unsigned int mid = randgen->Instance()->get( 0, archMouths.size()-1 );
					archMouth* head = &archMouths[mid];

					unsigned int axismode = randgen->Instance()->get(0,2);
					if ( axismode == 0 )
						head->x = randgen->Instance()->get( settings->getCVar("body_minheadsize"), settings->getCVar("body_maxheadsize") );
					else if ( axismode == 1 )
						head->y = randgen->Instance()->get( settings->getCVar("body_minheadsize"), settings->getCVar("body_maxheadsize") );
					else
						head->z = randgen->Instance()->get( settings->getCVar("body_minheadsize"), settings->getCVar("body_maxheadsize") );

					// reposition the constraints back to the resized bodypart
					repositiontoConstraints(head);

// 				cerr << "done resize head" << endl;
				continue;
			}

		// RESIZE HEAD SLIGHTLY
			modesum += settings->getCVar("body_percentmutateeffectresizehead_slightly");
			if ( mode <= modesum )
			{
// 				cerr << "resize head slightly" << endl;

					// pick a random head
					unsigned int mid = randgen->Instance()->get( 0, archMouths.size()-1 );
					archMouth* head = &archMouths[mid];

					unsigned int axismode = randgen->Instance()->get(0,2);
					unsigned int direction = randgen->Instance()->get(0,1);
					if ( axismode == 0 )
					{
						if ( direction == 0 )
							head->x += randgen->Instance()->get( 1, settings->getCVar("body_maxheadsize") / 10 );
						else
							head->x -= randgen->Instance()->get( 1, settings->getCVar("body_maxheadsize") / 10 );
					}
					else if ( axismode == 1 )
					{
						if ( direction == 0 )
							head->y += randgen->Instance()->get( 1, settings->getCVar("body_maxheadsize") / 10 );
						else
							head->y -= randgen->Instance()->get( 1, settings->getCVar("body_maxheadsize") / 10 );
					}
					else
					{
						if ( direction == 0 )
							head->z += randgen->Instance()->get( 1, settings->getCVar("body_maxheadsize") / 10 );
						else
							head->z -= randgen->Instance()->get( 1, settings->getCVar("body_maxheadsize") / 10 );
					}

					// see that they didn't go over their limits
					if ( head->x < settings->getCVar("body_minheadsize") )
						head->x = settings->getCVar("body_minheadsize");
					else if ( head->x > settings->getCVar("body_maxheadsize") )
						head->x = settings->getCVar("body_minheadsize");

					if ( head->y < settings->getCVar("body_minheadsize") )
						head->y = settings->getCVar("body_minheadsize");
					else if ( head->y > settings->getCVar("body_maxheadsize") )
						head->y = settings->getCVar("body_minheadsize");

					if ( head->z < settings->getCVar("body_minheadsize") )
						head->z = settings->getCVar("body_minheadsize");
					else if ( head->z > settings->getCVar("body_maxheadsize") )
						head->z = settings->getCVar("body_minheadsize");

					// reposition the constraints back to the resized bodypart
					repositiontoConstraints(head);

// 				cerr << "done resize head" << endl;
				continue;
			}

		// CHANGE CONSTRAINT LIMITS
			modesum += settings->getCVar("body_percentmutateeffectchangeconstraintlimits");
			if ( mode <= modesum )
			{
// 				cerr << "changing constraint limits" << endl;

				const unsigned int cid = randgen->Instance()->get( 0, archConstraints.size()-1 );
				archConstraint* co = &archConstraints[cid];
				co->limit_1		= -0.0001f * randgen->Instance()->get( 0, settings->getCVar("body_maxconstraintlimit") );
				co->limit_2		=  0.0001f * randgen->Instance()->get( 0, settings->getCVar("body_maxconstraintlimit") );
// 				co->limit_2		= -1.0f * co->limit_1;

// 				cerr << "done changing constraint limits" << endl;
				continue;
			}
	
		// CHANGE CONSTRAINT LIMITS SLIGHTLY
			modesum += settings->getCVar("body_percentmutateeffectchangeconstraintlimits_slightly");
			if ( mode <= modesum )
			{
// 				cerr << "changing constraint limits" << endl;

				const unsigned int cid = randgen->Instance()->get( 0, archConstraints.size()-1 );
				archConstraint* co = &archConstraints[cid];
				
				const int factor = randgen->Instance()->get(-100,100);
				const int mode = randgen->Instance()->get(0,1);
				
				if ( mode == 0 )
				{
					co->limit_1 += 0.0001f * factor;
					
					const float limit( 0.0001f * settings->getCVar("body_maxconstraintlimit") );

					if ( co->limit_1 > 0 )
						co->limit_1 = -1.0f * co->limit_1;
					
					
					if ( co->limit_1 < -limit )
						co->limit_1 = -limit;
					else if ( co->limit_1 > limit )
						co->limit_1 = limit;
				}
// 				co->limit_2 = -1.0f * co->limit_1;
				else
				{
					co->limit_2 += 0.0001f * factor;
					
					const float limit( 0.0001f * settings->getCVar("body_maxconstraintlimit") );

					if ( co->limit_2 < 0 )
						co->limit_2 = -1.0f * co->limit_2;
					
					
					if ( co->limit_2 < -limit )
						co->limit_2 = -limit;
					else if ( co->limit_2 > limit )
						co->limit_2 = limit;
				}

// 				cerr << "done changing constraint limits" << endl;
				continue;
			}
		// CHANGE CONSTRAINT ANGLES
			modesum += settings->getCVar("body_percentmutateeffectchangeconstraintangles");
			if ( mode <= modesum )
			{
// 				cerr << "changing constraint angles" << endl;

				const unsigned int cid = randgen->Instance()->get( 0, archConstraints.size()-1 );
				archConstraint* co = &archConstraints[cid];
				
				repositionConstraintAngle( co );

// 				co->rot_x_1		= ((float)randgen->Instance()->get( 0, 3141 ) - 1571) / 1000;
// 				co->rot_z_1		= ((float)randgen->Instance()->get( 0, 3141 ) - 1571) / 1000;
// 				co->rot_y_1		= ((float)randgen->Instance()->get( 0, 3141 ) - 1571) / 1000;

// 				co->rot_x_2		= ((float)randgen->Instance()->get( 0, 3141 ) - 1571) / 1000;
// 				co->rot_y_2		= ((float)randgen->Instance()->get( 0, 3141 ) - 1571) / 1000;
// 				co->rot_z_2		= ((float)randgen->Instance()->get( 0, 3141 ) - 1571) / 1000;

// 				cerr << "done changing constraint angles" << endl;
				continue;
			}

		// CHANGE CONSTRAINT ANGLES SLIGHTLY
			modesum += settings->getCVar("body_percentmutateeffectchangeconstraintangles_slightly");
			if ( mode <= modesum )
			{
// 				cerr << "changing constraint angles" << endl;

				const unsigned int cid = randgen->Instance()->get( 0, archConstraints.size()-1 );
				archConstraint* co = &archConstraints[cid];

// 				unsigned int XYZ = randgen->Instance()->get(0,2);
				
				const int body_maxconstraintangle( settings->getCVar("body_maxconstraintangle") );
				const float body_maxconstraintangle_float( 0.0001f * body_maxconstraintangle );
				
				const int factor( randgen->Instance()->get( -body_maxconstraintangle , body_maxconstraintangle ) );
				const float factor_float( 0.0001f * factor );
				const float factor_float_percent( 0.01f * factor_float );
				
				if ( co->XYZ1 == 0 )
				{
					co->rot_x_1 += factor_float_percent;
					if ( co->rot_x_1 < -body_maxconstraintangle_float ) co->rot_x_1 = -body_maxconstraintangle_float;
					if ( co->rot_x_1 >  body_maxconstraintangle_float ) co->rot_x_1 =  body_maxconstraintangle_float;
				}
				else if ( co->XYZ1 == 1 || co->XYZ1 == 2 )
				{
					co->rot_y_1 += factor_float_percent;
					if ( co->rot_y_1 < -body_maxconstraintangle_float ) co->rot_y_1 = -body_maxconstraintangle_float;
					if ( co->rot_y_1 >  body_maxconstraintangle_float ) co->rot_y_1 =  body_maxconstraintangle_float;
				}
				else
				{
					co->rot_z_1 += factor_float_percent;
					if ( co->rot_z_1 < -body_maxconstraintangle_float ) co->rot_z_1 = -body_maxconstraintangle_float;
					if ( co->rot_z_1 >  body_maxconstraintangle_float ) co->rot_z_1 =  body_maxconstraintangle_float;
				}


				
// 				unsigned int direction = randgen->Instance()->get(0,1);
// 				if ( direction == 0 )
// 				{
// 					if ( XYZ == 0 )
// 						co->rot_x_1 += 0.01f;
// 					else if ( XYZ == 1 )
// 						co->rot_y_1 += 0.01f;
// 					else
// 						co->rot_z_1 += 0.01f;
// 				}
// 				else
// 				{
// 					if ( XYZ == 0 )
// 						co->rot_x_1 -= 0.01f;
// 					else if ( XYZ == 1 )
// 						co->rot_y_1 -= 0.01f;
// 					else
// 						co->rot_z_1 -= 0.01f;
// 				}

// 				XYZ = randgen->Instance()->get(0,2);
// 				direction = randgen->Instance()->get(0,1);
// 				if ( direction == 0 )
// 				{
// 					if ( XYZ == 0 )
// 						co->rot_x_2 += 0.01f;
// 					else if ( XYZ == 1 )
// 						co->rot_y_2 += 0.01f;
// 					else
// 						co->rot_z_2 += 0.01f;
// 				}
// 				else
// 				{
// 					if ( XYZ == 0 )
// 						co->rot_x_2 -= 0.01f;
// 					else if ( XYZ == 1 )
// 						co->rot_y_2 -= 0.01f;
// 					else
// 						co->rot_z_2 -= 0.01f;
// 				}

// 				if ( co->rot_x_2 < -0.1571f )
// 					co->rot_x_2 = -0.1571f;
// 				if ( co->rot_x_2 > 0.1571f )
// 					co->rot_x_2 = 0.1571f;
// 				if ( co->rot_y_2 < -0.1571f )
// 					co->rot_y_2 = -0.1571f;
// 				if ( co->rot_y_2 > 0.1571f )
// 					co->rot_y_2 = 0.1571f;
// 				if ( co->rot_z_2 < -0.1571f )
// 					co->rot_z_2 = -0.1571f;
// 				if ( co->rot_z_2 > 0.1571f )
// 					co->rot_z_2 = 0.1571f;

// 				cerr << "done changing constraint angles" << endl;
				continue;
			}

		// REPOSITION A CONSTRAINT
			modesum += settings->getCVar("body_percentmutateeffectchangeconstraintposition");
			if ( mode <= modesum )
			{
// 				cerr << "changing constraint position" << endl;

				const unsigned int cid = randgen->Instance()->get( 0, archConstraints.size()-1 );
				archConstraint* co = &archConstraints[cid];

				int connID1 = findBodypart(co->id_1);
				int connID2;

				if ( co->isMouthConstraint )
					connID2 = findMouth(co->id_2);
				else
					connID2 = findBodypart(co->id_2);

				// pick one of 2 bodies to reconnect
				unsigned int body1or2 = randgen->Instance()->get( 1, 2 );

				if ( body1or2 == 1 )
					randomConstraintPosition(co, 1, connID1);
				else
					randomConstraintPosition(co, 2, connID2);

// 				cerr << "done changing constraint position" << endl;
				continue;
			}

		// REPOSITION A CONSTRAINT SLIGHTLY
			modesum += settings->getCVar("body_percentmutateeffectchangeconstraintposition_slightly");
			if ( mode <= modesum )
			{
// // 				cerr << "changing constraint position" << endl;
// 
// 				const unsigned int cid = randgen->Instance()->get( 0, archConstraints.size()-1 );
// 				archConstraint* co = &archConstraints[cid];
// 
// 				const int connID1 = findBodypart(co->id_1);
// 				int connID2;
// 
// 				if ( co->isMouthConstraint )
// 					connID2 = findMouth(co->id_2);
// 				else
// 					connID2 = findBodypart(co->id_2);
// 
// 				// pick one of 2 bodies to reconnect
// 				const unsigned int body1or2 = randgen->Instance()->get( 1, 2 );
// 				const unsigned int direction = randgen->Instance()->get( 0, 1 );
// 				const unsigned int axis1or2 = randgen->Instance()->get( 0, 1 );
// 
// 				if ( body1or2 == 1 ) {
// 					// now we know the plane to connect to, determine positions
// 					if ( co->XYZ1 == 0 ) { // X
// 						if ( direction == 0 ) {
// 							if ( axis1or2 == 0 )	co->pos_y_1 += archBodyparts[connID1].y / 100000;
// 							else			co->pos_z_1 += archBodyparts[connID1].z / 100000;
// 						} else {
// 							if ( axis1or2 == 0 )	co->pos_y_1 -= archBodyparts[connID1].y / 100000;
// 							else			co->pos_z_1 -= archBodyparts[connID1].z / 100000;
// 						}
// 					}
// 					else if ( co->XYZ1 == 1 ) { // Y
// 						if ( direction == 0 ) {
// 							if ( axis1or2 == 0 )	co->pos_x_1 += archBodyparts[connID1].x / 100000;
// 							else			co->pos_z_1 += archBodyparts[connID1].z / 100000;
// 						} else {
// 							if ( axis1or2 == 0 )	co->pos_x_1 -= archBodyparts[connID1].x / 100000;
// 							else			co->pos_z_1 -= archBodyparts[connID1].z / 100000;
// 						}
// 					}
// 					else { // Z
// 						if ( direction == 0 ) {
// 							if ( axis1or2 == 0 )	co->pos_x_1 += archBodyparts[connID1].x / 100000;
// 							else			co->pos_y_1 += archBodyparts[connID1].y / 100000;
// 						} else {
// 							if ( axis1or2 == 0 )	co->pos_x_1 -= archBodyparts[connID1].x / 100000;
// 							else			co->pos_y_1 -= archBodyparts[connID1].y / 100000;
// 						}
// 					}
// 					if ( co->pos_x_1 < archBodyparts[connID1].x/-2000 )
// 						co->pos_x_1 = archBodyparts[connID1].x/-2000;
// 					if ( co->pos_x_1 > archBodyparts[connID1].x/2000 )
// 						co->pos_x_1 = archBodyparts[connID1].x/2000;
// 					if ( co->pos_y_1 < archBodyparts[connID1].y/-2000 )
// 						co->pos_y_1 = archBodyparts[connID1].y/-2000;
// 					if ( co->pos_y_1 > archBodyparts[connID1].y/2000 )
// 						co->pos_y_1 = archBodyparts[connID1].y/2000;
// 					if ( co->pos_z_1 < archBodyparts[connID1].z/-2000 )
// 						co->pos_z_1 = archBodyparts[connID1].z/-2000;
// 					if ( co->pos_z_1 > archBodyparts[connID1].z/2000 )
// 						co->pos_z_1 = archBodyparts[connID1].z/2000;
// 				}
// 				else {
// 					if ( !co->isMouthConstraint ) {
// 						// now we know the plane to connect to, determine positions
// 						if ( co->XYZ1 == 0 ) { // X
// 							if ( direction == 0 ) {
// 								if ( axis1or2 == 0 )	co->pos_y_2 += archBodyparts[connID2].y / 100000;
// 								else			co->pos_z_2 += archBodyparts[connID2].z / 100000;
// 							} else {
// 								if ( axis1or2 == 0 )	co->pos_y_2 -= archBodyparts[connID2].y / 100000;
// 								else			co->pos_z_2 -= archBodyparts[connID2].z / 100000;
// 							}
// 						}
// 						else if ( co->XYZ1 == 1 ) { // Y
// 							if ( direction == 0 ) {
// 								if ( axis1or2 == 0 )	co->pos_x_2 += archBodyparts[connID2].x / 100000;
// 								else			co->pos_z_2 += archBodyparts[connID2].z / 100000;
// 							} else {
// 								if ( axis1or2 == 0 )	co->pos_x_2 -= archBodyparts[connID2].x / 100000;
// 								else			co->pos_z_2 -= archBodyparts[connID2].z / 100000;
// 							}
// 						}
// 						else { // Z
// 							if ( direction == 0 ) {
// 								if ( axis1or2 == 0 )	co->pos_x_2 += archBodyparts[connID2].x / 100000;
// 								else			co->pos_y_2 += archBodyparts[connID2].y / 100000;
// 							} else {
// 								if ( axis1or2 == 0 )	co->pos_x_2 -= archBodyparts[connID2].x / 100000;
// 								else			co->pos_y_2 -= archBodyparts[connID2].y / 100000;
// 							}
// 						}
// 						if ( co->pos_x_2 < archBodyparts[connID2].x/-2000 )
// 							co->pos_x_2 = archBodyparts[connID2].x/-2000;
// 						if ( co->pos_x_2 > archBodyparts[connID2].x/2000 )
// 							co->pos_x_2 = archBodyparts[connID2].x/2000;
// 						if ( co->pos_y_2 < archBodyparts[connID2].y/-2000 )
// 							co->pos_y_2 = archBodyparts[connID2].y/-2000;
// 						if ( co->pos_y_2 > archBodyparts[connID2].y/2000 )
// 							co->pos_y_2 = archBodyparts[connID2].y/2000;
// 						if ( co->pos_z_2 < archBodyparts[connID2].z/-2000 )
// 							co->pos_z_2 = archBodyparts[connID2].z/-2000;
// 						if ( co->pos_z_2 > archBodyparts[connID2].z/2000 )
// 							co->pos_z_2 = archBodyparts[connID2].z/2000;
// 					}
// 					else {
// 						if ( co->XYZ1 == 0 ) { // X
// 							if ( direction == 0 ) {
// 								if ( axis1or2 == 0 )	co->pos_y_2 += archMouths[connID2].y / 100000;
// 								else			co->pos_z_2 += archMouths[connID2].z / 100000;
// 							} else {
// 								if ( axis1or2 == 0 )	co->pos_y_2 -= archMouths[connID2].y / 100000;
// 								else			co->pos_z_2 -= archMouths[connID2].z / 100000;
// 							}
// 						}
// 						else if ( co->XYZ1 == 1 ) { // Y
// 							if ( direction == 0 ) {
// 								if ( axis1or2 == 0 )	co->pos_x_2 += archMouths[connID2].x / 100000;
// 								else			co->pos_z_2 += archMouths[connID2].z / 100000;
// 							} else {
// 								if ( axis1or2 == 0 )	co->pos_x_2 -= archMouths[connID2].x / 100000;
// 								else			co->pos_z_2 -= archMouths[connID2].z / 100000;
// 							}
// 						}
// 						else { // Z
// 							if ( direction == 0 ) {
// 								if ( axis1or2 == 0 )	co->pos_x_2 += archMouths[connID2].x / 100000;
// 								else			co->pos_y_2 += archMouths[connID2].y / 100000;
// 							} else {
// 								if ( axis1or2 == 0 )	co->pos_x_2 -= archMouths[connID2].x / 100000;
// 								else			co->pos_y_2 -= archMouths[connID2].y / 100000;
// 							}
// 						}
// 						if ( co->pos_x_2 < archMouths[connID2].x/-2000 )
// 							co->pos_x_2 = archMouths[connID2].x/-2000;
// 						if ( co->pos_x_2 > archMouths[connID2].x/2000 )
// 							co->pos_x_2 = archMouths[connID2].x/2000;
// 						if ( co->pos_y_2 < archMouths[connID2].y/-2000 )
// 							co->pos_y_2 = archMouths[connID2].y/-2000;
// 						if ( co->pos_y_2 > archMouths[connID2].y/2000 )
// 							co->pos_y_2 = archMouths[connID2].y/2000;
// 						if ( co->pos_z_2 < archMouths[connID2].z/-2000 )
// 							co->pos_z_2 = archMouths[connID2].z/-2000;
// 						if ( co->pos_z_2 > archMouths[connID2].z/2000 )
// 							co->pos_z_2 = archMouths[connID2].z/2000;
// 					}
// 				}
					
// 					randomConstraintPosition(co, 2, connID2);

// 				cerr << "done changing constraint position" << endl;
				continue;
			}

		// REMOVE AND ADD MOUTH
			modesum += settings->getCVar("body_percentmutateeffectrepositionhead");
			if ( mode <= modesum )
			{
// 				cerr << "remove and add mouth" << endl;
				
				for ( int i = 0; i < (int)archConstraints.size(); ++i )
				{
					archConstraint* c = &archConstraints[i];
					if ( c->isMouthConstraint && c->id_2 == archMouths[0].id )
					{
						archConstraints.erase(archConstraints.begin()+i);
						i--;
					}
				}

				removeMouth(0);

				addRandomMouth();

// 				cerr << "done remove and add mouth" << endl;
				continue;
			}

		// ALTER MINIMAL ENERGY FOR PROCREATION
			modesum += settings->getCVar("critter_mutate_minenergyproc");
			if ( mode <= modesum )
			{
				const float pct = 0.01f * randgen->Instance()->get( 1, 90 );
				int amount( m_minenergyproc * pct );
				amount = max( amount, 1 );
				
				if ( randgen->Instance()->get( 0, 1000 ) < 500 || m_minenergyproc == settings->getCVar("critter_minenergyproc") )
					m_minenergyproc -= amount;
				else
				{
					m_minenergyproc += amount;
					if ( m_minenergyproc > settings->getCVar("critter_minenergyproc") )
						m_minenergyproc = settings->getCVar("critter_minenergyproc");
				}
				continue;
			}

// 		// ALTER SIGHTRANGE
// 			modesum += settings->getCVar("critter_mutate_sightrange");
// 			if ( mode <= modesum )
// 			{
// 				const float pct = 0.01f * randgen->Instance()->get( 1, 20 );
// 				int amount( m_sightrange * pct );
// 				amount = std::max( amount, 1 );
// 				
// 				if ( randgen->Instance()->get( 0, 1000 ) < 500 )
// 					m_sightrange -= amount;
// 				else
// 					m_sightrange += amount;
// 				continue;
// 			}
			
		// ALTER MAXLIFETIME
			modesum += settings->getCVar("critter_mutate_maxlifetime");
			if ( mode <= modesum )
			{
				const float pct = 0.01f * randgen->Instance()->get( 1, 90 );
				int amount( m_maxlifetime * pct );
				amount = max( amount, 1 );
				
				if ( randgen->Instance()->get( 0, 1000 ) < 500 || m_maxlifetime == settings->getCVar("critter_maxlifetime") )
					m_maxlifetime -= amount;
				else
				{
					m_maxlifetime += amount;
					if ( m_maxlifetime > settings->getCVar("critter_maxlifetime") )
						m_maxlifetime = settings->getCVar("critter_maxlifetime");
				}
				
				
				continue;
			}
			
// 		// ALTER MUTATIONRATE
// 			modesum += settings->getCVar("body_percentmutateeffectchangemutationrate");
// 			if ( mode <= modesum )
// 			{
// 				const float pct = 0.01f * randgen->Instance()->get( 1, 90 );
// 				int amount( m_mutationrate * pct );
// 				amount = std::max( amount, 1 );
// 				
// 				if ( randgen->Instance()->get( 0, 1000 ) < 500 || m_mutationrate == settings->getCVar("body_mutationrate") )
// 					m_mutationrate -= amount;
// 				else
// 				{
// 					m_mutationrate += amount;
// 					if ( m_mutationrate > settings->getCVar("body_mutationrate") )
// 						m_mutationrate = settings->getCVar("body_mutationrate");
// 				}
// 				
// 				
// 				continue;
// 			}
// 			
// 		// ALTER MAXMUTATIONS
// 			modesum += settings->getCVar("body_percentmutateeffectchangemaxmutations");
// 			if ( mode <= modesum )
// 			{
// 				const float pct = 0.01f * randgen->Instance()->get( 1, 90 );
// 				int amount( m_maxmutations * pct );
// 				amount = std::max( amount, 1 );
// 				
// 				if ( randgen->Instance()->get( 0, 1000 ) < 500 || m_maxmutations == settings->getCVar("body_maxmutations") )
// 					m_maxmutations -= amount;
// 				else
// 				{
// 					m_maxmutations += amount;
// 					if ( m_maxmutations > settings->getCVar("body_maxmutations") )
// 						m_maxmutations = settings->getCVar("body_maxmutations");
// 				}
// 				
// 				
// 				continue;
// 			}
			
// 					+ settings->getCVar("body_percentmutateeffectchangemutationrate")
// 					+ settings->getCVar("body_percentmutateeffectchangemaxmutations")
			
			
// 					+ settings->getCVar("")
// 					+ settings->getCVar("critter_mutate_sightrange")
// 					+ settings->getCVar("")
			
			
			
		// if we reach here, none were processed, decrease runs by 1 to make sure we get a hit
			if ( modesum > 0 )
				runs++;
	}
}

int BodyArch::findBodypart( unsigned int id )
{
	for ( unsigned int i=0; i < archBodyparts.size(); ++i )
	{
		if ( archBodyparts[i].id == id )
		{
			return i;
		}
	}
// 	cerr << " NOT GOOD: cannot find a bodypart for the id " << id << endl;
	return -1;
}

int BodyArch::findMouth( unsigned int id )
{
	for ( unsigned int i=0; i < archMouths.size(); ++i )
	{
		if ( archMouths[i].id == id )
		{
			return i;
		}
	}
// 	cerr << " NOT GOOD " << endl;
// 	cerr << " NOT GOOD: cannot find a mouth for the id " << id << endl;
	return -1;
}

unsigned int BodyArch::getUniqueBodypartID()
{
	unsigned int id = 1000;
	bool found = true;
	while ( found )
	{
		found = false;
		for ( unsigned int i = 0; i < archBodyparts.size() && !found; ++i )
			if ( archBodyparts[i].id == id )
			{
				found = true;
				id++;
			}
	}
	return id;
}

unsigned int BodyArch::getUniqueConstraintID()
{
	unsigned int id = 1000;
	bool found = true;
	while ( found )
	{
		found = false;
		for ( unsigned int i = 0; i < archConstraints.size() && !found; ++i )
		{
			if ( archConstraints[i].constraint_id1 == id || archConstraints[i].constraint_id2 == id )
			{
				found = true;
				id++;
			}
		}
	}
	return id;
}

void BodyArch::copyFrom(const BodyArch* otherBody)
{
	color = otherBody->color;
	retinasize = otherBody->retinasize;
// 	m_maxmutations = otherBody->m_maxmutations;
// 	m_mutationrate = otherBody->m_mutationrate;
	m_maxmutations					= settings->getCVar("body_maxmutations");
	m_mutationrate					= settings->getCVar("body_mutationrate");

	for ( unsigned int i=0; i < otherBody->archBodyparts.size(); ++i )
	{
		const archBodypart *obp = &otherBody->archBodyparts[i];
		archBodyparts.push_back( archBodypart() );
		archBodypart *bp = &archBodyparts[archBodyparts.size()-1];
		
		bp->id		= obp->id;
		bp->type	= obp->type;
		bp->materialID	= obp->materialID;
		bp->x		= obp->x;
		bp->y		= obp->y;
		bp->z		= obp->z;
	}

	for ( unsigned int i=0; i < otherBody->archMouths.size(); ++i )
	{
		const archMouth *omo = &otherBody->archMouths[i];
		archMouths.push_back( archMouth() );
		archMouth *mo = &archMouths[archMouths.size()-1];
		
		mo->id		= omo->id;
		mo->x		= omo->x;
		mo->y		= omo->y;
		mo->z		= omo->z;
	}

	for ( unsigned int i=0; i < otherBody->archConstraints.size(); ++i )
	{
		const archConstraint *oco = &otherBody->archConstraints[i];
		archConstraints.push_back( archConstraint() );
		archConstraint *co = &archConstraints[archConstraints.size()-1];
		
		co->isMouthConstraint	= oco->isMouthConstraint;
		co->constraint_id1	= oco->constraint_id1;
		co->constraint_id2	= oco->constraint_id2;
		co->XYZ1			= oco->XYZ1;
// 		co->sign		= oco->sign;
		co->id_1		= oco->id_1;
		co->id_2		= oco->id_2;
		co->rot_x_1		= oco->rot_x_1;
		co->rot_x_2		= oco->rot_x_2;
		co->rot_y_1		= oco->rot_y_1;
		co->rot_y_2		= oco->rot_y_2;
		co->rot_z_1		= oco->rot_z_1;
		co->rot_z_2		= oco->rot_z_2;
		co->pos_x_1		= oco->pos_x_1;
		co->pos_x_2		= oco->pos_x_2;
		co->pos_y_1		= oco->pos_y_1;
		co->pos_y_2		= oco->pos_y_2;
		co->pos_z_1		= oco->pos_z_1;
		co->pos_z_2		= oco->pos_z_2;
		co->limit_1		= oco->limit_1;
		co->limit_2		= oco->limit_2;
	}
}

void BodyArch::setArch(string* content)
{
//	cerr << *content << endl;

	contentParser.reset();
	string line = contentParser.returnUntillStrip( '\n', *content );
	while ( !line.empty() )
	{
		line.append(" ");
		parseH.reset();
		if ( parseH.beginMatchesStrip( "color=", line ) )
		{
			string R = parseH.returnUntillStrip( ',', line );
			string G = parseH.returnUntillStrip( ',', line );
			string B = parseH.returnUntillStrip( ';', line );

			float r(0.0f);
			float g(0.0f);
			float b(0.0f);
			
			
			if(EOF == sscanf(R.c_str(), "%f", &r)) cerr << "ERROR INSERTING CRITTER (colorR)" << endl;
			if(EOF == sscanf(G.c_str(), "%f", &g)) cerr << "ERROR INSERTING CRITTER (colorG)" << endl;
			if(EOF == sscanf(B.c_str(), "%f", &b)) cerr << "ERROR INSERTING CRITTER (colorB)" << endl;
			
			color.setR(r);
			color.setG(g);
			color.setB(b);
			color.setA(0.0f);
		}

		else if ( parseH.beginMatchesStrip( "minenergyproc=", line ) )
		{
			string AD = parseH.returnUntillStrip( ';', line );
			//cerr << "AD: " << AD << endl;
			if(EOF == sscanf(AD.c_str(), "%d", &m_minenergyproc)) cerr << "ERROR INSERTING CRITTER" << endl;
		}

		else if ( parseH.beginMatchesStrip( "sightrange=", line ) )
		{
			string AD = parseH.returnUntillStrip( ';', line );
			//cerr << "AD: " << AD << endl;
			if(EOF == sscanf(AD.c_str(), "%d", &m_sightrange)) cerr << "ERROR INSERTING CRITTER" << endl;
		}

		else if ( parseH.beginMatchesStrip( "maxlifetime=", line ) )
		{
			string AD = parseH.returnUntillStrip( ';', line );
			//cerr << "AD: " << AD << endl;
			if(EOF == sscanf(AD.c_str(), "%d", &m_maxlifetime)) cerr << "ERROR INSERTING CRITTER" << endl;
		}
		
		else if ( parseH.beginMatchesStrip( "body_maxmutations=", line ) )
		{
			string AD = parseH.returnUntillStrip( ';', line );
			//cerr << "AD: " << AD << endl;
			if(EOF == sscanf(AD.c_str(), "%d", &m_maxmutations)) cerr << "ERROR INSERTING CRITTER" << endl;
		}
		
		else if ( parseH.beginMatchesStrip( "body_mutationrate=", line ) )
		{
			string AD = parseH.returnUntillStrip( ';', line );
			//cerr << "AD: " << AD << endl;
			if(EOF == sscanf(AD.c_str(), "%d", &m_mutationrate)) cerr << "ERROR INSERTING CRITTER" << endl;
		}
		
		else if ( parseH.beginMatchesStrip( "retinasize=", line ) )
		{
			string RES = parseH.returnUntillStrip( ';', line );
			//cerr << "RES: " << RES << endl;
			if(EOF == sscanf(RES.c_str(), "%d", &retinasize)) cerr << "ERROR INSERTING CRITTER" << endl;
		}
		else if ( parseH.beginMatchesStrip( "b ", line ) )
		{
			// b 99999 box 0 75 75 200

// 			cerr << "bodypart" << endl;

			archBodyparts.push_back( archBodypart() );
			archBodypart *bp = &archBodyparts[archBodyparts.size()-1];
			
			string bodypartID = parseH.returnUntillStrip( ' ', line );
			if(EOF == sscanf(bodypartID.c_str(), "%d", &bp->id)) cerr << "error in parsing body" << endl;
			string bodypartShape = parseH.returnUntillStrip( ' ', line );
			if ( bodypartShape == "box" )
			{
				// type = box
					bp->type = 0;

				// materialID
					string materialID = parseH.returnUntillStrip( ' ', line );
					if(EOF == sscanf(materialID.c_str(), "%d", &bp->materialID)) cerr << "error in parsing body" << endl;

				// dimesions X Y Z
					string X = parseH.returnUntillStrip( ' ', line );
					string Y = parseH.returnUntillStrip( ' ', line );
					string Z = parseH.returnUntillStrip( ' ', line );
					if(EOF == sscanf(X.c_str(), "%f", &bp->x)) cerr << "error in parsing body" << endl;
					if(EOF == sscanf(Y.c_str(), "%f", &bp->y)) cerr << "error in parsing body" << endl;
					if(EOF == sscanf(Z.c_str(), "%f", &bp->z)) cerr << "error in parsing body" << endl;


/*				cerr << endl << " id " << bp->id << endl;
				cerr << " type " << bp->type << endl;
				cerr << " material " << bp->materialID << endl;
				cerr << " x " << bp->x << endl;
				cerr << " y " << bp->y << endl;
				cerr << " z " << bp->z << endl;*/
			}
		}
		else if ( parseH.beginMatchesStrip( "c ", line ) )
		{
			// c 99999 1.5707963 0 0 0 0 -0.25 99998 1.5707963 0 0 0 0 0.25
// 			cm 0 1 1000 1001 1000 1.115 -0.752 -0.102 0.258 0.089 -0.133 -0.1103 1000 -1.4 -1.007 -1.287 -0.258 0.039 0.137 0.1103
// 			cm 0 1000 1001 1000 0 0 0 0.19475 -0.1 -0.07 0.0052 1000 0 0 0 -0.25625 -0.02 -0.08 -0.0052
// 			cerr << "constraint" << endl;

			archConstraint CO;
			archConstraints.push_back( CO );
			archConstraint *co = &archConstraints[archConstraints.size()-1];
			
			// XYZ & sign
				string XYZ = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(XYZ.c_str(), "%d", &co->XYZ1)) cerr << "error in parsing body" << endl;
				
// 				string SIGN = parseH.returnUntillStrip( ' ', line );
// 				if(EOF == sscanf(SIGN.c_str(), "%d", &co->sign)) cerr << "error in parsing body" << endl;

			// CONSTRAINT IDS
				string ID = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ID.c_str(), "%d", &co->constraint_id1)) cerr << "error in parsing body" << endl;
				ID = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ID.c_str(), "%d", &co->constraint_id2)) cerr << "error in parsing body" << endl;

			// CONNECTION TO BODYPART 1
				string ID_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ID_1.c_str(), "%d", &co->id_1)) cerr << "error in parsing body" << endl;
				
			// ROTATION
				string ROT_X_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_X_1.c_str(), "%f", &co->rot_x_1)) cerr << "error in parsing body" << endl;
				string ROT_Y_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_Y_1.c_str(), "%f", &co->rot_y_1)) cerr << "error in parsing body" << endl;
				string ROT_Z_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_Z_1.c_str(), "%f", &co->rot_z_1)) cerr << "error in parsing body" << endl;

			// POSITION
				string POS_X_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_X_1.c_str(), "%f", &co->pos_x_1)) cerr << "error in parsing body" << endl;
				string POS_Y_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_Y_1.c_str(), "%f", &co->pos_y_1)) cerr << "error in parsing body" << endl;
				string POS_Z_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_Z_1.c_str(), "%f", &co->pos_z_1)) cerr << "error in parsing body" << endl;

			// LIMIT
				string LIMIT_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(LIMIT_1.c_str(), "%f", &co->limit_1)) cerr << "error in parsing body" << endl;

			// CONNECTION TO BODYPART 2
				string ID_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ID_2.c_str(), "%d", &co->id_2)) cerr << "error in parsing body" << endl;

			// ROTATION
				string ROT_X_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_X_2.c_str(), "%f", &co->rot_x_2)) cerr << "error in parsing body" << endl;
				string ROT_Y_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_Y_2.c_str(), "%f", &co->rot_y_2)) cerr << "error in parsing body" << endl;
				string ROT_Z_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_Z_2.c_str(), "%f", &co->rot_z_2)) cerr << "error in parsing body" << endl;

			// POSITION
				string POS_X_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_X_2.c_str(), "%f", &co->pos_x_2)) cerr << "error in parsing body" << endl;
				string POS_Y_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_Y_2.c_str(), "%f", &co->pos_y_2)) cerr << "error in parsing body" << endl;
				string POS_Z_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_Z_2.c_str(), "%f", &co->pos_z_2)) cerr << "error in parsing body" << endl;

			// LIMIT
				string LIMIT_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(LIMIT_2.c_str(), "%f", &co->limit_2)) cerr << "error in parsing body" << endl;
		}
		else if ( parseH.beginMatchesStrip( "cm ", line ) )
		{
			// cm 99999 1.5707963 0 0 0 0 -0.25 99998 1.5707963 0 0 0 0 0.25
			// cm 0 1000 1001 1000 0 0 0 0.19475 -0.1 -0.07 0.0052 1000 0 0 0 -0.25625 -0.02 -0.08 -0.0052

// 			cerr << "constraint" << endl;

			archConstraint CO;
			archConstraints.push_back( CO );
			archConstraint *co = &archConstraints[archConstraints.size()-1];

			co->isMouthConstraint = true;

			// XYZ & sign
				string XYZ = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(XYZ.c_str(), "%d", &co->XYZ1)) cerr << "error in parsing body" << endl;
// 				string SIGN = parseH.returnUntillStrip( ' ', line );
// 				if(EOF == sscanf(SIGN.c_str(), "%d", &co->sign)) cerr << "error in parsing body" << endl;

			// CONSTRAINT IDS
				string ID = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ID.c_str(), "%d", &co->constraint_id1)) cerr << "error in parsing body" << endl;
				ID = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ID.c_str(), "%d", &co->constraint_id2)) cerr << "error in parsing body" << endl;

			// CONNECTION TO BODYPART 1
				string ID_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ID_1.c_str(), "%d", &co->id_1)) cerr << "error in parsing body" << endl;

			// ROTATION
				string ROT_X_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_X_1.c_str(), "%f", &co->rot_x_1)) cerr << "error in parsing body" << endl;
				string ROT_Y_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_Y_1.c_str(), "%f", &co->rot_y_1)) cerr << "error in parsing body" << endl;
				string ROT_Z_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_Z_1.c_str(), "%f", &co->rot_z_1)) cerr << "error in parsing body" << endl;

			// POSITION
				string POS_X_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_X_1.c_str(), "%f", &co->pos_x_1)) cerr << "error in parsing body" << endl;
				string POS_Y_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_Y_1.c_str(), "%f", &co->pos_y_1)) cerr << "error in parsing body" << endl;
				string POS_Z_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_Z_1.c_str(), "%f", &co->pos_z_1)) cerr << "error in parsing body" << endl;

			// LIMIT
				string LIMIT_1 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(LIMIT_1.c_str(), "%f", &co->limit_1)) cerr << "error in parsing body" << endl;

			// CONNECTION TO BODYPART 2
				string ID_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ID_2.c_str(), "%d", &co->id_2)) cerr << "error in parsing body" << endl;

			// ROTATION
				string ROT_X_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_X_2.c_str(), "%f", &co->rot_x_2)) cerr << "error in parsing body" << endl;
				string ROT_Y_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_Y_2.c_str(), "%f", &co->rot_y_2)) cerr << "error in parsing body" << endl;
				string ROT_Z_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(ROT_Z_2.c_str(), "%f", &co->rot_z_2)) cerr << "error in parsing body" << endl;

			// POSITION
				string POS_X_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_X_2.c_str(), "%f", &co->pos_x_2)) cerr << "error in parsing body" << endl;
				string POS_Y_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_Y_2.c_str(), "%f", &co->pos_y_2)) cerr << "error in parsing body" << endl;
				string POS_Z_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(POS_Z_2.c_str(), "%f", &co->pos_z_2)) cerr << "error in parsing body" << endl;

			// LIMIT
				string LIMIT_2 = parseH.returnUntillStrip( ' ', line );
				if(EOF == sscanf(LIMIT_2.c_str(), "%f", &co->limit_2)) cerr << "error in parsing body" << endl;
				
// 				std::cout << LIMIT_1 << " vs " << LIMIT_2 << std::endl;
				
		}
		else if ( parseH.beginMatchesStrip( "m ", line ) )
		{
			// m 99999 50, 50, 100
// 			cerr << "constraint" << endl;

			archMouth MO;
			archMouths.push_back( MO );
			archMouth *mo = &archMouths[archMouths.size()-1];

			string ID = parseH.returnUntillStrip( ' ', line );
			if(EOF == sscanf(ID.c_str(), "%d", &mo->id)) cerr << "error in parsing body" << endl;

			string X = parseH.returnUntillStrip( ' ', line );
			if(EOF == sscanf(X.c_str(), "%f", &mo->x)) cerr << "error in parsing body" << endl;

			string Y = parseH.returnUntillStrip( ' ', line );
			if(EOF == sscanf(Y.c_str(), "%f", &mo->y)) cerr << "error in parsing body" << endl;

			string Z = parseH.returnUntillStrip( ' ', line );
			if(EOF == sscanf(Z.c_str(), "%f", &mo->z)) cerr << "error in parsing body" << endl;
		}
		line = contentParser.returnUntillStrip( '\n', *content );
	}
}


string* BodyArch::getArch()
{
	stringstream buf;

	// neuron info
		buf << "color=" << color.r() << "," << color.g() << "," << color.b() << ";\n";
		buf << "retinasize=" << retinasize << ";\n";
		buf << "minenergyproc=" << m_minenergyproc << ";\n";
		buf << "sightrange=" << m_sightrange << ";\n";
		buf << "maxlifetime=" << m_maxlifetime << ";\n";
		buf << "body_maxmutations=" << m_maxmutations << ";\n";
		buf << "body_mutationrate=" << m_mutationrate << ";\n";
		
		
		for ( unsigned int i=0; i < archBodyparts.size(); ++i )
		{
			archBodypart *bp = &archBodyparts[i];
			buf << "b" << " " << bp->id << " " << "box" << " " << bp->materialID << " " << bp->x << " " << bp->y << " " << bp->z << "\n";
		}

		for ( unsigned int i=0; i < archMouths.size(); ++i )
		{
			archMouth *mo = &archMouths[i];
			buf << "m" << " " << mo->id << " " << mo->x << " " << mo->y << " " << mo->z << "\n";
		}

		for ( unsigned int i=0; i < archConstraints.size(); ++i )
		{
			archConstraint *co = &archConstraints[i];
			if ( co->isMouthConstraint )
				buf << "cm";
			else
				buf << "c";
			buf << " " << co->XYZ1;
			buf << " " << co->constraint_id1 << " " << co->constraint_id2 << " ";
			buf << co->id_1 << " " << co->rot_x_1 << " " << co->rot_y_1 << " " << co->rot_z_1 << " " << co->pos_x_1 << " " << co->pos_y_1 << " " << co->pos_z_1 << " " << co->limit_1 << " ";
			buf << co->id_2 << " " << co->rot_x_2 << " " << co->rot_y_2 << " " << co->rot_z_2 << " " << co->pos_x_2 << " " << co->pos_y_2 << " " << co->pos_z_2 << " " << co->limit_2;
			buf << "\n";
		}

	archBuffer = buf.str();
	return &archBuffer;
}

BodyArch::~BodyArch()
{
}

#ifndef ROUNDWORLD_H
#define ROUNDWORLD_H

#include "../entities/worldb.h"

class Roundworld : public WorldB {

	public:
		Roundworld(  boost::shared_ptr<BeGraphicsSystem> system, BeFilesystem& fileSystem, boost::shared_ptr<Textverbosemessage> textverbosemessage );
		~Roundworld();

		void		process(const BeTimer& timer);
		void		init();
// 		void		drawWithGrid();
	private:
		void		drawSphere(btScalar radius, int lats, int longs);
		btVector3	findPosition();
		void		makeFloor();
		void		drawfloor();
		void		childPositionOffset(btTransform* v);
		virtual void resetCamera();
		
// 		btCollisionObject* fixedGround;
// 		btCollisionShape* groundShape;
		btTransform groundTransform;
// 		boost::shared_ptr<BeGraphicsModelResource> m_model;
		
		virtual void updateCameraTransform( const float timeDelta );
// 		virtual void setLights();
		
};

#endif

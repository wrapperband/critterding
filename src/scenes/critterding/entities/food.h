#ifndef FOOD_H_INCLUDED
#define FOOD_H_INCLUDED

// #include "btBulletDynamicsCommon.h"

// #include "src/utils/settings.h"
// #include "displaylists.h"
#include "entity.h"
#include "body.h"

// #include "src/common/be_model_system.h"

class btDynamicsWorld;
class Food : public Entity
{
	public:
		Food();
		~Food();

		Body						body;
		void						createBody(btDynamicsWorld* m_dynamicsWorld, const btVector3& startOffset);

// 		float						position[16];

		unsigned int				lifetime;
		float						energyLevel;
		unsigned int				totalFrames;
		btDefaultMotionState* 		myMotionState;

	private:
// 		Settings					*settings;

// 		const int*					food_size;
};

#endif

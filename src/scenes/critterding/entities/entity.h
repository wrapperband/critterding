#ifndef ENTITY_H
#define ENTITY_H

#include "graphics/be_color.h"

enum entityType {
	CRITTER = 0,
	FOOD,
	WALL
};

class Entity
{
	public:
		Entity() {};
		virtual ~Entity() {};
		virtual void		draw() {};
		virtual void		draw( const bool do_color, const bool do_scale );

		entityType		type;
// 		float			color[4];
		BeColor			color;
		bool			isPicked;
	private:
};

#endif

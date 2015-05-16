#ifndef SEED_H
#define SEED_H

#include "../../brainz/brainz.h"
// #include "GL/gl.h"
#include "vector3f.h"
#include <vector>
#include <iostream>

class Seed
{
	public:
		Seed();
		~Seed();

		Brainz			brain;

		float			size;
		float			halfsize;
		float			energy;

		float			color[4];
		unsigned int		crittertype;
		unsigned int		adamdist;


		Vector3f		position;

		unsigned int		totalFrames;
		bool			pickedup;
	private:
};

#endif

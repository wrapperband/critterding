#ifndef CRITTERB_H
#define CRITTERB_H

#include "brainz/brainz.h"
#include "body.h"
#include "entity.h"
#include "resource/be_graphics_model_resource.h"

class Genotype;
class Genotypes;

class CritterB : public Entity
{
	public:
		CritterB(btDynamicsWorld* btWorld, long unsigned int id, const btTransform& startPos, unsigned char* retinap);
		CritterB(CritterB& , long unsigned int id, const btTransform& startPos, bool brainmutant, bool bodymutant);
		CritterB(std::string& critterstring, btDynamicsWorld* btWorld, const btTransform& startPos, unsigned char* retinap);
		~CritterB();

		Brainz			brain;
		Body			body;
		Genotype*		genotype;
		inline void		registerBrainInputOutputs();
// 		void			draw(bool drawFaces);
// 		void			draw( const bool do_color, const bool do_scale );
		void			move();
		
		bool			eaten;
		
		unsigned long		critterID;

// 		unsigned int		adamdist;
		btScalar		position[16];

// 		float			speciescolor[3];
// 		float			colorTrim;
		bool			loadError;

		float			energyLevel;
		float			energyUsed;
		unsigned int		totalFrames;

		// Inputs
		bool			canProcreate;
		bool			touchingFood;
		bool			touchingCritter;
		Entity*			touchedEntity;
		bool			beingTouched;
		bool			beingEaten;

		// Motor Func
 		bool			eat;
		bool			procreate;

		unsigned int		procreateTimeCount;
		unsigned int		procreateTimeTrigger;

		void			process();

		// Vision
// 		unsigned int		components;
		float			straal;
		unsigned char		*retina;
		unsigned int		items;
		void			place(const GLint v_inv);
		void 			place( const unsigned int pos_x, const unsigned int pos_y, const unsigned int size_x, const unsigned int size_y, const GLint v_inv );
// 		void			releaseFBO();
		void			calcFramePos(unsigned int pos);
		void			printVision();
		
		float			fitness_index;

// 		vector<unsigned int>	crittersWithinRange;
		
			const int*	colormode;
	boost::shared_ptr<BeGraphicsModelResource> m_model;

		btTransform m_render_offset;
	private:

		Settings*		settings;
		Genotypes*		genotypes;
// 		Raycast*		raycast;
// 		castResult		mouseRay;

		unsigned int		movementsmade;

		btDynamicsWorld*	btDynWorld;

		btVector3 getScreenDirection(const int& x, const int& y);

		inline void		initConst();
		inline void		procInputNeurons();
		inline void		createInputOutputNeurons();

		// Vision
			unsigned int		framePosX;
			unsigned int		framePosY;
			unsigned int		retinaColumnStart;
			unsigned int		retinaRowStart;
			unsigned int		retinaRowLength;
			unsigned int		visionPosition;

		// Settings pointers.. performance
			const unsigned int	retinasperrow;
// 			const int*	retinasperrow;
// 			const int*	critter_maxlifetime;
			const int*	critter_maxenergy;
// 			const int*	critter_sightrange;
			const int*	critter_procinterval;
			const int*	critter_minenergyproc;
// 			const int*	critter_raycastvision;

			const int*	brain_costhavingneuron;
			const int*	brain_costfiringneuron;
			const int*	brain_costfiringmotorneuron;
			const int*	brain_costhavingsynapse;
			const int*	body_costhavingbodypart;
			const int*	body_costtotalweight;
			
			const int*	setting_critter_mutate_minenergyproc;

		const float m_zNear;
		const float m_fovy;
		const float m_frustumHalf;
// 		float m_zFar;
		

};

#endif

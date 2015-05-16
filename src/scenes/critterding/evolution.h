#ifndef EVOLUTION_H_INCLUDED
#define EVOLUTION_H_INCLUDED

#include "gl/glwindow.h"
// #include <SDL/SDL.h>
#include "common/be_scene.h"
// #include "common/be_event_system.h"

// #include "entities/worldb.h"
// #include "modes/concavefloor.h"
#include "gui/canvas.h"
#include "gui/logbuffermessage.h"
#include "graphics/sc_graphics.h"
#include "gui/infobar.h"
#include "common/be_physics_debug_renderer.h"
#include "common/be_frame_limiter.h"

class WorldB;

class Evolution : public  BeScene {

	public:
		Evolution(BeFilesystem& filesystem);
		~Evolution();

		void process();

// 		void handleMouseMotionRel(int x, int y);
// 		void handleMouseMotionAbs(int x, int y);

		WorldB* world;

		// COMMANDS
			void decreaseenergy();
			void increaseenergy();
			void decreasefoodmaxenergy();
			void increasefoodmaxenergy();
			void dec_worldsizex();
			void inc_worldsizex();
			void dec_worldsizey();
			void inc_worldsizey();
			void dec_worldsizez();
			void inc_worldsizez();
			void selectCritterAll();
			void selectCritter(const unsigned int& c);
// 			void selectSpecies(const unsigned int& s);

			void camera_moveup();
			void camera_movedown();
			void camera_moveforward();
			void camera_movebackward();
			void camera_moveleft();
			void camera_moveright();
			void camera_lookup();
			void camera_lookdown();
			void camera_lookleft();
			void camera_lookright();
			void camera_rollleft();
			void camera_rollright();
// 			void camera_lookhorizontal();
// 			void camera_lookvertical();
// 			void camera_movehorizontal();
// 			void camera_movevertical();
			void unregisterCritterVID(int vectorID);
			void clear();
			void gui_selectCritter(int c);
			void gui_selectSpecies(int s);
			void gui_swap();

// 			void canvas_setx( float value );
// 			void canvas_sety( float value );
			void movePickedBodyX( const float value );
			void movePickedBodyY( const float value );
			void camera_lookleftright_mouse( const float value );
			void camera_lookupdown_mouse( const float value );

// 			void canvas_press();
// 			void canvas_release();
// 			void canvas_pressAlt();
// 			void canvas_releaseAlt();
			void canvas_swapchild(const std::string& child);

	private:
		Settings *settings;
		BeEventSystem *eventsystem;
		BeFilesystem& m_filesystem;
		void setLights();
		inline void initGLSL();
		
		const int* m_glsl;
		const int* m_hdr;
		bool m_glsl_initialised;
		boost::shared_ptr<ScGraphics> m_graphics;
// 		boost::shared_ptr<BeGraphicsEffect> m_effect;
// 		GLint m_v_inv;

		
		BePhysicsDebugRenderer	debugDrawer;

		unsigned int benchmark_start;
		const int* benchmark;
		const int* benchmark_frames;
		const int* drawscene;
		const int* drawdebug;
// 		const int* headless;
		const int* m_cameraSensitivity_move;
		const int* m_cameraSensitivity_look;

		const int* m_mode_race;
		const int* m_mode_round;
		
		const int* m_light_ambient_modelR;
		const int* m_light_ambient_modelG;
		const int* m_light_ambient_modelB;
		const int* m_light_ambientR;
		const int* m_light_ambientG;
		const int* m_light_ambientB;
		const int* m_light_diffuseR;
		const int* m_light_diffuseG;
		const int* m_light_diffuseB;
		const int* m_light_specularR;
		const int* m_light_specularG;
		const int* m_light_specularB;

		const int*	m_light_attenuation_constant;
		const int*	m_light_attenuation_linear;
		const int*	m_light_attenuation_quadratic;

		BeFrameLimiter		framelimiter;
		
		
		boost::shared_ptr<Canvas> m_canvas;

		double m_cameraTranslateSpeed;
		double m_cameraRotateSpeed;

		// GL WINDOW
		GLWindow*	m_glwindow;
		BeTimer		m_timer;
		boost::shared_ptr<BeGraphicsSystem> m_graphicsSystem;
		
		boost::shared_ptr<Logbuffer>	m_logBuffer;
		float cam_position[16];
		
		btTransform			m_select_up;
		
		unsigned int		m_currentFramesPerSecond;
		unsigned int		m_averageFramesPerSecond;
		
		boost::shared_ptr<Infobar> m_infobar;
		void			selectBody();
};
#endif

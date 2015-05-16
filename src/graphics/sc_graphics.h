#ifndef SC_GRAPHICS_H_INCLUDED
#define SC_GRAPHICS_H_INCLUDED

#include "be_graphics_system.h"
#include "filesystem/be_filesystem.h"
#include "common/be_model_system.h"

class ScGraphics
{
public:
	ScGraphics( boost::shared_ptr<BeGraphicsSystem> graphicsSystem, BeFilesystem& filesystem);
	~ScGraphics();

	void resetBrightTexture( const int w, const int h );
	
	boost::shared_ptr<BeGraphicsSystem> m_graphicsSystem;

	//Default Shader

	boost::shared_ptr<BeGraphicsEffect> m_effect;
	boost::shared_ptr<BeGraphicsEffect> m_critter_effect;
	GLint m_v_inv;
	GLint m_tex2Loc;
// 	boost::shared_ptr<BeGraphicsEffect> m_glsl_effect;

	//DOF Shaders

	boost::shared_ptr<BeGraphicsEffect> m_dofSceneEffect;
	boost::shared_ptr<BeGraphicsEffect> m_dofSkyboxEffect;
	boost::shared_ptr<BeGraphicsEffect> m_dofDownsampleEffect;
	boost::shared_ptr<BeGraphicsEffect> m_dofFilterXEffect;
	boost::shared_ptr<BeGraphicsEffect> m_dofFilterYEffect;
	boost::shared_ptr<BeGraphicsEffect> m_dofFinalEffect;

	boost::shared_ptr<BeTexture2D> m_dofSceneTexture;
	boost::shared_ptr<BeTexture2D> m_dofDownsampleTexture[3];
	boost::shared_ptr<BeFramebuffer> m_dofSceneFramebuffer;
	boost::shared_ptr<BeFramebuffer> m_dofDownsampleFramebuffer[3];


	//HDR Shaders

	boost::shared_ptr<BeGraphicsEffect> m_luminanceEffect2;
	boost::shared_ptr<BeGraphicsEffect> m_luminanceEffect3;
	boost::shared_ptr<BeGraphicsEffect> m_brightEffect;
	boost::shared_ptr<BeGraphicsEffect> m_gaussianBlurHorizonalEffect;
	boost::shared_ptr<BeGraphicsEffect> m_gaussianBlurVerticalEffect;
	boost::shared_ptr<BeGraphicsEffect> m_tonemapEffect;
	boost::shared_ptr<BeGraphicsEffect> m_addRgbEffect;


	boost::shared_ptr<BeTexture2D> m_sceneTexture;
	boost::shared_ptr<BeRenderbuffer> m_sceneRenderbuffer;
	boost::shared_ptr<BeFramebuffer> m_sceneFramebuffer;
	
	
	boost::shared_ptr<BeFramebuffer> m_brightFramebuffer;
	boost::shared_ptr<BeFramebuffer> m_tonemapFramebuffer;
	boost::shared_ptr<BeFramebuffer> m_gaussianFramebuffer[2];

	boost::shared_ptr<BeFramebuffer> m_luminanceFramebuffer2;

	boost::shared_ptr<BeFramebuffer> m_luminanceFramebuffer3[32];


	boost::shared_ptr<BeTexture2D> m_brightTexture;
	boost::shared_ptr<BeTexture2D> m_tonemapTexture;
	boost::shared_ptr<BeTexture2D> m_gaussianTexture[2];

	boost::shared_ptr<BeTexture2D> m_luminanceTexture2;

	boost::shared_ptr<BeTexture2D> m_luminanceTexture3[32];

private:
	int width_prev;
	int height_prev;
	


};

#endif
#include "enginesettingspanel.h"

Enginesettingspanel::Enginesettingspanel()
{
	isMovable = true;

	rowspacer = 15;
	col2 = 150;
	col3 = 200;
	
	m_localposition.set(2, 24);

	// FIXME camera sensitivity is missing

#if defined(HAVE_OPENMP)
	addSettingmutator("threads", 0, 0);
#endif
	addSettingmutator("fsX", 0, 0);
	addSettingmutator("fsY", 0, 0);
	addSettingmutator("fpslimit_frames", 0, 0);
	addSettingcheckbox("fpslimit", 0, 0);
	addSettingcheckbox("drawscene", 0, 0);
	addSettingcheckbox("glsl", 0, 0);
	addSettingcheckbox("hdr", 0, 0);
// 	addSettingcheckbox("drawshadows", 0, 0);
	addSettingcheckbox("window_fullscreen", 0, 0);
	addSettingmutator("camerasensitivity_move", 0, 0);
	addSettingmutator("camerasensitivity_look", 0, 0);
	addSettingmutator("camera_smoothfactor", 0, 0);
	
	
// 	addSettingcheckbox("sound", 0, 0);

	m_dimensions.set(col3 + 50,row-rowspacer+10+10);
	BeWidgetPtr w = addWidgetButton( "enginesettingspanel_close", Vector2i(2, 2), Vector2i(11, 11), "x", BeCommand("gui_togglepanel", "enginesettingspanel"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
}

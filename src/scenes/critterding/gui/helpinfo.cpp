#include "helpinfo.h"
#include "gui/textprinter.h"
#include <GL/glew.h>

Helpinfo::Helpinfo() : Panel(false)
{
	isMovable = true;
	m_localposition.set(2, 24);
	m_dimensions.set(420, 550);
	halfboxwidth = 210;
	halfboxheight = 270;
}

void Helpinfo::draw()
{
	if (m_active)
	{
		drawBackground();
		drawBorders();

	// print text
// 		glEnable(GL_TEXTURE_2D);

		int widthpos1 = getPosition().m_x + 20;
		int widthpos2 = getPosition().m_x + 100;
		int heightpos = getPosition().m_y;
		int vspace = 13;

		heightpos += vspace + 3;
		const auto delayed_heightpos1(heightpos);
		
		glColor3f(0.7f, 0.7f, 0.7f);
		heightpos += vspace + 3; printInfoLine(heightpos, widthpos1, widthpos2, "esc", "exit");

		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "tab", "toggle species panel");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F1", "toggle this help panel");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F2", "toggle small infobar");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F3", "toggle critter statistics");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F4", "toggle birth and death events panel");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F5", "toggle critters vs food graph");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F6", "toggle main settings panel");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F7", "toggle engine settings panel");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F8", "toggle hud");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F9", "toggle mutation panel");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F10", "toggle brain settings panel");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F11", "toggle body settings panel");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F12", "toggle population settings panel");
// 		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F9/F10", "dec/inc body mutation rate (%)");
// 		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "F11/F12", "dec/inc brain mutation rate (%)");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "+/-", "dec/inc energy in the system by 1 food unit");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "c", "switch critter color mode");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "d", "next debug drawing mode");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "f", "toggle fullscreen");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "h", "toggle render gui");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "i", "insert adam");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "k", "kill half of critters");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "l", "toggle fps limiter");
// 		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "m", "toggle mouse mode");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "p", "pause");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "r", "toggle render scene");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "s", "save profile to \"~/.critterding/save/profile/(profile).pro\"");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "v", "change camera view (follow mode)");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "pageup", "load all critters from \"~/.critterding/load\"");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "pagedown", "save all critters into \"~/.critterding/save/profile/(time)\"");

		heightpos += vspace + 3;
		const auto delayed_heightpos2(heightpos);

		heightpos += vspace + 3; printInfoLine(heightpos, widthpos1, widthpos2, "/", "dec camera sensitivity");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "*", "inc camera sensitivity");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "backspace", "reset camera");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "arrow up", "move camera forward");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "arrow down", "move camera backwards");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "arrow left", "strafe left");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "arrow right", "strafe right");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "home/end", "move up / down");
// 		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "End", "move down");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "numpad 2/8", "look up / down");
// 		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "NumKey 8", "look down");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "numpad 4/6", "look left / right");
// 		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "NumKey 6", "look right");
		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "numpad 1/3", "roll left / right");
// 		heightpos += vspace; printInfoLine(heightpos, widthpos1, widthpos2, "NumKey 3", "roll right");

// 		glDisable(GL_TEXTURE_2D);

		glColor3f(1.0f, 1.0f, 1.0f);
		printInfoLine(delayed_heightpos1, widthpos1, widthpos2, "World / Engine operations", "");
		printInfoLine(delayed_heightpos2, widthpos1, widthpos2, "Camera Operations", "");
		
	}
}

void Helpinfo::printInfoLine(float heightpos, float widthpos1, float widthpos2, const char* key, const char* expl)
{
	Textprinter::Instance()->print(Vector2i(widthpos1, heightpos), key);
	Textprinter::Instance()->print(Vector2i(widthpos2, heightpos), expl);
}

Helpinfo::~Helpinfo()
{
}

#include "DungeonEscape.hpp"
#include "gfx/pp2d.h"

FILE *debug_file;

u32 kDown, kHeld;

touchPosition touch;

PrintConsole bottomScreen, versionWin, killBox, debugBox;

FS_Archive sdmcArchive;

/// Room items
enum
{
	EMPTY = "empty",
	COINS = "coins",
	KILL = "kill"
};

class room {
public:
	std::vector<std::string> objects;
	room(std::vector<std::string>);
};

room::room(std::vector<std::string> fobjects) {
	objects = fobjects;
}

class level {
public:
	int width;
	int height;
	std::vector<room> rooms;
	level(int, int, std::vector<std::vector<std::string>>);
};

level::level(int fwidth, int fheight, std::vector<std::vector<std::string>> fdata) {
	width = fwidth;
	height = fheight;
	rooms.resize(fdata.size());
	for (int i = 0; i < fdata.size(); i++) {
		rooms[i].objects = fdata[i];
	}
}

//init
std::string versiontxtt = "  Beta ", versiontxtn = "01.00.00";
std::string buildnumber = "18.05.01.1142";

std::vector<std::vector<std::string>> floor11{
	{"coins"},
	{"coins", "kill"},
	{"kill"},
	{"empty"}
};

level floor1(2, 2, floor11);

std::vector<level> chapter1{
	level(2, 2, std::vector<std::vector<std::string>>{ {"coins"}, {"coins", "kill"}, {"kill"}, {"empty"}})
};

/*open SD card filesystem*/
void openSD()
{
	FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
}

/*close SD card filesystem*/
void closeSD()
{
	FSUSER_CloseArchive(sdmcArchive);
}

/*returns if area is being touched by stylus*/
bool touchInBox(touchPosition touch, int x, int y, int w, int h)
{
	int tx = touch.px;
	int ty = touch.py;
	kDown = hidKeysDown();
	if (kDown & KEY_TOUCH && tx > x && tx < x + w && ty > y && ty < y + h)
		return true;
	else
		return false;
}

int main(int argc, char **argv)
{
	//debug_file = fopen("sdmc:/3ds/de_debug.txt", "w");
	pp2d_init();
	pp2d_set_screen_color(GFX_TOP, ABGR8(255, 149, 149, 149));
	//romfsInit();
	//csndInit();
	//initSound();
	//init_game_textures();

	//initialize_audio();

	srand(time(NULL));

	consoleInit(GFX_BOTTOM, &bottomScreen); consoleInit(GFX_BOTTOM, &versionWin);
	consoleInit(GFX_BOTTOM, &killBox); consoleInit(GFX_BOTTOM, &debugBox);

	consoleSetWindow(&versionWin, 6, 26, 34, 4);
	consoleSetWindow(&killBox, 0, 28, 40, 2);
	consoleSetWindow(&debugBox, 18, 4, 9, 12);

	int bottom_screen_text = 0;

	// Main loop
	while (aptMainLoop()) {
		hidScanInput();
		kDown = hidKeysDown();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
		if (bottom_screen_text == 0) {
			consoleSelect(&killBox); consoleClear();
			consoleSelect(&versionWin); consoleClear();
			consoleSelect(&bottomScreen); consoleClear();

			consoleSelect(&killBox);
			std::cout << CRESET ANSI B_RED CEND;
			for (int i = 0; i < 80; i++)
				std::cout << " ";

			consoleSelect(&versionWin);
			std::cout << CRESET "     Tap red area any time to exit";
			std::cout << "Breakout Version: " ANSI RED CEND << versiontxtt << CRESET " " ANSI YELLOW CEND << versiontxtn;
			std::cout << ANSI B_RED ASEP GREEN CEND "              Build: " << buildnumber;

			consoleSelect(&bottomScreen);
			std::cout << CRESET ANSI "20;0" PEND;

			std::cout << CRESET ANSI "2;0" PEND "Press Select to begin." << std::endl;
			std::cout << "Press X to see what I'm working on or have planned." << std::endl;
			std::cout << "Press Y to open level editor." << std::endl << std::endl;

			for (int i = 0; i < chapter1[0].rooms.size(); i++) {
				std::cout << "room" << i << ": ";
				for (int j = 0; j < chapter1[0].rooms[i].objects.size(); j++)
					std::cout << chapter1[0].rooms[i].objects[j];
				std::cout << "\n";
			}

			bottom_screen_text = 1;
		}
		/*pp2d_begin_draw(GFX_TOP, GFX_LEFT);

		pp2d_end_draw();*/

		hidTouchRead(&touch);

		/*exit game if red box touched*/
		if (touchInBox(touch, 0, 224, 320, 16)) {
			std::cout << "Exiting...\n";
			break;
		}
	}
	// Exit services
	pp2d_exit();

	//exitSound();

	return 0;
}
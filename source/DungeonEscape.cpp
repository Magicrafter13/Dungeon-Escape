#include "DungeonEscape.hpp"
#include "gfx/pp2d.h"

FILE *debug_file;

u32 kDown, kHeld, kUp;

touchPosition touch;

PrintConsole bottomScreen, versionWin, killBox, debugBox;

FS_Archive sdmcArchive;

/// Room items
enum
{
	WALL = 0,
	EMPTY = 1,
	COINS = 2,
	KILL = 3,
	WAY1INL = 4,
	WAY1INR = 5,
	WAY1INU = 6,
	WAY1IND = 7,
	WAY1OUTL = 8,
	WAY1OUTR = 9,
	WAY1OUTU = 10,
	WAY1OUTD = 11,
	WALL_L = 12,
	WALL_R = 13,
	WALL_U = 14,
	WALL_D = 15,
	START = 16,
	SMALL_RIGHT = 17,
	SMALL_LEFT = 18,
	SMALL_UP = 19,
	SMALL_DOWN = 20,
	CRAWL_LR = 21,
	CRAWL_UD = 22,
	CRAWL_LU = 23,
	CRAWL_LD = 24,
	CRAWL_RU = 25,
	CRAWL_RD = 26,
	CRAWL_T_D = 27, //This means Left Right and Down (middle is down)
	CRAWL_T_U = 28,
	CRAWL_T_L = 29,
	CRAWL_T_R = 30,
	CRAWL_4 = 31,
	LOCK_L = 32,
	LOCK_R = 33,
	LOCK_U = 34,
	LOCK_D = 35
};

class room {
public:
	std::vector<int> objects;
	int activates_room;
	room(std::vector<int>);
	room(std::vector<int>, int);
};

room::room(std::vector<int> fobjects) {
	objects = fobjects;
}

room::room(std::vector<int> fobjects, int fa_r) {
	objects = fobjects;
	activates_room = fa_r;
}

class level {
public:
	int width;
	int height;
	std::vector<room> rooms;
	level(int, int, std::vector<room>);
};

level::level(int fwidth, int fheight, std::vector<room> fdata) {
	width = fwidth;
	height = fheight;
	rooms = fdata;
}

//init
std::string versiontxtt = "  Beta ", versiontxtn = "01.00.00";
std::string buildnumber = "18.05.01.1142";

std::vector<level> chapter1{
	level(8, 10, {
		room({WALL}),
		room({WALL}),
		room({START}),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({WALL}), //row 1
		room({WALL}),
		room({WALL}),
		room({EMPTY, WALL_L, WALL_R}),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({WALL}), //row 2
		room({EMPTY, WALL_L, WALL_U}),
		room({EMPTY, WALL_U, WALL_D}),
		room({EMPTY, SMALL_RIGHT}),
		room({EMPTY, CRAWL_LR}),
		room({EMPTY, CRAWL_LD}),
		room({WALL}),
		room({WALL}),
		room({WALL}), //row 3
		room({EMPTY, WALL_L, WALL_R}),
		room({WALL}),
		room({EMPTY, WALL_L, WALL_R}),
		room({WALL}),
		room({EMPTY, WALL_L, SMALL_UP, WALL_D}),
		room({EMPTY, WALL_U, LOCK_R}),
		room({EMPTY, WALL_U, WALL_D, WALL_R}),
		room({WALL}), //row 4
		room({EMPTY, WALL_L, WALL_R}),
		room({WALL}),
		room({})
	})
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
	romfsInit();
	csndInit();
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
		kHeld = hidKeysHeld();
		kUp = hidKeysUp();
		if (kDown & KEY_START) break;
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

			for (u32 i = 0; i < chapter1[0].rooms.size(); i++) {
				std::cout << "room" << i << ": ";
				for (u32 j = 0; j < chapter1[0].rooms[i].objects.size(); j++)
					std::cout << chapter1[0].rooms[i].objects[j];
				std::cout << "\n";
			}

			bottom_screen_text = 1;
		}

		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
		pp2d_draw_texture(0, 0, 0);
		pp2d_end_draw();

		hidTouchRead(&touch);

		/*exit game if red box touched*/
		if (touchInBox(touch, 0, 224, 320, 16)) {
			std::cout << "Exiting...\n";
			break;
		}
	}
	// Exit services
	pp2d_exit();
	romfsExit();
	csndExit();

	return 0;
}
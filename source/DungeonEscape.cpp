#include "DungeonEscape.hpp"
#include "gfx/pp2d.h"

FILE *debug_file;

u32 kDown, kHeld, kUp;

touchPosition touch;

PrintConsole bottomScreen, versionWin, killBox, debugBox;

FS_Archive sdmcArchive;

/// Room items
enum room_items {
	WALL = 0,
	EMPTY = 1,
	COINS = 2,
	KILL = 3,
	WAY1INL = 4, WAY1INR = 5, WAY1INU = 6, WAY1IND = 7,
	WAY1OUTL = 8, WAY1OUTR = 9, WAY1OUTU = 10, WAY1OUTD = 11,
	WALL_L = 12, WALL_R = 13, WALL_U = 14, WALL_D = 15,
	START = 16,
	SMALL_RIGHT = 17, SMALL_LEFT = 18, SMALL_UP = 19, SMALL_DOWN = 20,
	CRAWL_LR = 21, CRAWL_UD = 22, // - |
	CRAWL_LU = 23, CRAWL_LD = 24, // -^ -.
	CRAWL_RU = 25, CRAWL_RD = 26, // ^- .-
	CRAWL_T_D = 27, CRAWL_T_U = 28, CRAWL_T_L = 29, CRAWL_T_R = 30, //This means Up Right and Down (middle is right)
	CRAWL_4 = 31, // +
	LOCK_L = 32, LOCK_R = 33, LOCK_U = 34, LOCK_D = 35,
	PRESSURE_PLATE = 36,
	UNLOCK_L = 37, UNLOCK_R = 38, UNLOCK_U = 39, UNLOCK_D = 40,
	TELEPORT = 41, NULL_TELEPORT = 42,
	FORCE_U = 43, FORCE_D = 44, FORCE_L = 45, FORCE_R = 46,
	EXIT = 47, POWERUP = 48, HIDDEN = 49
};

/// Powerups
enum powerup_enum {
	TINY = 0,
	CROUCH = 1
};

class room {
public:
	std::vector<int> objects;
	int activates_room;
	std::vector<int> before_activation;
	std::vector<int> after_activation;
	bool has_before_and_after = false;
	std::string current_object_set = "before"; //set to toggle between before and after when activated
	int teleport_to;
	int powerup[2];
	room(std::vector<int>);
	room(std::vector<int>, int);
	room(std::vector<int>, std::vector<int>);
	room(std::vector<int>, int[2]);
};

room::room(std::vector<int> fobjects) {
	objects = fobjects;
}

room::room(std::vector<int> fobjects, int linked) {
	objects = fobjects;
	if (std::find(objects.begin(), objects.end(), TELEPORT) != objects.end()) {
		teleport_to = linked;
	}
	if (std::find(objects.begin(), objects.end(), PRESSURE_PLATE) != objects.end()) {
		activates_room = linked;
	}
}

room::room(std::vector<int> fobjects_b, std::vector<int> fobjects_a) {
	objects = fobjects_b;
	before_activation = fobjects_b;
	after_activation = fobjects_a;
	has_before_and_after = true;
}

room::room(std::vector<int> fobjects, int powerups[2]) {
	objects = fobjects;
	powerup[0] = powerups[0];
	powerup[1] = powerups[1];
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
		room({EMPTY, WALL_U, LOCK_R}, {EMPTY, WALL_U, UNLOCK_R}),
		room({EXIT, WALL_U, WALL_D, WALL_R}),
		room({WALL}), //row 4
		room({EMPTY, WALL_L, WALL_R}),
		room({WALL}),
		room({EMPTY, WALL_L, WALL_D}, {EMPTY, WALL_L}),
		room({EMPTY, WALL_U, WALL_D}),
		room({TELEPORT, WALL_U, WALL_R, WALL_D}, 49),
		room({EMPTY, CRAWL_UD}),
		room({WALL}),
		room({WALL}), //row 5
		room({EMPTY, WALL_L, WALL_R}),
		room({EMPTY, WALL_U, WALL_L, WALL_D}),
		room({WALL}, {EMPTY, WALL_R, WALL_D}), //room 43 (42 + 1)
		room({WALL_U, WALL_L, PRESSURE_PLATE}, 42),
		room({EMPTY, WALL_U, WALL_D, SMALL_RIGHT}),
		room({CRAWL_LU, PRESSURE_PLATE}, 29),
		room({WALL}),
		room({WALL}), //row 6
		room({EMPTY, WALL_L, WALL_R}),
		room({WALL_U, WALL_L, WALL_D, TELEPORT}, 36),
		room({EMPTY, WALL_U, WALL_D}),
		room({EMPTY, WALL_R, WALL_D}),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({WALL_U, WALL_L, WALL_R, FORCE_D}), //row 7
		room({EMPTY, WALL_L, WALL_R}),
		room({WALL}),
		room({TELEPORT, WALL_L, WALL_U, WALL_R}, 55),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({PRESSURE_PLATE, WALL_L, WALL_R}, 65), //row 8
		room({EMPTY, WALL_L, WALL_D}),
		room({EMPTY}, {EMPTY, WALL_L, WALL_R}),
		room({EMPTY}),
		room({HIDDEN, KILL, WALL_U, WALL_R, WALL_D}),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({TELEPORT, WALL_L, WALL_D, WALL_R}, 10), //row 9
		room({WALL}),
		room({WALL}),
		room({WALL_L, WALL_D, WALL_R, POWERUP}, new int[2]{TINY, 3}),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({WALL}),
		room({WALL}) //row 10
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
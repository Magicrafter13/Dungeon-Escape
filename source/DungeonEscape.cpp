#include "DungeonEscape.hpp"
#include "gfx/pp2d.h"

FILE *debug_file;

u32 kDown, kHeld, kUp;

touchPosition touch;

PrintConsole bottomScreen, versionWin, killBox, debugBox;

FS_Archive sdmcArchive;

size_t protected_textures = 0, textures = 0;

std::string error_code;
std::string error_message;

int game();

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

size_t
wallID,
coinsID,
killID,
way1inlID, way1inrID, way1inuID, way1indID,
way1outlID, way1outrID, way1outuID, way1outdID,
wall_lID, wall_rID, wall_uID, wall_dID,
wall_connector_lID, wall_connector_rID, wall_connector_uID, wall_connector_dID,
startID,
crawl_lrID, crawl_udID,
crawl_luID, crawl_ldID,
crawl_ruID, crawl_rdID,
crawl_t_dID, crawl_t_uID, crawl_t_lID, crawl_t_rID,
crawl_4ID,
lock_lID, lock_rID, lock_uID, lock_dID,
pressure_plateID,
teleportID, null_teleportID,
force_uID, force_dID, force_lID, force_rID,
exitID, powerupID, hiddenID, playerID,
pause_scr, ps_0, ps_1, ps_2, ps_3, titleID;
std::vector<size_t> ps_arrow(4);

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
	bool hasObject(int object) {
		return (std::find(objects.begin(), objects.end(), object) != objects.end());
	}
	bool hasObjectsAnd(std::vector<int> object) {
		bool it_does = true;
		for (unsigned int i = 0; i < object.size(); i++)
			if (!hasObject(object[i]))
				it_does = false;
		return it_does;
	}
	bool hasObjectsOr(std::vector<int> object) {
		bool has_one = false;
		for (unsigned int i = 0; i < object.size(); i++)
			if (hasObject(object[i]))
				has_one = true;
		return has_one;
	}
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

class player {
	int default_x;
	int default_y;
	int default_texture;
	int default_location;
public:
	int x;
	int y;
	int texture;
	int location; //(easy for mapping, instead of using x and y, just use how far into the level's vector the player is)
	bool is_tiny = false;
	player(int, int, int);
	void reset() {
		x = default_x;
		y = default_y;
		texture = default_texture;
		location = default_location;
		is_tiny = false;
	}
};

player::player(int fx, int fy, int ftexture) {
	x = fx; default_x = fx;
	y = fy; default_y = fy;
	texture = ftexture; default_texture = ftexture;
	location = 0; default_location = 0;
}

//init
std::string versiontxtt = "  Beta ", versiontxtn = "01.00.00";
std::string buildnumber = "18.05.01.1142";

std::vector<level> chapter1{
	level(8, 10, {
		room({WALL}),
		room({WALL}),
		room({START, WALL_L, WALL_U, WALL_R}),
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
		room({EMPTY, WALL_U, WALL_D}, {EMPTY, WALL}),
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

void load_textures_p() {
	size_t id = 0;
	size_t i = id;
	std::vector<std::string> pause_images = {
		"",
		"_text_0",
		"_text_1",
		"_text_2",
		"_text_3",
		"_arrow_0",
		"_arrow_1",
		"_arrow_2",
		"_arrow_3"
	};
	std::vector<size_t*> pause_images_p = {
		&pause_scr,
		&ps_0,
		&ps_1,
		&ps_2,
		&ps_3,
		&ps_arrow[0],
		&ps_arrow[1],
		&ps_arrow[2],
		&ps_arrow[3]
	};
	for (id = i; id < i + pause_images.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/temp_pausescreen" + pause_images[id - i] + ".png").c_str());
		*pause_images_p[id - i] = id;
		protected_textures++;
	}
	i = id;
	std::vector<std::string> title_screen = {
		"title"
	};
	std::vector<size_t*> title_screen_p = {
		&titleID
	};
	for (id = i; id < i + title_screen.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/" + title_screen[id - i] + ".png").c_str());
		*title_screen_p[id - i] = id;
		protected_textures++;
	}
	//load textures and add to protected_textures
}

void load_textures() {
	size_t id = protected_textures;
	size_t i = id;
	std::vector<std::string> walls = {
		"_l",
		"_r",
		"_u",
		"_d",
		"",
		"_connector_l",
		"_connector_r",
		"_connector_u",
		"_connector_d"
	};
	std::vector<size_t*> walls_p = {
		&wall_lID,
		&wall_rID,
		&wall_uID,
		&wall_dID,
		&wallID,
		&wall_connector_lID,
		&wall_connector_rID,
		&wall_connector_uID,
		&wall_connector_dID
	};
	for (id = i; id < i + walls.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/wall" + walls[id - i] + ".png").c_str());
		*walls_p[id - i] = id;
		textures++;
	}
	i = id;
	std::vector<std::string> crawl_space = {
		"_lr",
		"_ud",
		"_lu",
		"_ld",
		"_ru",
		"_rd",
		"_4"
	};
	std::vector<size_t*> crawl_space_p = {
		&crawl_lrID,
		&crawl_udID,
		&crawl_luID,
		&crawl_ldID,
		&crawl_ruID,
		&crawl_rdID,
		&crawl_4ID
	};
	for (id = i; id < i + crawl_space.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/crawl" + crawl_space[id - i] + ".png").c_str());
		*crawl_space_p[id - i] = id;
		textures++;
	}
	i = id;
	std::vector<std::string> misc = {
		"pressure_plate",
		"lock_l",
		"lock_r",
		"lock_u",
		"lock_d",
		"teleport",
		"player"
	};
	std::vector<size_t*> misc_p = {
		&pressure_plateID,
		&lock_lID,
		&lock_rID,
		&lock_uID,
		&lock_dID,
		&teleportID,
		&playerID
	};
	for (id = i; id < i + misc.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/" + misc[id - i] + ".png").c_str());
		*misc_p[id - i] = id;
		textures++;
	}
}

player player1(-1, -1, 0);

void unload_textures() {
	for (size_t i = protected_textures; i < protected_textures + textures; i++)
		pp2d_free_texture(i);
}

bool paused = false;
int paused_selection = 0;
std::string paused_level = "top";

int main(int argc, char **argv)
{
	//debug_file = fopen("sdmc:/3ds/de_debug.txt", "w");
	pp2d_init();
	pp2d_set_screen_color(GFX_TOP, ABGR8(255, 149, 149, 149));
	romfsInit();
	csndInit();
	load_textures_p();
	load_textures();

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
		if (kDown & KEY_START) {
			player1.reset();
			player1.texture = playerID;
			int temp = 0, tempX = 0, tempY = 0;
			while (player1.x == -1) {
				if (chapter1[0].rooms[temp].hasObject(START)) {
					player1.x = tempX;
					player1.y = tempY;
					player1.location = temp;
				}
				else {
					temp++;
					tempX++;
					if (tempX >= chapter1[0].width) {
						tempX = 0;
						tempY++;
					}
				}
			}
			int ret_val = 0;
			while (ret_val == 0)
				ret_val = game();
			if (ret_val == 2)
				break;
			if (ret_val == 3) {
				//error found
				consoleSelect(&bottomScreen); consoleClear();
				std::cout << ANSI B_BLUE CEND;
				for (int i = 0; i < 1120; i++)
					std::cout << " ";
				std::cout << ANSI B_RED CEND;
				for (int i = 0; i < 80; i++)
					std::cout << " ";
				std::cout << PRESET ANSI B_BLUE ASEP WHITE CEND;
				std::cout << "Dungeon Escape has encountered an error.\n\n";
				std::cout << ANSI B_BLUE ASEP RED CEND;
				std::cout << "Err Code: " << error_code << "\n\n";
				std::cout << "Message: " << error_message << "\n\n";
				std::cout << ANSI B_BLUE ASEP WHITE CEND;
				std::cout << "Please go to our GitHub page: https://github.com/Magicrafter13/Dungeon-Escape \n";
				std::cout << "And submit a new issue on the issues\npage.\n";
				std::cout << "(Unless this is already a known issue.)\n";
				std::cout << "Also, make sure to provide the exact \nsteps to recreate this error/glitch.\n";
				std::cout << "     Thank you for your time, and sorry about this error.\n\n";
				std::cout << "Press any key, or tap the bottom most area of the screen to exit the program.\n";
				bool ready_exit = false;
				while (!ready_exit) {
					hidScanInput();
					kDown = hidKeysDown();
					pp2d_begin_draw(GFX_TOP, GFX_LEFT);
					pp2d_draw_texture(0, 400, 240);
					pp2d_end_draw();
					if (kDown & (KEY_A | KEY_B | KEY_Y | KEY_X | KEY_L | KEY_R | KEY_ZL | KEY_ZR | KEY_SELECT | KEY_START | KEY_LEFT | KEY_RIGHT | KEY_UP | KEY_DOWN))
						ready_exit = true;

					hidTouchRead(&touch);

					/*exit game if red box touched*/
					if (touchInBox(touch, 0, 224, 320, 16)) {
						std::cout << "Exiting...\n";
						ready_exit = true;
					}
				}
				break;
			}
			paused = false;
			paused_level = "top";
			paused_selection = 0;
			bottom_screen_text = 0;
		}
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

			for (u32 i = 0; i < chapter1[0].rooms.size() - 60; i++) {
				std::cout << "room" << i << ": ";
				for (u32 j = 0; j < chapter1[0].rooms[i].objects.size(); j++)
					std::cout << chapter1[0].rooms[i].objects[j] << ", ";
				std::cout << "\n";
			}

			std::cout << wall_lID << " " << wall_rID << " " << wall_uID << " " << wall_dID << " " << wallID << "\n";

			bottom_screen_text = 1;
		}

		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
		pp2d_draw_texture(titleID, 0, 0);
		pp2d_end_draw();

		hidTouchRead(&touch);

		/*exit game if red box touched*/
		if (touchInBox(touch, 0, 224, 320, 16)) {
			std::cout << "Exiting...\n";
			break;
		}
	}
	// Exit services
	unload_textures();
	pp2d_exit();
	romfsExit();
	csndExit();

	return 0;
}

int paused_action(int action) {
	if (paused_level == "top") {
		if (action == 3) {
			return 1;
		}
		else {
			error_code = "ps-act_top_act_invalid";
			error_message = "Invalid pause menu action.";
			return 3;
		}
	}
	else {
		error_code = "ps-act_lvl_invalid";
		error_message = "Invalid pause menu level.";
		return 3;
	}
	//else
}

void paused_return_level() {
	if (paused_level == "top") {
		paused = false;
		paused_selection = 0;
	}
}

int game() {
	int ret_val = 0;
	pp2d_begin_draw(GFX_TOP, GFX_LEFT);
	pp2d_draw_texture(playerID, 12 * 16, 7 * 16); //(13, 8)
	int temp = 0;
	for (int y = 0; y < chapter1[0].height; y++) {
		for (int x = 0; x < chapter1[0].width; x++) {
			int rel_x = 12 - (player1.x - x);
			int rel_y = 7 - (player1.y - y);
			if (chapter1[0].rooms[temp].hasObject(WALL_L)) {
				pp2d_draw_texture(wall_lID, rel_x * 16, rel_y * 16);
				if (chapter1[0].rooms[temp - chapter1[0].width].hasObject(WALL_L))
					pp2d_draw_texture(wall_connector_lID, rel_x * 16, (rel_y * 16) - 2);
			}
			if (chapter1[0].rooms[temp].hasObject(WALL_R)) {
				pp2d_draw_texture(wall_rID, 16 * rel_x, 16 * rel_y);
				if (chapter1[0].rooms[temp - chapter1[0].width].hasObject(WALL_R))
					pp2d_draw_texture(wall_connector_rID, (rel_x * 16) + 13, (rel_y * 16) - 2);
			}
			if (chapter1[0].rooms[temp].hasObject(WALL_U)) {
				pp2d_draw_texture(wall_uID, 16 * rel_x, 16 * rel_y);
				if (chapter1[0].rooms[temp - 1].hasObject(WALL_U))
					pp2d_draw_texture(wall_connector_uID, (rel_x * 16) - 2, rel_y * 16);
			}
			if (chapter1[0].rooms[temp].hasObject(WALL_D)) {
				pp2d_draw_texture(wall_dID, 16 * rel_x, 16 * rel_y);
				if (chapter1[0].rooms[temp - 1].hasObject(WALL_D))
					pp2d_draw_texture(wall_connector_dID, (rel_x * 16) - 2, (rel_y * 16) + 13);
			}
			if (chapter1[0].rooms[temp].hasObject(WALL))
				pp2d_draw_texture(wallID, 16 * rel_x, 16 * rel_y);
			if (chapter1[0].rooms[temp].hasObject(CRAWL_LR))
				pp2d_draw_texture(crawl_lrID, 16 * rel_x, 16 * rel_y);
			if (chapter1[0].rooms[temp].hasObject(CRAWL_UD))
				pp2d_draw_texture(crawl_udID, 16 * rel_x, 16 * rel_y);
			if (chapter1[0].rooms[temp].hasObject(CRAWL_LU))
				pp2d_draw_texture(crawl_luID, 16 * rel_x, 16 * rel_y);
			if (chapter1[0].rooms[temp].hasObject(CRAWL_LD))
				pp2d_draw_texture(crawl_ldID, 16 * rel_x, 16 * rel_y);
			if (chapter1[0].rooms[temp].hasObject(CRAWL_RU))
				pp2d_draw_texture(crawl_ruID, 16 * rel_x, 16 * rel_y);
			if (chapter1[0].rooms[temp].hasObject(CRAWL_RD))
				pp2d_draw_texture(crawl_rdID, 16 * rel_x, 16 * rel_y);
			if (chapter1[0].rooms[temp].hasObject(PRESSURE_PLATE))
				pp2d_draw_texture(pressure_plateID, 16 * rel_x, 16 * rel_y);
			if (chapter1[0].rooms[temp].hasObject(LOCK_L)) {
				pp2d_draw_texture(lock_lID, 16 * rel_x, 16 * rel_y);
				pp2d_draw_texture(lock_rID, 16 * (rel_x - 1), 16 * rel_y);
			}
			if (chapter1[0].rooms[temp].hasObject(LOCK_R)) {
				pp2d_draw_texture(lock_rID, 16 * rel_x, 16 * rel_y);
				pp2d_draw_texture(lock_lID, 16 * (rel_x + 1), 16 * rel_y);
			}
			if (chapter1[0].rooms[temp].hasObject(LOCK_U)) {
				pp2d_draw_texture(lock_uID, 16 * rel_x, 16 * rel_y);
				pp2d_draw_texture(lock_dID, 16 * rel_x, 16 * (rel_y - 1));
			}
			if (chapter1[0].rooms[temp].hasObject(LOCK_D)) {
				pp2d_draw_texture(lock_dID, 16 * rel_x, 16 * rel_y);
				pp2d_draw_texture(lock_uID, 16 * rel_x, 16 * (rel_y + 1));
			}
			if (chapter1[0].rooms[temp].hasObject(TELEPORT))
				pp2d_draw_texture(teleportID, 16 * rel_x, 16 * rel_y);
			temp++;
		}
	}
	if (paused) {
		pp2d_draw_texture(pause_scr, 0, 0);
		pp2d_draw_texture(ps_0, 99, 0);
		pp2d_draw_texture(ps_1, 99, 0);
		pp2d_draw_texture(ps_2, 99, 0);
		pp2d_draw_texture(ps_3, 99, 0);
		pp2d_draw_texture(ps_arrow[paused_selection], 99, 0);
	}
	pp2d_end_draw();

	hidTouchRead(&touch);

	/*exit game if red box touched*/
	if (touchInBox(touch, 0, 224, 320, 16)) {
		std::cout << "Exiting...\n";
		return 2;
	}

	hidScanInput();
	kDown = hidKeysDown();

	if (kDown & KEY_START) {
		paused = (paused ? false : true);
		if (!paused) {
			paused_selection = 0;
			paused_level = "top";
		}
	}

	if (paused) {
		if (kDown & KEY_UP) {
			paused_selection--;
			if (paused_selection == -1)
				paused_selection = 3;
		}
		else if (kDown & KEY_DOWN) {
			paused_selection++;
			if (paused_selection == 4)
				paused_selection = 0;
		}
		else if (kDown & KEY_A) {
			ret_val = paused_action(paused_selection);
		}
		else if (kDown & KEY_B) {
			paused_return_level();
		}
	}
	else {
		if (kDown & KEY_LEFT) {
			if (!chapter1[0].rooms[player1.location].hasObjectsOr({WALL_L, CRAWL_UD, CRAWL_RU, CRAWL_RD})) {
				if (chapter1[0].rooms[player1.location].hasObject(SMALL_LEFT)) {
					if (player1.is_tiny) {
						player1.x--;
						player1.location -= 1;
					}
					else {
						//play sound?
					}
				}
				else if (chapter1[0].rooms[player1.location].hasObject(LOCK_L)) {
					//if (player1.hasObject(KEY) {
						//unlock
					//}
					//else {
						//play sound?
					//}
				}
				else {
					player1.x--;
					player1.location -= 1;
				}
			}
			else {
				//play sound?
			}
		}
		else if (kDown & KEY_RIGHT) {
			if (!chapter1[0].rooms[player1.location].hasObject(WALL_R)) {
				if (chapter1[0].rooms[player1.location].hasObject(SMALL_RIGHT)) {
					if (player1.is_tiny) {
						player1.x++;
						player1.location += 1;
					}
					else {
						//play sound?
					}
				}
				else if (chapter1[0].rooms[player1.location].hasObject(LOCK_R)) {
					//if (player1.hasObject(KEY) {
						//unlock
					//}
					//else {
						//play sound?
					//}
				}
				else if (!chapter1[0].rooms[player1.location].hasObjectsOr({CRAWL_UD, CRAWL_LU, CRAWL_LD})) {
					player1.x++;
					player1.location += 1;
				}
			}
		}
		else if (kDown & KEY_UP) {
			if (!chapter1[0].rooms[player1.location].hasObject(WALL_U)) {
				if (chapter1[0].rooms[player1.location].hasObject(SMALL_UP)) {
					if (player1.is_tiny) {
						player1.y--;
						player1.location -= chapter1[0].width;
					}
					else {
						//play sound?
					}
				}
				else if (chapter1[0].rooms[player1.location].hasObject(LOCK_U)) {
					//if (player1.hasObject(KEY) {
						//unlock
					//}
					//else {
						//play sound?
					//}
				}
				else if (!chapter1[0].rooms[player1.location].hasObjectsOr({CRAWL_LR, CRAWL_LD, CRAWL_RD})) {
					player1.y--;
					player1.location -= chapter1[0].width;
				}
			}
		}
		else if (kDown & KEY_DOWN) {
			if (!chapter1[0].rooms[player1.location].hasObject(WALL_D)) {
				if (chapter1[0].rooms[player1.location].hasObject(SMALL_DOWN)) {
					if (player1.is_tiny) {
						player1.y++;
						player1.location += chapter1[0].width;
					}
					else {
						//play sound?
					}
				}
				else if (chapter1[0].rooms[player1.location].hasObject(LOCK_D)) {
					//if (player1.hasObject(KEY) {
						//unlock
					//}
					//else {
						//play sound?
					//}
				}
				else if (!chapter1[0].rooms[player1.location].hasObjectsOr({CRAWL_LR, CRAWL_LU, CRAWL_RU})) {
					player1.y++;
					player1.location += chapter1[0].width;
				}
			}
		}
		else if (kDown & KEY_X) {
			player1.is_tiny = (player1.is_tiny ? false : true);
		}
	}

	return ret_val;
}

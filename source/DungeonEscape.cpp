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
bool error = false;

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
	EXIT = 47, POWERUP = 48, HIDDEN = 49,
	SPIKES = 50, CHEST = 51,
	SPIKE_WALL_L = 52, SPIKE_WALL_R = 53, SPIKE_WALL_U = 54, SPIKE_WALL_D = 55
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
pause_scr, ps_0, ps_1, ps_2, ps_3, titleID, inventoryID, inventory_selectedID,
error_50x50, emptyID, floorID,
small_invID, none_leftID, counterID, player_smallID, endID,
spike_wall_lID, spike_wall_rID, spike_wall_uID, spike_wall_dID;
std::vector<size_t> ps_arrow(4), nxx(10), xnx(10), xxn(10);

/// Powerups
enum powerup_enum {
	TINY = 0,
	CROUCH = 1,
	//COINS already defined as 2
	KEY = 3,
	LIFE = 4
};

class room {
	std::vector<int> dobjects;
	std::vector<int> dbefore_activation, dafter_activation;
	int dpowerup[2];
public:
	std::vector<int> objects;
	int activates_room;
	std::vector<int> before_activation;
	std::vector<int> after_activation;
	bool has_before_and_after = false;
	std::string current_object_set = "before"; //set to toggle between before and after when activated
	int teleport_to;
	int powerup[2];
	bool activates_multiple;
	std::vector<int> rooms_activated;
	room(std::vector<int>);
	room(std::vector<int>, int);
	room(std::vector<int>, std::vector<int>, bool);
	room(std::vector<int>, int[2]);
	room(std::vector<int>, std::vector<int>);
	room(std::vector<int>, std::vector<int>, bool, std::vector<int>);
	room(std::vector<int>, std::vector<int>, int[2], bool);
	void reset() {
		objects = dobjects;
		before_activation = dbefore_activation;
		after_activation = dafter_activation;
		current_object_set = "before";
		powerup[0] = dpowerup[0]; powerup[1] = dpowerup[1];
	}
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
	void activate() {
		if (has_before_and_after) {
			if (current_object_set == "before") {
				current_object_set = "after";
				objects = after_activation;
			}
			else if (current_object_set == "after") {
				current_object_set = "before";
				objects = before_activation;
			}
			else {
				error_code = "rm_chng_obj-set";
				error_message = "Could not determine which object\nset the room needed.";
				error = true;
			}
		}
		else {
			if (current_object_set == "before") {
				current_object_set = "after";
				objects = after_activation;
			}
		}
	}
	void addObject(int object) {
		objects.push_back(object);
	}
	void removeObject(int object) {
		std::vector<int> temp_objects = objects;
		objects.clear();
		for (unsigned int i = 0; i < temp_objects.size(); i++)
			if (temp_objects[i] != object)
				objects.push_back(temp_objects[i]);
	}
};

room::room(std::vector<int> fobjects) {
	objects = fobjects;
	dobjects = fobjects;
}

room::room(std::vector<int> fobjects, int linked) {
	objects = fobjects;
	dobjects = fobjects;
	if (std::find(objects.begin(), objects.end(), TELEPORT) != objects.end()) {
		teleport_to = linked;
	}
	if (std::find(objects.begin(), objects.end(), PRESSURE_PLATE) != objects.end()) {
		activates_room = linked;
	}
}

room::room(std::vector<int> fobjects_b, std::vector<int> fobjects_a, bool hbaa) {
	objects = fobjects_b;
	dobjects = fobjects_b;
	before_activation = fobjects_b;
	dbefore_activation = fobjects_b;
	after_activation = fobjects_a;
	dafter_activation = fobjects_a;
	has_before_and_after = hbaa;
	activates_multiple = false;
}

room::room(std::vector<int> fobjects, int powerups[2]) {
	objects = fobjects;
	dobjects = fobjects;
	powerup[0] = powerups[0];
	dpowerup[0] = powerups[0];
	powerup[1] = powerups[1];
	dpowerup[1] = powerups[1];
}

room::room(std::vector<int> fobjects, std::vector<int> rooms_to_activate) {
	activates_multiple = true;
	rooms_activated = rooms_to_activate;
	objects = fobjects;
	dobjects = fobjects;
}

room::room(std::vector<int> fobjects_b, std::vector<int> fobjects_a, bool hbaa, std::vector<int> rooms_to_activate) {
	objects = fobjects_b;
	dobjects = fobjects_b;
	before_activation = fobjects_b;
	dbefore_activation = fobjects_b;
	after_activation = fobjects_a;
	dafter_activation = fobjects_a;
	has_before_and_after = hbaa;
	activates_multiple = true;
	rooms_activated = rooms_to_activate;
}

room::room(std::vector<int> fobjects_b, std::vector<int> fobjects_a, int powerups[2], bool hbaa) {
	objects = fobjects_b;
	dobjects = fobjects_b;
	before_activation = fobjects_b;
	dbefore_activation = fobjects_b;
	after_activation = fobjects_a;
	dafter_activation = fobjects_a;
	has_before_and_after = hbaa;
	activates_multiple = false;
	powerup[0] = powerups[0];
	dpowerup[0] = powerups[0];
	powerup[1] = powerups[1];
	dpowerup[1] = powerups[1];
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

class level_set {
public:
	int level_count;
	std::vector<level> levels;
	level_set(int, std::vector<level>);
};

level_set::level_set(int lvl_count, std::vector<level> lvls) {
	level_count = lvl_count;
	levels = lvls;
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
	std::vector<int> objects;
	std::vector<int> inventory = {
		0, //TINY/SMALL
		0, //CRAWL
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	};
	bool entered_small = false;
	int lives = 3;
	player(int, int, int);
	std::vector<bool> hidden_map;
	bool force_u, force_d, force_l, force_r;
	void reset() {
		x = default_x;
		y = default_y;
		texture = default_texture;
		location = default_location;
		is_tiny = false;
		for (unsigned int i = 0; i < inventory.size(); i++)
			inventory[i] = 0;
		for (unsigned int i = 0; i < hidden_map.size(); i++)
			hidden_map[i] = true;
		entered_small = false;
		lives = 3;
	}
	void addObject(int object) {
		objects.push_back(object);
	}
	void removeObject(int object) {
		std::vector<int> temp_objects = objects;
		objects.clear();
		for (unsigned int i = 0; i < temp_objects.size(); i++)
			if (temp_objects[i] != object)
				objects.push_back(object);
	}
	bool hasObject(int object) {
		return (std::find(objects.begin(), objects.end(), object) != objects.end());
	}
	void addInventory(int object[2]) {
		inventory[object[0]] += object[1];
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

std::vector<level_set> game_data {
	level_set(8, std::vector<level>{
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
			room({EMPTY, WALL_U, LOCK_R, SMALL_DOWN}, {EMPTY, WALL_U, UNLOCK_R, SMALL_DOWN}, false),
			room({EXIT, WALL_U, WALL_D, WALL_R}),
			room({WALL}), //row 4
			room({EMPTY, WALL_L, WALL_R}),
			room({WALL}),
			room({EMPTY, WALL_L, WALL_D}, {EMPTY, WALL_L}, true),
			room({EMPTY, WALL_U, WALL_D}),
			room({TELEPORT, WALL_U, WALL_R, WALL_D}, 49),
			room({EMPTY, CRAWL_UD}),
			room({WALL}),
			room({WALL}), //row 5
			room({EMPTY, WALL_L, WALL_R}),
			room({WALL}, {EMPTY, WALL_U, WALL_L, WALL_D}, true),
			room({WALL}, {EMPTY, WALL_R, WALL_D}, true), //room 43 (42 + 1)
			room({WALL_U, WALL_L, PRESSURE_PLATE}, {34, 41, 42}),
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
			room({PRESSURE_PLATE, WALL_L, WALL_R}, {64, 65, 66}), //row 8
			room({EMPTY, WALL_L, WALL_D}, {EMPTY, WALL_L, WALL_D, WALL_R}, true),
			room({EMPTY, WALL_U, WALL_D}, {EMPTY, WALL}, true),
			room({EMPTY}, {EMPTY, WALL_L}, true),
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
		}),
		level(9, 9, {
			room({START, WALL_U, WALL_L, WALL_D}),
			room({EMPTY, WALL_U, SMALL_DOWN, SMALL_RIGHT}),
			room({EMPTY, CRAWL_LR}),
			room({EMPTY, SMALL_LEFT, WALL_U, WALL_D, SMALL_RIGHT}),
			room({EMPTY, CRAWL_LD}),
			room({POWERUP, WALL_U, CRAWL_UD}, new int[2]{CROUCH, 3}),
			room({WALL}),
			room({WALL}),
			room({WALL}), //row 1
			room({WALL}),
			room({EMPTY, CRAWL_UD}),
			room({EMPTY, WALL_U, WALL_L}),
			room({EMPTY, WALL_U, WALL_D}),
			room({EMPTY, SMALL_DOWN, SMALL_UP, SMALL_RIGHT}),
			room({EMPTY, CRAWL_LU}),
			room({WALL}),
			room({WALL}),
			room({WALL}), //row 2
			room({HIDDEN, WALL_U, WALL_L, WALL_R, KILL}),
			room({WALL_L, WALL_D, WALL_R, SMALL_UP}, new int[2]{TINY, 3}),
			room({EMPTY, WALL_L, WALL_R}),
			room({HIDDEN, TELEPORT, WALL_L, WALL_U, WALL_R}, 24),
			room({SMALL_UP, WALL_L, WALL_D, SPIKES}),
			room({WALL_U, WALL_R, WALL_D, POWERUP}, new int[2]{KEY, 1}),
			room({WALL_U, WALL_L, WALL_D, NULL_TELEPORT}),
			room({WALL_U, WALL_D, SPIKES}),
			room({WALL_U, WALL_R, CHEST}), //row 3
			room({WALL_L, WALL_D, EMPTY}),
			room({HIDDEN, PRESSURE_PLATE, WALL_U, WALL_R}, {36, 37}),
			room({EMPTY, WALL_L, WALL_D}),
			room({EMPTY}),
			room({HIDDEN, WALL_U, WALL_R, WALL_D, KILL}),
			room({EMPTY, WALL_L, WALL_U}),
			room({EMPTY, WALL_U, WALL_D}),
			room({EMPTY, WALL_U, WALL_R}),
			room({EMPTY, WALL_L, WALL_R}, {EMPTY, WALL_L, WALL_R, WALL_D}, true), //row 4
			room({HIDDEN, WALL}, {HIDDEN, EXIT, WALL_U, WALL_L, WALL_D}, true),
			room({EMPTY, WALL_L}, {EMPTY}, true),
			room({EMPTY, WALL_U, WALL_R}),
			room({HIDDEN, WALL_L, WALL_D, WALL_R, KILL}),
			room({WALL}),
			room({EMPTY, WALL_L, WALL_R}),
			room({WALL}),
			room({SPIKE_WALL_L, SPIKE_WALL_D, PRESSURE_PLATE}, {SPIKE_WALL_L, SPIKE_WALL_D, PRESSURE_PLATE, WALL_R}, true, {43, 44, 35}),
			room({EMPTY, WALL_R, WALL_D}, {WALL}, true), //row 5
			room({WALL}),
			room({HIDDEN, WALL_L, WALL_D, WALL_R, PRESSURE_PLATE}, {36, 37}),
			room({EMPTY, WALL_L, WALL_D}),
			room({EMPTY, WALL_U, WALL_D}),
			room({SPIKE_WALL_U, WALL_D}, new int[2]{COINS, 2}),
			room({WALL_D}, {WALL_D, POWERUP}, new int[2]{LIFE, 1}, false),
			room({HIDDEN, WALL_U, WALL_D, TELEPORT, SPIKE_WALL_R}, 60),
			room({WALL}),
			room({WALL}), //row 6
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL}, {WALL}, true),
			room({WALL_U, WALL_L, WALL_R, TELEPORT}, 51),
			room({WALL}),
			room({WALL}), //row 7
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL_U, WALL_L, WALL_D, HIDDEN, PRESSURE_PLATE}, {50, 59}),
			room({EMPTY}),
			room({HIDDEN, WALL_U, WALL_D, WALL_R, POWERUP}, new int[2]{COINS, 50}),
			room({WALL}), //row 8
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL}),
			room({WALL_L, WALL_D, WALL_R, HIDDEN, POWERUP}, new int[2]{TINY, 3}),
			room({WALL}),
			room({WALL}) //row 9
		}),
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
		"title",
		"inventory",
		"inventory_selector",
		"error_50x50",
		"empty",
		"floor",
		"black_80x80"
	};
	std::vector<size_t*> title_screen_p = {
		&titleID,
		&inventoryID,
		&inventory_selectedID,
		&error_50x50,
		&emptyID,
		&floorID,
		&hiddenID
	};
	for (id = i; id < i + title_screen.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/" + title_screen[id - i] + ".png").c_str());
		*title_screen_p[id - i] = id;
		protected_textures++;
	}
	i = id;
	std::vector<std::string> powerups = {
		"small_inv",
		"none_left",
		"0xx",
		"1xx",
		"2xx",
		"3xx",
		"4xx",
		"5xx",
		"6xx",
		"7xx",
		"8xx",
		"9xx",
		"x0x",
		"x1x",
		"x2x",
		"x3x",
		"x4x",
		"x5x",
		"x6x",
		"x7x",
		"x8x",
		"x9x",
		"xx0",
		"xx1",
		"xx2",
		"xx3",
		"xx4",
		"xx5",
		"xx6",
		"xx7",
		"xx8",
		"xx9",
		"counter"
	};
	std::vector<size_t*> powerups_p = {
		&small_invID,
		&none_leftID,
		&nxx[0],
		&nxx[1],
		&nxx[2],
		&nxx[3],
		&nxx[4],
		&nxx[5],
		&nxx[6],
		&nxx[7],
		&nxx[8],
		&nxx[9],
		&xnx[0],
		&xnx[1],
		&xnx[2],
		&xnx[3],
		&xnx[4],
		&xnx[5],
		&xnx[6],
		&xnx[7],
		&xnx[8],
		&xnx[9],
		&xxn[0],
		&xxn[1],
		&xxn[2],
		&xxn[3],
		&xxn[4],
		&xxn[5],
		&xxn[6],
		&xxn[7],
		&xxn[8],
		&xxn[9],
		&counterID
	};
	for (id = i; id < i + powerups.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/" + powerups[id - i] + ".png").c_str());
		*powerups_p[id - i] = id;
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
	std::vector<std::string> one_way = {
		"outl",
		"outr",
		"outu",
		"outd",
		"inl",
		"inr",
		"inu",
		"ind"
	};
	std::vector<size_t*> one_way_p = {
		&way1outlID,
		&way1outrID,
		&way1outuID,
		&way1outdID,
		&way1inlID,
		&way1inrID,
		&way1inuID,
		&way1indID
	};
	for (id = i; id < i + one_way_p.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/way1" + one_way[id - i] + ".png").c_str());
		*one_way_p[id - i] = id;
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
		"player",
		"temp_powerup",
		"player_tiny",
		"kill",
		"end",
		"spike_wall_u",
		"spike_wall_d",
		"spike_wall_l",
		"spike_wall_r"
	};
	std::vector<size_t*> misc_p = {
		&pressure_plateID,
		&lock_lID,
		&lock_rID,
		&lock_uID,
		&lock_dID,
		&teleportID,
		&playerID,
		&powerupID,
		&player_smallID,
		&killID,
		&endID,
		&spike_wall_uID,
		&spike_wall_dID,
		&spike_wall_lID,
		&spike_wall_rID
	};
	for (id = i; id < i + misc.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/" + misc[id - i] + ".png").c_str());
		*misc_p[id - i] = id;
		textures++;
	}
}

player player1(-1, -1, 0);
int lvl = 0;
int chapter = 0;

void unload_textures() {
	for (size_t i = protected_textures; i < protected_textures + textures; i++)
		pp2d_free_texture(i);
}

bool paused = false;
int paused_selection = 0;
std::string paused_level = "top";
bool inventory = false;
int inventory_selection = 0;

int inventoryItem(int powerup) {
	switch (powerup) {
	case 0:
		return small_invID;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
		return emptyID;
		break;
	default:
		return error_50x50;
	}
}

void tryInventory(int selection) {
	switch (selection) {
	case 0:
		if (player1.inventory[0] > 0 && !player1.is_tiny) {
			player1.inventory[0]--;
			player1.is_tiny = true;
		}
		break;
	}
}

int main(int argc, char **argv)
{
	//debug_file = fopen("sdmc:/3ds/de_debug.txt", "w");
	pp2d_init();
	pp2d_set_screen_color(GFX_TOP, ABGR8(255, 0, 0, 0));
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
			chapter = 0;
			lvl = 0;
			player1.reset();
			player1.texture = playerID;
			int temp = 0, tempX = 0, tempY = 0;
			level *temp_level = &game_data[chapter].levels[lvl];
			while (player1.x == -1) {
				if (temp_level->rooms[temp].hasObject(START)) {
					player1.x = tempX;
					player1.y = tempY;
					player1.location = temp;
				}
				else {
					temp++;
					tempX++;
					if (tempX >= temp_level->width) {
						tempX = 0;
						tempY++;
					}
				}
			}
			for (unsigned int i = 0; i < temp_level->rooms.size(); i++)
				temp_level->rooms[i].reset();
			player1.hidden_map.resize(temp_level->rooms.size());
			player1.hidden_map.clear();
			for (unsigned int i = 0; i < temp_level->rooms.size(); i++) {
				player1.hidden_map[i] = true;
			}
			player1.hidden_map[player1.location] = false;
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

			bottom_screen_text = 1;
		}

		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
		pp2d_draw_texture(titleID, 0, 0);
		pp2d_draw_texture(wall_connector_lID, 0, 0);
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
	int tRoom = 0;
	level *curLevel = &game_data[chapter].levels[lvl];
	for (int y = 0; y < curLevel->height; y++) {
		for (int x = 0; x < curLevel->width; x++) {
			int rel_x = 2 - (player1.x - x);
			int rel_y = 1 - (player1.y - y);
			if (rel_x >= 0 && rel_y >= 0 && rel_x <= curLevel->width && rel_y <= curLevel->height && !curLevel->rooms[tRoom].hasObject(WALL) && !player1.hidden_map[tRoom])
				pp2d_draw_texture(floorID, 80 * rel_x, 80 * rel_y);
			if (!player1.hidden_map[tRoom]) {
				if (curLevel->rooms[tRoom].hasObject(WALL_L))
					pp2d_draw_texture(wall_lID, rel_x * 80, rel_y * 80);
				if (curLevel->rooms[tRoom].hasObject(WALL_R))
					pp2d_draw_texture(wall_rID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WALL_U))
					pp2d_draw_texture(wall_uID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WALL_D))
					pp2d_draw_texture(wall_dID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WALL))
					pp2d_draw_texture(wallID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(CRAWL_LR))
					pp2d_draw_texture(crawl_lrID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(CRAWL_UD))
					pp2d_draw_texture(crawl_udID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(CRAWL_LU))
					pp2d_draw_texture(crawl_luID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(CRAWL_LD))
					pp2d_draw_texture(crawl_ldID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(CRAWL_RU))
					pp2d_draw_texture(crawl_ruID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(CRAWL_RD))
					pp2d_draw_texture(crawl_rdID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(PRESSURE_PLATE))
					pp2d_draw_texture(pressure_plateID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(LOCK_L)) {
					pp2d_draw_texture(lock_lID, 80 * rel_x, 80 * rel_y);
					pp2d_draw_texture(lock_rID, 80 * (rel_x - 1), 80 * rel_y);
				}
				if (curLevel->rooms[tRoom].hasObject(LOCK_R)) {
					pp2d_draw_texture(lock_rID, 80 * rel_x, 80 * rel_y);
					pp2d_draw_texture(lock_lID, 80 * (rel_x + 1), 80 * rel_y);
				}
				if (curLevel->rooms[tRoom].hasObject(LOCK_U)) {
					pp2d_draw_texture(lock_uID, 80 * rel_x, 80 * rel_y);
					pp2d_draw_texture(lock_dID, 80 * rel_x, 80 * (rel_y - 1));
				}
				if (curLevel->rooms[tRoom].hasObject(LOCK_D)) {
					pp2d_draw_texture(lock_dID, 80 * rel_x, 80 * rel_y);
					pp2d_draw_texture(lock_uID, 80 * rel_x, 80 * (rel_y + 1));
				}
				if (curLevel->rooms[tRoom].hasObject(WAY1OUTL))
					pp2d_draw_texture(way1outlID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WAY1OUTR))
					pp2d_draw_texture(way1outrID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WAY1OUTU))
					pp2d_draw_texture(way1outuID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WAY1OUTD))
					pp2d_draw_texture(way1outdID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WAY1INL))
					pp2d_draw_texture(way1inlID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WAY1INR))
					pp2d_draw_texture(way1inrID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WAY1INU))
					pp2d_draw_texture(way1inuID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(WAY1IND))
					pp2d_draw_texture(way1indID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(SPIKE_WALL_L))
					pp2d_draw_texture(spike_wall_lID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(SPIKE_WALL_R))
					pp2d_draw_texture(spike_wall_rID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(SPIKE_WALL_U))
					pp2d_draw_texture(spike_wall_uID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(SPIKE_WALL_D))
					pp2d_draw_texture(spike_wall_dID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObjectsOr({TELEPORT, NULL_TELEPORT}))
					pp2d_draw_texture(teleportID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(POWERUP))
					pp2d_draw_texture(powerupID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(KILL))
					pp2d_draw_texture(killID, 80 * rel_x, 80 * rel_y);
				if (curLevel->rooms[tRoom].hasObject(EXIT))
					pp2d_draw_texture(endID, 80 * rel_x, 80 * rel_y);
			}
			if (curLevel->rooms[tRoom].hasObject(WALL_L) && tRoom - curLevel->width >= 0)
				if (curLevel->rooms[tRoom - curLevel->width].hasObject(WALL_L))
					pp2d_draw_texture(wall_connector_lID, (rel_x * 80), ((rel_y * 80) - 10));
			if (curLevel->rooms[tRoom].hasObject(WALL_R) && tRoom - curLevel->width >= 0)
				if (curLevel->rooms[tRoom - curLevel->width].hasObject(WALL_R))
					pp2d_draw_texture(wall_connector_rID, ((rel_x * 80) + 65), ((rel_y * 80) - 10));
			if (curLevel->rooms[tRoom].hasObject(WALL_U) && tRoom - 1 >= 0)
				if (curLevel->rooms[tRoom - 1].hasObject(WALL_U))
					pp2d_draw_texture(wall_connector_uID, ((rel_x * 80) - 10), (rel_y * 80));
			if (curLevel->rooms[tRoom].hasObject(WALL_D) && tRoom - 1 >= 0)
				if (curLevel->rooms[tRoom - 1].hasObject(WALL_D))
					pp2d_draw_texture(wall_connector_dID, ((rel_x * 80) - 10), ((rel_y * 80) + 65));
			if (curLevel->rooms[tRoom].hasObject(WALL) && !player1.hidden_map[tRoom])
				pp2d_draw_texture(wallID, 80 * rel_x, 80 * rel_y);
			tRoom++;
		}
	}
	tRoom = 0;
	for (int y = 0; y < curLevel->height; y++) {
		for (int x = 0; x < curLevel->width; x++) {
			int rel_x = 2 - (player1.x - x);
			int rel_y = 1 - (player1.y - y);
			if (player1.hidden_map[tRoom])
				pp2d_draw_texture(hiddenID, 80 * rel_x, 80 * rel_y);
			tRoom++;
		}
	}
	if (player1.is_tiny)
		pp2d_draw_texture(player_smallID, 2 * 80, 1 * 80); //(3, 2)
	else
		pp2d_draw_texture(playerID, 2 * 80, 1 * 80);
	if (paused) {
		pp2d_draw_texture(pause_scr, 0, 0);
		pp2d_draw_texture(ps_0, 99, 0);
		pp2d_draw_texture(ps_1, 99, 0);
		pp2d_draw_texture(ps_2, 99, 0);
		pp2d_draw_texture(ps_3, 99, 0);
		pp2d_draw_texture(ps_arrow[paused_selection], 99, 0);
	}
	if (inventory) {
		pp2d_draw_texture(inventoryID, 0, 0);
		int selector = 0;
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 8; x++) {
				pp2d_draw_texture(inventoryItem(selector), x * 50, (y * 50) + 40);
				if (selector == inventory_selection)
					pp2d_draw_texture(inventory_selectedID, x * 50, (y * 50) + 40);
				if (player1.inventory[selector] == 0)
					if (selector < 2)
						pp2d_draw_texture(none_leftID, x * 50, (y * 50) + 40);
				selector++;
			}
		}
		pp2d_draw_texture(inventoryItem(inventory_selection), 6 * 50, (3 * 50) + 40);
		pp2d_draw_texture(counterID, 7 * 50, (3 * 50) + 40);
		std::string temp_numbers = "";
		if (player1.inventory[inventory_selection] < 100)
			temp_numbers += "0";
		if (player1.inventory[inventory_selection] < 10)
			temp_numbers += "0";
		temp_numbers += std::to_string(player1.inventory[inventory_selection]);
		int first = temp_numbers.at(0) - '0';
		int second = temp_numbers.at(1) - '0';
		int third = temp_numbers.at(2) - '0';
		pp2d_draw_texture(nxx[first], 7 * 50, (3 * 50) + 40);
		pp2d_draw_texture(xnx[second], 7 * 50, (3 * 50) + 40);
		pp2d_draw_texture(xxn[third], 7 * 50, (3 * 50) + 40);
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

	bool triggerKill = false;

	if (kDown & KEY_START) {
		if (!inventory) {
			paused = (paused ? false : true);
			if (!paused) {
				paused_selection = 0;
				paused_level = "top";
			}
		}
	}

	bool has_moved = false;

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
	else if (inventory) {
		if (kDown & KEY_B)
			inventory = false;
		else if (kDown & KEY_UP)
			inventory_selection -= 8;
		else if (kDown & KEY_DOWN)
			inventory_selection += 8;
		else if (kDown & KEY_LEFT)
			inventory_selection--;
		else if (kDown & KEY_RIGHT)
			inventory_selection++;
		else if (kDown & KEY_A)
			tryInventory(inventory_selection);
		if (inventory_selection <= -1)
			inventory_selection = 0;
		if (inventory_selection >= 32)
			inventory_selection = 31;
		if (kDown & (KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT))
			std::cout << inventory_selection << " ";
	}
	else {
		room curRoom = curLevel->rooms[player1.location];
		if (kDown & KEY_LEFT) {
			if (!curRoom.hasObjectsOr({WALL_L, CRAWL_UD, CRAWL_RU, CRAWL_RD, WAY1OUTL, LOCK_L})) {
				if (curRoom.hasObject(SPIKE_WALL_L))
					triggerKill = true;
				if (curRoom.hasObject(SMALL_LEFT)) {
					if (player1.is_tiny) {
						player1.x--;
						player1.location -= 1;
						has_moved = true;
						player1.entered_small = true;
					}
					else {
						//play sound?
					}
				}
				else {
					player1.x--;
					player1.location -= 1;
					has_moved = true;
				}
			}
			else {
				//play sound?
			}
		}
		else if (kDown & KEY_RIGHT) {
			if (!curRoom.hasObjectsOr({WALL_R, CRAWL_UD, CRAWL_LU, CRAWL_LD, WAY1OUTR, LOCK_R})) {
				if (curRoom.hasObject(SPIKE_WALL_R))
					triggerKill = true;
				if (curRoom.hasObject(SMALL_RIGHT)) {
					if (player1.is_tiny) {
						player1.x++;
						player1.location += 1;
						has_moved = true;
						player1.entered_small = true;
					}
					else {
						//play sound?
					}
				}
				else {
					player1.x++;
					player1.location += 1;
					has_moved = true;
				}
			}
		}
		else if (kDown & KEY_UP) {
			if (!curRoom.hasObjectsOr({WALL_U, CRAWL_LR, CRAWL_LD, CRAWL_RD, WAY1OUTU, LOCK_U})) {
				if (curRoom.hasObject(SPIKE_WALL_U))
					triggerKill = true;
				if (curRoom.hasObject(SMALL_UP)) {
					if (player1.is_tiny) {
						player1.y--;
						player1.location -= curLevel->width;
						has_moved = true;
						player1.entered_small = true;
					}
					else {
						//play sound?
					}
				}
				else {
					player1.y--;
					player1.location -= curLevel->width;
					has_moved = true;
				}
			}
		}
		else if (kDown & KEY_DOWN) {
			if (!curRoom.hasObjectsOr({WALL_D, CRAWL_LR, CRAWL_LU, CRAWL_RU, WAY1OUTD, LOCK_D})) {
				if (curRoom.hasObject(SPIKE_WALL_D))
					triggerKill = true;
				if (curRoom.hasObject(SMALL_DOWN)) {
					if (player1.is_tiny) {
						player1.y++;
						player1.location += curLevel->width;
						has_moved = true;
						player1.entered_small = true;
					}
					else {
						//play sound?
					}
				}
				else {
					player1.y++;
					player1.location += curLevel->width;
					has_moved = true;
				}
			}
		}
		else if (kDown & KEY_X) {
			player1.is_tiny = (player1.is_tiny ? false : true);
		}
		else if (kDown & KEY_Y) {
			inventory = true;
		}
	}

	int curLoc = player1.location;
	size_t curLoc_t = curLoc;
	player1.hidden_map[curLoc] = false;
	if (curLoc - curLevel->width >= 0)
		if (!curLevel->rooms[curLoc].hasObjectsOr({WALL_U, WAY1OUTU}) || curLevel->rooms[curLoc - curLevel->width].hasObject(WALL))
			if (!curLevel->rooms[curLoc - curLevel->width].hasObject(HIDDEN))
				player1.hidden_map[curLoc - curLevel->width] = false;
	if (curLoc_t + curLevel->width < curLevel->rooms.size())
		if (!curLevel->rooms[curLoc].hasObjectsOr({WALL_D, WAY1OUTD}) || curLevel->rooms[curLoc + curLevel->width].hasObject(WALL))
			if (!curLevel->rooms[curLoc + curLevel->width].hasObject(HIDDEN))
				player1.hidden_map[curLoc + curLevel->width] = false;
	if (curLoc - 1 >= 0)
		if (!curLevel->rooms[curLoc].hasObjectsOr({WALL_L, WAY1OUTL}) || curLevel->rooms[curLoc - 1].hasObject(WALL))
			if (!curLevel->rooms[curLoc - 1].hasObject(HIDDEN))
				player1.hidden_map[curLoc - 1] = false;
	if (curLoc_t + 1 < curLevel->rooms.size())
		if (!curLevel->rooms[curLoc].hasObjectsOr({WALL_R, WAY1OUTR}) || curLevel->rooms[curLoc + 1].hasObject(WALL))
			if (!curLevel->rooms[curLoc + 1].hasObject(HIDDEN))
				player1.hidden_map[curLoc + 1] = false;

	if (player1.force_u) {
		if (curLevel->rooms[player1.location].hasObjectsOr({WALL_U, SMALL_UP, LOCK_U, WAY1OUTU})) {
			player1.force_u = false;
		}
		else {
			if (curLevel->rooms[player1.location].hasObject(SPIKE_WALL_U))
				triggerKill = true;
			else {
				has_moved = true;
				player1.location -= curLevel->width;
				player1.y--;
			}
		}
	}
	if (player1.force_d) {
		if (curLevel->rooms[player1.location].hasObjectsOr({WALL_D, SMALL_DOWN, LOCK_D, WAY1OUTD})) {
			player1.force_d = false;
		}
		else {
			if (curLevel->rooms[player1.location].hasObject(SPIKE_WALL_D))
				triggerKill = true;
			else {
				has_moved = true;
				player1.location += curLevel->width;
				player1.y++;
			}
		}
	}
	if (player1.force_l) {
		if (curLevel->rooms[player1.location].hasObjectsOr({WALL_L, SMALL_LEFT, LOCK_L, WAY1OUTL})) {
			player1.force_l = false;
		}
		else {
			if (curLevel->rooms[player1.location].hasObject(SPIKE_WALL_L))
				triggerKill = true;
			else {
				has_moved = true;
				player1.location--;
				player1.x--;
			}
		}
	}
	if (player1.force_r) {
		if (curLevel->rooms[player1.location].hasObjectsOr({WALL_R, SMALL_RIGHT, LOCK_R, WAY1OUTR})) {
			player1.force_r = false;
		}
		else {
			if (curLevel->rooms[player1.location].hasObject(SPIKE_WALL_R))
				triggerKill = true;
			else {
				has_moved = true;
				player1.location++;
				player1.x++;
			}
		}
	}
	if (has_moved) {
		room* curRoom = &curLevel->rooms[player1.location];
		if (curRoom->hasObject(TELEPORT)) {
			int teleport_to = curRoom->teleport_to;
			int temp = -1;
			player1.force_l = false;
			player1.force_r = false;
			player1.force_u = false;
			player1.force_d = false;
			for (int y = 0; y < curLevel->height; y++) {
				for (int x = 0; x < curLevel->width; x++) {
					temp++;
					if (temp == teleport_to) {
						player1.x = x;
						player1.y = y;
						player1.location = teleport_to;
					}
				}
			}
		}
		curRoom = &curLevel->rooms[player1.location];
		if (curRoom->hasObject(PRESSURE_PLATE)) {
			if (curRoom->activates_multiple) {
				for (unsigned int i = 0; i < curRoom->rooms_activated.size(); i++)
					curLevel->rooms[curRoom->rooms_activated[i]].activate();
			}
			else
				curLevel->rooms[curRoom->activates_room].activate();
		}
		if (curRoom->hasObject(POWERUP)) {
			player1.addInventory(curRoom->powerup);
			curRoom->removeObject(POWERUP);
		}
		if (!curRoom->hasObjectsOr({CRAWL_4, CRAWL_LR, CRAWL_UD, CRAWL_LU, CRAWL_LD, CRAWL_RU, CRAWL_RD}) && player1.entered_small) {
			player1.entered_small = false;
			player1.is_tiny = false;
		}
		if (curRoom->hasObject(FORCE_U)) {
			player1.force_l = false;
			player1.force_r = false;
			player1.force_d = false;
			player1.force_u = true;
		}
		if (curRoom->hasObject(FORCE_D)) {
			player1.force_l = false;
			player1.force_r = false;
			player1.force_u = false;
			player1.force_d = true;
		}
		if (curRoom->hasObject(FORCE_L)) {
			player1.force_u = false;
			player1.force_d = false;
			player1.force_r = false;
			player1.force_l = true;
		}
		if (curRoom->hasObject(FORCE_R)) {
			player1.force_u = false;
			player1.force_d = false;
			player1.force_l = false;
			player1.force_r = true;
		}
		if (curRoom->hasObject(KILL) || triggerKill) {
			int lives = player1.lives - 1;
			std::vector<int> inventory = player1.inventory;
			player1.reset();
			player1.lives = lives;
			player1.inventory = inventory;
			int temp = 0, tempX = 0, tempY = 0;
			while (player1.x == -1) {
				if (curLevel->rooms[temp].hasObject(START)) {
					player1.x = tempX;
					player1.y = tempY;
					player1.location = temp;
				}
				else {
					temp++;
					tempX++;
					if (tempX >= curLevel->width) {
						tempX = 0;
						tempY++;
					}
				}
			}
		}
		if (curRoom->hasObject(EXIT)) {
			int lives = player1.lives;
			std::vector<int> inventory = player1.inventory;
			player1.reset();
			player1.lives = lives;
			player1.inventory = inventory;
			lvl++;
			if (lvl == game_data[chapter].level_count) {
				lvl = 0;
				chapter++;
				if (chapter == game_data.size()) {
					//game win or whatever
					ret_val = 1;
				}
			}
			curLevel = &game_data[chapter].levels[lvl];
			int temp = 0, tempX = 0, tempY = 0;
			while (player1.x == -1) {
				if (curLevel->rooms[temp].hasObject(START)) {
					player1.x = tempX;
					player1.y = tempY;
					player1.location = temp;
				}
				else {
					temp++;
					tempX++;
					if (tempX >= curLevel->width) {
						tempX = 0;
						tempY++;
					}
				}
			}
			player1.hidden_map.resize(curLevel->rooms.size());
			player1.hidden_map.clear();
			for (unsigned int i = 0; i < curLevel->rooms.size(); i++) {
				player1.hidden_map[i] = true;
			}
		}
	}

	curLoc = player1.location;
	player1.hidden_map[curLoc] = false;

	if (player1.lives <= 0)
		return 1;

	return ret_val;
}

#include "DungeonEscape.hpp"
#include "levels.hpp"

#define MAX_SPRITES   88
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

size_t st_null = 0;

std::string buildnumber = "18.06.08.1309";

static C2D_SpriteSheet spriteSheet;

void draw_texture(size_t id, float x, float y) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, id), x, y, 1.0f, NULL, 1.0f, 1.0f);
}

FILE *debug_file;

u32 kDown, kHeld, kUp;

touchPosition touch;

PrintConsole bottomScreen, versionWin, killBox, debugBox;
C3D_RenderTarget* top, *bot;

FS_Archive sdmcArchive;

size_t protected_textures = 0, textures = 0;

std::string error_code;
std::string error_message;
std::string debug_log = "Debugger started:\n";
bool error = false, debug = false, update_debug_screen = true;

int game();

/*class level_set {
public:
int level_count;
std::vector<level> levels;
level_set(int, std::vector<level>);
};

level_set::level_set(int lvl_count, std::vector<level> lvls) {
level_count = lvl_count;
levels = lvls;
}*/

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

void init_textures() {
	wallID = sprites_wall_idx;
	coinsID = 0;
	killID = sprites_kill_idx;
	way1inlID = sprites_way1inl_idx; way1inrID = sprites_way1inr_idx; way1inuID = sprites_way1inu_idx; way1indID = sprites_way1ind_idx;
	way1outlID = sprites_way1outl_idx; way1outrID = sprites_way1outr_idx; way1outuID = sprites_way1outu_idx; way1outdID = sprites_way1outd_idx;
	wall_lID = sprites_wall_l_idx; wall_rID = sprites_wall_r_idx; wall_uID = sprites_wall_u_idx; wall_dID = sprites_wall_d_idx;
	wall_connector_lID = sprites_wall_connector_l_idx; wall_connector_rID = sprites_wall_connector_r_idx; wall_connector_uID = sprites_wall_connector_u_idx; wall_connector_dID = sprites_wall_connector_d_idx;
	startID = 0;
	crawl_lrID = sprites_crawl_lr_idx; crawl_udID = sprites_crawl_ud_idx;
	crawl_luID = sprites_crawl_lu_idx; crawl_ldID = sprites_crawl_ld_idx;
	crawl_ruID = sprites_crawl_ru_idx; crawl_rdID = sprites_crawl_rd_idx;
	crawl_t_dID = 0; crawl_t_uID = 0; crawl_t_lID = 0; crawl_t_rID = 0;
	crawl_4ID = sprites_crawl_4_idx;
	lock_lID = sprites_lock_l_idx; lock_rID = sprites_lock_r_idx; lock_uID = sprites_lock_u_idx; lock_dID = sprites_lock_d_idx;
	pressure_plateID = sprites_pressure_plate_idx;
	teleportID = sprites_teleport_idx; null_teleportID = sprites_teleport_idx;
	force_uID = 0; force_dID = 0; force_lID = 0; force_rID = 0;
	exitID = sprites_end_idx; powerupID = sprites_temp_powerup_idx; hiddenID = sprites_black_80x80_idx; playerID = sprites_player_idx;
	pause_scr = sprites_temp_pausescreen_idx; ps_0 = sprites_temp_pausescreen_text_0_idx; ps_1 = sprites_temp_pausescreen_text_1_idx; ps_2 = sprites_temp_pausescreen_text_2_idx; ps_3 = sprites_temp_pausescreen_text_3_idx; titleID = sprites_title_idx; inventoryID = sprites_inventory_idx; inventory_selectedID = sprites_inventory_selector_idx;
	error_50x50 = sprites_error_50x50_idx; emptyID = sprites_empty_idx; floorID = sprites_floor_idx;
	small_invID = sprites_small_inv_idx; none_leftID = sprites_none_left_idx; counterID = sprites_counter_idx; player_smallID = sprites_player_tiny_idx; endID = sprites_exit_idx;
	spike_wall_lID = sprites_spike_wall_l_idx; spike_wall_rID = sprites_spike_wall_r_idx; spike_wall_uID = sprites_spike_wall_u_idx; spike_wall_dID = sprites_spike_wall_d_idx;
	ps_arrow[0] = sprites_temp_pausescreen_arrow_0_idx; ps_arrow[1] = sprites_temp_pausescreen_arrow_1_idx; ps_arrow[2] = sprites_temp_pausescreen_arrow_2_idx; ps_arrow[3] = sprites_temp_pausescreen_arrow_3_idx;
	nxx[0] = sprites_0xx_idx; nxx[1] = sprites_1xx_idx; nxx[2] = sprites_2xx_idx; nxx[3] = sprites_3xx_idx; nxx[4] = sprites_4xx_idx; nxx[5] = sprites_5xx_idx; nxx[6] = sprites_6xx_idx; nxx[7] = sprites_7xx_idx; nxx[8] = sprites_8xx_idx; nxx[9] = sprites_9xx_idx;
	xnx[0] = sprites_x0x_idx; xnx[1] = sprites_x1x_idx; xnx[2] = sprites_x2x_idx; xnx[3] = sprites_x3x_idx; xnx[4] = sprites_x4x_idx; xnx[5] = sprites_x5x_idx; xnx[6] = sprites_x6x_idx; xnx[7] = sprites_x7x_idx; xnx[8] = sprites_x8x_idx; xnx[9] = sprites_x9x_idx;
	xxn[0] = sprites_xx0_idx; xxn[1] = sprites_xx1_idx; xxn[2] = sprites_xx2_idx; xxn[3] = sprites_xx3_idx; xxn[4] = sprites_xx4_idx; xxn[5] = sprites_xx5_idx; xxn[6] = sprites_xx6_idx; xxn[7] = sprites_xx7_idx; xxn[8] = sprites_xx8_idx; xxn[9] = sprites_xx9_idx;
}

size_t getTexID(room_items item) {
	switch (item) {
	case WALL: return wallID;
	case KILL: return killID;
	case WAY1INL: return way1inlID;
	case WAY1INR: return way1inrID;
	case WAY1INU: return way1inuID;
	case WAY1IND: return way1indID;
	case WAY1OUTL: return way1outlID;
	case WAY1OUTR: return way1outrID;
	case WAY1OUTU: return way1outuID;
	case WAY1OUTD: return way1outdID;
	case WALL_L: return wall_lID;
	case WALL_R: return wall_rID;
	case WALL_U: return wall_uID;
	case WALL_D: return wall_dID;
	case CRAWL_4: return crawl_4ID;
	case CRAWL_LR: return crawl_lrID;
	case CRAWL_UD: return crawl_udID;
	case CRAWL_LU: return crawl_luID;
	case CRAWL_LD: return crawl_ldID;
	case CRAWL_RU: return crawl_ruID;
	case CRAWL_RD: return crawl_rdID;
	case LOCK_L: return lock_lID;
	case LOCK_R: return lock_rID;
	case LOCK_U: return lock_uID;
	case LOCK_D: return lock_dID;
	case PRESSURE_PLATE: return pressure_plateID;
	case TELEPORT: case NULL_TELEPORT: return teleportID;
	case EXIT: return exitID;
	case POWERUP: return powerupID;
	case HIDDEN: return hiddenID;
	case SPIKE_WALL_L: return spike_wall_lID;
	case SPIKE_WALL_R: return spike_wall_rID;
	case SPIKE_WALL_U: return spike_wall_uID;
	case SPIKE_WALL_D: return spike_wall_dID;
	default: return error_50x50;
	}
}

size_t getTexID(powerup_enum item) {
	switch (item) {
	case TINY: return small_invID;
	default: return error_50x50;
	}
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
	void reset_force() {
		force_u = false;
		force_d = false;
		force_l = false;
		force_r = false;
	}
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
		reset_force();
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

level current_level = getLevel(0, 0);

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

player player1(-1, -1, 0);
int lvl = 0;
int chapter = 0;

bool paused = false;
int paused_selection = 0;
std::string paused_level = "top";
bool inventory = false;
int inventory_selection = 0;

int inventoryItem(int powerup) {
	switch (powerup) {
	case 0: return getTexID(TINY);
	case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
	case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26:
	case 27: case 28: case 29: case 30: case 31: return emptyID;
	default: return error_50x50;
	}
}

void tryInventory(int selection) {
	if (debug) {
		debug_log += "Testing available inventory.\n";
		update_debug_screen = true;
	}
	switch (selection) {
	case 0:
		if (player1.inventory[0] > 0 && !player1.is_tiny) {
			player1.inventory[0]--;
			player1.is_tiny = true;
		}
		break;
	default:
		if (player1.inventory[selection] > 0)
			player1.inventory[selection]--;
	}
}

int level_count[8] = {
	8,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

int main(int argc, char **argv)
{
	//debug_file = fopen("sdmc:/3ds/de_debug.txt", "w");
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	romfsInit();
	csndInit();

	srand(time(NULL));

	hidScanInput();
	kDown = hidKeysDown();
	kHeld = hidKeysHeld();

	if ((kDown & KEY_X) || (kHeld & KEY_X)) {
		debug = true;
		consoleInit(GFX_BOTTOM, &bottomScreen); consoleInit(GFX_BOTTOM, &versionWin);
		consoleInit(GFX_BOTTOM, &killBox); consoleInit(GFX_BOTTOM, &debugBox);
	}

	// Create screens
	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	if (!debug) bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	init_textures();

	consoleSetWindow(&bottomScreen, 0, 0, 24, 26);
	consoleSetWindow(&versionWin, 6, 26, 34, 4);
	consoleSetWindow(&killBox, 0, 28, 40, 2);
	consoleSetWindow(&debugBox, 23, 0, 16, 26);

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
			current_level = getLevel(0, 0);
			while (player1.x == -1) {
				if (current_level.rooms[temp].hasObject(START)) {
					player1.x = tempX;
					player1.y = tempY;
					player1.location = temp;
				}
				else {
					temp++;
					tempX++;
					if (tempX >= current_level.width) {
						tempX = 0;
						tempY++;
					}
				}
			}
			player1.hidden_map.resize(current_level.rooms.size());
			player1.hidden_map.clear();
			for (unsigned int i = 0; i < current_level.rooms.size(); i++)
				player1.hidden_map[i] = true;
			player1.hidden_map[player1.location] = false;
			int ret_val = 0;
			while (ret_val == 0)
				ret_val = game();
			if (ret_val == 2)
				break;
			if (ret_val == 3) {
				//error found
				C2D_Fini();
				C3D_Fini();
				gfxExit();
				gfxInitDefault();
				C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
				C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
				top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
				C2D_Prepare();
				C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
				C2D_TargetClear(top, C2D_Color32f(0.6f, 0.6f, 0.6f, 1.0f));
				C3D_FrameEnd(0);
				consoleInit(GFX_BOTTOM, &bottomScreen); consoleInit(GFX_BOTTOM, &versionWin);
				consoleInit(GFX_BOTTOM, &killBox); consoleInit(GFX_BOTTOM, &debugBox);
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
				std::cout << "Press any key, or tap the bottom most\narea of the screen to exit the program.\n";
				bool ready_exit = false;
				while (!ready_exit) {
					hidScanInput();
					kDown = hidKeysDown();
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
		if (debug) {
			if (update_debug_screen) {
				consoleSelect(&killBox); consoleClear();
				consoleSelect(&versionWin); consoleClear();
				consoleSelect(&bottomScreen); consoleClear();

				consoleSelect(&killBox);
				std::cout << CRESET ANSI B_RED CEND;
				for (int i = 0; i < 80; i++)
					std::cout << " ";

				consoleSelect(&versionWin);
				std::cout << CRESET "     Tap red area any time to exit";
				std::cout << ANSI GREEN CEND "Build:" ANSI B_RED ASEP BRIGHT ASEP GREEN CEND "               " << buildnumber;

				consoleSelect(&bottomScreen);
				std::cout << debug_log;

				update_debug_screen = false;
			}
		}

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.6f, 0.6f, 0.6f, 1.0f));
		C2D_SceneBegin(top);
		draw_texture(titleID , 0, 0);
		if (!debug) {
			C2D_SceneBegin(bot);
			draw_texture(endID, 0, 0);
		}
		C3D_FrameEnd(0);

		hidTouchRead(&touch);

		/*exit game if red box touched*/
		if (touchInBox(touch, 0, 224, 320, 16)) {
			std::cout << "Exiting...\n";
			break;
		}
	}
	// Exit services
	// Delete graphics
	C2D_SpriteSheetFree(spriteSheet);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
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
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
	C2D_SceneBegin(top);
	int tRoom = 0;
	for (int y = 0; y < current_level.height; y++) {
		for (int x = 0; x < current_level.width; x++) {
			int rel_x = 2 - (player1.x - x);
			int rel_y = 1 - (player1.y - y);
			if (rel_x >= 0 && rel_y >= 0 && rel_x <= current_level.width && rel_y <= current_level.height && !current_level.rooms[tRoom].hasObject(WALL) && !player1.hidden_map[tRoom])
				draw_texture(floorID, 80 * rel_x, 80 * rel_y);
			if (!player1.hidden_map[tRoom]) {
				std::vector<room_items> item_arr  {
					WALL_L, WALL_R, WALL_U, WALL_D, WALL,
					CRAWL_LR, CRAWL_UD, CRAWL_LU, CRAWL_LD, CRAWL_RU, CRAWL_RD,
					PRESSURE_PLATE,
					WAY1OUTL, WAY1OUTR, WAY1OUTU, WAY1OUTD,
					WAY1INL, WAY1INR, WAY1INU, WAY1IND,
					SPIKE_WALL_L, SPIKE_WALL_R, SPIKE_WALL_U, SPIKE_WALL_D,
					TELEPORT, NULL_TELEPORT,
					POWERUP, KILL, EXIT
				};
				for (unsigned int i = 0; i < item_arr.size(); i++) {
					if (current_level.rooms[tRoom].hasObject(item_arr[i]))
						draw_texture(getTexID(item_arr[i]), rel_x * 80, rel_y * 80);
				}
				if (current_level.rooms[tRoom].hasObject(LOCK_L)) {
					draw_texture(lock_lID, 80 * rel_x, 80 * rel_y);
					draw_texture(lock_rID, 80 * (rel_x - 1), 80 * rel_y);
				}
				if (current_level.rooms[tRoom].hasObject(LOCK_R)) {
					draw_texture(lock_rID, 80 * rel_x, 80 * rel_y);
					draw_texture(lock_lID, 80 * (rel_x + 1), 80 * rel_y);
				}
				if (current_level.rooms[tRoom].hasObject(LOCK_U)) {
					draw_texture(lock_uID, 80 * rel_x, 80 * rel_y);
					draw_texture(lock_dID, 80 * rel_x, 80 * (rel_y - 1));
				}
				if (current_level.rooms[tRoom].hasObject(LOCK_D)) {
					draw_texture(lock_dID, 80 * rel_x, 80 * rel_y);
					draw_texture(lock_uID, 80 * rel_x, 80 * (rel_y + 1));
				}
			}
			if (current_level.rooms[tRoom].hasObject(WALL_L) && tRoom - current_level.width >= 0)
				if (current_level.rooms[tRoom - current_level.width].hasObject(WALL_L))
					draw_texture(wall_connector_lID, (rel_x * 80), ((rel_y * 80) - 10));
			if (current_level.rooms[tRoom].hasObject(WALL_R) && tRoom - current_level.width >= 0)
				if (current_level.rooms[tRoom - current_level.width].hasObject(WALL_R))
					draw_texture(wall_connector_rID, ((rel_x * 80) + 65), ((rel_y * 80) - 10));
			if (current_level.rooms[tRoom].hasObject(WALL_U) && tRoom - 1 >= 0)
				if (current_level.rooms[tRoom - 1].hasObject(WALL_U))
					draw_texture(wall_connector_uID, ((rel_x * 80) - 10), (rel_y * 80));
			if (current_level.rooms[tRoom].hasObject(WALL_D) && tRoom - 1 >= 0)
				if (current_level.rooms[tRoom - 1].hasObject(WALL_D))
					draw_texture(wall_connector_dID, ((rel_x * 80) - 10), ((rel_y * 80) + 65));
			if (current_level.rooms[tRoom].hasObject(WALL) && !player1.hidden_map[tRoom])
				draw_texture(wallID, 80 * rel_x, 80 * rel_y);
			tRoom++;
		}
	}
	tRoom = 0;
	for (int y = 0; y < current_level.height; y++) {
		for (int x = 0; x < current_level.width; x++) {
			int rel_x = 2 - (player1.x - x);
			int rel_y = 1 - (player1.y - y);
			if (player1.hidden_map[tRoom])
				draw_texture(hiddenID, 80 * rel_x, 80 * rel_y);
			tRoom++;
		}
	}
	draw_texture((player1.is_tiny ? player_smallID : playerID), 2 * 80, 1 * 80);
	if (paused) {
		draw_texture(pause_scr, 0, 0);
		draw_texture(ps_0, 99, 0);
		draw_texture(ps_1, 99, 0);
		draw_texture(ps_2, 99, 0);
		draw_texture(ps_3, 99, 0);
		draw_texture(ps_arrow[paused_selection], 99, 0);
	}
	if (inventory) {
		draw_texture(inventoryID, 0, 0);
		int selector = 0;
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 8; x++) {
				draw_texture(inventoryItem(selector), x * 50, (y * 50) + 40);
				if (selector == inventory_selection)
					draw_texture(inventory_selectedID, x * 50, (y * 50) + 40);
				if (player1.inventory[selector] == 0)
					if (selector < 2)
						draw_texture(none_leftID, x * 50, (y * 50) + 40);
				selector++;
			}
		}
		draw_texture(inventoryItem(inventory_selection), 6 * 50, (3 * 50) + 40);
		draw_texture(counterID, 7 * 50, (3 * 50) + 40);
		std::string temp_numbers = "";
		if (player1.inventory[inventory_selection] < 100)
			temp_numbers += "0";
		if (player1.inventory[inventory_selection] < 10)
			temp_numbers += "0";
		temp_numbers += std::to_string(player1.inventory[inventory_selection]);
		int first = temp_numbers.at(0) - '0';
		int second = temp_numbers.at(1) - '0';
		int third = temp_numbers.at(2) - '0';
		draw_texture(nxx[first], 7 * 50, (3 * 50) + 40);
		draw_texture(xnx[second], 7 * 50, (3 * 50) + 40);
		draw_texture(xxn[third], 7 * 50, (3 * 50) + 40);
	}
	if (!debug) {
		//draw to bottom screen
	}
	C3D_FrameEnd(0);
	if (debug) {
		if (update_debug_screen) {
			consoleSelect(&bottomScreen);
			consoleClear();
			std::cout << debug_log;
			update_debug_screen = false;
		}
		consoleSelect(&debugBox);
	}

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
			if (debug) {
				debug_log += "Pause menu opened.\n";
				update_debug_screen = true;
			}
		}
	}

	bool has_moved = false;
	bool invalid_move = false;

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
			if (debug) {
				debug_log += "Pause Menu Option Selected\n";
				update_debug_screen = true;
				std::cout << "paused_action returned: " << ret_val << std::endl;
			}
		}
		else if (kDown & KEY_B) {
			paused_return_level();
		}
		if (debug && (kDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT))) std::cout << "sel: " << paused_selection << std::endl;
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
		if (inventory_selection >= player1.inventory.size())
			inventory_selection = player1.inventory.size() - 1;
		if (debug && (kDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT))) std::cout << "sel: " << inventory_selection << std::endl;
	}
	else {
		room curRoom = current_level.rooms[player1.location];
		if (kDown & KEY_LEFT) {
			if (!curRoom.hasObjectsOr({ WALL_L, CRAWL_UD, CRAWL_RU, CRAWL_RD, WAY1OUTL, LOCK_L })) {
				if (curRoom.hasObject(SPIKE_WALL_L))
					triggerKill = true;
				if (curRoom.hasObject(SMALL_LEFT)) {
					if (player1.is_tiny) {
						player1.x--;
						player1.location -= 1;
						has_moved = true;
						player1.entered_small = true;
					}
					else
						invalid_move = true;
				}
				else {
					player1.x--;
					player1.location -= 1;
					has_moved = true;
				}
			}
			else
				invalid_move = true;
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
					else
						invalid_move = true;
				}
				else {
					player1.x++;
					player1.location += 1;
					has_moved = true;
				}
			}
			else
				invalid_move = true;
		}
		else if (kDown & KEY_UP) {
			if (!curRoom.hasObjectsOr({WALL_U, CRAWL_LR, CRAWL_LD, CRAWL_RD, WAY1OUTU, LOCK_U})) {
				if (curRoom.hasObject(SPIKE_WALL_U))
					triggerKill = true;
				if (curRoom.hasObject(SMALL_UP)) {
					if (player1.is_tiny) {
						player1.y--;
						player1.location -= current_level.width;
						has_moved = true;
						player1.entered_small = true;
					}
					else
						invalid_move = true;
				}
				else {
					player1.y--;
					player1.location -= current_level.width;
					has_moved = true;
				}
			}
			else
				invalid_move = true;
		}
		else if (kDown & KEY_DOWN) {
			if (!curRoom.hasObjectsOr({WALL_D, CRAWL_LR, CRAWL_LU, CRAWL_RU, WAY1OUTD, LOCK_D})) {
				if (curRoom.hasObject(SPIKE_WALL_D))
					triggerKill = true;
				if (curRoom.hasObject(SMALL_DOWN)) {
					if (player1.is_tiny) {
						player1.y++;
						player1.location += current_level.width;
						has_moved = true;
						player1.entered_small = true;
					}
					else
						invalid_move = true;
				}
				else {
					player1.y++;
					player1.location += current_level.width;
					has_moved = true;
				}
			}
			else
				invalid_move = true;
		}
		else if (kDown & KEY_X) {
			player1.is_tiny = (player1.is_tiny ? false : true);
		}
		else if (kDown & KEY_Y) {
			inventory = true;
		}
		if (invalid_move) {
			//play sound
		}
	}

	int curLoc = player1.location;
	size_t curLoc_t = curLoc;
	player1.hidden_map[curLoc] = false;
	if (curLoc - current_level.width >= 0)
		if (!current_level.rooms[curLoc].hasObjectsOr({WALL_U, WAY1OUTU}) || current_level.rooms[curLoc - current_level.width].hasObject(WALL))
			if (!current_level.rooms[curLoc - current_level.width].hasObject(HIDDEN))
				player1.hidden_map[curLoc - current_level.width] = false;
	if (curLoc_t + current_level.width < current_level.rooms.size())
		if (!current_level.rooms[curLoc].hasObjectsOr({WALL_D, WAY1OUTD}) || current_level.rooms[curLoc + current_level.width].hasObject(WALL))
			if (!current_level.rooms[curLoc + current_level.width].hasObject(HIDDEN))
				player1.hidden_map[curLoc + current_level.width] = false;
	if (curLoc - 1 >= 0)
		if (!current_level.rooms[curLoc].hasObjectsOr({WALL_L, WAY1OUTL}) || current_level.rooms[curLoc - 1].hasObject(WALL))
			if (!current_level.rooms[curLoc - 1].hasObject(HIDDEN))
				player1.hidden_map[curLoc - 1] = false;
	if (curLoc_t + 1 < current_level.rooms.size())
		if (!current_level.rooms[curLoc].hasObjectsOr({WALL_R, WAY1OUTR}) || current_level.rooms[curLoc + 1].hasObject(WALL))
			if (!current_level.rooms[curLoc + 1].hasObject(HIDDEN))
				player1.hidden_map[curLoc + 1] = false;

	int fo = (player1.force_u || player1.force_d || player1.force_l || player1.force_r ? (player1.force_u || player1.force_d ? (player1.force_u ? 1 : 2) : (player1.force_l ? 3 : 4)) : 0);
	if (fo > 0) {
		if (current_level.rooms[player1.location].hasObjectsOr((fo == 1 || fo == 2 ? (fo == 1 ? std::vector<int>{WALL_U, SMALL_UP, LOCK_U, WAY1OUTU} : std::vector<int>{ WALL_D, SMALL_DOWN, LOCK_D, WAY1OUTD }) : (fo == 3 ? std::vector<int>{WALL_L, SMALL_LEFT, LOCK_L, WAY1OUTL} : std::vector<int>{ WALL_R, SMALL_RIGHT, LOCK_R, WAY1OUTR })))) {
			player1.reset_force();
		}
		else {
			if (current_level.rooms[player1.location].hasObject((fo == 1 || fo == 2 ? (fo == 1 ? SPIKE_WALL_U : SPIKE_WALL_D) : (fo == 3 ? SPIKE_WALL_L : SPIKE_WALL_R))))
				triggerKill = true;
			else {
				has_moved = true;
				player1.location += (fo == 1 || fo == 2 ? (fo == 1 ? -current_level.width : current_level.width) : (fo == 3 ? -1 : 1));
				player1.x += (fo == 3 || fo == 4 ? (fo == 3 ? -1 : 1) : 0);
				player1.y += (fo == 1 || fo == 2 ? (fo == 1 ? -1 : 1) : 0);
			}
		}
	}
	if (has_moved) {
		room* curRoom = &current_level.rooms[player1.location];
		if (debug) {
			for (unsigned int i = 0; i < curRoom->objects.size(); i++)
				std::cout << " " << curRoom->objects[i];
			std::cout << std::endl;
		}
		if (curRoom->hasObject(TELEPORT)) {
			int teleport_to = curRoom->teleport_to;
			int temp_position = -1;
			player1.reset_force();
			for (int y = 0; y < current_level.height; y++) {
				for (int x = 0; x < current_level.width; x++) {
					temp_position++;
					if (temp_position == teleport_to) {
						player1.x = x;
						player1.y = y;
						player1.location = teleport_to;
					}
				}
			}
		}
		curRoom = &current_level.rooms[player1.location];
		if (curRoom->hasObject(PRESSURE_PLATE)) {
			if (curRoom->activates_multiple) {
				for (unsigned int i = 0; i < current_level.rooms[curRoom->rooms_activated[i]].objects.size(); i++)
					debug_log += current_level.rooms[curRoom->rooms_activated[i]].objects[i] + " ";
				debug_log += "\n";
				for (unsigned int i = 0; i < curRoom->rooms_activated.size(); i++)
					current_level.rooms[curRoom->rooms_activated[i]].activate();
				for (unsigned int i = 0; i < current_level.rooms[curRoom->rooms_activated[i]].objects.size(); i++)
					debug_log += current_level.rooms[curRoom->rooms_activated[i]].objects[i] + " ";
				debug_log += "\n";
			}
			else {
				for (unsigned int i = 0; i < current_level.rooms[curRoom->activates_room].objects.size(); i++)
					debug_log += current_level.rooms[curRoom->activates_room].objects[i] + " ";
				debug_log += "\n";
				current_level.rooms[curRoom->activates_room].activate();
				for (unsigned int i = 0; i < current_level.rooms[curRoom->activates_room].objects.size(); i++)
					debug_log += current_level.rooms[curRoom->activates_room].objects[i] + " ";
				debug_log += "\n";
			}
		}
		if (curRoom->hasObject(POWERUP)) {
			player1.addInventory(curRoom->powerup);
			curRoom->removeObject(POWERUP);
		}
		if (!curRoom->hasObjectsOr({CRAWL_4, CRAWL_LR, CRAWL_UD, CRAWL_LU, CRAWL_LD, CRAWL_RU, CRAWL_RD}) && player1.entered_small) {
			player1.entered_small = false;
			player1.is_tiny = false;
		}
		if (curRoom->hasObjectsOr({FORCE_U, FORCE_D, FORCE_L, FORCE_R})) {
			player1.force_l = curRoom->hasObject(FORCE_L);
			player1.force_r = curRoom->hasObject(FORCE_R);
			player1.force_d = curRoom->hasObject(FORCE_D);
			player1.force_u = curRoom->hasObject(FORCE_U);
		}
		if (curRoom->hasObject(KILL) || triggerKill) {
			int lives = player1.lives - 1;
			std::vector<int> inventory = player1.inventory;
			player1.reset();
			player1.lives = lives;
			player1.inventory = inventory;
			int temp = 0, tempX = 0, tempY = 0;
			while (player1.x == -1) {
				if (current_level.rooms[temp].hasObject(START)) {
					player1.x = tempX;
					player1.y = tempY;
					player1.location = temp;
				}
				else {
					temp++;
					tempX++;
					if (tempX >= current_level.width) {
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
			if (lvl == level_count[chapter]) {
				lvl = 0;
				chapter++;
				if (chapter == game_data_size) {
					//game win or whatever
					ret_val = 1;
				}
			}
			current_level = getLevel(chapter, lvl);
			int temp = 0, tempX = 0, tempY = 0;
			while (player1.x == -1) {
				if (current_level.rooms[temp].hasObject(START)) {
					player1.x = tempX;
					player1.y = tempY;
					player1.location = temp;
				}
				else {
					temp++;
					tempX++;
					if (tempX >= current_level.width) {
						tempX = 0;
						tempY++;
					}
				}
			}
			player1.hidden_map.resize(current_level.rooms.size());
			player1.hidden_map.clear();
			for (unsigned int i = 0; i < current_level.rooms.size(); i++)
				player1.hidden_map[i] = true;
		}
	}

	curLoc = player1.location;
	player1.hidden_map[curLoc] = false;

	if (player1.lives <= 0)
		return 1;

	return ret_val;
}

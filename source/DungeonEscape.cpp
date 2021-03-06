#include "DungeonEscape.hpp"
#include "levels.hpp"
#include "static.hpp"

#define MAX_SPRITES   88
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

size_t st_null = 0;

static std::string buildnumber = "18.06.17.1640";

std::string current_state = "main_menu";

static C2D_SpriteSheet spriteSheet;
static C2D_SpriteSheet uiSpriteSheet;

void draw_texture(size_t id, float x, float y, bool is_ui_element) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage((is_ui_element ? uiSpriteSheet : spriteSheet), id), x, y, 1.0f, NULL, 1.0f, 1.0f);
}

FILE *debug_file;

u32 kDown, kHeld;

touchPosition touch;

PrintConsole bottomScreen, versionWin, killBox, debugBox;

C3D_RenderTarget* top, *bot;

FS_Archive sdmcArchive;

std::string error_code, error_message;
std::vector<std::string> debug_log{
	"Debugger started:\n",
	""
};
bool error = false, debug = false, update_debug_screen = true;

size_t
wallID,
coinsID,
killID,
way1inlID, way1inrID, way1inuID, way1indID,
way1outlID, way1outrID, way1outuID, way1outdID,
wall_lID, wall_rID, wall_uID, wall_dID,
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
titleID, inventoryID, inventory_selectedID,
pause_scr, pause_cursor,
error_50x50, emptyID, floorID,
small_invID, none_leftID, counterID, player_smallID, endID,
spike_wall_lID, spike_wall_rID, spike_wall_uID, spike_wall_dID,
empty_corner_lu, empty_corner_ld, empty_corner_ru, empty_corner_rd,
wall_corner_lu, wall_corner_ld, wall_corner_ru, wall_corner_rd,
life_invID, coins_invID, inv_invID;
std::vector<size_t> ps_arrow(4), nxx(10), xnx(10), xxn(10);

void init_textures() {
	wallID = sprites_wall_idx;
	coinsID = 0;
	killID = sprites_kill_idx;
	way1inlID = sprites_way1inl_idx; way1inrID = sprites_way1inr_idx; way1inuID = sprites_way1inu_idx; way1indID = sprites_way1ind_idx;
	way1outlID = sprites_way1outl_idx; way1outrID = sprites_way1outr_idx; way1outuID = sprites_way1outu_idx; way1outdID = sprites_way1outd_idx;
	wall_lID = sprites_wall_l_idx; wall_rID = sprites_wall_r_idx; wall_uID = sprites_wall_u_idx; wall_dID = sprites_wall_d_idx;
	empty_corner_lu = sprites_corner_lu_idx; empty_corner_ld = sprites_corner_ld_idx; empty_corner_ru = sprites_corner_ru_idx; empty_corner_rd = sprites_corner_rd_idx;
	wall_corner_lu = sprites_wall_corner_lu_idx; wall_corner_ld = sprites_wall_corner_ld_idx; wall_corner_ru = sprites_wall_corner_ru_idx; wall_corner_rd = sprites_wall_corner_rd_idx;
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
	titleID = ui_title_idx; inventoryID = ui_inventory_idx; inventory_selectedID = ui_inventory_selector_idx;
	pause_scr = ui_pausescreen_idx; pause_cursor = ui_cursor_idx;
	error_50x50 = ui_error_50x50_idx; emptyID = ui_empty_idx; floorID = sprites_floor_idx;
	small_invID = ui_small_inv_idx; none_leftID = ui_none_left_idx; counterID = ui_counter_idx; player_smallID = sprites_player_tiny_idx; endID = ui_exit_idx;
	spike_wall_lID = sprites_spike_wall_l_idx; spike_wall_rID = sprites_spike_wall_r_idx; spike_wall_uID = sprites_spike_wall_u_idx; spike_wall_dID = sprites_spike_wall_d_idx;
	nxx[0] = ui_0xx_idx; nxx[1] = ui_1xx_idx; nxx[2] = ui_2xx_idx; nxx[3] = ui_3xx_idx; nxx[4] = ui_4xx_idx; nxx[5] = ui_5xx_idx; nxx[6] = ui_6xx_idx; nxx[7] = ui_7xx_idx; nxx[8] = ui_8xx_idx; nxx[9] = ui_9xx_idx;
	xnx[0] = ui_x0x_idx; xnx[1] = ui_x1x_idx; xnx[2] = ui_x2x_idx; xnx[3] = ui_x3x_idx; xnx[4] = ui_x4x_idx; xnx[5] = ui_x5x_idx; xnx[6] = ui_x6x_idx; xnx[7] = ui_x7x_idx; xnx[8] = ui_x8x_idx; xnx[9] = ui_x9x_idx;
	xxn[0] = ui_xx0_idx; xxn[1] = ui_xx1_idx; xxn[2] = ui_xx2_idx; xxn[3] = ui_xx3_idx; xxn[4] = ui_xx4_idx; xxn[5] = ui_xx5_idx; xxn[6] = ui_xx6_idx; xxn[7] = ui_xx7_idx; xxn[8] = ui_xx8_idx; xxn[9] = ui_xx9_idx;
	life_invID = ui_life_inv_idx; coins_invID = ui_coins_inv_idx; inv_invID = ui_invincible_inv_idx;
}

int pause_arrow[2][4] = {
	{60, 60, 49, 63},
	{48, 93, 138, 183}
};

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
	case CROUCH: return inv_invID;
	default: return error_50x50;
	}
}

class player {
	int default_x;
	int default_y;
	int default_texture;
	int default_location;
public:
	int coins = 0;
	int keys = 0;
	int x;
	int y;
	int texture;
	int location; //(easy for mapping, instead of using x and y, just use how far into the level's vector the player is)
	bool is_tiny = false;
	std::vector<int> objects;
	std::vector<int> inventory = {
		0, //TINY/SMALL
		0, //CRAWL - Probably change to an "invincibility" powerup
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

player player1(-1, -1, 0);
int lvl = 0;
int chapter = 0;

bool paused = false;
int paused_selection = 0;
std::string paused_level = "top";
bool inventory = false;
int inventory_selection = 0;

size_t inventoryItem(int powerup) {
	return getTexID((powerup_enum) powerup);
	/*switch (powerup) {
	case 0: return getTexID(TINY);
	case 1: return getTexID(CROUCH);
	case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
	case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26:
	case 27: case 28: case 29: case 30: case 31: return emptyID;
	default: return error_50x50;
	}*/
}

void tryInventory(int selection) {
	echo_debug(true, "Testing available inventory.\n", debug);
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

static int level_count[8] = {
	8,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

int paused_action(int action) {
	if (paused_level == "top") {
		if (action == 3) {
			return 1;
		}
		else {
			error = true;
			error_code = "ps-act_top_act_invalid";
			error_message = "Invalid pause menu action.";
			return 3;
		}
	}
	else {
		error = true;
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

	getHidInputs(kDown, kHeld);

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
	uiSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/ui.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);
	if (!uiSpriteSheet) svcBreak(USERBREAK_PANIC);

	init_textures();

	consoleSetWindow(&bottomScreen, 0, 0, 24, 26);
	consoleSetWindow(&versionWin, 6, 26, 34, 4);
	consoleSetWindow(&killBox, 0, 28, 40, 2);
	consoleSetWindow(&debugBox, 24, 0, 16, 26);

	int bottom_screen_text = 0;

	// Main loop
	while (aptMainLoop()) {
		if (current_state == "main_menu") {
			getHidInputs(kDown, kHeld);
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
				current_state = "game_running";
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
					std::cout << debug_log[0];

					update_debug_screen = false;
				}
			}

			// Render the scene
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.6f, 0.6f, 0.6f, 1.0f));
			C2D_SceneBegin(top);
			draw_texture(titleID, 0, 0, true);
			if (!debug) {
				C2D_TargetClear(bot, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
				C2D_SceneBegin(bot);
				draw_texture(endID, 0, 224, true);
			}
			C3D_FrameEnd(0);

			hidTouchRead(&touch);

			/*exit game if red box touched*/
			if (touchInBox(touch, 0, 224, 320, 16, kDown)) {
				std::cout << "Exiting...\n";
				break;
			}
		}
		else if (current_state == "game_running") {
			int event_handler = 0;
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
			C2D_SceneBegin(top);
			int tRoom = 0;
			for (int y = 0; y < current_level.height; y++) {
				for (int x = 0; x < current_level.width; x++) {
					int rel_x = 2 - (player1.x - x);
					int rel_y = 1 - (player1.y - y);
					if (rel_x >= 0 && rel_y >= 0 && rel_x <= current_level.width && rel_y <= current_level.height && !current_level.rooms[tRoom].hasObject(WALL) && !player1.hidden_map[tRoom])
						draw_texture(floorID, 80 * rel_x, 80 * rel_y, false);
					if (!player1.hidden_map[tRoom]) {
						std::vector<room_items> item_arr{
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
							if (current_level.rooms[tRoom].hasObject(item_arr[i])) {
								int pos_add_x = (item_arr[i] == WALL_R ? 70 : 0);
								int pos_add_y = (item_arr[i] == WALL_D ? 70 : 0);
								draw_texture(getTexID(item_arr[i]), rel_x * 80 + pos_add_x, rel_y * 80 + pos_add_y, false);
							}
						}
						if (!current_level.rooms[tRoom].hasObject(WALL)) {
							if (!current_level.rooms[tRoom].hasObjectsOr({ WALL_L, SPIKE_WALL_L })) {
								if (!current_level.rooms[tRoom].hasObjectsOr({ WALL_U, SPIKE_WALL_U }))
									draw_texture(wall_corner_lu, rel_x * 80, rel_y * 80, false);
								if (!current_level.rooms[tRoom].hasObjectsOr({ WALL_D, SPIKE_WALL_D }))
									draw_texture(wall_corner_ld, rel_x * 80, rel_y * 80 + 70, false);
							}
							if (!current_level.rooms[tRoom].hasObjectsOr({ WALL_R, SPIKE_WALL_R })) {
								if (!current_level.rooms[tRoom].hasObjectsOr({ WALL_U, SPIKE_WALL_U }))
									draw_texture(wall_corner_ru, rel_x * 80 + 70, rel_y * 80, false);
								if (!current_level.rooms[tRoom].hasObjectsOr({ WALL_D, SPIKE_WALL_D }))
									draw_texture(wall_corner_rd, rel_x * 80 + 70, rel_y * 80 + 70, false);
							}
							if (current_level.rooms[tRoom].hasObject(WALL_L)) {
								if (current_level.rooms[tRoom].hasObject(WALL_U))
									draw_texture(empty_corner_lu, rel_x * 80 + 10, rel_y * 80 + 10, false);
								if (current_level.rooms[tRoom].hasObject(WALL_D))
									draw_texture(empty_corner_ld, rel_x * 80 + 10, rel_y * 80 + 60, false);
							}
							if (current_level.rooms[tRoom].hasObject(WALL_R)) {
								if (current_level.rooms[tRoom].hasObject(WALL_U))
									draw_texture(empty_corner_ru, rel_x * 80 + 60, rel_y * 80 + 10, false);
								if (current_level.rooms[tRoom].hasObject(WALL_D))
									draw_texture(empty_corner_rd, rel_x * 80 + 60, rel_y * 80 + 60, false);
							}
						}
						if (current_level.rooms[tRoom].hasObject(LOCK_L)) {
							draw_texture(lock_lID, 80 * rel_x, 80 * rel_y, false);
							draw_texture(lock_rID, 80 * (rel_x - 1), 80 * rel_y, false);
						}
						if (current_level.rooms[tRoom].hasObject(LOCK_R)) {
							draw_texture(lock_rID, 80 * rel_x, 80 * rel_y, false);
							draw_texture(lock_lID, 80 * (rel_x + 1), 80 * rel_y, false);
						}
						if (current_level.rooms[tRoom].hasObject(LOCK_U)) {
							draw_texture(lock_uID, 80 * rel_x, 80 * rel_y, false);
							draw_texture(lock_dID, 80 * rel_x, 80 * (rel_y - 1), false);
						}
						if (current_level.rooms[tRoom].hasObject(LOCK_D)) {
							draw_texture(lock_dID, 80 * rel_x, 80 * rel_y, false);
							draw_texture(lock_uID, 80 * rel_x, 80 * (rel_y + 1), false);
						}
					}
					if (current_level.rooms[tRoom].hasObject(WALL) && !player1.hidden_map[tRoom])
						draw_texture(wallID, 80 * rel_x, 80 * rel_y, false);
					tRoom++;
				}
			}
			tRoom = 0;
			for (int y = 0; y < current_level.height; y++) {
				for (int x = 0; x < current_level.width; x++) {
					int rel_x = 2 - (player1.x - x);
					int rel_y = 1 - (player1.y - y);
					if (player1.hidden_map[tRoom])
						draw_texture(hiddenID, 80 * rel_x, 80 * rel_y, false);
					tRoom++;
				}
			}
			draw_texture((player1.is_tiny ? player_smallID : playerID), 2 * 80, 1 * 80, false);
			if (paused) {
				if (kHeld & KEY_X)
					draw_texture(ui_asdf_idx, 100, 0, true);
				else
					draw_texture(pause_scr, 100, 0, true);
				draw_texture(pause_cursor, pause_arrow[0][paused_selection] + 100, pause_arrow[1][paused_selection], true);
			}
			if (inventory) {
				draw_texture(inventoryID, 0, 0, true);
				int selector = 0;
				for (int y = 0; y < 4; y++) {
					for (int x = 0; x < 6; x++) {
						draw_texture(inventoryItem(selector), x * 50, (y * 50) + 40, true);
						if (selector == inventory_selection)
							draw_texture(inventory_selectedID, x * 50, (y * 50) + 40, true);
						if (player1.inventory[selector] == 0)
							if (selector < 2)
								draw_texture(none_leftID, x * 50, (y * 50) + 40, true);
						selector++;
					}
				}
				std::vector<int> side_values = { player1.lives, player1.coins, NULL, player1.inventory[inventory_selection] };
				std::vector<size_t> side_sprites = { life_invID, coins_invID, NULL, inventoryItem(inventory_selection) };
				for (int counter = 0; counter < side_values.size(); counter++) {
					if (counter != 2) { //temporary, until a third thing to have counted is added
						std::string temp_numbers = "";
						if (side_values[counter] < 100)
							temp_numbers += "0";
						if (side_values[counter] < 10)
							temp_numbers += "0";
						temp_numbers += std::to_string(side_values[counter]);
						int first = temp_numbers.at(0) - '0';
						int second = temp_numbers.at(1) - '0';
						int third = temp_numbers.at(2) - '0';
						draw_texture(counterID, 7 * 50, (counter * 50) + 40, true);
						draw_texture(nxx[first], 7 * 50, (counter * 50) + 40, true);
						draw_texture(xnx[second], 7 * 50, (counter * 50) + 40, true);
						draw_texture(xxn[third], 7 * 50, (counter * 50) + 40, true);
						draw_texture(side_sprites[counter], 6 * 50, (counter * 50) + 40, true);
					}
				}
			}
			if (!debug) {
				//draw to bottom screen
			}
			C3D_FrameEnd(0);

			hidTouchRead(&touch);

			/*exit game if red box touched*/
			if (touchInBox(touch, 0, 224, 320, 16, kDown)) {
				consoleSelect(&bottomScreen);
				std::cout << "Exiting...\n";
				return 2;
			}

			getHidInputs(kDown, kHeld);

			bool triggerKill = false;

			if (kDown & KEY_START) {
				if (!inventory) {
					paused = (paused ? false : true);
					if (!paused) {
						paused_selection = 0;
						paused_level = "top";
					}
					echo_debug(true, "Pause menu opened.\n", debug);
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
					event_handler = paused_action(paused_selection);
					echo_debug(true, "Pause Menu Option Selected\n", debug);
					echo_debug(false,  "paused_action returned: " + std::to_string(event_handler) + "\n", debug);
				}
				else if (kDown & KEY_B) {
					paused_return_level();
				}
				if (kDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT)) echo_debug(false, "sel: " + std::to_string(paused_selection) + "\n", debug);
			}
			else if (inventory) {
				if (kDown & KEY_B)
					inventory = false;
				else if (kDown & KEY_UP)
					inventory_selection -= 6;
				else if (kDown & KEY_DOWN)
					inventory_selection += 6;
				else if (kDown & KEY_LEFT)
					inventory_selection--;
				else if (kDown & KEY_RIGHT)
					inventory_selection++;
				else if (kDown & KEY_A)
					tryInventory(inventory_selection);
				if (inventory_selection <= -1)
					inventory_selection = 0;
				if (inventory_selection >= player1.inventory.size() - 4)
					inventory_selection = player1.inventory.size() - 5;
				if (kDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT)) echo_debug(false, "sel: " + std::to_string(inventory_selection) + "\n", debug);
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
					if (!curRoom.hasObjectsOr({ WALL_R, CRAWL_UD, CRAWL_LU, CRAWL_LD, WAY1OUTR, LOCK_R })) {
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
					if (!curRoom.hasObjectsOr({ WALL_U, CRAWL_LR, CRAWL_LD, CRAWL_RD, WAY1OUTU, LOCK_U })) {
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
					if (!curRoom.hasObjectsOr({ WALL_D, CRAWL_LR, CRAWL_LU, CRAWL_RU, WAY1OUTD, LOCK_D })) {
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
					echo_debug(true, "Invalid Move\n", debug);
				}
			}

			int curLoc = player1.location;
			size_t curLoc_t = curLoc;
			player1.hidden_map[curLoc] = false;
			if (curLoc - current_level.width >= 0)
				if (!current_level.rooms[curLoc].hasObjectsOr({ WALL_U, WAY1OUTU }) || current_level.rooms[curLoc - current_level.width].hasObject(WALL))
					if (!current_level.rooms[curLoc - current_level.width].hasObject(HIDDEN))
						player1.hidden_map[curLoc - current_level.width] = false;
			if (curLoc_t + current_level.width < current_level.rooms.size())
				if (!current_level.rooms[curLoc].hasObjectsOr({ WALL_D, WAY1OUTD }) || current_level.rooms[curLoc + current_level.width].hasObject(WALL))
					if (!current_level.rooms[curLoc + current_level.width].hasObject(HIDDEN))
						player1.hidden_map[curLoc + current_level.width] = false;
			if (curLoc - 1 >= 0)
				if (!current_level.rooms[curLoc].hasObjectsOr({ WALL_L, WAY1OUTL }) || current_level.rooms[curLoc - 1].hasObject(WALL))
					if (!current_level.rooms[curLoc - 1].hasObject(HIDDEN))
						player1.hidden_map[curLoc - 1] = false;
			if (curLoc_t + 1 < current_level.rooms.size())
				if (!current_level.rooms[curLoc].hasObjectsOr({ WALL_R, WAY1OUTR }) || current_level.rooms[curLoc + 1].hasObject(WALL))
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
				echo_debug(false, "Objects:", debug);
				for (unsigned int i = 0; i < curRoom->objects.size(); i++)
					echo_debug(false, " " + std::to_string(curRoom->objects[i]), debug);
				echo_debug(false, "\n", debug);
				if (curRoom->hasObject(TELEPORT)) {
					echo_debug(true, "Teleporting Player From A to B\n", debug);
					echo_debug(false, "A = " + std::to_string(player1.location) + "\n", debug);
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
					echo_debug(false, "B = " + std::to_string(player1.location) + "\n", debug);
				}
				curRoom = &current_level.rooms[player1.location];
				if (curRoom->hasObject(PRESSURE_PLATE)) {
					echo_debug(true, "Pressure Plate activated\n", debug);
					if (curRoom->has_special) {
						special* special_data = &curRoom->special_data;
						extra* extra_data = &current_level.extra_data;
						int var_id = curRoom->special_data.var_id;
						echo_debug(true, "Room has Special Action\n", debug);
						echo_debug(false, "Special Type: " + curRoom->special_data.type + "\n", debug);
						if (special_data->type == "bool") {
							echo_debug(false, "extra_bool[" + std::to_string(var_id) + "]\n  From: " + std::string(extra_data->extra_bool[var_id] ? "true" : "false") + "\n", debug);
							extra_data->extra_bool[var_id] = (extra_data->extra_bool[var_id] ? false : true);
							echo_debug(false, "  To: " + std::string(extra_data->extra_bool[var_id] ? "true" : "false") + "\n", debug);
						}
						if (special_data->type == "int") {
							echo_debug(false, "Action: " + special_data->action + "\n", debug);
							if (special_data->action == "add") {
								echo_debug(false, "extra_int[" + std::to_string(var_id) + "]\n  From: " + std::to_string(extra_data->extra_int[var_id]) + "\n", debug);
								extra_data->extra_int[var_id] += special_data->int_value;
								echo_debug(false, "  To: " + std::to_string(extra_data->extra_int[var_id]) + "\n", debug);
							}
							else if (special_data->action == "set") {
								echo_debug(false, "extra_int[" + std::to_string(var_id) + "]\n  From: " + std::to_string(extra_data->extra_int[var_id]) + "\n", debug);
								extra_data->extra_int[var_id] = special_data->int_value;
								echo_debug(false, "  To: " + std::to_string(extra_data->extra_int[var_id]) + "\n", debug);
							}
						}
						if (special_data->type == "string") {
							if (special_data->action == "add") {
								echo_debug(false, "extra_string[" + std::to_string(var_id) + "]\n  From: " + extra_data->extra_string[var_id] + "\n", debug);
								extra_data->extra_string[var_id] += special_data->string_value;
								echo_debug(false, "  To: " + extra_data->extra_string[var_id] + "\n", debug);
							}
							else if (special_data->action == "set") {
								echo_debug(false, "extra_string[" + std::to_string(var_id) + "]\n  From: " + extra_data->extra_string[var_id] + "\n", debug);
								extra_data->extra_string[var_id] = special_data->string_value;
								echo_debug(false, "  To: " + extra_data->extra_string[var_id] + "\n", debug);
							}
						}
					}
					else {
						if (curRoom->activates_multiple) {
							echo_debug(false, "Rooms:", debug);
							for (unsigned int i = 0; i < curRoom->rooms_activated.size(); i++) {
								current_level.rooms[curRoom->rooms_activated[i]].activate();
								echo_debug(false, " " + std::to_string(curRoom->rooms_activated[i]), debug);
							}
						}
						else {
							current_level.rooms[curRoom->activates_room].activate();
							echo_debug(false, "Room: " + std::to_string(curRoom->activates_room), debug);
						}
						echo_debug(false, "\n", debug);
					}
				}
				if (curRoom->hasObject(POWERUP)) {
					player1.addInventory(curRoom->powerup);
					echo_debug(true, "Powerup added\n", debug);
					echo_debug(false, "Added " + std::to_string(curRoom->powerup[1]) + " of power-" + std::to_string(curRoom->powerup[0]) + "\n", debug);
					curRoom->removeObject(POWERUP);
				}
				if (!curRoom->hasObjectsOr({ CRAWL_4, CRAWL_LR, CRAWL_UD, CRAWL_LU, CRAWL_LD, CRAWL_RU, CRAWL_RD }) && player1.entered_small) {
					player1.entered_small = false;
					player1.is_tiny = false;
				}
				if (current_level.has_custom_function) {
					custom_level_function(current_level);
					echo_debug(true, "Running Custom Function " + std::to_string(current_level.custom_function) + "\n", debug);
				}
				if (curRoom->hasObjectsOr({ FORCE_U, FORCE_D, FORCE_L, FORCE_R })) {
					player1.force_l = curRoom->hasObject(FORCE_L);
					player1.force_r = curRoom->hasObject(FORCE_R);
					player1.force_d = curRoom->hasObject(FORCE_D);
					player1.force_u = curRoom->hasObject(FORCE_U);
					echo_debug(true, "Player forced in [D]irection\n", debug);
					echo_debug(false, "D = " + std::string(player1.force_l ? "Left" : (player1.force_r ? "Right" : (player1.force_u ? "Up" : "Down"))) + "\n", debug);
				}
				if (player1.inventory[24] > 0) {
					echo_debug(true, "Moving Lives\n", debug);
					echo_debug(false, "Player had " + std::to_string(player1.inventory[24]) + "\nLives in Inv'", debug);
					player1.lives += player1.inventory[24];
					player1.inventory[24] = 0;
				}
				if (player1.inventory[25] > 0) {
					echo_debug(true, "Moving Coins\n", debug);
					echo_debug(false, "Player had " + std::to_string(player1.inventory[25]) + "\nCoins in Inv'", debug);
					player1.coins += player1.inventory[25];
					player1.inventory[25] = 0;
				}
				if (player1.inventory[26] > 0) {
					echo_debug(true, "Moving Keys\n", debug);
					echo_debug(false, "Player had " + std::to_string(player1.inventory[26]) + "\nKeys in Inv'", debug);
					player1.keys += player1.inventory[26];
					player1.inventory[26] = 0;
				}
				if (curRoom->hasObject(KILL) || triggerKill) {
					int lives = player1.lives - 1;
					echo_debug(true, "Lives Remaining: " + std::to_string(lives) + "\n", debug);
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
							event_handler = 1;
						}
					}
					current_level = getLevel(chapter, lvl);
					echo_debug(true, "Loaded:\n     Chapter " + std::to_string(chapter) + "\n     Level " + std::to_string(lvl) + "\n", debug);
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
				event_handler = 1;

			if (error)
				event_handler = 2;

			switch (event_handler) {
			case 0:
				break;
			case 1:
				current_state = "main_menu"; //game over eventually
				break;
			case 2:
				current_state = "error";
				break;
			default:
				current_state = "error";
				error = true;
				error_code = "err_event-handler_unexpected_value";
				error_message = "The game could not determine if the game should continue, game over, return to the main menu, or display the error screen. (Yes, we are aware of the irony.)";
				break;
			}
		}
		else if (current_state == "error") {
			if (debug) {
				debug_log[0] += "\n" + error_code + "\n\n" + error_message + "\n";
				//output to debug file
			}
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
				getHidInputs(kDown, kHeld);
				if (kDown & (KEY_A | KEY_B | KEY_Y | KEY_X | KEY_L | KEY_R | KEY_ZL | KEY_ZR | KEY_SELECT | KEY_START | KEY_LEFT | KEY_RIGHT | KEY_UP | KEY_DOWN))
					ready_exit = true;

				hidTouchRead(&touch);

				/*exit game if red box touched*/
				if (touchInBox(touch, 0, 224, 320, 16, kDown)) {
					std::cout << "Exiting...\n";
					ready_exit = true;
				}
			}
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

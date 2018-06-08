#include "levels.hpp"

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

level getLevel(int chapter, int lvl) {
	level return_data(0, 0, std::vector<room>{room({ WALL })});
	switch (chapter) {
	case 0:
		switch (lvl) {
		case 0:
			return_data = level(8, 10, {
			room({ WALL }),
			room({ WALL }),
			room({ START, WALL_L, WALL_U, WALL_R }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }), //row 1
			room({ WALL }),
			room({ WALL }),
			room({ EMPTY, WALL_L, WALL_R }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }), //row 2
			room({ EMPTY, WALL_L, WALL_U }),
			room({ EMPTY, WALL_U, WALL_D }),
			room({ EMPTY, SMALL_RIGHT }),
			room({ EMPTY, CRAWL_LR }),
			room({ EMPTY, CRAWL_LD }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }), //row 3
			room({ EMPTY, WALL_L, WALL_R }),
			room({ WALL }),
			room({ EMPTY, WALL_L, WALL_R }),
			room({ WALL }),
			room({ EMPTY, WALL_L, SMALL_UP, WALL_D }),
			room({ EMPTY, WALL_U, LOCK_R, SMALL_DOWN },{ EMPTY, WALL_U, UNLOCK_R, SMALL_DOWN }, false),
			room({ EXIT, WALL_U, WALL_D, WALL_R }),
			room({ WALL }), //row 4
			room({ EMPTY, WALL_L, WALL_R }),
			room({ WALL }),
			room({ EMPTY, WALL_L, WALL_D },{ EMPTY, WALL_L }, true),
			room({ EMPTY, WALL_U, WALL_D }),
			room({ TELEPORT, WALL_U, WALL_R, WALL_D }, 49),
			room({ EMPTY, CRAWL_UD }),
			room({ WALL }),
			room({ WALL }), //row 5
			room({ EMPTY, WALL_L, WALL_R }),
			room({ WALL },{ EMPTY, WALL_U, WALL_L, WALL_D }, true),
			room({ WALL },{ EMPTY, WALL_R, WALL_D }, true), //room 43 (42 + 1)
			room({ WALL_U, WALL_L, PRESSURE_PLATE },{ 34, 41, 42 }),
			room({ EMPTY, WALL_U, WALL_D, SMALL_RIGHT }),
			room({ CRAWL_LU, PRESSURE_PLATE }, 29),
			room({ WALL }),
			room({ WALL }), //row 6
			room({ EMPTY, WALL_L, WALL_R }),
			room({ WALL_U, WALL_L, WALL_D, TELEPORT }, 36),
			room({ EMPTY, WALL_U, WALL_D }),
			room({ EMPTY, WALL_R, WALL_D }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL_U, WALL_L, WALL_R, FORCE_D }), //row 7
			room({ EMPTY, WALL_L, WALL_R }),
			room({ WALL }),
			room({ TELEPORT, WALL_L, WALL_U, WALL_R }, 55),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ PRESSURE_PLATE, WALL_L, WALL_R },{ 64, 65, 66 }), //row 8
			room({ EMPTY, WALL_L, WALL_D },{ EMPTY, WALL_L, WALL_D, WALL_R }, true),
			room({ EMPTY, WALL_U, WALL_D },{ EMPTY, WALL }, true),
			room({ EMPTY },{ EMPTY, WALL_L }, true),
			room({ HIDDEN, KILL, WALL_U, WALL_R, WALL_D }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ TELEPORT, WALL_L, WALL_D, WALL_R }, 10), //row 9
			room({ WALL }),
			room({ WALL }),
			room({ WALL_L, WALL_D, WALL_R, POWERUP }, new int[2]{ TINY, 3 }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }) //row 10
			});
			break;
		case 1:
			return_data = level(9, 9, {
			room({ START, WALL_U, WALL_L, WALL_D }),
			room({ EMPTY, WALL_U, SMALL_DOWN, SMALL_RIGHT }),
			room({ EMPTY, CRAWL_LR }),
			room({ EMPTY, SMALL_LEFT, WALL_U, WALL_D, SMALL_RIGHT }),
			room({ EMPTY, CRAWL_LD }),
			room({ POWERUP, WALL_U, CRAWL_UD }, new int[2]{ CROUCH, 3 }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }), //row 1
			room({ WALL }),
			room({ EMPTY, CRAWL_UD }),
			room({ EMPTY, WALL_U, WALL_L }),
			room({ EMPTY, WALL_U, WALL_D }),
			room({ EMPTY, SMALL_DOWN, SMALL_UP, SMALL_RIGHT }),
			room({ EMPTY, CRAWL_LU }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }), //row 2
			room({ HIDDEN, WALL_U, WALL_L, WALL_R, KILL }),
			room({ WALL_L, WALL_D, WALL_R, SMALL_UP }, new int[2]{ TINY, 3 }),
			room({ EMPTY, WALL_L, WALL_R }),
			room({ HIDDEN, TELEPORT, WALL_L, WALL_U, WALL_R }, 24),
			room({ SMALL_UP, WALL_L, WALL_D, SPIKES }),
			room({ WALL_U, WALL_R, WALL_D, POWERUP }, new int[2]{ KEY, 1 }),
			room({ WALL_U, WALL_L, WALL_D, NULL_TELEPORT }),
			room({ WALL_U, WALL_D, SPIKES }),
			room({ WALL_U, WALL_R, CHEST }), //row 3
			room({ WALL_L, WALL_D, EMPTY }),
			room({ HIDDEN, PRESSURE_PLATE, WALL_U, WALL_R },{ 36, 37 }),
			room({ EMPTY, WALL_L, WALL_D }),
			room({ EMPTY }),
			room({ HIDDEN, WALL_U, WALL_R, WALL_D, KILL }),
			room({ EMPTY, WALL_L, WALL_U }),
			room({ EMPTY, WALL_U, WALL_D }),
			room({ EMPTY, WALL_U, WALL_R }),
			room({ EMPTY, WALL_L, WALL_R },{ EMPTY, WALL_L, WALL_R, WALL_D }, true), //row 4
			room({ HIDDEN, WALL },{ HIDDEN, EXIT, WALL_U, WALL_L, WALL_D }, true),
			room({ EMPTY, WALL_L },{ EMPTY }, true),
			room({ EMPTY, WALL_U, WALL_R }),
			room({ HIDDEN, WALL_L, WALL_D, WALL_R, KILL }),
			room({ WALL }),
			room({ EMPTY, WALL_L, WALL_R }),
			room({ WALL }),
			room({ SPIKE_WALL_L, SPIKE_WALL_D, PRESSURE_PLATE },{ SPIKE_WALL_L, SPIKE_WALL_D, PRESSURE_PLATE, WALL_R }, true,{ 43, 44, 35 }),
			room({ EMPTY, WALL_R, WALL_D },{ WALL }, true), //row 5
			room({ WALL }),
			room({ HIDDEN, WALL_L, WALL_D, WALL_R, PRESSURE_PLATE },{ 36, 37 }),
			room({ EMPTY, WALL_L, WALL_D }),
			room({ EMPTY, WALL_U, WALL_D }),
			room({ SPIKE_WALL_U, WALL_D }, new int[2]{ COINS, 2 }),
			room({ WALL_D },{ WALL_D, POWERUP }, new int[2]{ LIFE, 1 }, false),
			room({ HIDDEN, WALL_U, WALL_D, TELEPORT, SPIKE_WALL_R }, 60),
			room({ WALL }),
			room({ WALL }), //row 6
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL },{ WALL }, true),
			room({ WALL_U, WALL_L, WALL_R, TELEPORT }, 51),
			room({ WALL }),
			room({ WALL }), //row 7
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL_U, WALL_L, WALL_D, HIDDEN, PRESSURE_PLATE },{ 50, 59 }),
			room({ EMPTY }),
			room({ HIDDEN, WALL_U, WALL_D, WALL_R, POWERUP }, new int[2]{ COINS, 50 }),
			room({ WALL }), //row 8
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL }),
			room({ WALL_L, WALL_D, WALL_R, HIDDEN, POWERUP }, new int[2]{ TINY, 3 }),
			room({ WALL }),
			room({ WALL }) //row 9
			});
			break;
		case 2:
			return_data = level(8, 10, {
				room({ WALL }),
				room({ WALL }),
				room({ START, WALL_L, WALL_U, WALL_R }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }), //row 1
				room({ WALL }),
				room({ WALL }),
				room({ EMPTY, WALL_L, WALL_R }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }), //row 2
				room({ EMPTY, WALL_L, WALL_U }),
				room({ EMPTY, WALL_U, WALL_D }),
				room({ EMPTY, SMALL_RIGHT }),
				room({ EMPTY, CRAWL_LR }),
				room({ EMPTY, CRAWL_LD }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }), //row 3
				room({ EMPTY, WALL_L, WALL_R }),
				room({ WALL }),
				room({ EMPTY, WALL_L, WALL_R }),
				room({ WALL }),
				room({ EMPTY, WALL_L, SMALL_UP, WALL_D }),
				room({ EMPTY, WALL_U, LOCK_R, SMALL_DOWN },{ EMPTY, WALL_U, UNLOCK_R, SMALL_DOWN }, false),
				room({ EXIT, WALL_U, WALL_D, WALL_R }),
				room({ WALL }), //row 4
				room({ EMPTY, WALL_L, WALL_R }),
				room({ WALL }),
				room({ EMPTY, WALL_L, WALL_D },{ EMPTY, WALL_L }, true),
				room({ EMPTY, WALL_U, WALL_D }),
				room({ TELEPORT, WALL_U, WALL_R, WALL_D }, 49),
				room({ EMPTY, CRAWL_UD }),
				room({ WALL }),
				room({ WALL }), //row 5
				room({ EMPTY, WALL_L, WALL_R }),
				room({ WALL },{ EMPTY, WALL_U, WALL_L, WALL_D }, true),
				room({ WALL },{ EMPTY, WALL_R, WALL_D }, true), //room 43 (42 + 1)
				room({ WALL_U, WALL_L, PRESSURE_PLATE },{ 34, 41, 42 }),
				room({ EMPTY, WALL_U, WALL_D, SMALL_RIGHT }),
				room({ CRAWL_LU, PRESSURE_PLATE }, 29),
				room({ WALL }),
				room({ WALL }), //row 6
				room({ EMPTY, WALL_L, WALL_R }),
				room({ WALL_U, WALL_L, WALL_D, TELEPORT }, 36),
				room({ EMPTY, WALL_U, WALL_D }),
				room({ EMPTY, WALL_R, WALL_D }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL_U, WALL_L, WALL_R, FORCE_D }), //row 7
				room({ EMPTY, WALL_L, WALL_R }),
				room({ WALL }),
				room({ TELEPORT, WALL_L, WALL_U, WALL_R }, 55),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ PRESSURE_PLATE, WALL_L, WALL_R },{ 64, 65, 66 }), //row 8
				room({ EMPTY, WALL_L, WALL_D },{ EMPTY, WALL_L, WALL_D, WALL_R }, true),
				room({ EMPTY, WALL_U, WALL_D },{ EMPTY, WALL }, true),
				room({ EMPTY },{ EMPTY, WALL_L }, true),
				room({ HIDDEN, KILL, WALL_U, WALL_R, WALL_D }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ TELEPORT, WALL_L, WALL_D, WALL_R }, 10), //row 9
				room({ WALL }),
				room({ WALL }),
				room({ WALL_L, WALL_D, WALL_R, POWERUP }, new int[2]{ TINY, 3 }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }) //row 10
				});
			break;
		}
		break;
	}
	if (return_data.width == 0) error = true;
	return return_data;
}

#include "DungeonEscape.hpp"
#include "levels.hpp"

level return_data(0, 0, {room({ WALL })});

level getLevel(int chapter, int lvl) {
	switch (chapter) {
	case 0:
		switch (lvl) {
		case 0:
			return_data.width = 8;
			return_data.height = 10;
			return_data.rooms = {
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
			room({ EMPTY, WALL_U, LOCK_R, SMALL_DOWN },{ EMPTY, WALL_U, /*UNLOCK_R,*/ SMALL_DOWN }, false),
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
			};
			break;
		case 1:
			return_data.width = 9;
			return_data.height = 9;
			return_data.rooms = {
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
			};
			break;
		case 2:
			return_data.width = 6;
			return_data.height = 10;
			return_data.rooms = {
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL }), //row 1
				room({ WALL }),
				room({ WALL_L, WALL_U, EMPTY }),
				room({ WALL_U, WALL_D, EMPTY }),
				room({ WALL_U, WALL_D, EMPTY }),
				room({ SPIKE_WALL_U, SPIKE_WALL_R, EMPTY }),
				room({ WALL }), //row 2
				room({ WALL }),
				room({ WALL_L, WALL_R, EMPTY }), //originally had an "easy bossfight"
				room({ WALL_U, WALL_L, WALL_D, POWERUP }, new int[2]{ COINS, 80 }),
				room({ WALL_U, WALL_D, SPIKES, EMPTY }),
				room({ WALL_R }),
				room({ WALL }), //row 3
				room({ WALL }),
				room({ WALL_L, WALL_R, EMPTY }),
				room({ WALL }),
				room({ WALL_L, WALL_U, WALL_R, TELEPORT }, 51),
				room({ WALL_L, SPIKE_WALL_R, EMPTY }),
				room({ WALL }), //row 4
				room({ WALL_L, WALL_U, EMPTY, WAY1OUTR }),
				room({ WALL_R, EMPTY, WAY1INL, WAY1IND }),
				room({ HIDDEN, WALL_U, WALL_L, WALL_D, KILL }),
				room({ EMPTY }),
				room({ NULL_TELEPORT, WALL_R, WALL_D }),
				room({ WALL }), //row 5
				room({ WALL_L, WALL_R, EMPTY }), //originally called you a loser for taking the pussy route
				room({ WALL_L, WALL_R, HIDDEN, WAY1OUTU }), //originally had a "hard bossfight"
				room({ WALL_L, WALL_U, WALL_D, START }),
				room({ EMPTY, SMALL_RIGHT, SMALL_DOWN }),
				room({ CRAWL_LD, HIDDEN }),
				room({ WALL }), //row 6
				room({ WALL_L, WALL_R, EMPTY }),
				room({ WALL_L, WALL_R, WAY1IND, POWERUP }, new int[2]{ COINS, 4 }),
				room({ WALL_L, WALL_U, WALL_R, HIDDEN, POWERUP }, new int[2]{ COINS, 50 }),
				room({ HIDDEN, CRAWL_UD }),
				room({ WALL_L, WALL_R, HIDDEN, POWERUP }, new int[2]{ COINS, 2 }),
				room({ WALL }), //row 7
				room({ WALL_L, WALL_D, EMPTY, WAY1INR }),
				room({ WAY1OUTL, WAY1OUTU, WALL_D, WALL_R, EXIT }),
				room({ WALL_L, WALL_D, POWERUP }, new int[2]{ COINS, 10 }),
				room({ EMPTY, WALL_R, WALL_D, SMALL_UP }),
				room({ HIDDEN, TELEPORT, WALL_L, WALL_D, WALL_R }, 28),
				room({ WALL }), //row 8
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL_L, WALL_R, WALL_U, NULL_TELEPORT }),
				room({ WALL }),
				room({ WALL }), //row 9              MISSING ROOM BETWEEN ROW 9 AND 10
				room({ WALL }),
				room({ WALL }),
				room({ WALL }),
				room({ WALL_L, WALL_D, WALL_R, KILL, HIDDEN }),
				room({ WALL }),
				room({ WALL }) //row 10
			};
			break;
		}
		break;
	}
	if (return_data.width == 0) error = true;
	return return_data;
}

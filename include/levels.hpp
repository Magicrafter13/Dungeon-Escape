#pragma once

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
	SPIKE_WALL_L = 52, SPIKE_WALL_R = 53, SPIKE_WALL_U = 54, SPIKE_WALL_D = 55,
	UBER = 56
};

/// Powerups
enum powerup_enum {
	TINY = 0,
	CROUCH = 1,
	//COINS already defined as 2
	KEY = 3,
	LIFE = 4
};

class special {
public:
	std::string type;
	int var_id;
	std::string action;
	int int_value;
	std::string string_value;
	special(int fvar_id) {
		type = "bool";
		var_id = fvar_id;
	}
	/*please make the action "add" or "set"*/
	special(int fvar_id, std::string faction, int value) {
		type = "int";
		var_id = fvar_id;
		action = faction;
		int_value = value;
	}
	/*please make the action "add" or "set"*/
	special(int fvar_id, std::string faction, std::string value) {
		type = "string";
		var_id = fvar_id;
		action = faction;
		string_value = value;
	}
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
	bool has_special = false;
	special special_data = special(0);
	room(std::vector<int> fobjects) {
		objects = fobjects;
		dobjects = fobjects;
	}
	room(std::vector<int> fobjects, int linked) {
		objects = fobjects;
		dobjects = fobjects;
		if (std::find(objects.begin(), objects.end(), TELEPORT) != objects.end()) {
			teleport_to = linked;
		}
		if (std::find(objects.begin(), objects.end(), PRESSURE_PLATE) != objects.end()) {
			activates_room = linked;
			activates_multiple = false;
		}
	}
	room(std::vector<int> fobjects_b, std::vector<int> fobjects_a, bool hbaa) {
		objects = fobjects_b;
		dobjects = fobjects_b;
		before_activation = fobjects_b;
		dbefore_activation = fobjects_b;
		after_activation = fobjects_a;
		dafter_activation = fobjects_a;
		has_before_and_after = hbaa;
		activates_multiple = false;
	}
	room(std::vector<int> fobjects, int powerups[2]) {
		objects = fobjects;
		dobjects = fobjects;
		powerup[0] = powerups[0];
		dpowerup[0] = powerups[0];
		powerup[1] = powerups[1];
		dpowerup[1] = powerups[1];
	}
	room(std::vector<int> fobjects, std::vector<int> rooms_to_activate) {
		activates_multiple = true;
		rooms_activated = rooms_to_activate;
		objects = fobjects;
		dobjects = fobjects;
	}
	room(std::vector<int> fobjects_b, std::vector<int> fobjects_a, bool hbaa, std::vector<int> rooms_to_activate) {
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
	room(std::vector<int> fobjects_b, std::vector<int> fobjects_a, int powerups[2], bool hbaa) {
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
	room(std::vector<int> fobjects, special fspecial) {
		objects = fobjects;
		dobjects = fobjects;
		special_data = fspecial;
		has_special = true;
	}
	room(std::vector<int> fobjects_a, std::vector<int> fobjects_b, special fspecial, bool hbaa) {
		objects = fobjects_a;
		dobjects = fobjects_a;
		before_activation = fobjects_a;
		dbefore_activation = fobjects_a;
		after_activation = fobjects_b;
		dafter_activation = fobjects_b;
		has_before_and_after = hbaa;
		activates_multiple = false;
		special_data = fspecial;
		has_special = true;
	}
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
			if (current_object_set == "before")
				objects = after_activation;
			else if (current_object_set == "after")
				objects = before_activation;
			else {
				error_code = "rm_chng_obj-set";
				error_message = "Could not determine which object\nset the room needed.";
				error = true;
			}
			current_object_set = (current_object_set == "before" ? "after" : "before");
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

class extra {
public:
	std::vector<bool> extra_bool;
	std::vector<int> extra_int;
	std::vector<std::string> extra_string;
	extra(std::vector<bool> fbool) {
		extra_bool = fbool;
	}
	extra(std::vector<int> fint) {
		extra_int = fint;
	}
	extra(std::vector<std::string> fstring) {
		extra_string = fstring;
	}
};

class level {
public:
	int width;
	int height;
	extra extra_data = extra(std::vector<int>({ 0 }));
	std::vector<room> rooms;
	bool has_custom_function;
	int custom_function;
	level(int fwidth, int fheight, extra fextra, std::vector<room> fdata, bool fhas_custom_function, int fcustom_function) {
		width = fwidth;
		height = fheight;
		extra_data = fextra;
		rooms = fdata;
		has_custom_function = fhas_custom_function;
		custom_function = fcustom_function;
	}
};

void custom_level_function(level &current_level);

extern level getLevel(int chapter, int lvl);

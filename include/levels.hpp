#pragma once
#include "DungeonEscape.hpp"

#ifndef level_h
#define level_h
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

class level {
public:
	int width;
	int height;
	std::vector<room> rooms;
	level(int, int, std::vector<room>);
};

extern level getLevel(int chapter, int lvl);

#endif

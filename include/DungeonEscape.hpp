#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include "sprites.h"
#include "ui.h"
#include "audio/filesystem.h"
#include "audio/sfx.h"
#include "file/file_access.hpp"
#include <limits>
#include <vector>
#include <algorithm>

extern std::string error_code, error_message;
extern bool error;

extern size_t st_null;

extern PrintConsole bottomScreen, versionWin, killBox, debugBox;

extern std::vector<std::string> debug_log;

#define game_data_size 1

#define ANSI "\x1b["
#define RED "31"
#define B_RED "41"
#define GREEN "32"
#define B_GREEN "42"
#define YELLOW "33"
#define B_YELLOW "43"
#define BLUE "34"
#define B_BLUE "44"
#define MAGENTA "35"
#define B_MAGENTA "45"
#define CYAN "36"
#define B_CYAN "46"
#define WHITE "37"
#define B_WHITE "47"
#define BRIGHT "2"
#define CRESET "\x1b[0m"
#define PRESET "\x1b[0;0H"
#define ASEP ";"
#define CEND "m"
#define PEND "H"

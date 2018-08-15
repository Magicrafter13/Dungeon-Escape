#pragma once
//This file is intended to host functions that will never, or rarely change to help make compiling quicker

void getHidInputs(u32 &kDown, u32 &kHeld);

bool touchInBox(touchPosition touch, int x, int y, int w, int h, u32 &kDown);

void echo_debug(bool main_window, std::string output, bool debugger_is_active);

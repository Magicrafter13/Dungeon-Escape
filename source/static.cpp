#include "DungeonEscape.hpp"

void getHidInputs(u32 &kDown, u32 &kHeld) {
	hidScanInput();
	kDown = hidKeysDown();
	kHeld = hidKeysHeld();
}

/*returns if area is being touched by stylus*/
bool touchInBox(touchPosition touch, int x, int y, int w, int h, u32 &kDown)
{
	int tx = touch.px;
	int ty = touch.py;
	kDown = hidKeysDown();
	if (kDown & KEY_TOUCH && tx > x && tx < x + w && ty > y && ty < y + h)
		return true;
	else
		return false;
}

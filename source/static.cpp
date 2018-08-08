#include "DungeonEscape.hpp"

void getHidInputs(u32 &kDown, u32 &kHeld) {
	hidScanInput();
	kDown = hidKeysDown();
	kHeld = hidKeysHeld();
}

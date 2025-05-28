//
// BetterCarryingEachOther 2016.03.11
// key state control module (header-only with inline)
//

#pragma once

class Key {
private:
	inline static bool keyStatePend[256] = { false };
	inline static bool keyStateAccept[256] = { false };

public:
	inline static void keyPressed(unsigned char key, int x, int y) {
		keyStatePend[key] = true;
	}

	inline static void keyReleased(unsigned char key, int x, int y) {
		keyStatePend[key] = false;
	}

	inline static bool keyCheckPressed(unsigned char key) {
		return keyStatePend[key] && !keyStateAccept[key];
	}

	inline static bool keyCheckReleased(unsigned char key) {
		return !keyStatePend[key] && keyStateAccept[key];
	}

	inline static bool keyCheckOn(unsigned char key) {
		return keyStatePend[key];
	}

	inline static void keyUpdate() {
		for (int i = 0; i < 256; i++) {
			keyStateAccept[i] = keyStatePend[i];
		}
	}
};

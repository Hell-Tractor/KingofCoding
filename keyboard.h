#pragma once

#include <qmap.h>
#include <qlayout.h>
#include "key.h"

class keyboard {
	public:
		QMap<char, key*> keys;
		QVBoxLayout* keyboard_layout;
		QHBoxLayout* layouts[3];

		keyboard();
	private:
		const static char posToChar[3][16];
};
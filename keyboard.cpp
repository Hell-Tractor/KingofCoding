#include "keyboard.h"

const char keyboard::posToChar[3][16] = {"QWERTYUIOP","ASDFGHJKL", "ZXCVBNM"};

keyboard::keyboard() {
	this->keyboard_layout = new QVBoxLayout;
	for (int i = 0; i < 3; ++i) {
		this->layouts[i] = new QHBoxLayout;
		this->layouts[i]->addStretch();
		for (int j = 0; posToChar[i][j]; ++j) {
			key* temp = new key(posToChar[i][j]);
			keys.insert(posToChar[i][j], temp);
			this->layouts[i]->addWidget(temp);
		}
		this->layouts[i]->addStretch();
		this->keyboard_layout->addLayout(this->layouts[i]);
	}
}
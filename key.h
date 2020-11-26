#pragma once

#include <qlabel.h>

class key : public QLabel {
	public:
		key(char ch, QWidget* parent = 0);
		void change_color(QString col);
};
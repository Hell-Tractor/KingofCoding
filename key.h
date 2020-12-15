#pragma once

#include <qlabel.h>

class key : public QLabel {
	public:
		key(char ch, QWidget* parent = 0);
		void highLight();
		void reset_color();
	private:
		int text_size;
		QString keySyle;
		QString defaultColor;
};
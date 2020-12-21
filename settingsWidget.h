#pragma once
#include <QtWidgets>

class settingsWidget final: public QWidget {
	Q_OBJECT

	public:
		settingsWidget(QWidget* parent = nullptr);
		
		void retranslateUi(QWidget* parent = nullptr);
		void showInterface();
		void paintEvent(QPaintEvent* event);

		QPushButton* exitbtn, * continuebtn, * savebtn;
};
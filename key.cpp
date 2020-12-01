#include "key.h"
#include <QSettings>

key::key(char ch, QWidget* parent) : QLabel(parent) {
	QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
	int size = settings->value("/keyboard/key_size").toInt();
	text_size = settings->value("/keyboard/text_size").toInt();
	delete settings;

	this->setText(QString(ch));
	this->setStyleSheet("background-color:yellow");
	QFont ft;
	ft.setPointSize(text_size);
	this->setFont(ft);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setFixedWidth(size);
	this->setFixedHeight(size);
	this->setAlignment(Qt::AlignCenter);
}

void key::change_color(QString col) {
	this->setStyleSheet(QString("background-color:") + col);
}
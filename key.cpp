#include "key.h"
#include <QSettings>

key::key(char ch, QWidget* parent) : QLabel(parent) {
	this->keySyle = "background-color: %1; border: 5px solid LightSlateGray";
	this->defaultColor = "white";

	QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
	int size = settings->value("/keyboard/key_size").toInt();
	text_size = settings->value("/keyboard/text_size").toInt();
	delete settings;

	this->setText(QString(ch));
	this->setStyleSheet(this->keySyle.arg(this->defaultColor));
	QFont ft;
	ft.setPointSize(text_size);
	ft.setFamily("Consolas");
	this->setFont(ft);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setFixedWidth(size);
	this->setFixedHeight(size);
	this->setAlignment(Qt::AlignCenter);
}

void key::highLight() {
	this->setStyleSheet(this->keySyle.arg("pink"));
}

void key::reset_color() {
	this->setStyleSheet(this->keySyle.arg(this->defaultColor));
}
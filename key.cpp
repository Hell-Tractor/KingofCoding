#include "key.h"

key::key(char ch, QWidget* parent) : QLabel(parent) {
	this->setText(QString(ch));
	this->setStyleSheet("background-color:yellow");
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setFixedWidth(50);
	this->setFixedHeight(50);
	this->setAlignment(Qt::AlignCenter);
}

void key::change_color(QString col) {
	this->setStyleSheet(QString("background-color:") + col);
}
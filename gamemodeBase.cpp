#include "gamemodeBase.h"

gamemodeBase::gamemodeBase(QWidget* parent) : QWidget(parent) {
	this->keyBoard = new keyboard;
	currentState = gameState::UN_STARTED;
}

gamemodeBase::~gamemodeBase() {
	if (this->keyBoard)
		delete this->keyBoard;
}

gamemodeBase::gameState gamemodeBase::GameState() const{
	return currentState;
}
#pragma once
#include "gamemodeBase.h"
#include <QFile>

class stageMode final : public gamemodeBase {
	public:
		stageMode(QWidget* parent = Q_NULLPTR);
		~stageMode();

		void retranslateUi();
		void handleKeyPress(int key);
		void wrongKeyWarning();
		void setCurrentStage(QString stage);
		void setTime(int time);
		void addTime(int delt);
		bool updateText();
		void init();
		void cleanUp(gameState state);

	private:
		QString currentText[3];
		int i, health, gametime/*ms*/;
		QFile* gameText;
		QString currentStage;
		int LENGTH_PER_LABEL, INIT_HEALTH;

		QLabel* textZone[3];
		QLabel* healthLabel, * timeLabel;

		QVBoxLayout* gameLayout;
		QHBoxLayout* stageLayout[3];
		QHBoxLayout* scoreboardLayout;
};
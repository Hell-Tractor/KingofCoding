#pragma once
#include "gamemodeBase.h"

class endlessMode final : public gamemodeBase {
	public:
		endlessMode(QWidget* parent = Q_NULLPTR);
		~endlessMode();
		
		void retranslateUi();
		void handleKeyPress(int key);
		void init();
		void cleanUp(gameState state);
		QString getText();
		void wrongKeyWarning();
		void addTime(int delt);

	private:
		QVBoxLayout* gameLayout;
		QLabel* textLabel;
		QLabel* healthLabel, * scoreLabel, * timeLabel;
		QHBoxLayout* scoreboardLayout, * textLayout;

		int INIT_HEALTH, INIT_TIME, HEALTH_REC_PERCENTAGE;
		int i, health, count, gameTime;
		double score;
		QString currentText;
		bool isRec;
		QByteArrayList keyList;

		QLabel* bgLabel;
};
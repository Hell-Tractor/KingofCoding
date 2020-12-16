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
		void setTime(int time);
		void addTime(int delt);

	private:
		QVBoxLayout* gameLayout;
		QLabel* textLabel;
		QLabel* healthLabel, * scoreLabel, * timeLabel;
		QHBoxLayout* scoreboardLayout, * textLayout;

		int i, health, count, gameTime;
		double score;
		QString currentText;
		bool isRec;
		QByteArrayList keyList;
};
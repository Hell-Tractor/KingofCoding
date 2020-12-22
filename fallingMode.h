#pragma once
#include "gamemodeBase.h"
#include <QTextEdit>
#include <QGraphicsOpacityEffect>


class fallingMode final : public gamemodeBase {
	Q_OBJECT

	public:
		fallingMode(QWidget* parent = Q_NULLPTR);
		~fallingMode();

		void retranslateUi();
		void handleKeyPress(int key);
		void init();
		void cleanUp(gameState state);
		void moveDown();
		void handleMiss();
		void pauseGame();
		void resumeGame();

	private:
		void generateLetter();
	
		class node {
			public:
				static int HEIGHT_PER_MOVE;
				int x, y;
				char c;
				int color;
				bool toRemove;
				QTextEdit* label;
		};

		const static QString PERFECT, GREAT, GOOD, MISS;
		const static double PERFECT_BONUS, GREAT_BONUS, GOOD_BONUS, MISS_BONUS;
		const static QString COLOR_SET[7];
		
		int INIT_HEALTH, INIT_SPEED, HEALTH_REC;
		int KEY_SIZE;
		int STAGE_WIDTH, STAGE_HEIGHT;
		int JUDGING_LINE_HEIGHT, LEVEL_LABEL_HEIGHT;
		int SCORE_PER_LEVEL;

		QVBoxLayout* gameLayout;
		QWidget* gameScreen, * screenShield;
		QHBoxLayout* scoreboardLayout;
		QLabel* healthLabel, * scoreLabel, * statusLabel;
		QLabel* judgingLineLabel;
		QLabel* levelLabel, * pauseLabel;
		QGraphicsOpacityEffect* hideOpacity;

		QByteArrayList allLetters;
		QVector<node> letters;
		QFont LETTERFT;
		bool hasHightLight;
		double score;
		int health;
		int level;

		int currentX;
		int currentColor;
};
#pragma once
#include "gamemodeBase.h"

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
		
	private:
		void generateLetter();
	
		class node {
			public:
				static int HEIGHT_PER_MOVE;
				int x, y;
				char c;
				int color;
				bool toRemove;
				QLabel* label;
		};

		const static QString PERFECT, GREAT, GOOD, MISS;
		const static double PERFECT_BONUS, GREAT_BONUS, GOOD_BONUS, MISS_BONUS;
		const static int MAX_HEALTH;
		const static QString COLOR_SET[7];
		
		static int KEY_SIZE, GEN_INTERVAL;
		int STAGE_WIDTH, STAGE_HEIGHT;
		int JUDGING_LINE_HEIGHT, LEVEL_LABEL_HEIGHT;

		QVBoxLayout* gameLayout;
		QWidget* gameScreen;
		QHBoxLayout* scoreboardLayout;
		QLabel* healthLabel, * scoreLabel, * statusLabel;
		QLabel* judgingLineLabel;
		QLabel* levelLabel;

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
#pragma once
#include <QWidget>
#include "keyboard.h"

class gamemodeBase : public QWidget {
	Q_OBJECT

	public:
		enum class gameState {
			UN_STARTED, WAITING, RUNNING, WIN, LOSE
		};

		gamemodeBase(QWidget* parent = Q_NULLPTR);
		virtual ~gamemodeBase();
		
		virtual void retranslateUi() = 0;
		virtual void handleKeyPress(int key) = 0;
		virtual void init() = 0;
		virtual void cleanUp(gameState state) = 0;
		virtual gameState GameState() const;
	
	signals:
		void exitGameInterface();

	protected:

		keyboard* keyBoard;
		gameState currentState;
};
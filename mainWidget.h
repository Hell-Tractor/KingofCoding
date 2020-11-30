#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainWidget.h"
#include <qqueue.h>
#include <qfile.h>
#include <qtimer.h>

class mainWidget : public QWidget {
    Q_OBJECT

  public:
    mainWidget(QWidget *parent = Q_NULLPTR);
    ~mainWidget();
    enum widget {
      MENU, MODE_SELECT, STAGE_SELECT, GAME
    };

    enum gameMode {
      STAGE, ENDLESS
    };

    enum gameState {
      UN_STARTED, WAITING, RUNNING, WIN, LOSE
    };

  signals:
    void GameStart(int mode);
    void GameEnds(int mode, int state);

  private slots:
    void setCurrentStage(QListWidgetItem* item);
    void initGame(int mode);
    void cleanUpGame(int mode, int state);
    void Loop();

  private:
    bool updateText();
    void keyPressEvent(QKeyEvent* e);
    void handleKeyPress_Stage(int key);
    void handleKeyPress_Endless(int key);
    void initStage();
    void initEndless();
    void wrongKeyWarning();
    QString getText();

    const static int length_per_label;
    const int time_interval;

    Ui::mainWidgetClass ui;
    QString currentStage, currentText[3];
    QFile* gameText;
    int gamestate;
    int health, gametime, gamemode, score;
    QTimer* timer;
    QByteArrayList keylist;

    int i; //record current position
};

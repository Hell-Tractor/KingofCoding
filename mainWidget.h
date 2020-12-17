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
      MENU, MODE_SELECT, STAGE_SELECT, ENDLESSMODE, STAGEMODE, FALLINGMODE, ABOUT
    };

    enum class gameMode {
      NONE, STAGE, ENDLESS, FALLING
    };

  signals:
    void GameStart(gameMode mode);

  private slots:
    void setCurrentStage(QListWidgetItem* item);
    void initGame(gameMode mode);
    void Loop();

  private:
    void keyPressEvent(QKeyEvent* e);

    int time_interval;

    Ui::mainWidgetClass ui;
    QTimer* timer;
    
    gameMode gamemode;
};

#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainWidget.h"
#include <qqueue.h>
#include <qfile.h>

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

  signals:
    void GameStart(int mode);

  private slots:
    void setCurrentStage(QListWidgetItem* item);
    void initGame(int mode);

  private:
    bool updateText();
    void keyPressEvent(QKeyEvent* e);

    Ui::mainWidgetClass ui;
    const static int length_per_label;
    QString currentStage, currentText[3];
    QFile* gameText;
    bool isGaming;
    int health;
};

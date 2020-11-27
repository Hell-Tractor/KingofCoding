#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainWidget.h"

class mainWidget : public QWidget {
    Q_OBJECT

  public:
    mainWidget(QWidget *parent = Q_NULLPTR);
    enum widget {
      MENU, MODESELECT, STAGEMODE, ENDLESSMODE
    };


  private slots:
    
  private:
    void startStageGame(QString& stageName);
    void updateText(QString& text);

    Ui::mainWidgetClass ui;
};

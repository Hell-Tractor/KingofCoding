#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainWidget.h"

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    mainWidget(QWidget *parent = Q_NULLPTR);

private slots:
  
private:
    Ui::mainWidgetClass ui;
};

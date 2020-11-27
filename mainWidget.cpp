#include "mainWidget.h"
#include "stdafx.h"

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent) {
   ui.setupUi(this);


   /******************connect********************/
   connect(ui.startbtn, &QPushButton::clicked, this, [=]() {
     ui.stackedWidget->setCurrentIndex(widget::MODESELECT);
            });
   connect(ui.stagebtn, &QPushButton::clicked, this, [=]() {
     ui.stackedWidget->setCurrentIndex(widget::STAGEMODE);
     this->startStageGame(QString("Hello World"));
           });
   connect(ui.backToMenu, &QPushButton::clicked, this, [=]() {
     ui.stackedWidget->setCurrentIndex(widget::MENU);
           });
   /******************connect********************/
}

void mainWidget::startStageGame(QString& stageName) {
  //read 10 char, input, read 5 char, loop
  QFile text(QString("./stages/") + stageName + ".stg");
  if (!text.exists())
    throw "runtimeError";
  text.open(QIODevice::ReadOnly | QIODevice::Text);
  char c;
  QString str;
  int cnt = 0;
  while (text.getChar(&c)) {
    qDebug() << "read: " << c;
    if (isalpha(c)) {
      str.push_back(c);
      cnt++;
    }
    if (cnt == 5) {
      this->updateText(str);
      cnt = 0;
      str.clear();
    }
  }
  if (!str.isEmpty())
    this->updateText(str);
  text.close();
}

void mainWidget::updateText(QString& text) {
  for (int i = 0; i < 2; ++i)
    ui.textZone[i]->setText(ui.textZone[i + 1]->text());
  ui.textZone[2]->setText(QString("<font face=Inconsolata size=15>") + text + "</font>");
}
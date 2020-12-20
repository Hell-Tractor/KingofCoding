#include "mainWidget.h"
#include "stdafx.h"
#include <qmessagebox.h>
#include <qglobal.h>

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent) {
  ui.setupUi(this);
  setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
  setFixedSize(this->width(), this->height());
   
  /********************init********************/
  QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
  time_interval = settings->value("/game/refresh_interval").toInt();
  delete settings;
  gamemode = gameMode::NONE;
  timer = new QTimer();
  timer->setTimerType(Qt::TimerType::PreciseTimer);
  /********************init********************/

  /******************connect********************/
  //buttons
  connect(ui.tutorialbtn, &QPushButton::clicked, this, [=]() {
    QDesktopServices::openUrl(QUrl("https://github.com/Hell-Tractor/KingofCoding"));
          });
  connect(ui.startbtn, &QPushButton::clicked, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::MODE_SELECT);
          });
  connect(ui.stagebtn, &QPushButton::clicked, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::STAGE_SELECT);
          });
  connect(ui.backToMenu, &QPushButton::clicked, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::MENU);
          });
  connect(ui.backToMode, &QPushButton::clicked, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::MODE_SELECT);
          });
  connect(ui.endlessbtn, &QPushButton::clicked, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::ENDLESSMODE);
    emit GameStart(gameMode::ENDLESS);
          });
  connect(ui.fallingbtn, &QPushButton::clicked, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::FALLINGMODE);
    emit GameStart(gameMode::FALLING);
          });
  connect(ui.aboutbtn, &QPushButton::clicked, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::ABOUT);
          });
  connect(ui.aboutToMenu, &QPushButton::clicked, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::MENU);
          });
  connect(ui.stageModeWidget, &stageMode::exitGameInterface, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::STAGE_SELECT);
          });
  connect(ui.endlessModeWidget, &endlessMode::exitGameInterface, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::MODE_SELECT);
          });
  connect(ui.fallingModeWidget, &fallingMode::exitGameInterface, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::MODE_SELECT);
          });

  connect(ui.stage_list_widget, &QListWidget::itemDoubleClicked, this, &mainWidget::setCurrentStage);
  connect(this, &mainWidget::GameStart, this, &mainWidget::initGame);
  connect(this->timer, &QTimer::timeout, this, &mainWidget::Loop);
  /******************connect********************/

  timer->start(time_interval);
}

mainWidget::~mainWidget() {
}

void mainWidget::keyPressEvent(QKeyEvent* e) {
  switch (gamemode) {
    case gameMode::STAGE:
      ui.stageModeWidget->handleKeyPress(e->key());
      break;
    case gameMode::ENDLESS:
      ui.endlessModeWidget->handleKeyPress(e->key());
      break;
    case gameMode::FALLING:
      ui.fallingModeWidget->handleKeyPress(e->key());
      break;
    case gameMode::NONE:
      break;
    default:
      throw "Unknown game mode";
      break;
  }
}

void mainWidget::setCurrentStage(QListWidgetItem* item) {
  ui.stageModeWidget->setCurrentStage(item->text());
  ui.stackedWidget->setCurrentIndex(widget::STAGEMODE);
  emit GameStart(gameMode::STAGE);
}

void mainWidget::initGame(gameMode mode) {
  gamemode = mode;  
  switch (mode) {
    case gameMode::STAGE:
      ui.stageModeWidget->init();
      break;
    case gameMode::ENDLESS:
      ui.endlessModeWidget->init();
      break;
    case gameMode::FALLING:
      ui.fallingModeWidget->init();
      break;
    default:
      throw "Unknown game mode";
      return;
  }
}

void mainWidget::Loop() {
  //update time
  if (ui.stageModeWidget->GameState() == gamemodeBase::gameState::RUNNING && gamemode == gameMode::STAGE)
    ui.stageModeWidget->addTime(time_interval);
  if (ui.endlessModeWidget->GameState() == gamemodeBase::gameState::RUNNING && gamemode == gameMode::ENDLESS)
    ui.endlessModeWidget->addTime(-time_interval);
  if (ui.fallingModeWidget->GameState() == gamemodeBase::gameState::RUNNING && gamemode == gameMode::FALLING) {
    ui.fallingModeWidget->moveDown();
  }
  
  ui.stackedWidget->update();
}
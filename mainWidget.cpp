#include "mainWidget.h"
#include "stdafx.h"
#include <qmessagebox.h>

const int mainWidget::length_per_label = 10;

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent), time_interval(10) {
  ui.setupUi(this);
   
  /********************init********************/
  gamestate = gameState::UN_STARTED;
  health = gametime = 0;
  timer = new QTimer();
  timer->setTimerType(Qt::TimerType::CoarseTimer);
  /********************init********************/

  /******************Thread*********************/
  /******************Thread*********************/

  /******************connect********************/
  //buttons
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

  connect(ui.stage_list_widget, &QListWidget::itemDoubleClicked, this, &mainWidget::setCurrentStage);
  connect(this, &mainWidget::GameStart, this, &mainWidget::initGame);
  connect(this, &mainWidget::GameEnds, this, &mainWidget::cleanUpGame);
  connect(this->timer, &QTimer::timeout, this, &mainWidget::Loop);
  /******************connect********************/

  timer->start(20);
}

mainWidget::~mainWidget() {
}

void mainWidget::keyPressEvent(QKeyEvent* e) {
  if (gamestate != gameState::RUNNING && gamestate != gameState::WAITING)
    return;
  if (gamestate == gameState::WAITING)
    gamestate = gameState::RUNNING;
  qDebug() << "key pressed: " << (char)e->key();
  
  static int i = 0; //record current position
  if (e->key() == currentText[1][i].toUpper().unicode()) {
    //restore key color
    ui.keyboard_->keys[e->key()]->change_color("yellow");
    //set text color
    ui.textZone[1]->setText(QString("<font face=Inconsolata size=15 color=red>") + currentText[1].left(i + 1) +
                            "</font><font face=Inconsolata size=15>" + currentText[1].right(currentText[1].length() - i - 1) + "</font>");
    i++;
    //check legality of i
    if (i == currentText[1].length()) {
      if (!updateText()) {
        emit GameEnds(gameMode::STAGE, gameState::WIN);
        i = 0;
        return;
      }
      i = 0;
    }
    //change key color
    ui.keyboard_->keys[currentText[1][i].toUpper().unicode()]->change_color("pink");
  } else {
    //wrong key pressed
    health--;
    ui.health_label->setText(QString("Health: ") + QString::number(health));
    //died
    if (health == 0) {
      emit GameEnds(gameMode::STAGE, gameState::LOSE);
      i = 0;
      return;
    }
  }
  
}

bool mainWidget::updateText() {
  QString text;
  char c;
  //read text from file
  while (this->gameText->getChar(&c)) {
    if (isalpha(c))
      text.push_back(c);
    if (text.length() == length_per_label)
      break;
  }
  qDebug() << text;
  //update textZone
  for (int i = 0; i < 2; ++i) {
    ui.textZone[i]->setText(ui.textZone[i + 1]->text());
    currentText[i] = currentText[i + 1];
  }
  ui.textZone[2]->setText(QString("<font face=Inconsolata size=15>") + text + "</font>");
  currentText[2] = text;
  return !currentText[1].isEmpty();
}

void mainWidget::setCurrentStage(QListWidgetItem* item) {
  currentStage = item->text();
  qDebug() << "currentStage changed: " << currentStage;

  ui.stackedWidget->setCurrentIndex(widget::GAME);
  emit GameStart(gameMode::STAGE);
}

void mainWidget::initGame(int mode) {
  health = 3;
  gametime = 0;
  //initial health display
  ui.health_label->setText(QString("Health: ") + QString::number(health));

  //set text file
  gameText = new QFile(QString("./stages/") + currentStage + ".stg");
  gameText->open(QIODevice::ReadOnly | QIODevice::Text);

  //update text
  this->updateText();
  this->updateText();
  
  //change key color
  qDebug() << (char)currentText[1][0].toUpper().unicode();
  ui.keyboard_->keys[currentText[1][0].toUpper().unicode()]->change_color("pink");

  gamestate = gameState::WAITING;
}

void mainWidget::Loop() {
  //update time
  if (gamestate == gameState::RUNNING) {
    gametime += time_interval;
    ui.time_label->setText(QString("Time Used: ") + QString::number(gametime / 1000.0, 'f', 2));
  }
  
  ui.stackedWidget->repaint();
}

void mainWidget::cleanUpGame(int mode, int state) {
  //update gamestate
  gamestate = state;

  //show message and block current thread
  if (state == gameState::WIN)
    QMessageBox::information(this, "Congratulations!", QString("Your time: ") + QString::number(gametime / 1000.0, 'f', 2), QMessageBox::Yes);
  else if (state == gameState::LOSE)
    QMessageBox::information(this, "You Lose", "You have lost all of your health", QMessageBox::Yes);
  else
    throw "UnKnown GameState";
  
  //change widget and initial labels
  ui.stackedWidget->setCurrentIndex(widget::STAGE_SELECT);
  ui.time_label->setText(QString("Time Used: 0.00"));
  for (auto&& i : ui.textZone)
    i->setText("");
  for (auto&& i : ui.keyboard_->keys)
    i->change_color("yellow");

  //close gamefile
  gameText->close();
  delete gameText;

  //update gamestate
  gamestate = gameState::UN_STARTED;
}
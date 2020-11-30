#include "mainWidget.h"
#include "stdafx.h"
#include <qmessagebox.h>
#include <qglobal.h>

const int mainWidget::length_per_label = 20;

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent), time_interval(20) {
  ui.setupUi(this);
   
  /********************init********************/
  gamestate = gameState::UN_STARTED;
  gamemode = -1;
  gameText = nullptr;
  health = gametime = 0;
  score = 0;
  timer = new QTimer();
  timer->setTimerType(Qt::TimerType::PreciseTimer);
  i = 0;
  /********************init********************/

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
  connect(ui.endlessbtn, &QPushButton::clicked, this, [=]() {
    ui.stackedWidget->setCurrentIndex(widget::GAME);
    emit GameStart(gameMode::ENDLESS);
          });

  connect(ui.stage_list_widget, &QListWidget::itemDoubleClicked, this, &mainWidget::setCurrentStage);
  connect(this, &mainWidget::GameStart, this, &mainWidget::initGame);
  connect(this, &mainWidget::GameEnds, this, &mainWidget::cleanUpGame);
  connect(this->timer, &QTimer::timeout, this, &mainWidget::Loop);
  /******************connect********************/

  timer->start(time_interval);
}

mainWidget::~mainWidget() {
}

void mainWidget::keyPressEvent(QKeyEvent* e) {
  if (gamestate != gameState::RUNNING && gamestate != gameState::WAITING)
    return;
  if (gamestate == gameState::WAITING)
    gamestate = gameState::RUNNING;
  qDebug() << "key pressed: " << (char)e->key();
  
  switch (gamemode) {
    case gameMode::STAGE:
      this->handleKeyPress_Stage(e->key());
      break;
    case gameMode::ENDLESS:
      this->handleKeyPress_Endless(e->key());
      break;
    default:
      throw "Unknown game mode";
      break;
  }
}

void mainWidget::handleKeyPress_Endless(int key) {
  if (key == currentText[1][i].toUpper().unicode()) {
    //restore key color
    ui.keyboard_->keys[key]->change_color("yellow");
    //set text color
    ui.textZone[1]->setText(QString("<font face=Consolas size=15 color=red>") + currentText[1].left(i + 1) +
                            "</font><font face=Consolas size=15>" + currentText[1].right(currentText[1].length() - i - 1) + "</font>");
    i++;
    if (i == currentText[1].length()) {
      //add score
      score++;
      ui.score_label->setText(QString("Score: ") + QString::number(score));

      //reset timer
      gametime = 5000;

      //get new key
      i = 0;
      currentText[1] = getText();
      ui.textZone[1]->setText(QString("<font face=Consolas size=15>") + currentText[1] + "</font>");
    }
    //change key color
    ui.keyboard_->keys[currentText[1][i].toUpper().unicode()]->change_color("pink");
  } else {
    //restore key color
    ui.keyboard_->keys[currentText[1][i].toUpper().unicode()]->change_color("yellow");
    
    this->wrongKeyWarning();
    //get new key
    currentText[1] = getText();
    ui.textZone[1]->setText(QString("<font face=Consolas size=15>") + currentText[1] + "</font>");
    ui.keyboard_->keys[currentText[1][i].toUpper().unicode()]->change_color("pink");
  }
}

void mainWidget::handleKeyPress_Stage(int key) {
  if (key == currentText[1][i].toUpper().unicode()) {
    //restore key color
    ui.keyboard_->keys[key]->change_color("yellow");
    //set text color
    ui.textZone[1]->setText(QString("<font face=Consolas size=15 color=red>") + currentText[1].left(i + 1) +
                            "</font><font face=Consolas size=15>" + currentText[1].right(currentText[1].length() - i - 1) + "</font>");
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
  } else
    this->wrongKeyWarning();
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
  ui.textZone[2]->setText(QString("<font face=Consolas size=15>") + text + "</font>");
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
  gamemode = mode;  
  switch (mode) {
    case gameMode::STAGE:
      this->initStage();
      break;
    case gameMode::ENDLESS:
      this->initEndless();
      break;
    default:
      throw "Unknown game mode";
      return;
  }

  gamestate = gameState::WAITING;
}

void mainWidget::initEndless() {
  //get keys list
  QFile keys("./keys");
  if (!keys.exists())
    throw "document keys lost!";
  keys.open(QIODevice::ReadOnly | QIODevice::Text);
  keylist = keys.readAll().split('\n');
  qDebug() << keylist[0] << ' ' << keylist.length();

  //init varible
  health = 3;
  gametime = 5 * 1000;
  score = 0;
  i = 0;

  //init label
  ui.time_label->setText("Time left: 5.00");
  ui.score_label->setText("Score: 0");
  ui.score_label->show();
  ui.health_label->setText("Health: 3");


  //init random
  qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

  //show first word
  currentText[1] = getText();
  ui.textZone[1]->setText(QString("<font face=Consolas size=15>") + currentText[1] + "</font>");

  //change keyboard color
  ui.keyboard_->keys[currentText[1][0].toUpper().unicode()]->change_color("pink");
}

void mainWidget::initStage() {
  i = 0;
  health = 3;
  gametime = 0;
  //initial label
  ui.health_label->setText(QString("Health: ") + QString::number(health));
  ui.time_label->setText(QString("Time Used: 0.00"));

  //hide score_label
  ui.score_label->hide();

  //set text file
  gameText = new QFile(QString("./stages/") + currentStage + ".stg");
  gameText->open(QIODevice::ReadOnly | QIODevice::Text);

  //update text
  this->updateText();
  this->updateText();

  //change key color
  qDebug() << (char)currentText[1][0].toUpper().unicode();
  ui.keyboard_->keys[currentText[1][0].toUpper().unicode()]->change_color("pink");
}

void mainWidget::wrongKeyWarning() {
  //wrong key pressed
  health--;
  ui.health_label->setText(QString("Health: ") + QString::number(health));
  //wrong key warning
  ui.health_label->setStyleSheet("background-color: red");

  //died
  if (health == 0) {
    emit GameEnds(gamemode, gameState::LOSE);
    i = 0;
    return;
  }

  //dewarn
  QTimer::singleShot(100, Qt::TimerType::CoarseTimer, [=]() {
    ui.health_label->setStyleSheet("");
                     });
  if (gamemode == gameMode::ENDLESS)
    i = 0;
}

void mainWidget::Loop() {
  //update time
  if (gamestate == gameState::RUNNING && gamemode == gameMode::STAGE) {
    gametime += time_interval;
    ui.time_label->setText(QString("Time Used: ") + QString::number(gametime / 1000.0, 'f', 2));
  }
  if (gamestate == gameState::RUNNING && gamemode == gameMode::ENDLESS) {
    gametime -= time_interval;
    if (gametime <= 0) {
      this->wrongKeyWarning();
      if (gamestate == gameState::RUNNING)
        gametime = 5000;
    }
    ui.time_label->setText(QString("Time Left: ") + QString::number(gametime / 1000.0, 'f', 2));
  }
  
  ui.stackedWidget->repaint();
}

void mainWidget::cleanUpGame(int mode, int state) {
  //update gamestate
  gamestate = state;
  gamemode = mode;

  //show message and block current thread
  if (state == gameState::WIN)
    QMessageBox::information(this, "Congratulations!", QString("Your time: ") + QString::number(gametime / 1000.0, 'f', 2), QMessageBox::Yes);
  else if (state == gameState::LOSE && mode == gameMode::STAGE)
    QMessageBox::information(this, "Game Over!", "You have lost all of your health", QMessageBox::Yes);
  else if (state == gameState::LOSE && mode == gameMode::ENDLESS) {
    QMessageBox::information(this, "Game Over!", QString("You have lost all of your health\nYour final score: ") + QString::number(score), QMessageBox::Yes);
  } else
    throw "UnKnown GameState";
  
  for (auto&& i : ui.textZone)
    i->setText("");
  for (auto&& i : ui.keyboard_->keys)
    i->change_color("yellow");
  ui.health_label->setStyleSheet("");

  if (mode == gameMode::STAGE) {
    //change widget and initial labels
    ui.stackedWidget->setCurrentIndex(widget::STAGE_SELECT);

    //close gamefile
    gameText->close();
    delete gameText;
    gameText = nullptr;
  } else if (mode == gameMode::ENDLESS) {
    //change widget and initial labels
    ui.stackedWidget->setCurrentIndex(widget::MODE_SELECT);
  }

  //update gamestate
  gamestate = gameState::UN_STARTED;
}

QString mainWidget::getText() {
  return keylist[qrand() % keylist.length()];
}
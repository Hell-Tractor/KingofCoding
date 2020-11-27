#include "ui_mainWidget.h"

void Ui_mainWidgetClass::setupUi(QWidget* mainWidgetClass) {
  if (mainWidgetClass->objectName().isEmpty())
    mainWidgetClass->setObjectName(QString::fromUtf8("mainWidgetClass"));
  mainWidgetClass->resize(600, 400);
  /**********************************/
  QHBoxLayout* stackedWidgetLayout = new QHBoxLayout;
  stackedWidget = new QStackedWidget;
  stackedWidgetLayout->addWidget(stackedWidget);
  mainWidgetClass->setLayout(stackedWidgetLayout);

  //menu
  QWidget* menu = new QWidget;
  QHBoxLayout* menu_title = new QHBoxLayout;
  title = new QLabel;
  menu_title->addStretch();
  menu_title->addWidget(title);
  menu_title->addStretch();
  standingbtn = new QPushButton;
  startbtn = new QPushButton;
  QHBoxLayout* menu_btn = new QHBoxLayout;
  menu_btn->addStretch();
  menu_btn->addWidget(standingbtn);
  menu_btn->addStretch();
  menu_btn->addWidget(startbtn);
  menu_btn->addStretch();
  QVBoxLayout* menulayout = new QVBoxLayout;
  menulayout->addLayout(menu_title);
  menulayout->addLayout(menu_btn);
  menu->setLayout(menulayout);
  stackedWidget->addWidget(menu);

  //mode select
  QWidget* mode_select = new QWidget;
  QHBoxLayout* selectm_title_layout = new QHBoxLayout;
  selectm_title = new QLabel;
  selectm_title_layout->addStretch();
  selectm_title_layout->addWidget(selectm_title);
  selectm_title_layout->addStretch();
  QHBoxLayout* selectbtn_layout = new QHBoxLayout;
  stagebtn = new QPushButton;
  endlessbtn = new QPushButton;
  backToMenu = new QPushButton;
  selectbtn_layout->addStretch();
  selectbtn_layout->addWidget(stagebtn);
  selectbtn_layout->addStretch();
  selectbtn_layout->addWidget(endlessbtn);
  selectbtn_layout->addStretch();
  selectbtn_layout->addWidget(backToMenu);
  selectbtn_layout->addStretch();
  QVBoxLayout* mode_select_layout = new QVBoxLayout;
  mode_select_layout->addLayout(selectm_title_layout);
  mode_select_layout->addLayout(selectbtn_layout);
  mode_select->setLayout(mode_select_layout);
  stackedWidget->addWidget(mode_select);

  //game
  QWidget* game = new QWidget;
  QVBoxLayout* game_layout = new QVBoxLayout;
  //stage
  QHBoxLayout* stage_layouts[3];
  for (int i = 0; i < 3; ++i) {
    stage_layouts[i] = new QHBoxLayout;
    textZone[i] = new QLabel;
    stage_layouts[i]->addWidget(textZone[i]);
    stage_layouts[i]->addStretch();
    game_layout->addLayout(stage_layouts[i]);
  }
  //scoreboard
  //keyboard
  keyboard_ = new keyboard;
  game_layout->addLayout(keyboard_->keyboard_layout);
  game->setLayout(game_layout);
  stackedWidget->addWidget(game);
  /**********************************/
  retranslateUi(mainWidgetClass);

  QMetaObject::connectSlotsByName(mainWidgetClass);
}

void Ui_mainWidgetClass::retranslateUi(QWidget* mainWidgetClass) {
  mainWidgetClass->setWindowTitle(QCoreApplication::translate("mainWidgetClass", "King of Coding", nullptr));
  //menu
  this->title->setText("King of Coding");
  this->standingbtn->setText("Top Standings");
  this->startbtn->setText("Start Game");
  
  //mode select
  this->selectm_title->setText("select mode");
  this->stagebtn->setText("Stage mode");
  this->endlessbtn->setText("Endless mode");
  this->backToMenu->setText("Back");
  
  //game
  //for (int i = 0; i < 3; ++i) {
  //  this->textZone[i]->setText("<font size=15 color=red face=Inconsolata>TEST</font>");
  //}

  this->stackedWidget->setCurrentIndex(0);
}
#include "ui_mainWidget.h"

#include <qdir.h>
#include <qdebug.h>

void Ui_mainWidgetClass::setupUi(QWidget* mainWidgetClass) {
  if (mainWidgetClass->objectName().isEmpty())
    mainWidgetClass->setObjectName(QString::fromUtf8("mainWidgetClass"));
  /**************settings***************/
  QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
  int width = settings->value("/mainWidget/width").toInt();
  int height = settings->value("/mainWidget/height").toInt();
  delete settings;
  /**************settings***************/
  mainWidgetClass->resize(width, height);
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
  aboutbtn = new QPushButton;
  standingbtn = new QPushButton;
  startbtn = new QPushButton;
  QHBoxLayout* menu_btn = new QHBoxLayout;
  menu_btn->addStretch();
  menu_btn->addWidget(aboutbtn);
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

  //stage select
  QWidget* stage_select = new QWidget;
  QVBoxLayout* stage_select_layout = new QVBoxLayout;
  stage_select_title = new QLabel;
  stage_list_widget = new QListWidget;
  stage_select_layout->addWidget(stage_select_title);
  stage_select_layout->addWidget(stage_list_widget);
  //button
  QHBoxLayout* btn_layout = new QHBoxLayout;
  btn_layout->addStretch();
  backToMode = new QPushButton;
  btn_layout->addWidget(backToMode);
  stage_select_layout->addLayout(btn_layout);
  stage_select->setLayout(stage_select_layout);
  stackedWidget->addWidget(stage_select);

  //game
  QWidget* game = new QWidget;
  QVBoxLayout* game_layout = new QVBoxLayout;
  //stage
  QHBoxLayout* stage_layouts[3];
  for (int i = 0; i < 3; ++i) {
    stage_layouts[i] = new QHBoxLayout;
    textZone[i] = new QLabel;
    //textZone[i]->setFixedWidth(500);
    //textZone[i]->setFixedHeight(40);
    stage_layouts[i]->addWidget(textZone[i]);
    stage_layouts[i]->addStretch();
    game_layout->addLayout(stage_layouts[i]);
  }
  //scoreboard
  QHBoxLayout* scoreboard_layout = new QHBoxLayout;
  health_label = new QLabel;
  score_label = new QLabel;
  time_label = new QLabel;
  scoreboard_layout->addWidget(health_label);
  scoreboard_layout->addStretch();
  scoreboard_layout->addWidget(score_label);
  scoreboard_layout->addStretch();
  scoreboard_layout->addWidget(time_label);
  game_layout->addLayout(scoreboard_layout);
  //keyboard
  keyboard_ = new keyboard;
  game_layout->addLayout(keyboard_->keyboard_layout);
  //apply
  game->setLayout(game_layout);
  stackedWidget->addWidget(game);

  //about
  QWidget* about = new QWidget;
  QVBoxLayout* about_layout = new QVBoxLayout;
  aboutInfo = new QLabel;
  about_layout->addWidget(aboutInfo);
  QHBoxLayout* about_btn_layout = new QHBoxLayout;
  aboutToMenu = new QPushButton;
  about_btn_layout->addStretch();
  about_btn_layout->addWidget(aboutToMenu);
  about_layout->addLayout(about_btn_layout);
  about->setLayout(about_layout);
  stackedWidget->addWidget(about);
  /**********************************/
  retranslateUi(mainWidgetClass);

  QMetaObject::connectSlotsByName(mainWidgetClass);
}

void Ui_mainWidgetClass::retranslateUi(QWidget* mainWidgetClass) {
  mainWidgetClass->setWindowTitle(QCoreApplication::translate("mainWidgetClass", "King of Coding-v2.1.0-alpha", nullptr));
  //menu
  this->title->setText("King of Coding\nv2.1.0-alpha");
  this->title->setAlignment(Qt::AlignCenter);
  this->standingbtn->setText("Top Standings");
  this->startbtn->setText("Start Game");
  this->aboutbtn->setText("About");
  
  //mode select
  this->selectm_title->setText("select mode");
  this->stagebtn->setText("Stage mode");
  this->endlessbtn->setText("Endless mode");
  this->backToMenu->setText("Back");
  
  //stage select
  this->stage_select_title->setText("Select Stage");
  QDir dir("./stages");
  dir.setNameFilters(QStringList("*.stg"));
  QFileInfoList stages = dir.entryInfoList();
  for (auto&& i : stages)
    this->stage_list_widget->addItem(i.fileName().split('.')[0]);
  this->backToMode->setText("Back");

  //game
  this->health_label->setText("Health: ");
  this->time_label->setText("Time Used: 0.00");
  this->score_label->setText("Score: 0");

  //about
  this->aboutToMenu->setText("Back");
  this->aboutbtn->setText("About");
  this->aboutInfo->setText("<font size=\"3px\" face=Consolas>\
                           < h1 style = \"text-align: center;\" > King of Coding< / h1>\
                           < / font>\
                           <font size = \"2px\" face=Consolas>\
                           <p style = \"text-align: left\">Version: 2.1.0 - alpha< / p>\
                           <p style = \"text-align: left\">Author : Chen Yiming< / p>\
                           <p style = \"text-align: left\">Other : This program is only for personal use.<br><font style = \"color: red;\">Any acts of plagiarism is prohibitted.< / font>< / p>\
                           < / font>");

  this->stackedWidget->setCurrentIndex(0);
}
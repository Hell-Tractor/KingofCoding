#include "ui_mainWidget.h"

#include <qdir.h>
#include <qdebug.h>

void Ui_mainWidgetClass::setupUi(QWidget* mainWidgetClass) {
  if (mainWidgetClass->objectName().isEmpty())
    mainWidgetClass->setObjectName(QString::fromUtf8("mainWidgetClass"));
  mainWidgetClass->setWindowIcon(QIcon("./icons/ico.png"));
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
  tutorialbtn = new QPushButton;
  startbtn = new QPushButton;
  QHBoxLayout* menu_btn = new QHBoxLayout;
  menu_btn->addStretch();
  menu_btn->addWidget(aboutbtn);
  menu_btn->addStretch();
  menu_btn->addWidget(tutorialbtn);
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
  fallingbtn = new QPushButton;
  backToMenu = new QPushButton;
  selectbtn_layout->addStretch();
  selectbtn_layout->addWidget(stagebtn);
  selectbtn_layout->addStretch();
  selectbtn_layout->addWidget(endlessbtn);
  selectbtn_layout->addStretch();
  selectbtn_layout->addWidget(fallingbtn);
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

  //endlessmode widget
  endlessModeWidget = new endlessMode(nullptr);
  stackedWidget->addWidget(endlessModeWidget);

  //stagemode widget
  stageModeWidget = new stageMode(nullptr);
  stackedWidget->addWidget(stageModeWidget);

  //fallingmode widget
  fallingModeWidget = new fallingMode(nullptr);
  stackedWidget->addWidget(fallingModeWidget);

  //about
  QWidget* about = new QWidget;
  QVBoxLayout* about_layout = new QVBoxLayout;
  about_layout->addStretch();
  aboutTitle = new QLabel;
  about_layout->addWidget(aboutTitle);
  aboutInfo = new QLabel;
  about_layout->addWidget(aboutInfo);
  about_layout->addStretch();
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
  /**********Read version info************/
  QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
  QString versiontag = settings->value("/version/tag").toString();
  QString version = settings->value("/version/version").toString();
  delete settings;
  /***************************************/

  mainWidgetClass->setWindowTitle(QCoreApplication::translate("mainWidgetClass", QString("King of Coding-v%1%2").arg(version, (versiontag == "release" ? "" : QString("-") + versiontag)).toStdString().c_str(), nullptr));
  //menu
  QFont titleft;
  titleft.setPointSize(70);
  titleft.setFamily("Curlz MT");
  titleft.setBold(true);
  this->title->setFont(titleft);
  this->title->setText("<font color=pink>King</font>\t<font color=blue>of</font>\t<font color=indigo>Coding</font>");
  this->title->setAlignment(Qt::AlignCenter);
  QFont btnft;
  btnft.setPointSize(20);
  btnft.setFamily("Bauhaus 93");
  this->tutorialbtn->setFont(btnft);
  this->tutorialbtn->setText("Tutorials");
  this->startbtn->setFont(btnft);
  this->startbtn->setText("Start");
  this->aboutbtn->setFont(btnft);
  this->aboutbtn->setText("About");
  
  //mode select
  QFont selectmTitleFt;
  selectmTitleFt.setPointSize(50);
  selectmTitleFt.setFamily("Berlin Sans FB Demi");
  this->selectm_title->setFont(selectmTitleFt);
  this->selectm_title->setText("select mode");
  this->stagebtn->setFont(btnft);
  this->stagebtn->setText("Stage mode");
  this->endlessbtn->setFont(btnft);
  this->endlessbtn->setText("Endless mode");
  this->fallingbtn->setFont(btnft);
  this->fallingbtn->setText("Falling mode");
  this->backToMenu->setFont(btnft);
  this->backToMenu->setText("Back");
  
  //stage select
  QFont stageSelectTitleFt;
  stageSelectTitleFt.setPointSize(20);
  stageSelectTitleFt.setFamily("Berlin Sans FB Demi");
  this->stage_select_title->setFont(stageSelectTitleFt);
  this->stage_select_title->setText("Select Stage");
  QDir dir("./stages");
  dir.setNameFilters(QStringList("*.stg"));
  QFileInfoList stages = dir.entryInfoList();
  QFont stageListFt;
  stageListFt.setPointSize(15);
  stageListFt.setFamily("Consolas");
  this->stage_list_widget->setFont(stageListFt);
  for (auto&& i : stages)
    this->stage_list_widget->addItem(i.fileName().split('.')[0]);

  this->backToMode->setFont(btnft);
  this->backToMode->setText("Back");

  //game widget
  this->endlessModeWidget->retranslateUi();
  this->stageModeWidget->retranslateUi();
  this->fallingModeWidget->retranslateUi();

  //about
  this->aboutToMenu->setFont(btnft);
  this->aboutToMenu->setText("Back");
  this->aboutbtn->setFont(btnft);
  this->aboutbtn->setText("About");
  QFont aboutTitleFt;
  aboutTitleFt.setPointSize(40);
  aboutTitleFt.setFamily("Curlz MT");
  aboutTitleFt.setBold(true);
  this->aboutTitle->setFont(aboutTitleFt);
  this->aboutTitle->setAlignment(Qt::AlignCenter);
  this->aboutTitle->setText("<font color=pink>King</font>\t<font color=blue>of</font>\t<font color=indigo>Coding</font>");
  QFont aboutInfoFt;
  aboutInfoFt.setPointSize(20);
  aboutInfoFt.setFamily("Consolas");
  this->aboutInfo->setFont(aboutInfoFt);
  QString ver = QString("Version: %1%2").arg(version, (versiontag == "release" ? "" : QString("-") + versiontag));
  this->aboutInfo->setText(QString("<font>%1<br>Author: Chen Yiming<br>Other: This program is only for personal use.</font><br><font color=red>Any actions of plagiarism is prohibitted.</font>").arg(ver));

  this->stackedWidget->setCurrentIndex(0);
}
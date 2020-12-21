#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qstackedwidget.h>
#include <qlayout.h>
#include <qlistwidget.h>
#include <qsettings.h>
#include "keyboard.h"
#include "stageMode.h"
#include "endlessMode.h"
#include "fallingMode.h"
#include "settingsWidget.h"

QT_BEGIN_NAMESPACE

class Ui_mainWidgetClass
{
  public:
    QStackedWidget* stackedWidget;
    //menu
    QPushButton* startbtn, * aboutbtn, * tutorialbtn;
    QLabel* title;

    //mode select
    QPushButton* stagebtn, * endlessbtn, * fallingbtn, * backToMenu;
    QLabel* selectm_title;
    
    //stage select
    QLabel* stage_select_title;
    QListWidget* stage_list_widget;
    QPushButton* backToMode;

    //gamemode
    stageMode* stageModeWidget;
    endlessMode* endlessModeWidget;
    fallingMode* fallingModeWidget;

    //about
    QPushButton* aboutToMenu;
    QLabel* aboutInfo, * aboutTitle;

    //settings
    settingsWidget* settingWidget;

    void setupUi(QWidget* mainWidgetClass);
    void retranslateUi(QWidget* mainWidgetClass);
};

namespace Ui {
    class mainWidgetClass: public Ui_mainWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H

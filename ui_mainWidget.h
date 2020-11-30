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
#include "keyboard.h"

QT_BEGIN_NAMESPACE

class Ui_mainWidgetClass
{
  public:
    QStackedWidget* stackedWidget;
    //menu
    QPushButton* startbtn, * standingbtn;
    QLabel* title;

    //mode select
    QPushButton* stagebtn, * endlessbtn, * backToMenu;
    QLabel* selectm_title;
    
    //stage select
    QLabel* stage_select_title;
    QListWidget* stage_list_widget;
    QPushButton* backToMode;

    //game
    keyboard* keyboard_;
    QLabel* textZone[3], * health_label, * time_label, * score_label;

    void setupUi(QWidget* mainWidgetClass);
    void retranslateUi(QWidget* mainWidgetClass);
};

namespace Ui {
    class mainWidgetClass: public Ui_mainWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H

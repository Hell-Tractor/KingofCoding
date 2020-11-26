#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qstackedwidget.h>
#include <qlayout.h>

QT_BEGIN_NAMESPACE

class Ui_mainWidgetClass
{
  public:
    QStackedWidget* stackedWidget;
    QPushButton* startbtn, * standingbtn;
    QLabel* title;

    void setupUi(QWidget *mainWidgetClass)
    {
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
        //mainWidgetClass->setLayout(menulayout);
        menu->setLayout(menulayout);
        stackedWidget->addWidget(menu);

        //mode select

        /**********************************/
        retranslateUi(mainWidgetClass);
       
        QMetaObject::connectSlotsByName(mainWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *mainWidgetClass)
    {
        mainWidgetClass->setWindowTitle(QCoreApplication::translate("mainWidgetClass", "King of Coding", nullptr));
        this->title->setText("King of Coding");
        this->standingbtn->setText("Top Standings");
        this->startbtn->setText("Start Game");

        this->stackedWidget->setCurrentIndex(0);
    } // retranslateUi

};

namespace Ui {
    class mainWidgetClass: public Ui_mainWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H

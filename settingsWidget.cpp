#include "settingsWidget.h"

settingsWidget::settingsWidget(QWidget* parent) : QWidget(parent) {
  exitbtn = new QPushButton;
  continuebtn = new QPushButton;
  QVBoxLayout* settingsWidgetLayout = new QVBoxLayout;
  QHBoxLayout* exitContLayout = new QHBoxLayout;
  exitContLayout->addStretch();
  exitContLayout->addWidget(exitbtn);
  exitContLayout->addWidget(continuebtn);
  exitContLayout->addStretch();
  settingsWidgetLayout->addStretch();
  settingsWidgetLayout->addLayout(exitContLayout);
  settingsWidgetLayout->addStretch();
  this->setLayout(settingsWidgetLayout);
}

void settingsWidget::retranslateUi(QWidget* parent) {
  this->resize(parent->size());
  this->setObjectName("settingsWidget");
  this->setStyleSheet("#settingsWidget{background-color: rgba(0, 0, 0, 0.6);}");
  QString btnStyle = "color: #528B8B; background-color: rgba(220, 220, 220, 0.8); text-align: center;";
  QSize settingsBtnSize(220, 50);
  QFont settingsBtnFt;
  settingsBtnFt.setPointSize(20);
  settingsBtnFt.setFamily("Yu Gothic");
  settingsBtnFt.setBold(true);
  exitbtn->setStyleSheet(btnStyle);
  exitbtn->setFont(settingsBtnFt);
  exitbtn->setText("Exit");
  exitbtn->setFixedSize(settingsBtnSize);
  exitbtn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  continuebtn->setStyleSheet(btnStyle);
  continuebtn->setFont(settingsBtnFt);
  continuebtn->setText("Resume");
  continuebtn->setFixedSize(settingsBtnSize);
  continuebtn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  this->hide();
}

void settingsWidget::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void settingsWidget::showInterface() {
  int height = this->height();
  this->show();
  QPropertyAnimation* showAnimation = new QPropertyAnimation(this, "geometry");
  showAnimation->setDuration(1000);
  
  showAnimation->setStartValue(QRect(0, -height, this->width(), this->height()));
  showAnimation->setEndValue(QRect(0, 0, this->width(), this->height()));
  showAnimation->setEasingCurve(QEasingCurve::OutBounce);

  showAnimation->start();
}
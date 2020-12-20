#include "endlessMode.h"
#include <QtMath>
#include <QFile>
#include <stdexcept>
#include <QTime>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>

endlessMode::endlessMode(QWidget* parent) : gamemodeBase(parent) {
	/**************settings***************/
	QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
	INIT_HEALTH = settings->value("/endlessMode/init_health").toInt();
	INIT_TIME = settings->value("/endlessMode/init_time").toInt();
	HEALTH_REC_PERCENTAGE = settings->value("/endlessMode/health_rec_percentage").toInt();
	delete settings;
	/**************settings***************/

	//get keys list
	QFile keys("./keys");
	if (!keys.exists())
		throw std::runtime_error("File \"keys\" lost!");
	keys.open(QIODevice::ReadOnly | QIODevice::Text);
	keyList = keys.readAll().split('\n');

	//init
	i = 0;
	health = INIT_HEALTH;
	count = 0;
	score = 0.0;
	gameTime = INIT_TIME;
	isRec = false;
	
	//background
	bgLabel = new QLabel(this);

	gameLayout = new QVBoxLayout;

	//stage
	gameLayout->addStretch();
	textLayout = new QHBoxLayout;
	textLabel = new QLabel;
	textLayout->addStretch();
	textLayout->addWidget(textLabel);
	textLayout->addStretch();
	gameLayout->addLayout(textLayout);
	gameLayout->addStretch();
	//scoreboard
	scoreboardLayout = new QHBoxLayout;
	healthLabel = new QLabel;
	scoreLabel = new QLabel;
	timeLabel = new QLabel;
	scoreboardLayout->addWidget(healthLabel);
	scoreboardLayout->addStretch();
	scoreboardLayout->addWidget(scoreLabel);
	scoreboardLayout->addStretch();
	scoreboardLayout->addWidget(timeLabel);
	gameLayout->addLayout(scoreboardLayout);
	//keyboard
	keyBoard = new keyboard;
	gameLayout->addLayout(keyBoard->keyboard_layout);
	//apply
	this->setLayout(gameLayout);
}

void endlessMode::retranslateUi() {
	/**************settings***************/
	QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
	int bgWidgetWidth = settings->value("/mainWidget/width").toInt();
	int bgWidgetHeight = settings->value("/mainWidget/height").toInt();
	delete settings;
	/**************settings***************/

	//background
	bgLabel->setPixmap(QPixmap("./icons/endlessbg.png"));
	bgLabel->setScaledContents(true);
	bgLabel->resize(bgWidgetWidth, bgWidgetHeight);

	QFont scoreboardFt;
	scoreboardFt.setPointSize(15);
	scoreboardFt.setFamily("Comic Sans MS");
	this->healthLabel->setFont(scoreboardFt);
	this->healthLabel->setText(QString("<img src=\"./icons/health.png\">Health: ") + QString::number(INIT_HEALTH));
	this->timeLabel->setFont(scoreboardFt);
	this->timeLabel->setText(QString("<img src=\"./icons/time.png\">Time left: ") + QString::number(INIT_TIME / 1000.0, 'f', 2));
	this->timeLabel->setMinimumWidth(270);
	this->scoreLabel->setFont(scoreboardFt);
	this->scoreLabel->setText("<img src=\"./icons/score.png\">Score: ");
	QFont textZoneFt;
	textZoneFt.setPointSize(30);
	textZoneFt.setFamily("Consolas");
	this->textLabel->setFont(textZoneFt);
}

endlessMode::~endlessMode() {

}

void endlessMode::handleKeyPress(int key) {
	if (key == ' ') {
		if (this->currentState == gameState::RUNNING)
			this->currentState = gameState::WAITING;
		return;
	}
	//check gamestate
	if (this->currentState == gameState::WAITING)
		this->currentState = gameState::RUNNING;
	if (this->currentState != gameState::RUNNING)
		return;

	if (key == currentText[i].toUpper().unicode()) {
		//restore key color
		this->keyBoard->keys[key]->reset_color();
		//set text color
		this->textLabel->setText(QString("<font color=red>") + currentText.left(i + 1) +
														 "</font><font color=black>" + currentText.right(currentText.length() - i - 1) + "</font>");
		++i;
		//check legality of i
		if (i == currentText.length()) {
			//recover health
			if (this->isRec) {
				this->health++;
				this->healthLabel->setText(QString("<img src=\"./icons/health.png\">Health: ") + QString::number(health));
			}
			//add score
			score += gameTime / 1000.0 + (count / 5) * 1.2;
			count++;
			this->scoreLabel->setText(QString("<img src=\"./icons/score.png\">Score: ") + QString::number(score, 'f', 2));

			//reset timer
			gameTime = INIT_TIME - qLn(count + 1) * 1000;
			if (gameTime < 2000)
				gameTime = 2000;

			//get new key
			i = 0;
			currentText = getText();
			this->textLabel->setText(currentText);

			//generate health recovery
			if (qrand() % 100 <= HEALTH_REC_PERCENTAGE) {
				this->isRec = true;
				this->textLabel->setStyleSheet("background-image: url(./icons/HealthRecBg.png); background-repeat: repeat-xy;");
			} else {
				this->isRec = false;
				this->textLabel->setStyleSheet("");
			}
		}

		//change key color
		this->keyBoard->keys[currentText[i].toUpper().unicode()]->highLight();
	} else {
		//restore key color
		this->keyBoard->keys[currentText[i].toUpper().unicode()]->reset_color();

		this->wrongKeyWarning();

		//reset timer
		gameTime = INIT_TIME - qLn(count + 1) * 1000;
		if (gameTime < 2000)
			gameTime = 2000;

		//get new key
		i = 0;
		currentText = getText();
		this->textLabel->setText(currentText);
		
		//generate health recovery
		if (qrand() % 100 <= HEALTH_REC_PERCENTAGE) {
			this->isRec = true;
			this->textLabel->setStyleSheet("background-image: url(./icons/HealthRecBg.png); background-repeat: repeat-xy;");
		} else {
			this->isRec = false;
			this->textLabel->setStyleSheet("");
		}
	}
}

void endlessMode::init() {
	//init variable
	health = INIT_HEALTH;
	gameTime = INIT_TIME;
	score = 0.0;
	i = 0;
	count = 0;
	isRec = false;

	//init label
	this->timeLabel->setText(QString("<img src=\"./icons/time.png\">Time left: ") + QString::number(gameTime / 1000.0, 'f', 2));
	this->scoreLabel->setText("<img src=\"./icons/score.png\">Score: 0.00");
	this->healthLabel->setText(QString("<img src=\"./icons/health.png\">Health: ") + QString::number(health));

	//init random
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

	//show first word
	currentText = getText();
	this->textLabel->setText(currentText);

	//change key color
	this->keyBoard->keys[currentText[0].toUpper().unicode()]->highLight();

	this->currentState = gameState::WAITING;
}

void endlessMode::cleanUp(gameState state) {
	this->currentState = state;

	QMessageBox::information(this, "Game Over!", QString("You have lost all of your health\nYour final score: ") + QString::number(score), QMessageBox::Yes);
	
	this->textLabel->setText("");
	this->textLabel->setStyleSheet("");
	for (auto&& i : this->keyBoard->keys)
		i->reset_color();
	this->healthLabel->setStyleSheet("");

	emit exitGameInterface();

	currentState = gameState::UN_STARTED;
}

inline QString endlessMode::getText() {
	return keyList[qrand() % keyList.length()];
}

void endlessMode::wrongKeyWarning() {
	//update health
	--health;
	this->healthLabel->setText(QString("<img src=\"./icons/health.png\">Health: ") + QString::number(health));

	//warning
	this->healthLabel->setStyleSheet("background-color: red");

	//died
	if (health == 0) {
		this->cleanUp(gameState::LOSE);
		i = 0;
		return;
	}

	//dewarn
	QTimer::singleShot(100, Qt::TimerType::CoarseTimer, [=]() {
		this->healthLabel->setStyleSheet("");
										 });
}

void endlessMode::addTime(int delt) {
	this->gameTime += delt;
	if (this->gameTime <= 0) {
		this->wrongKeyWarning();
		if (this->currentState == gameState::RUNNING) {
			this->gameTime = INIT_TIME - qLn(count + 1) * 1000;
			if (this->gameTime < 2000)
				this->gameTime = 2000;
		}
	}
	this->timeLabel->setText(QString("<img src=\"./icons/time.png\">Time Left: ") + QString::number(this->gameTime / 1000.0, 'f', 2));
}
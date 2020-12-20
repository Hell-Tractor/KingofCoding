#include "stageMode.h"
#include <stdexcept>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>

stageMode::stageMode(QWidget* parent) : gamemodeBase(parent) {
	/**************settings***************/
	QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
	INIT_HEALTH = settings->value("/stageMode/init_health").toInt();
	LENGTH_PER_LABEL = settings->value("/stageMode/length_per_label").toInt();
	delete settings;
	/**************settings***************/

	//init
	i = 0;
	health = INIT_HEALTH;
	gametime = 0;
	gameText = nullptr;

	//widget
	gameLayout = new QVBoxLayout;

	//text show
	for (int i = 0; i < 3; ++i) {
		stageLayout[i] = new QHBoxLayout;
		textZone[i] = new QLabel;
		stageLayout[i]->addWidget(textZone[i]);
		stageLayout[i]->addStretch();
		gameLayout->addLayout(stageLayout[i]);
	}

	//scoreboard
	scoreboardLayout = new QHBoxLayout;
	healthLabel = new QLabel;
	timeLabel = new QLabel;
	scoreboardLayout->addWidget(healthLabel);
	scoreboardLayout->addStretch();
	scoreboardLayout->addWidget(timeLabel);
	gameLayout->addLayout(scoreboardLayout);

	gameLayout->addLayout(this->keyBoard->keyboard_layout);

	//apply
	this->setLayout(gameLayout);
}

void stageMode::retranslateUi() {
	QFont scoreBoardFt;
	scoreBoardFt.setPointSize(15);
	scoreBoardFt.setFamily("Comic Sans MS");
	this->healthLabel->setFont(scoreBoardFt);
	this->healthLabel->setText("<img src=\"./icons/health.png\">Health: ");
	this->timeLabel->setFont(scoreBoardFt);
	this->timeLabel->setText("<img src=\"./icons/time.png\">Time Used: 0.00");
	this->timeLabel->setMinimumWidth(270);
	QFont textZoneFt;
	textZoneFt.setPointSize(30);
	textZoneFt.setFamily("Consolas");
	for (auto&& i : this->textZone)
		i->setFont(textZoneFt);
}

stageMode::~stageMode() {
}

void stageMode::handleKeyPress(int key) {
	if (key == ' ') {
		if (this->currentState == gameState::RUNNING)
			this->currentState = gameState::WAITING;
		return;
	}

	if (this->currentState == gameState::WAITING)
		this->currentState = gameState::RUNNING;
	if (this->currentState != gameState::RUNNING)
		return;
	if (key == currentText[1][i].toUpper().unicode()) {
		//restore key color
		this->keyBoard->keys[key]->reset_color();
		//set text color
		this->textZone[1]->setText(QString("<font color=red>") + currentText[1].left(i + 1) +
															 "</font><font color=black>" + currentText[1].right(currentText[1].length() - i - 1) + "</font>");
		++i;
		//check legality of i
		if (i == currentText[1].length()) {
			i = 0;
			if (!this->updateText()) {
				this->cleanUp(gameState::WIN);
				return;
			}
		}

		//change key color
		this->keyBoard->keys[currentText[1][i].toUpper().unicode()]->highLight();
	} else
		this->wrongKeyWarning();
}

void stageMode::init() {
	//reset key color
	for (auto&& i : this->keyBoard->keys)
		i->reset_color();
	this->healthLabel->setStyleSheet("");

	//init variable
	i = 0;
	health = INIT_HEALTH;
	gametime = 0;

	//init label
	this->healthLabel->setText(QString("<img src=\"./icons/health.png\">Health: ") + QString::number(health));
	this->timeLabel->setText(QString("<img src=\"./icons/time.png\">Time Used: 0.00"));

	//set text file
	gameText = new QFile(QString("./stages/") + currentStage + ".stg");
	if (!gameText->open(QIODevice::ReadOnly | QIODevice::Text))
		throw std::logic_error("Unknown Stage");
	
	//update text;
	this->updateText();
	this->updateText();

	//change key color
	this->keyBoard->keys[currentText[1][0].toUpper().unicode()]->highLight();

	this->currentState = gameState::WAITING;
}

void stageMode::cleanUp(gameState state) {
	//update gamestate
	currentState = state;
	
	//show message and block current thread
	if (state == gameState::WIN)
		QMessageBox::information(this, "Congratulations!", QString("Your time: ") + QString::number(gametime / 1000.0, 'f', 2), QMessageBox::Yes);
	else if (state == gameState::LOSE)
		QMessageBox::information(this, "Game Over!", "You have lost all of your health", QMessageBox::Yes);
	else
		throw std::logic_error("Unknown GameState");

	for (auto&& i : this->textZone)
		i->setText("");
	for (auto&& i : this->keyBoard->keys)
		i->reset_color();
	this->healthLabel->setStyleSheet("");

	gameText->close();
	delete gameText;
	gameText = nullptr;

	emit exitGameInterface();

	currentState = gameState::UN_STARTED;
}

bool stageMode::updateText() {
	QString text;
	char c;
	//read text from file
	while (this->gameText->getChar(&c)) {
		if (isalpha(c))
			text.push_back(c);
		if (text.length() == LENGTH_PER_LABEL)
			break;
	}
	
	//update textZone
	for (int i = 0; i < 2; ++i) {
		this->textZone[i]->setText(this->textZone[i + 1]->text());
		this->currentText[i] = this->currentText[i + 1];
	}
	this->textZone[2]->setText(QString("<font color=black>") + text + "</font>");
	this->currentText[2] = text;
	return !currentText[1].isEmpty();
}

void stageMode::setTime(int time) {
	this->gametime = time;
	this->timeLabel->setText(QString("<img src=\"./icons/time.png\">Time Used: ") + QString::number(this->gametime / 1000.0, 'f', 2));
}

void stageMode::addTime(int delt) {
	this->gametime += delt;
	this->timeLabel->setText(QString("<img src=\"./icons/time.png\">Time Used: ") + QString::number(this->gametime / 1000.0, 'f', 2));
}

void stageMode::setCurrentStage(QString stage) {
	this->currentStage = stage;
}

void stageMode::wrongKeyWarning() {
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
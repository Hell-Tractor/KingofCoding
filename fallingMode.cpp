#include "fallingMode.h"
#include <qDebug>
#include <QTimer>
#include <QFile>
#include <stdexcept>
#include <QMessageBox>
#include <QTime>
#include <QSettings>

int fallingMode::GEN_INTERVAL = 20 * 8;
int fallingMode::KEY_SIZE = 65;
int fallingMode::node::HEIGHT_PER_MOVE = 4;

const QString fallingMode::PERFECT = "<font color=Gold>PERFECT</font>";
const QString fallingMode::GREAT = "<font color=LimeGreen>GREAT</font>";
const QString fallingMode::GOOD = "<font color=Turquoise>GOOD</font>";
const QString fallingMode::MISS = "<font color=DarkOrchid>MISS</font>";

const double fallingMode::PERFECT_BONUS = 2.0;
const double fallingMode::GREAT_BONUS = 1.5;
const double fallingMode::GOOD_BONUS = 1.0;
const double fallingMode::MISS_BONUS = 0.0;

const int fallingMode::MAX_HEALTH = 10;
const QString fallingMode::COLOR_SET[7] = {"OrangeRed", "Orange", "Gold", "LimeGreen", "DarkTurquoise", "DeepSkyBlue", "Purple"};

fallingMode::fallingMode(QWidget* parent) : gamemodeBase(parent) {
	//read settings
	QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
	SCORE_PER_LEVEL = settings->value("fallingMode/score_per_level").toInt();
	delete settings;

	//get letters list
	QFile letters("./letters");
	if (!letters.exists())
		throw std::runtime_error("File \"letters\" lost!");
	letters.open(QIODevice::ReadOnly | QIODevice::Text);
	allLetters = letters.readAll().split('\n');

	//init
	STAGE_WIDTH = 850;
	STAGE_HEIGHT = 500;
	JUDGING_LINE_HEIGHT = 420;
	LEVEL_LABEL_HEIGHT = 100;
	LETTERFT.setPointSize(30);
	LETTERFT.setFamily("Malgun Gothic");
	LETTERFT.setBold(true);
	currentX = 0;
	currentColor = 0;
	health = MAX_HEALTH;
	score = 0.0;
	hasHightLight = false;
	level = 1;

	gameLayout = new QVBoxLayout;
	
	//screen
	gameScreen = new QWidget;
	judgingLineLabel = new QLabel(gameScreen);
	levelLabel = new QLabel(gameScreen);
	gameLayout->addWidget(gameScreen);

	//scoreboard
	scoreboardLayout = new QHBoxLayout;
	healthLabel = new QLabel;
	statusLabel = new QLabel;
	scoreLabel = new QLabel;
	scoreboardLayout->addWidget(healthLabel);
	scoreboardLayout->addStretch();
	scoreboardLayout->addWidget(statusLabel);
	scoreboardLayout->addStretch();
	scoreboardLayout->addWidget(scoreLabel);
	gameLayout->addLayout(scoreboardLayout);

	//keyboard
	keyBoard = new keyboard;
	gameLayout->addLayout(keyBoard->keyboard_layout);

	this->setLayout(gameLayout);
}

void fallingMode::retranslateUi() {
	gameScreen->setFixedSize(STAGE_WIDTH, STAGE_HEIGHT);
	gameScreen->setStyleSheet("background-color: lightgrey");
	
	//level Label
	QFont levelFt;
	levelFt.setPointSize(50);
	levelFt.setFamily("MS UI Gothic");
	levelLabel->setFont(levelFt);
	levelLabel->setText("LEVEL 1");
	levelLabel->setStyleSheet("color: white");
	levelLabel->setAlignment(Qt::AlignCenter);
	levelLabel->setGeometry(0, LEVEL_LABEL_HEIGHT, STAGE_WIDTH, 200);

	//juding Line
	judgingLineLabel->setGeometry(0, JUDGING_LINE_HEIGHT, STAGE_WIDTH, 5);
	judgingLineLabel->setStyleSheet("background-color: black");

	//scoreboard
	QFont scoreboardFt;
	scoreboardFt.setFamily("Comic Sans MS");
	scoreboardFt.setPointSize(15);
	this->healthLabel->setFont(scoreboardFt);
	this->healthLabel->setText("<img src=\"./icons/health.png\">Health: " + QString::number(MAX_HEALTH));
	QFont statusFt;
	statusFt.setFamily("Lucida Handwriting");
	statusFt.setPointSize(15);
	this->statusLabel->setFont(statusFt);
	this->statusLabel->setText("");
	this->statusLabel->setFixedWidth(200);
	this->statusLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
	this->scoreLabel->setFont(scoreboardFt);
	this->scoreLabel->setText("<img src=\"./icons/score.png\">Score: 0.00");
}

fallingMode::~fallingMode() {

}

void fallingMode::handleKeyPress(int key) {
	if (key == ' ') {
		if (this->currentState == gameState::RUNNING) {
			this->levelLabel->setText("PAUSE");
			this->currentState = gameState::WAITING;
		}
		else if (this->currentState == gameState::WAITING) {
			this->levelLabel->setText(QString("LEVEL ") + QString::number(level));
			this->currentState = gameState::RUNNING;
		}
		return;
	}

	if (this->currentState != gameState::RUNNING)
		return;

	if (this->letters.empty())
		generateLetter();

	if (key == toupper(this->letters.front().c)) {
		//check accuracy
		double bonus;
		int delt = abs(letters.front().y + (KEY_SIZE >> 1) - JUDGING_LINE_HEIGHT);
		if (delt <= KEY_SIZE * 0.4) {
			this->statusLabel->setText(PERFECT);
			bonus = PERFECT_BONUS;
		} else if (delt <= KEY_SIZE * 0.8) {
			this->statusLabel->setText(GREAT);
			bonus = GREAT_BONUS;
		} else if (delt <= KEY_SIZE * 1.5) {
			this->statusLabel->setText(GOOD);
			bonus = GOOD_BONUS;
		} else {
			bonus = MISS_BONUS;
			handleMiss();
		}
		
		score += 1 * bonus;
		this->scoreLabel->setText(QString("<img src=\"./icons/score.png\">Score: ") + QString::number(score, 'f', 2));

		//level up
		if (score > level * SCORE_PER_LEVEL) {
			level++;
			node::HEIGHT_PER_MOVE++;

			for (int i = 0; i < 3; ++i)
				QTimer::singleShot(200 * i, Qt::TimerType::CoarseTimer, [=]() {
					this->levelLabel->setText("");
					QTimer::singleShot(100, Qt::TimerType::CoarseTimer, [=]() {
						this->levelLabel->setText("LEVEL UP!\nHEALTH +5");
														 });
													 });
			
			QTimer::singleShot(2000, Qt::TimerType::CoarseTimer, [=]() {
				this->levelLabel->setText(QString("LEVEL ") + QString::number(level));
												 });

			health += 5;
			this->healthLabel->setText("<img src=\"./icons/health.png\">Health: " + QString::number(health));
		}

		//restore key color
		this->keyBoard->keys[key]->reset_color();
		letters.front().toRemove = true;
		letters.front().label->hide();
		delete letters.front().label;
		letters.pop_front();

		//high light next key
		if (letters.empty())
			generateLetter();
		this->keyBoard->keys[toupper(letters.front().c)]->highLight();
	}
}

void fallingMode::init() {
	this->generateLetter();

	//init variable
	hasHightLight = false;
	score = 0.0;
	currentX = 0;
	currentColor = 0;
	health = MAX_HEALTH;
	level = 1;
	node::HEIGHT_PER_MOVE = 4;

	//init random
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

	//init label
	this->scoreLabel->setText("<img src=\"./icons/score.png\">Score: 0.00");
	this->statusLabel->setText("");
	this->healthLabel->setText("<img src=\"./icons/health.png\">Health: " + QString::number(MAX_HEALTH));
	this->levelLabel->setText("LEVEL 1");

	//reset keyboard
	for (auto&& i : keyBoard->keys)
		i->reset_color();

	this->currentState = gameState::WAITING;
}

void fallingMode::cleanUp(gameState state) {
	this->currentState = state;

	QMessageBox::information(this, "Game Over!", QString("You have lost all of your health\nYour final score: ") + QString::number(score), QMessageBox::Yes);
	
	this->statusLabel->setText("");
	for (auto&& i : this->keyBoard->keys)
		i->reset_color();

	emit exitGameInterface();

	QTimer::singleShot(100, Qt::TimerType::CoarseTimer, [=]() {
		while (!letters.empty()) {
			letters.front().label->hide();
			delete letters.front().label;
			letters.pop_front();
		}
										 });

	currentState = gameState::UN_STARTED;
}

void fallingMode::moveDown() {
	if (letters.empty())
		generateLetter();
	if (!hasHightLight) {
		this->keyBoard->keys[toupper(letters.front().c)]->highLight();
		hasHightLight = true;
	}
	for (auto i = letters.begin(); i != letters.end(); ++i) {
		i->y += node::HEIGHT_PER_MOVE;
		if (i->y + KEY_SIZE > STAGE_HEIGHT) {
			i->label->hide();
			delete i->label;
			i->toRemove = true;
			this->keyBoard->keys[toupper(i->c)]->reset_color();
			hasHightLight = false;
			handleMiss();
		} else {
			if (!hasHightLight) {
				this->keyBoard->keys[toupper(i->c)]->highLight();
				hasHightLight = true;
			}
			i->label->setGeometry(i->x, i->y, KEY_SIZE, KEY_SIZE);
		}
	}
	while (!letters.empty() && letters.front().toRemove)
		letters.pop_front();
}

void fallingMode::generateLetter() {
	QString currentLetter = allLetters[qrand() % allLetters.length()];
	letters.clear();
	int cnt = 0;
	for (auto&& i : currentLetter) {
		node temp;
		temp.c = i.unicode();
		temp.x = currentX;
		currentX += KEY_SIZE;
		if (currentX + KEY_SIZE > STAGE_WIDTH)
			currentX = 0;
		temp.y = 0 - KEY_SIZE * (cnt++) * 1.2;
		temp.color = 0x000000;
		temp.toRemove = false;
		temp.label = new QLabel(this->gameScreen);
		temp.label->setText(QString(temp.c));
		temp.label->setFont(LETTERFT);
		temp.label->setGeometry(temp.x, temp.y, KEY_SIZE, KEY_SIZE);
		temp.label->setAlignment(Qt::AlignCenter);
		temp.label->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
		temp.label->setStyleSheet(QString("background-color: transparent; color: ") + COLOR_SET[(currentColor = (currentColor + 1) % 7)]);
		temp.label->show();
		letters.push_back(temp);
	}
}

void fallingMode::handleMiss() {
	this->statusLabel->setText(MISS);

	health--;
	this->healthLabel->setText("<img src=\"./icons/health.png\">Health: " + QString::number(health));

	if (health == 0) {
		this->cleanUp(gameState::LOSE);
		return;
	}
}
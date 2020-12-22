#include "fallingMode.h"
#include <QTimer>
#include <QFile>
#include <stdexcept>
#include <QMessageBox>
#include <QTime>
#include <QSettings>
#include <QTextCharFormat>
#include <QPropertyAnimation>

int fallingMode::node::HEIGHT_PER_MOVE = 0;

const QString fallingMode::PERFECT = "<font color=Gold>PERFECT</font>";
const QString fallingMode::GREAT = "<font color=LimeGreen>GREAT</font>";
const QString fallingMode::GOOD = "<font color=Turquoise>GOOD</font>";
const QString fallingMode::MISS = "<font color=DarkOrchid>MISS</font>";

const double fallingMode::PERFECT_BONUS = 2.0;
const double fallingMode::GREAT_BONUS = 1.5;
const double fallingMode::GOOD_BONUS = 1.0;
const double fallingMode::MISS_BONUS = 0.0;

const QString fallingMode::COLOR_SET[7] = {"OrangeRed", "Orange", "Gold", "LimeGreen", "DarkTurquoise", "DeepSkyBlue", "Purple"};

fallingMode::fallingMode(QWidget* parent) : gamemodeBase(parent) {
	//read settings
	QSettings* settings = new QSettings("./settings.ini", QSettings::IniFormat);
	SCORE_PER_LEVEL = settings->value("/fallingMode/score_per_level").toInt();
	INIT_HEALTH = settings->value("/fallingMode/init_health").toInt();
	KEY_SIZE = settings->value("/fallingMode/key_size").toInt();
	INIT_SPEED = settings->value("/fallingMode/init_speed").toInt();
	HEALTH_REC = settings->value("/fallingMode/health_rec").toInt();
	delete settings;

	node::HEIGHT_PER_MOVE = INIT_SPEED;
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
	health = INIT_HEALTH;
	score = 0.0;
	hasHightLight = false;
	level = 1;

	gameLayout = new QVBoxLayout;
	
	//screen
	QHBoxLayout* screenLayout = new QHBoxLayout;
	screenLayout->addStretch();
	gameScreen = new QWidget;
	screenShield = new QWidget(gameScreen);
	hideOpacity = new QGraphicsOpacityEffect(this->screenShield);
	pauseLabel = new QLabel(screenShield);
	judgingLineLabel = new QLabel(gameScreen);
	levelLabel = new QLabel(gameScreen);
	screenLayout->addWidget(gameScreen);
	screenLayout->addStretch();
	gameLayout->addLayout(screenLayout);

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
	
	//shield screen
	screenShield->resize(gameScreen->size());
	screenShield->setStyleSheet("background-color: black");
	screenShield->raise();
	screenShield->hide();
	this->screenShield->setGraphicsEffect(hideOpacity);
	
	//level Label
	QFont levelFt;
	levelFt.setPointSize(50);
	levelFt.setFamily("MS UI Gothic");
	levelLabel->setFont(levelFt);
	levelLabel->setText("LEVEL 1");
	levelLabel->setStyleSheet("color: white");
	levelLabel->setAlignment(Qt::AlignCenter);
	levelLabel->setGeometry(0, LEVEL_LABEL_HEIGHT, STAGE_WIDTH, 200);

	//pause label
	pauseLabel->setFont(levelFt);
	pauseLabel->setText("PAUSE");
	pauseLabel->setStyleSheet("color: white");
	pauseLabel->setAlignment(Qt::AlignCenter);
	pauseLabel->setGeometry(0, LEVEL_LABEL_HEIGHT, STAGE_WIDTH, 200);

	//juding Line
	judgingLineLabel->setGeometry(0, JUDGING_LINE_HEIGHT, STAGE_WIDTH, 5);
	judgingLineLabel->setStyleSheet("background-color: black");

	//scoreboard
	QFont scoreboardFt;
	scoreboardFt.setFamily("Comic Sans MS");
	scoreboardFt.setPointSize(15);
	this->healthLabel->setFont(scoreboardFt);
	this->healthLabel->setText("<img src=\"./icons/health.png\">Health: " + QString::number(INIT_HEALTH));
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
		if (this->currentState == gameState::RUNNING)
			this->pauseGame();
		else if (this->currentState == gameState::WAITING)
			this->resumeGame();
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
						this->levelLabel->setText(QString("LEVEL UP!\nHEALTH +") + QString::number(HEALTH_REC));
														 });
													 });
			
			QTimer::singleShot(2000, Qt::TimerType::CoarseTimer, [=]() {
				this->levelLabel->setText(QString("LEVEL ") + QString::number(level));
												 });

			health += HEALTH_REC;
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
	health = INIT_HEALTH;
	level = 1;
	node::HEIGHT_PER_MOVE = INIT_SPEED;

	//init random
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

	//init label
	this->scoreLabel->setText("<img src=\"./icons/score.png\">Score: 0.00");
	this->statusLabel->setText("");
	this->healthLabel->setText("<img src=\"./icons/health.png\">Health: " + QString::number(INIT_HEALTH));
	this->levelLabel->setText("LEVEL 1");
	this->screenShield->hide();

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

	this->screenShield->hide();

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
		temp.label = new QTextEdit(this->gameScreen);
		QTextCharFormat format;
		format.setTextOutline(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
		temp.label->mergeCurrentCharFormat(format);
		temp.label->setDisabled(true);
		temp.label->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		temp.label->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		temp.label->setStyleSheet(QString("border: none; background-color: transparent; color: ") + COLOR_SET[(currentColor = (currentColor + 1) % 7)]);
		temp.label->setText(QString(temp.c));
		temp.label->setFont(LETTERFT);
		temp.label->setGeometry(temp.x, temp.y, KEY_SIZE, KEY_SIZE);
		temp.label->setAlignment(Qt::AlignCenter);
		temp.label->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
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

void fallingMode::pauseGame() {
	if (this->currentState == gameState::RUNNING) {
		this->currentState = gameState::WAITING;
		if (this->screenShield->isVisible())
			return;
		this->pauseLabel->setText("PAUSE");
		this->screenShield->show();
		this->screenShield->raise();
		this->hideOpacity->setOpacity(1);
	}
}

void fallingMode::resumeGame() {
	if (this->screenShield->isVisible()) {
		static bool isanimating = false;
		if (isanimating)
			return;
		isanimating = true;
		QPropertyAnimation* hideAnimation = new QPropertyAnimation(hideOpacity, "opacity");
		hideAnimation->setDuration(2000);
		hideAnimation->setStartValue(1);
		hideAnimation->setEndValue(0.6);
		hideAnimation->setEasingCurve(QEasingCurve::Linear);
		hideAnimation->start();
		for (int i = 0; i < 3; ++i)
			QTimer::singleShot(1000 * i, Qt::TimerType::PreciseTimer, [=]() {
				this->pauseLabel->setText(QString::number(3 - i));
												 });
		QTimer::singleShot(3000, Qt::TimerType::PreciseTimer, [&]() {
			this->screenShield->hide();
			this->currentState = gameState::RUNNING;
			isanimating = false;
											 });
	} else
		this->currentState = gameState::RUNNING;
}
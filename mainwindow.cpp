#include "mainwindow.h"
#include <QApplication>
#include <QTimer>
#include <QFont>
#include <QSpacerItem>
#include <QMessageBox>
#include <QTime>
#include <QScrollArea>
#include <QFrame>
#include <QScrollBar>
#include <QFile>
#include <QDebug>

BattleScreen::BattleScreen(QWidget *parent)
    : QWidget(parent), team(nullptr), battleSystem(nullptr)
{
    setupUI();

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &BattleScreen::updateUI);
    updateTimer->start(100);
}

void BattleScreen::setupUI()
{
    QLabel *backgroundLabel = new QLabel(this);
    backgroundLabel->setGeometry(0, 0, 1200, 700);

    QPixmap backgroundPixmap(":/img/img/battlescreenbackgroundimg2.jpg");

    if (!backgroundPixmap.isNull()) {
        backgroundPixmap = backgroundPixmap.scaled(1200, 700, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        backgroundLabel->setPixmap(backgroundPixmap);
    } else {
        backgroundLabel->setStyleSheet("background-color: #1a1a1a;");
        qWarning() << "Background image not found!";
    }

    backgroundLabel->lower();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 20, 30, 20);

    QVBoxLayout *teamLayout = new QVBoxLayout();
    teamLayout->setAlignment(Qt::AlignTop);

    QLabel *teamTitle = new QLabel("YOUR TEAM");
    teamTitle->setAlignment(Qt::AlignCenter);
    QFont teamFont = teamTitle->font();
    teamFont.setPointSize(18);
    teamFont.setBold(true);
    teamTitle->setFont(teamFont);
    teamTitle->setStyleSheet("color: blue;");
    teamLayout->addWidget(teamTitle);
    teamLayout->addSpacing(5);

    for (int i = 0; i < 3; ++i) {
        QFrame *charFrame = new QFrame();
        charFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
        charFrame->setLineWidth(1);
        charFrame->setFixedHeight(100);

        charFrame->setStyleSheet(
            "QFrame {"
            "    background-color: #95c8d8;"
            "}"
            );

        QHBoxLayout *charLayout = new QHBoxLayout(charFrame);
        charLayout->setContentsMargins(5, 5, 5, 5);
        charLayout->setSpacing(5);

        QLabel *turnIndicator = new QLabel("→");
        turnIndicator->setFixedWidth(20);
        turnIndicator->setAlignment(Qt::AlignCenter);
        QFont arrowFont = turnIndicator->font();
        arrowFont.setPointSize(12);
        arrowFont.setBold(true);
        turnIndicator->setFont(arrowFont);
        turnIndicator->setStyleSheet("color: gray;");
        turnIndicators.append(turnIndicator);
        charLayout->addWidget(turnIndicator);

        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(2);
        infoLayout->setContentsMargins(0, 0, 5, 0);

        QLabel *nameLabel = new QLabel("Character " + QString::number(i+1));
        QFont nameFont = nameLabel->font();
        nameFont.setPointSize(10);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);

        if (i == 1) {
            nameLabel->setStyleSheet("color: gold; font-weight: bold;");
        }
        if (i==0 || i==2) {
            nameLabel->setStyleSheet("color: white; font-weight: bold;");
        }

        characterNames.append(nameLabel);
        infoLayout->addWidget(nameLabel);

        QProgressBar *healthBar = new QProgressBar();
        healthBar->setRange(0, 100);
        healthBar->setTextVisible(true);
        healthBar->setFixedHeight(12);

        if (i == 1) {
            healthBar->setStyleSheet(
                "QProgressBar {"
                "    border: 1px solid grey;"
                "    border-radius: 2px;"
                "    text-align: center;"
                "    font-size: 8px;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: gold;"
                "    border-radius: 1px;"
                "}"
                );
        } else {
            healthBar->setStyleSheet(
                "QProgressBar {"
                "    border: 1px solid grey;"
                "    border-radius: 2px;"
                "    text-align: center;"
                "    font-size: 8px;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: #4CAF50;"
                "    border-radius: 1px;"
                "}"
                );
        }

        healthBars.append(healthBar);
        infoLayout->addWidget(healthBar);

        QProgressBar *ultimateBar = new QProgressBar();
        ultimateBar->setRange(0, 100);
        ultimateBar->setTextVisible(true);
        ultimateBar->setFixedHeight(12);
        ultimateBar->setStyleSheet(
            "QProgressBar {"
            "    border: 1px solid grey;"
            "    border-radius: 2px;"
            "    text-align: center;"
            "    font-size: 8px;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: #2196F3;"
            "    border-radius: 1px;"
            "}"
            );
        ultimateBars.append(ultimateBar);
        infoLayout->addWidget(ultimateBar);

        QLabel *statusLabel = new QLabel("Ready");
        statusLabel->setAlignment(Qt::AlignCenter);
        QFont statusFont = statusLabel->font();
        statusFont.setPointSize(8);
        statusLabel->setFont(statusFont);
        characterStatus.append(statusLabel);
        infoLayout->addWidget(statusLabel);

        charLayout->addLayout(infoLayout);

        QLabel *characterImage = new QLabel();
        characterImage->setFixedSize(150,138);
        characterImage->setAlignment(Qt::AlignTop);
        characterImage->setStyleSheet(
            "QLabel {"
            "    background-color: transparent;"
            "    background-position: relative;"
            "    margin-bottom:50%;"
            "    background-repeat: no-repeat;"
            "    padding-top: 5px;"
            "}"
            );

        characterImages.append(characterImage);
        characterImage->setAlignment(Qt::AlignTop);
        charLayout->addWidget(characterImage);

        teamLayout->addWidget(charFrame);
        teamLayout->addSpacing(5);
    }

    QVBoxLayout *controlsLayout = new QVBoxLayout();
    controlsLayout->setAlignment(Qt::AlignLeft);

    abilityPointsLabel = new QLabel("Ability Points: 3");
    QFont pointsFont = abilityPointsLabel->font();
    pointsFont.setPointSize(12);
    pointsFont.setBold(true);
    abilityPointsLabel->setFont(pointsFont);
    controlsLayout->addWidget(abilityPointsLabel);

    turnLabel = new QLabel("Current Turn: -");
    turnLabel->setFont(pointsFont);
    controlsLayout->addWidget(turnLabel);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(15);
    normalAttackButton = new QPushButton("NORMAL\nATTACK");
    abilityButton = new QPushButton("ABILITY");
    ultimateButton = new QPushButton("ULTIMATE");

    normalAttackButton->setFixedSize(120, 120);
    abilityButton->setFixedSize(120, 120);
    ultimateButton->setFixedSize(120, 120);

    QFont buttonFont = normalAttackButton->font();
    buttonFont.setPointSize(10);
    buttonFont.setBold(true);
    normalAttackButton->setFont(buttonFont);
    abilityButton->setFont(buttonFont);
    ultimateButton->setFont(buttonFont);

    normalAttackButton->setStyleSheet(
        "QPushButton {"
        "    background-image: url(:/img/img/NormalAtkImg.png);"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: none;"
        "    border-radius: 60px;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 10px;"
        "    padding-top: 80px;"
        "    background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "    border: 1px solid #dark-blue;"
        "    background-color: blue;"
        "}"
        "QPushButton:pressed {"
        "    border: 1px solid #dark-blue;"
        "    background-color: blue;"
        "}"
        "QPushButton:disabled {"
        "    opacity: 0.5;"
        "    color: #888888;"
        "}"
        );

    abilityButton->setStyleSheet(
        "QPushButton {"
        "    background-image: url(:/img/img/AbilityImg.png);"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: none;"
        "    border-radius: 60px;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 10px;"
        "    padding-top: 80px;"
        "    background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "    border: 1px solid #dark-blue;"
        "    background-color: blue;"
        "}"
        "QPushButton:pressed {"
        "    border: 1px solid #dark-blue;"
        "    background-color: blue;"
        "}"
        "QPushButton:disabled {"
        "    opacity: 0.5;"
        "    color: #888888;"
        "}"
        );

    ultimateButton->setStyleSheet(
        "QPushButton {"
        "    background-image: url(:/img/img/UltimateImg.png);"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: none;"
        "    border-radius: 60px;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 10px;"
        "    padding-top: 80px;"
        "    background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "    border: 1px solid #dark-blue;"
        "    background-color: blue;"
        "}"
        "QPushButton:pressed {"
        "    border: 1px solid #dark-blue;"
        "    background-color: blue;"
        "}"
        "QPushButton:disabled {"
        "    opacity: 0.5;"
        "    color: #888888;"
        "}"
        );

    connect(normalAttackButton, &QPushButton::clicked, this, &BattleScreen::useNormalAttack);
    connect(abilityButton, &QPushButton::clicked, this, &BattleScreen::useAbility);
    connect(ultimateButton, &QPushButton::clicked, this, &BattleScreen::useUltimate);

    buttonsLayout->addWidget(normalAttackButton);
    buttonsLayout->addWidget(abilityButton);
    buttonsLayout->addWidget(ultimateButton);

    controlsLayout->addLayout(buttonsLayout);
    teamLayout->addLayout(controlsLayout);

    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setAlignment(Qt::AlignCenter);

    QLabel *logTitle = new QLabel("BATTLE LOG");
    logTitle->setAlignment(Qt::AlignCenter);
    QFont logTitleFont = logTitle->font();
    logTitleFont.setPointSize(25);
    logTitleFont.setBold(true);
    logTitle->setStyleSheet("color:white");
    logTitle->setFont(logTitleFont);
    centerLayout->addWidget(logTitle);

    logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    logTextEdit->setWordWrapMode(QTextOption::WordWrap);
    logTextEdit->setFixedWidth(380);
    logTextEdit->setFixedHeight(450);

    logTextEdit->setStyleSheet(
        "QTextEdit {"
        "    border: 3px solid #666;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "    background-color: transparent;"
        "    font-family: monospace;"
        "    font-size: 10pt;"
        "    font-weight: bold;"
        "    color: white;"
        "}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: transparent;"
        "    width: 10px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #888;"
        "    border-radius: 5px;"
        "    min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #666;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );

    logTextEdit->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(logTextEdit);
    scrollArea->setStyleSheet("background-color:transparent");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedSize(400, 470);
    scrollArea->setFrameStyle(QFrame::NoFrame);

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    centerLayout->addWidget(scrollArea);

    QVBoxLayout *enemyLayout = new QVBoxLayout();
    enemyLayout->setAlignment(Qt::AlignTop);
    enemyLayout->setSpacing(10);

    QLabel *enemyTitle = new QLabel("7 DEADLY SINS");
    enemyTitle->setAlignment(Qt::AlignCenter);
    QFont enemyFont = enemyTitle->font();
    enemyFont.setPointSize(20);
    enemyFont.setBold(true);
    enemyTitle->setFont(enemyFont);
    enemyTitle->setStyleSheet("color: #8B0000;");
    enemyLayout->addWidget(enemyTitle);
    enemyLayout->addSpacing(5);

    enemyProgressLabel = new QLabel("Sin 1 of 7");
    enemyProgressLabel->setAlignment(Qt::AlignCenter);
    QFont progressFont = enemyProgressLabel->font();
    progressFont.setPointSize(12);
    progressFont.setBold(true);
    enemyProgressLabel->setFont(progressFont);
    enemyProgressLabel->setStyleSheet("color: #8B0000;");
    enemyLayout->addWidget(enemyProgressLabel);
    enemyLayout->addSpacing(5);

    QFrame *enemyFrame = new QFrame();
    enemyFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    enemyFrame->setLineWidth(2);
    enemyFrame->setFixedSize(320, 320);

    enemyFrame->setStyleSheet(
        "QFrame {"
        "    background-color: pink;"
        "}"
        );

    QVBoxLayout *enemyFrameLayout = new QVBoxLayout(enemyFrame);
    enemyFrameLayout->setAlignment(Qt::AlignCenter);
    enemyFrameLayout->setSpacing(5);
    enemyFrameLayout->setContentsMargins(10, 10, 10, 10);

    enemyTurnIndicator = new QLabel("←");
    enemyTurnIndicator->setFixedHeight(30);
    enemyTurnIndicator->setAlignment(Qt::AlignCenter);
    QFont enemyArrowFont = enemyTurnIndicator->font();
    enemyArrowFont.setPointSize(24);
    enemyArrowFont.setBold(true);
    enemyTurnIndicator->setFont(enemyArrowFont);
    enemyTurnIndicator->setStyleSheet("color: gray;");
    enemyFrameLayout->addWidget(enemyTurnIndicator);

    enemyNameLabel = new QLabel("Enemy");
    QFont enemyNameFont = enemyNameLabel->font();
    enemyNameFont.setPointSize(16);
    enemyNameFont.setBold(true);
    enemyNameLabel->setFont(enemyNameFont);
    enemyNameLabel->setStyleSheet("color: #8B0000;");
    enemyFrameLayout->addWidget(enemyNameLabel);
    enemyFrameLayout->addSpacing(5);

    enemyImageLabel = new QLabel();
    enemyImageLabel->setFixedSize(240, 228);
    enemyImageLabel->setAlignment(Qt::AlignCenter);
    enemyImageLabel->setStyleSheet(
        "QLabel {"
        "    margin-bottom:50%;"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        );

    QPixmap defaultEnemyPixmap(":/img/img/EnemyDefault.png");
    if (!defaultEnemyPixmap.isNull()) {
        QPixmap scaledPixmap = defaultEnemyPixmap.scaled(
            enemyImageLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );
        enemyImageLabel->setPixmap(scaledPixmap);
    } else {
        enemyImageLabel->setText("ENEMY\nIMAGE");
        enemyImageLabel->setAlignment(Qt::AlignCenter);
    }

    enemyFrameLayout->addWidget(enemyImageLabel);
    enemyFrameLayout->addSpacing(10);

    QLabel *enemyHealthText = new QLabel("Health:");
    QFont healthTextFont = enemyHealthText->font();
    healthTextFont.setPointSize(10);
    healthTextFont.setBold(true);
    enemyHealthText->setFont(healthTextFont);
    enemyFrameLayout->addWidget(enemyHealthText);

    enemyHealthBar = new QProgressBar();
    enemyHealthBar->setRange(0, 100);
    enemyHealthBar->setTextVisible(true);
    enemyHealthBar->setFixedSize(200, 20);
    enemyHealthBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid #666;"
        "    border-radius: 4px;"
        "    text-align: center;"
        "    font-weight: bold;"
        "    font-size: 9pt;"
        "    color: #333;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #f44336;"
        "    border-radius: 2px;"
        "}"
        );
    enemyFrameLayout->addWidget(enemyHealthBar);

    enemyLayout->addWidget(enemyFrame, 0, Qt::AlignCenter);

    mainLayout->addLayout(teamLayout, 2);
    mainLayout->addLayout(centerLayout, 2);
    mainLayout->addLayout(enemyLayout, 2);
}

void BattleScreen::initialize(Team *team, QVector<Enemy*> enemies, BattleSystem *battleSystem)
{
    cleanupConnections();

    this->team = team;
    this->enemies = enemies;
    this->battleSystem = battleSystem;

    if (battleSystem) {
        connect(battleSystem, &BattleSystem::battleStarted, this, &BattleScreen::onBattleStarted);
        connect(battleSystem, &BattleSystem::turnChanged, this, &BattleScreen::onTurnChanged);
        connect(battleSystem, &BattleSystem::abilityPointsChanged, this, &BattleScreen::onAbilityPointsChanged);
        connect(battleSystem, &BattleSystem::attackPerformed, this, &BattleScreen::onAttackPerformed);
        connect(battleSystem, &BattleSystem::enemyAttackPerformed, this, &BattleScreen::onEnemyAttackPerformed);
        connect(battleSystem, &BattleSystem::enemyChanged, this, &BattleScreen::onEnemyChanged);
        connect(battleSystem, &BattleSystem::enemyDefeated, this, &BattleScreen::onEnemyDefeated);
        connect(battleSystem, &BattleSystem::battleEnded, this, [this](bool victory) {
            addToLog(victory ? "=== VICTORY! ===" : "=== DEFEAT! ===");
            normalAttackButton->setEnabled(false);
            abilityButton->setEnabled(false);
            ultimateButton->setEnabled(false);
        });
    }

    if (team) {
        for (int i = 0; i < team->getSize(); ++i) {
            Character *character = team->getCharacterAt(i);
            if (character) {
                connect(character, &Character::healthChanged, this, [this, i]() {
                    updateCharacterUI(i);
                });
                connect(character, &Character::ultimateChanged, this, [this, i]() {
                    updateCharacterUI(i);
                });
                connect(character, &Character::damageMultiplierChanged, this, [this, i]() {
                    updateCharacterUI(i);
                });
            }
        }
    }

    if (battleSystem && battleSystem->getCurrentEnemy()) {
        Enemy* currentEnemy = battleSystem->getCurrentEnemy();
        connect(currentEnemy, &Enemy::healthChanged, this, [this](int health) {
            enemyHealthBar->setValue(health);
            enemyHealthBar->setFormat(QString("%1/%2 HP")
                                          .arg(health)
                                          .arg(enemyHealthBar->maximum()));
        });

        connect(currentEnemy, &Enemy::immunityChanged, this, [this](bool isImmune) {
            qDebug() << "Enemy immunity changed:" << isImmune;
            updateUI();
        });
    }

    updateUI();

    addToLog("=== BATTLE INITIALIZED ===");
    addToLog("Defeat all 7 deadly sins to win!");
    addToLog("Protect Dantus at all costs!");

    qDebug() << "BattleScreen initialized with" << (team ? team->getSize() : 0) << "characters and"
             << (enemies.size()) << "enemies";
}

void BattleScreen::updateUI()
{
    try {
        if (!team) return;

        if (battleSystem) {
            Enemy* currentEnemy = battleSystem->getCurrentEnemy();
            if (currentEnemy) {
                try {
                    enemyNameLabel->setText(QString("%1").arg(currentEnemy->getName()));

                    enemyHealthBar->setRange(0, currentEnemy->getMaxHealth());
                    enemyHealthBar->setValue(currentEnemy->getHealth());
                    enemyHealthBar->setFormat(QString("%1/%2 HP")
                                                  .arg(currentEnemy->getHealth())
                                                  .arg(currentEnemy->getMaxHealth()));

                    enemyProgressLabel->setText(QString("Sin %1 of 7")
                                                    .arg(battleSystem->getCurrentEnemyIndex() + 1));

                    try {
                        updateEnemyImage(currentEnemy->getName());
                    } catch (const std::exception& e) {
                        qWarning() << "Error updating enemy image:" << e.what();
                        enemyImageLabel->setText(currentEnemy->getName());
                    }

                    if (currentEnemy->isImmune()) {
                        enemyNameLabel->setStyleSheet("color: #9C27B0; font-weight: bold;");
                        enemyHealthBar->setStyleSheet(
                            "QProgressBar {"
                            "    border: 2px solid #9C27B0;"
                            "    border-radius: 4px;"
                            "    text-align: center;"
                            "    font-weight: bold;"
                            "    font-size: 9pt;"
                            "    color: #9C27B0;"
                            "    background-color: #F3E5F5;"
                            "}"
                            "QProgressBar::chunk {"
                            "    background-color: #9C27B0;"
                            "    border-radius: 2px;"
                            "}"
                            );
                        enemyProgressLabel->setText(QString("Sin %1 of 7 [IMMUNE]")
                                                        .arg(battleSystem->getCurrentEnemyIndex() + 1));
                        enemyProgressLabel->setStyleSheet("color: #9C27B0; font-weight: bold;");
                    } else {
                        enemyNameLabel->setStyleSheet("color: #8B0000; font-weight: bold;");

                        if (!currentEnemy->isAlive()) {
                            enemyHealthBar->setStyleSheet(
                                "QProgressBar {"
                                "    border: 2px solid #666;"
                                "    border-radius: 4px;"
                                "    text-align: center;"
                                "    font-weight: bold;"
                                "    font-size: 9pt;"
                                "    color: #333;"
                                "}"
                                "QProgressBar::chunk {"
                                "    background-color: #888;"
                                "    border-radius: 2px;"
                                "}"
                                );
                            enemyProgressLabel->setStyleSheet("color: #666; font-weight: bold;");
                        } else {
                            enemyHealthBar->setStyleSheet(
                                "QProgressBar {"
                                "    border: 2px solid #666;"
                                "    border-radius: 4px;"
                                "    text-align: center;"
                                "    font-weight: bold;"
                                "    font-size: 9pt;"
                                "    color: #333;"
                                "}"
                                "QProgressBar::chunk {"
                                "    background-color: #f44336;"
                                "    border-radius: 2px;"
                                "}"
                                );
                            enemyProgressLabel->setStyleSheet("color: #8B0000; font-weight: bold;");
                        }
                    }
                } catch (const std::exception& e) {
                    qCritical() << "Error updating enemy UI:" << e.what();
                }
            }
        }

        for (int i = 0; i < team->getSize(); ++i) {
            try {
                updateCharacterUI(i);
            } catch (const std::exception& e) {
                qWarning() << "Error updating character UI for index" << i << ":" << e.what();
            }
        }

        try {
            updateTurnIndicator();
        } catch (const std::exception& e) {
            qWarning() << "Error updating turn indicator:" << e.what();
        }

        if (battleSystem && battleSystem->isBattleActive()) {
            int currentTurn = battleSystem->getCurrentTurn();
            Character *currentChar = team->getCharacterAt(currentTurn);

            if (currentChar) {
                bool canUseAbility = battleSystem->getAbilityPoints() > 0 && currentChar->isAlive();
                bool canUseUltimate = currentChar->isUltimateReady() && currentChar->isAlive();

                normalAttackButton->setEnabled(currentChar->isAlive());
                abilityButton->setEnabled(canUseAbility);
                ultimateButton->setEnabled(canUseUltimate);

                turnLabel->setText(QString("Current Turn: %1").arg(currentChar->getName()));

                if (currentChar->hasDamageBuff()) {
                    turnLabel->setStyleSheet("color: #FF9800; font-weight: bold;");
                } else {
                    turnLabel->setStyleSheet("color: white; font-weight: bold;");
                }
            }
        } else {
            normalAttackButton->setEnabled(false);
            abilityButton->setEnabled(false);
            ultimateButton->setEnabled(false);
            turnLabel->setText("Battle Ended");
            turnLabel->setStyleSheet("color: #666;");
        }

        if (battleSystem) {
            abilityPointsLabel->setText(QString("Ability Points: %1").arg(battleSystem->getAbilityPoints()));

            if (battleSystem->getAbilityPoints() == 0) {
                abilityPointsLabel->setStyleSheet("color: gray; font-weight: bold;");
            } else if (battleSystem->getAbilityPoints() == 5) {
                abilityPointsLabel->setStyleSheet("color: gold; font-weight: bold;");
            } else {
                abilityPointsLabel->setStyleSheet("color: white; font-weight: bold;");
            }
        }

    } catch (const std::exception& e) {
        qCritical() << "Unhandled exception in updateUI:" << e.what();
    } catch (...) {
        qCritical() << "Unknown exception in updateUI";
    }
}

void BattleScreen::updateTurnIndicator()
{
    if (!battleSystem || !team) return;

    for (QLabel* indicator : turnIndicators) {
        indicator->setStyleSheet("color: gray;");
        indicator->setText("→");
    }
    enemyTurnIndicator->setStyleSheet("color: gray;");

    if (battleSystem->isBattleActive()) {
        int currentTurn = battleSystem->getCurrentTurn();

        if (currentTurn >= 0 && currentTurn < turnIndicators.size()) {
            turnIndicators[currentTurn]->setStyleSheet("color: green; font-weight: bold;");
            turnIndicators[currentTurn]->setText("⇒");
        }
    }
}

void BattleScreen::updateCharacterUI(int index)
{
    if (!team || index >= team->getSize()) return;

    Character *character = team->getCharacterAt(index);
    if (!character) return;

    if (index == 1) {
        characterNames[index]->setText("Dantus");
        if (character->hasDamageBuff()) {
            characterNames[index]->setStyleSheet("color: #FF9800; font-weight: bold;");
        } else {
            characterNames[index]->setStyleSheet("color: gold; font-weight: bold;");
        }
    } else {
        characterNames[index]->setText(character->getName());
        if (character->hasDamageBuff()) {
            characterNames[index]->setStyleSheet("color: #FF9800; font-weight: bold;");
        } else {
            characterNames[index]->setStyleSheet("color: green; font-weight: bold;");
        }
    }

    healthBars[index]->setRange(0, character->getMaxHealth());
    healthBars[index]->setValue(character->getHealth());

    ultimateBars[index]->setValue(static_cast<int>(character->getUltimateCharge()));

    QString status;
    QString style = "";

    if (!character->isAlive()) {
        status = "DEFEATED";
        style = "color: red; font-weight: bold;";
        healthBars[index]->setStyleSheet(
            "QProgressBar {"
            "    border: 2px solid grey;"
            "    border-radius: 5px;"
            "    text-align: center;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: #666;"
            "    border-radius: 3px;"
            "}"
            );
    }  else if (character->hasDamageBuff()) {
        status = QString("BUFFED! +%1% DMG").arg(character->getDamageMultiplier() - 100);
        style = "color: #FF9800; font-weight: bold; background-color: #FFF3E0; padding: 2px;";
    }  else if (character->isUltimateReady()) {
        status = "ULTIMATE READY!";
        style = "color: #9C27B0; font-weight: bold;";
        ultimateBars[index]->setStyleSheet(
            "QProgressBar {"
            "    border: 2px solid #9C27B0;"
            "    border-radius: 5px;"
            "    text-align: center;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: #9C27B0;"
            "    border-radius: 3px;"
            "}"
            );
    }  else {
        status = "";
        style = "";
    }

    characterStatus[index]->setText(status);
    characterStatus[index]->setStyleSheet(style);

    if (index == 1) {
        if (!character->isAlive()) {
            healthBars[index]->setStyleSheet(
                "QProgressBar {"
                "    border: 2px solid grey;"
                "    border-radius: 5px;"
                "    text-align: center;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: #666;"
                "    border-radius: 3px;"
                "}"
                );
        } else if (character->hasDamageBuff()) {
            healthBars[index]->setStyleSheet(
                "QProgressBar {"
                "    border: 2px solid #FF9800;"
                "    border-radius: 5px;"
                "    text-align: center;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: gold;"
                "    border-radius: 3px;"
                "}"
                );
        } else {
            healthBars[index]->setStyleSheet(
                "QProgressBar {"
                "    border: 2px solid gold;"
                "    border-radius: 5px;"
                "    text-align: center;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: gold;"
                "    border-radius: 3px;"
                "}"
                );
        }
    } else {
        if (!character->isAlive()) {
            healthBars[index]->setStyleSheet(
                "QProgressBar {"
                "    border: 2px solid grey;"
                "    border-radius: 5px;"
                "    text-align: center;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: #666;"
                "    border-radius: 3px;"
                "}"
                );
        } else if (character->hasDamageBuff()) {
            healthBars[index]->setStyleSheet(
                "QProgressBar {"
                "    border: 2px solid #FF9800;"
                "    border-radius: 5px;"
                "    text-align: center;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: #FF9800;"
                "    border-radius: 3px;"
                "}"
                );
        } else {
            healthBars[index]->setStyleSheet(
                "QProgressBar {"
                "    border: 2px solid #4CAF50;"
                "    border-radius: 5px;"
                "    text-align: center;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: #4CAF50;"
                "    border-radius: 3px;"
                "}"
                );
        }
    }

    if (!character->isUltimateReady()) {
        ultimateBars[index]->setStyleSheet(
            "QProgressBar {"
            "    border: 2px solid grey;"
            "    border-radius: 5px;"
            "    text-align: center;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: #2196F3;"
            "    border-radius: 3px;"
            "}"
            );
    }

    if (index < characterImages.size()) {
        QString characterType = character->getName();
        QString imagePath;

        if (characterType == "Gabriel") {
            imagePath = ":/img/img/heroimg1.png";
        } else if (characterType == "Dantus") {
            imagePath = ":/img/img/heroimg2.png";
        } else if (characterType == "Cain") {
            imagePath = ":/img/img/heroimg3.png";
        } else if (characterType == "Magdalene") {
            imagePath = ":/img/img/heroimg4.png";
        } else if (characterType == "Esther"){
            imagePath = ":/img/img/heroimg5.png";
        }

        QPixmap pixmap;
        if (QFile::exists(imagePath)) {
            pixmap.load(imagePath);
        }

        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(
                characterImages[index]->width(),
                characterImages[index]->height(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                );

            characterImages[index]->setPixmap(scaledPixmap);
            characterImages[index]->setScaledContents(false);
            characterImages[index]->setAlignment(Qt::AlignTop | Qt::AlignCenter);
        } else {
            characterImages[index]->setText(characterType + "\n" + character->getName());
            characterImages[index]->setAlignment(Qt::AlignTop | Qt::AlignCenter);
        }
    }
}

void BattleScreen::updateEnemyImage(const QString &enemyName)
{
    try {
        if (!enemyImageLabel) {
            qWarning() << "enemyImageLabel is null!";
            return;
        }

        QString imagePath;

        if (enemyName == "Gluttony") {
            imagePath = ":/img/img/gluttonyimg.png";
        } else if (enemyName == "Lust") {
            imagePath = ":/img/img/lustimg.png";
        } else if (enemyName == "Greed") {
            imagePath = ":/img/img/greedimg.png";
        } else if (enemyName == "Sloth") {
            imagePath = ":/img/img/slothimg.png";
        } else if (enemyName == "Wrath") {
            imagePath = ":/img/img/wrathimg.png";
        } else if (enemyName == "Envy") {
            imagePath = ":/img/img/envyimg.png";
        } else if (enemyName == "Pride") {
            imagePath = ":/img/img/prideimg.png";
        } else {
            imagePath = ":/img/img/sundaypfp2.png";
        }

        QPixmap enemyPixmap;
        if (QFile::exists(imagePath)) {
            enemyPixmap.load(imagePath);
        } else {
            qWarning() << "Image file does not exist:" << imagePath;
        }

        if (!enemyPixmap.isNull()) {
            QPixmap scaledPixmap = enemyPixmap.scaled(
                enemyImageLabel->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                );
            enemyImageLabel->setPixmap(scaledPixmap);
            enemyImageLabel->setScaledContents(false);
        } else {
            enemyImageLabel->setText(enemyName + "\n[IMAGE]");
            enemyImageLabel->setAlignment(Qt::AlignCenter);
            enemyImageLabel->setStyleSheet(
                "QLabel {"
                "    font-weight: bold;"
                "    color: #666;"
                "    background-color: #f0f0f0;"
                "    border: 1px solid #ccc;"
                "    border-radius: 5px;"
                "    padding: 5px;"
                "}"
                );
        }

    } catch (const std::exception& e) {
        qCritical() << "Error in updateEnemyImage:" << e.what();
        if (enemyImageLabel) {
            enemyImageLabel->setText("ERROR\n" + enemyName);
        }
    } catch (...) {
        qCritical() << "Unknown error in updateEnemyImage";
    }
}

void BattleScreen::addToLog(const QString &message)
{
    if (!logTextEdit) return;

    QString currentText = logTextEdit->toPlainText();
    QString timeStamp = QTime::currentTime().toString("[hh:mm:ss] ");
    QString formattedMessage = timeStamp + message;

    if (!currentText.isEmpty()) {
        formattedMessage = "\n" + formattedMessage;
    }

    logTextEdit->append(formattedMessage);

    QTextCursor cursor = logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    logTextEdit->setTextCursor(cursor);

    QScrollBar *verticalScrollBar = logTextEdit->verticalScrollBar();
    if (verticalScrollBar) {
        verticalScrollBar->setValue(verticalScrollBar->maximum());
    }
}

void BattleScreen::useNormalAttack()
{
    if (battleSystem && battleSystem->isBattleActive()) {
        battleSystem->performAttack("Normal", battleSystem->getCurrentTurn());
    }
}

void BattleScreen::useAbility()
{
    if (battleSystem && battleSystem->isBattleActive()) {
        battleSystem->performAttack("Ability", battleSystem->getCurrentTurn());
    }
}

void BattleScreen::useUltimate()
{
    if (battleSystem && battleSystem->isBattleActive()) {
        battleSystem->performAttack("Ultimate", battleSystem->getCurrentTurn());
    }
}

void BattleScreen::onBattleStarted()
{
    addToLog("Battle started! Your team attacks first.");
}

void BattleScreen::onTurnChanged(int newTurn)
{
    Character *character = team->getCharacterAt(newTurn);
    if (character) {
        turnLabel->setText(QString("Current Turn: %1").arg(character->getName()));
    }
}

void BattleScreen::onAbilityPointsChanged(int newPoints)
{
    abilityPointsLabel->setText(QString("Ability Points: %1").arg(newPoints));
}

void BattleScreen::onAttackPerformed(const QString &message)
{
    addToLog(message);
}

void BattleScreen::onEnemyAttackPerformed(const QString &message)
{
    enemyTurnIndicator->setStyleSheet("color: red; font-weight: bold;");
    enemyTurnIndicator->setText("⇐");

    for (QLabel* indicator : turnIndicators) {
        indicator->setStyleSheet("color: gray;");
        indicator->setText("→");
    }

    addToLog(message);

    QTimer::singleShot(1500, this, [this]() {
        if (battleSystem && battleSystem->isBattleActive()) {
            enemyTurnIndicator->setStyleSheet("color: gray;");
            enemyTurnIndicator->setText("←");
            updateTurnIndicator();
        }
    });
}







MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), playerTeam(new Team()), battleSystem(nullptr)
{
    try {
        qDebug() << "MainWindow constructor called";

        setWindowTitle("ZEZ PROJEKT");
        setFixedSize(1200, 700);

        stackedWidget = new QStackedWidget(this);
        setCentralWidget(stackedWidget);

        try {
            createCharacters();
        } catch (const std::exception& e) {
            qCritical() << "Error creating characters:" << e.what();
            QMessageBox::warning(this, "Warning",
                                 QString("Error creating characters: %1\nUsing default characters.").arg(e.what()));
            createDefaultCharacters();
        }

        try {
            createEnemies();
        } catch (const std::exception& e) {
            qCritical() << "Error creating enemies:" << e.what();
            QMessageBox::warning(this, "Warning",
                                 QString("Error creating enemies: %1").arg(e.what()));
        }

        try {
            setupMainMenu();
            setupDifficultySelect();
            setupCharacterSelect();
        } catch (const std::exception& e) {
            qCritical() << "Error setting up UI:" << e.what();
            throw;
        }

        battleScreen = new BattleScreen();
        stackedWidget->addWidget(battleScreen);

        try {
            setupVictoryScreen();
            setupDefeatScreen();
        } catch (const std::exception& e) {
            qCritical() << "Error setting up result screens:" << e.what();
        }

        qDebug() << "MainWindow constructor finished";

    } catch (const std::exception& e) {
        qCritical() << "Fatal error in MainWindow constructor:" << e.what();
        QMessageBox::critical(this, "Fatal Error",
                              QString("Failed to initialize application: %1").arg(e.what()));
        throw;
    }
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destructor called";

    if (battleScreen) {
        battleScreen->cleanupConnections();
        delete battleScreen;
    }

    delete playerTeam;

    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    if (battleSystem) {
        delete battleSystem;
    }

    for (Character *character : availableCharacters) {
        delete character;
    }
}

void MainWindow::createCharacters()
{
    qDebug() << "Creating characters...";

    for (Character *character : availableCharacters) {
        if (character) {
            delete character;
        }
    }
    availableCharacters.clear();

    availableCharacters.append(new HealerCharacter("Dantus", 200, 25, 50, 100));
    availableCharacters.append(new DPSCharacter("Gabriel", 150, 25, 40, 100));
    availableCharacters.append(new ShielderCharacter("Cain", 150, 20, 50, 100));
    availableCharacters.append(new AmplifierCharacter("Magdalene", 110, 15, 10, 25));
    availableCharacters.append(new DPSCharacter("Esther", 120, 30, 45, 110));

    qDebug() << "Created" << availableCharacters.size() << "characters";
    for (int i = 0; i < availableCharacters.size(); ++i) {
        qDebug() << "Character" << i << ":" << availableCharacters[i]->getName();
    }
}

void MainWindow::createEnemies()
{
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    enemies.append(new GluttonyEnemy());
    enemies.append(new LustEnemy());
    enemies.append(new GreedEnemy());
    enemies.append(new SlothEnemy());
    enemies.append(new WrathEnemy());
    enemies.append(new EnvyEnemy());
    enemies.append(new PrideEnemy());

    qDebug() << "Created" << enemies.size() << "enemies";
}

void MainWindow::setupMainMenu()
{
    mainMenuScreen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(mainMenuScreen);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel("TURN-BASED GAME");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QPushButton *startButton = new QPushButton("START GAME");
    QPushButton *exitButton = new QPushButton("EXIT");

    startButton->setFixedSize(200, 50);
    exitButton->setFixedSize(200, 50);

    QFont buttonFont = startButton->font();
    buttonFont.setPointSize(14);
    startButton->setFont(buttonFont);
    exitButton->setFont(buttonFont);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::goToDifficultySelect);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitGame);

    layout->addWidget(titleLabel);
    layout->addSpacing(50);
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(exitButton, 0, Qt::AlignCenter);

    stackedWidget->addWidget(mainMenuScreen);
}

void MainWindow::setupDifficultySelect()
{
    difficultyScreen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(difficultyScreen);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel("SELECT DIFFICULTY");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QPushButton *easyButton = new QPushButton("EASY");
    QPushButton *mediumButton = new QPushButton("MEDIUM");
    QPushButton *hardButton = new QPushButton("HARD");
    QPushButton *backButton = new QPushButton("BACK");

    easyButton->setFixedSize(200, 50);
    mediumButton->setFixedSize(200, 50);
    hardButton->setFixedSize(200, 50);
    backButton->setFixedSize(200, 50);

    QFont buttonFont = easyButton->font();
    buttonFont.setPointSize(14);
    easyButton->setFont(buttonFont);
    mediumButton->setFont(buttonFont);
    hardButton->setFont(buttonFont);
    backButton->setFont(buttonFont);

    connect(easyButton, &QPushButton::clicked, this, &MainWindow::setDifficultyEasy);
    connect(mediumButton, &QPushButton::clicked, this, &MainWindow::setDifficultyMedium);
    connect(hardButton, &QPushButton::clicked, this, &MainWindow::setDifficultyHard);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::goToMainMenu);

    layout->addWidget(titleLabel);
    layout->addSpacing(50);
    layout->addWidget(easyButton, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(mediumButton, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(hardButton, 0, Qt::AlignCenter);
    layout->addSpacing(40);
    layout->addWidget(backButton, 0, Qt::AlignCenter);

    stackedWidget->addWidget(difficultyScreen);
}

void MainWindow::setupCharacterSelect()
{
    qDebug() << "Setting up character select screen";

    characterSelectScreen = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(characterSelectScreen);

    QLabel *titleLabel = new QLabel("SELECT 3 CHARACTERS");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    selectedCountLabel = new QLabel("Selected: 0/3");
    selectedCountLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(selectedCountLabel);
    mainLayout->addSpacing(20);

    enemyProgressLabel = new QLabel("Enemies: 7 total");
    enemyProgressLabel->setAlignment(Qt::AlignCenter);
    enemyProgressLabel->setStyleSheet("color: #8B0000; font-weight: bold;");

    QHBoxLayout *holyOneLayout = new QHBoxLayout();
    holyOneLayout->setAlignment(Qt::AlignCenter);

    QPushButton *holyOneButton = new QPushButton("Dantus (Healer)\nHP: 200 | PROTECT AT ALL COSTS!");
    holyOneButton->setFixedSize(400, 100);
    holyOneButton->setStyleSheet(
        "QPushButton {"
        "    background-color: gold;"
        "    border: 3px solid #DAA520;"
        "    border-radius: 10px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "    padding: 10px;"
        "    color: #8B4513;"
        "}"
        "QPushButton:hover {"
        "    background-color: #FFD700;"
        "    border: 3px solid #B8860B;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #DAA520;"
        "}"
        );

    connect(holyOneButton, &QPushButton::clicked, this, &MainWindow::selectHolyOne);
    holyOneLayout->addWidget(holyOneButton);

    mainLayout->addLayout(holyOneLayout);
    mainLayout->addSpacing(30);

    QLabel *otherCharsLabel = new QLabel("Other Characters (Select 2):");
    otherCharsLabel->setAlignment(Qt::AlignCenter);
    QFont otherFont = otherCharsLabel->font();
    otherFont.setPointSize(16);
    otherFont.setBold(true);
    otherCharsLabel->setFont(otherFont);
    mainLayout->addWidget(otherCharsLabel);
    mainLayout->addSpacing(10);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setHorizontalSpacing(30);
    gridLayout->setVerticalSpacing(50);

    QPushButton *char1Button = new QPushButton("Gabriel (DPS)\nHP: 150 | DMG: 25/40/100");
    QPushButton *char2Button = new QPushButton("Cain (Shielder)\nHP: 150 | DMG: 20/0/0");
    QPushButton *char3Button = new QPushButton("Magdalene (Amplifier)\nHP: 110 | DMG: 15/0/0");
    QPushButton *char4Button = new QPushButton("Esther (DPS)\nHP: 120 | DMG: 30/45/110");

    char1Button->setFixedSize(250, 70);
    char2Button->setFixedSize(250, 70);
    char3Button->setFixedSize(250, 70);
    char4Button->setFixedSize(250, 70);

    QString normalButtonStyle =
        "QPushButton {"
        "    background-color: #f0f0f0;"
        "    border: 2px solid #cccccc;"
        "    border-radius: 8px;"
        "    font-weight: bold;"
        "    padding: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e0e0e0;"
        "    border: 2px solid #aaaaaa;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #d0d0d0;"
        "}";

    char1Button->setStyleSheet(normalButtonStyle);
    char2Button->setStyleSheet(normalButtonStyle);
    char3Button->setStyleSheet(normalButtonStyle);
    char4Button->setStyleSheet(normalButtonStyle);

    connect(char1Button, &QPushButton::clicked, this, &MainWindow::selectGabriel);
    connect(char2Button, &QPushButton::clicked, this, &MainWindow::selectCain);
    connect(char3Button, &QPushButton::clicked, this, &MainWindow::selectMagdalene);
    connect(char4Button, &QPushButton::clicked, this, &MainWindow::selectEsther);

    gridLayout->addWidget(char1Button, 0, 0, Qt::AlignCenter);
    gridLayout->addWidget(char2Button, 0, 1, Qt::AlignCenter);
    gridLayout->addWidget(char3Button, 1, 0, Qt::AlignCenter);
    gridLayout->addWidget(char4Button, 1, 1, Qt::AlignCenter);

    mainLayout->addLayout(gridLayout);
    mainLayout->addSpacing(30);

    QLabel *teamLabel = new QLabel("Your Team:");
    teamLabel->setAlignment(Qt::AlignCenter);
    QFont teamFont = teamLabel->font();
    teamFont.setPointSize(16);
    teamFont.setBold(true);
    teamLabel->setFont(teamFont);
    mainLayout->addWidget(teamLabel);

    QLabel *teamNoteLabel = new QLabel("Dantus must be in the middle");
    teamNoteLabel->setAlignment(Qt::AlignCenter);
    teamNoteLabel->setStyleSheet("color: #8B4513; font-weight: bold; font-size: 12px;");
    mainLayout->addWidget(teamNoteLabel);
    mainLayout->addSpacing(10);

    QHBoxLayout *teamLayout = new QHBoxLayout();
    teamLayout->setSpacing(20);
    teamLayout->setAlignment(Qt::AlignCenter);

    teamSlotLabels.clear();
    for (int i = 0; i < 3; ++i) {
        QLabel *slotLabel = new QLabel();
        slotLabel->setAlignment(Qt::AlignCenter);
        slotLabel->setFixedSize(150, 80);

        if (i == 1) {
            slotLabel->setText("Dantus\n[Required]");
            slotLabel->setStyleSheet(
                "QLabel {"
                "    border: 3px solid gold;"
                "    border-radius: 5px;"
                "    padding: 10px;"
                "    background-color: #FFF8DC;"
                "    font-weight: bold;"
                "    color: #8B7355;"
                "}"
                );
        } else {
            slotLabel->setText("[Empty Slot]");
            slotLabel->setStyleSheet(
                "QLabel {"
                "    border: 2px dashed gray;"
                "    border-radius: 5px;"
                "    padding: 10px;"
                "    background-color: #f8f8f8;"
                "    color: #888888;"
                "}"
                );
        }

        teamLayout->addWidget(slotLabel);
        teamSlotLabels.append(slotLabel);
    }

    mainLayout->addLayout(teamLayout);

    QLabel *warningLabel = new QLabel("GAME OVER IF DANTUS DIES!");
    warningLabel->setAlignment(Qt::AlignCenter);
    warningLabel->setStyleSheet(
        "QLabel {"
        "    color: #FF0000;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "    background-color: #FFF0F0;"
        "    border: 2px solid #FF0000;"
        "    border-radius: 5px;"
        "    padding: 8px;"
        "    margin: 10px;"
        "}"
        );
    mainLayout->addWidget(warningLabel);
    mainLayout->addSpacing(20);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(30);
    buttonLayout->setAlignment(Qt::AlignCenter);

    QPushButton *backButton = new QPushButton("← BACK");
    QPushButton *clearButton = new QPushButton("CLEAR TEAM");
    QPushButton *startButton = new QPushButton("START BATTLE ▶");

    backButton->setFixedSize(150, 45);
    clearButton->setFixedSize(150, 45);
    startButton->setFixedSize(180, 50);

    QFont buttonFont = startButton->font();
    buttonFont.setPointSize(12);
    buttonFont.setBold(true);

    backButton->setFont(buttonFont);
    clearButton->setFont(buttonFont);
    startButton->setFont(buttonFont);

    backButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #e0e0e0;"
        "    border: 2px solid #aaaaaa;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #d0d0d0;"
        "}"
        );

    clearButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #ffcccc;"
        "    border: 2px solid #ff6666;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #ffbbbb;"
        "}"
        );

    startButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    border: 2px solid #2E7D32;"
        "    border-radius: 5px;"
        "    color: white;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #cccccc;"
        "    border: 2px solid #999999;"
        "    color: #666666;"
        "}"
        );

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startBattle);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::goToDifficultySelect);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearTeam);

    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(startButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    stackedWidget->addWidget(characterSelectScreen);
    qDebug() << "Character select screen setup complete";
}

void MainWindow::setupVictoryScreen()
{
    victoryScreen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(victoryScreen);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel("VICTORY!");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(48);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: gold;");

    QLabel *messageLabel = new QLabel("Congratulations!");
    messageLabel->setAlignment(Qt::AlignCenter);
    QFont messageFont = messageLabel->font();
    messageFont.setPointSize(18);
    messageLabel->setFont(messageFont);

    QPushButton *menuButton = new QPushButton("MAIN MENU");
    menuButton->setFixedSize(200, 50);
    QFont buttonFont = menuButton->font();
    buttonFont.setPointSize(16);
    menuButton->setFont(buttonFont);

    connect(menuButton, &QPushButton::clicked, this, &MainWindow::goToMainMenu);

    layout->addWidget(titleLabel);
    layout->addSpacing(30);
    layout->addWidget(messageLabel);
    layout->addSpacing(50);
    layout->addWidget(menuButton, 0, Qt::AlignCenter);

    stackedWidget->addWidget(victoryScreen);
}

void MainWindow::setupDefeatScreen()
{
    defeatScreen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(defeatScreen);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel("DEFEAT!");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(48);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: red;");

    QLabel *messageLabel = new QLabel("Dantus has fallen...");
    messageLabel->setAlignment(Qt::AlignCenter);
    QFont messageFont = messageLabel->font();
    messageFont.setPointSize(18);
    messageLabel->setFont(messageFont);

    QPushButton *menuButton = new QPushButton("MAIN MENU");
    menuButton->setFixedSize(200, 50);
    QFont buttonFont = menuButton->font();
    buttonFont.setPointSize(16);
    menuButton->setFont(buttonFont);

    connect(menuButton, &QPushButton::clicked, this, &MainWindow::goToMainMenu);

    layout->addWidget(titleLabel);
    layout->addSpacing(30);
    layout->addWidget(messageLabel);
    layout->addSpacing(50);
    layout->addWidget(menuButton, 0, Qt::AlignCenter);

    stackedWidget->addWidget(defeatScreen);
}




void MainWindow::selectHolyOne()
{
    qDebug() << "Selecting Dantus";

    if (selectedCharacters.size() >= 3) {
        QMessageBox::information(this, "Team Full", "You already have 3 characters selected! Clear team to select new ones.");
        return;
    }

    for (Character* character : selectedCharacters) {
        if (character == availableCharacters[0]) {
            QMessageBox::information(this, "Already Selected", "Dantus is already in your team!");
            return;
        }
    }

    if (selectedCharacters.size() != 1) {
        QMessageBox::information(this, "Invalid Selection", "Select one other character first, then Dantus!");
        return;
    }

    selectedCharacters.append(availableCharacters[0]);
    playerTeam->addCharacter(availableCharacters[0]);
    updateSelectedCount();
    qDebug() << "Dantus selected";
}

void MainWindow::selectGabriel()
{
    selectOtherCharacter(1);
}

void MainWindow::selectCain()
{
    selectOtherCharacter(2);
}

void MainWindow::selectMagdalene()
{
    selectOtherCharacter(3);
}

void MainWindow::selectEsther()
{
    selectOtherCharacter(4);
}

void MainWindow::selectOtherCharacter(int index)
{
    qDebug() << "Selecting character at index" << index;

    if (selectedCharacters.size() >= 3) {
        QMessageBox::information(this, "Team Full", "You already have 3 characters selected! Clear team to select new ones.");
        return;
    }

    for (Character* character : selectedCharacters) {
        if (character == availableCharacters[index]) {
            QMessageBox::information(this, "Already Selected", "This character is already in your team!");
            return;
        }
    }

    if (selectedCharacters.size() == 0) {
        selectedCharacters.append(availableCharacters[index]);
        playerTeam->addCharacter(availableCharacters[index]);
        updateSelectedCount();
        qDebug() << "First character selected:" << availableCharacters[index]->getName();
    }
    else if (selectedCharacters.size() == 2) {
        selectedCharacters.append(availableCharacters[index]);
        playerTeam->addCharacter(availableCharacters[index]);
        updateSelectedCount();
        qDebug() << "Third character selected:" << availableCharacters[index]->getName();
    }
    else {
        QMessageBox::information(this, "Invalid Selection", "Select Dantus as your second character!");
    }
}

void MainWindow::updateSelectedCount()
{
    int selectedCount = selectedCharacters.size();
    selectedCountLabel->setText(QString("Selected: %1/3").arg(selectedCount));

    qDebug() << "Updating selected count:" << selectedCount << "characters selected";

    for (int i = 0; i < 3; ++i) {
        if (i < selectedCount) {
            Character* character = selectedCharacters[i];
            QString displayText;

            if (i == 1 && character == availableCharacters[0]) {
                displayText = QString("Dantus\n(Healer)\nHP: %1").arg(character->getMaxHealth());
                teamSlotLabels[i]->setStyleSheet(
                    "QLabel {"
                    "    border: 3px solid gold;"
                    "    border-radius: 5px;"
                    "    padding: 10px;"
                    "    background-color: gold;"
                    "    font-weight: bold;"
                    "    color: #8B4513;"
                    "}"
                    );
            } else if (i == 1 && character != availableCharacters[0]) {
                displayText = "INVALID!";
                teamSlotLabels[i]->setStyleSheet(
                    "QLabel {"
                    "    border: 3px solid red;"
                    "    border-radius: 5px;"
                    "    padding: 10px;"
                    "    background-color: #ffcccc;"
                    "    font-weight: bold;"
                    "    color: red;"
                    "}"
                    );
            } else {
                displayText = QString("%1\n(%2)\nHP: %3")
                                  .arg(character->getName())
                                  .arg(character->getType())
                                  .arg(character->getMaxHealth());
                teamSlotLabels[i]->setStyleSheet(
                    "QLabel {"
                    "    border: 2px solid #4CAF50;"
                    "    border-radius: 5px;"
                    "    padding: 10px;"
                    "    background-color: #e8f5e8;"
                    "    font-weight: bold;"
                    "    color: #2E7D32;"
                    "}"
                    );
            }

            teamSlotLabels[i]->setText(displayText);
        } else {
            if (i == 1) {
                teamSlotLabels[i]->setText("Dantus\n[Required]");
                teamSlotLabels[i]->setStyleSheet(
                    "QLabel {"
                    "    border: 3px solid gold;"
                    "    border-radius: 5px;"
                    "    padding: 10px;"
                    "    background-color: #FFF8DC;"
                    "    font-weight: bold;"
                    "    color: #8B7355;"
                    "}"
                    );
            } else {
                teamSlotLabels[i]->setText("[Empty Slot]");
                teamSlotLabels[i]->setStyleSheet(
                    "QLabel {"
                    "    border: 2px dashed gray;"
                    "    border-radius: 5px;"
                    "    padding: 10px;"
                    "    background-color: #f8f8f8;"
                    "    color: #888888;"
                    "}"
                    );
            }
        }
    }
}

void MainWindow::clearTeam()
{
    qDebug() << "Clearing team";
    selectedCharacters.clear();
    playerTeam->clearTeam();
    updateSelectedCount();
}




void MainWindow::startBattle()
{
    try {
        qDebug() << "Starting battle...";

        if (selectedCharacters.size() < 3) {
            QMessageBox::warning(this, "Team Incomplete", "Please select 3 characters!");
            return;
        }

        bool holyOneFound = false;
        bool holyOneInMiddle = false;

        for (int i = 0; i < selectedCharacters.size(); ++i) {
            if (selectedCharacters[i] == availableCharacters[0]) {
                holyOneFound = true;
                if (i == 1) {
                    holyOneInMiddle = true;
                }
                break;
            }
        }

        if (!holyOneFound) {
            QMessageBox::warning(this, "Invalid Team", "The Holy One must be in your team!");
            return;
        }

        if (!holyOneInMiddle) {
            QMessageBox::warning(this, "Invalid Team", "The Holy One must be in the middle position!");
            return;
        }

        qDebug() << "Team is valid. Creating enemies...";

        try {
            createEnemies();
        } catch (const std::exception& e) {
            qCritical() << "Error creating enemies:" << e.what();
            QMessageBox::critical(this, "Error", "Failed to create enemies. Cannot start battle.");
            return;
        }

        try {
            playerTeam->resetAllCharacters();
        } catch (const std::exception& e) {
            qCritical() << "Error resetting characters:" << e.what();
        }

        if (battleSystem) {
            battleSystem->deleteLater();
            battleSystem = nullptr;
        }

        try {
            battleSystem = new BattleSystem(playerTeam, enemies);
        } catch (const std::exception& e) {
            qCritical() << "Error creating BattleSystem:" << e.what();
            QMessageBox::critical(this, "Error", "Failed to initialize battle system.");
            return;
        }

        try {
            connect(battleSystem, &BattleSystem::battleEnded, this, &MainWindow::endBattle);
            connect(battleSystem, &BattleSystem::enemyChanged, this, &MainWindow::onEnemyChanged);
            connect(battleSystem, &BattleSystem::enemyDefeated, this, &MainWindow::onEnemyDefeated);
        } catch (const std::exception& e) {
            qCritical() << "Error connecting signals:" << e.what();
        }

        try {
            battleScreen->initialize(playerTeam, enemies, battleSystem);
        } catch (const std::exception& e) {
            qCritical() << "Error initializing battle screen:" << e.what();
            QMessageBox::critical(this, "Error", "Failed to initialize battle screen.");
            return;
        }

        try {
            battleSystem->startBattle();
        } catch (const std::exception& e) {
            qCritical() << "Error starting battle:" << e.what();
            QMessageBox::critical(this, "Error", "Failed to start battle.");
            return;
        }

        goToBattle();

        qDebug() << "Battle started successfully";

    } catch (const std::exception& e) {
        qCritical() << "Unhandled exception in startBattle:" << e.what();
        QMessageBox::critical(this, "Fatal Error",
                              QString("Failed to start battle: %1").arg(e.what()));
    } catch (...) {
        qCritical() << "Unknown exception in startBattle";
        QMessageBox::critical(this, "Fatal Error", "Unknown error starting battle.");
    }
}

void BattleScreen::onEnemyChanged(int currentIndex, int totalEnemies)
{
    qDebug() << "BattleScreen: Enemy changed -" << currentIndex << "/" << totalEnemies;

    if (battleSystem) {
        Enemy* currentEnemy = battleSystem->getCurrentEnemy();
        if (currentEnemy) {
            enemyNameLabel->setText(currentEnemy->getName());

            enemyHealthBar->setRange(0, currentEnemy->getMaxHealth());
            enemyHealthBar->setValue(currentEnemy->getHealth());
            enemyHealthBar->setFormat(QString("%1/%2 HP")
                                          .arg(currentEnemy->getHealth())
                                          .arg(currentEnemy->getMaxHealth()));

            enemyProgressLabel->setText(QString("Sin %1 of %2").arg(currentIndex + 1).arg(totalEnemies));

            updateEnemyImage(currentEnemy->getName());

            connect(currentEnemy, &Enemy::healthChanged, this, [this, currentEnemy](int health) {
                enemyHealthBar->setValue(health);
                enemyHealthBar->setFormat(QString("%1/%2 HP")
                                              .arg(health)
                                              .arg(currentEnemy->getMaxHealth()));
            });

            connect(currentEnemy, &Enemy::immunityChanged, this, [this](bool isImmune) {
                updateUI();
            });
        }
    }
}

void BattleScreen::onEnemyDefeated(const QString &enemyName)
{
    qDebug() << "BattleScreen: Enemy defeated -" << enemyName;

    enemyHealthBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid green;"
        "    border-radius: 4px;"
        "    text-align: center;"
        "    font-weight: bold;"
        "    font-size: 9pt;"
        "    color: #333;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #4CAF50;"
        "    border-radius: 2px;"
        "}"
        );

    QTimer::singleShot(1000, this, [this]() {
        if (battleSystem && battleSystem->isBattleActive()) {
            updateUI();
        }
    });
}

void MainWindow::onEnemyChanged(int currentIndex, int totalEnemies)
{
    qDebug() << "MainWindow: Enemy changed to index:" << currentIndex << "/" << totalEnemies;

    if (enemyProgressLabel) {
        enemyProgressLabel->setText(QString("Enemy %1 of %2").arg(currentIndex + 1).arg(totalEnemies));
    }
}

void MainWindow::onEnemyDefeated(const QString &enemyName)
{
    qDebug() << "Enemy defeated:" << enemyName;

    if (battleScreen) {
        battleScreen->onEnemyDefeated(enemyName);
    }

    QMessageBox::information(this, "Enemy Defeated!",
                             QString("%1 has been defeated!").arg(enemyName));
}





void MainWindow::goToDifficultySelect()
{
    stackedWidget->setCurrentWidget(difficultyScreen);
}

void MainWindow::goToCharacterSelect()
{
    qDebug() << "Going to character select screen";
    selectedCharacters.clear();
    playerTeam->clearTeam();
    updateSelectedCount();
    stackedWidget->setCurrentWidget(characterSelectScreen);
}

void MainWindow::goToBattle()
{
    stackedWidget->setCurrentWidget(battleScreen);
}

void MainWindow::goToMainMenu()
{
    if (battleScreen) {
        battleScreen->resetBattleState();
    }

    resetGame();

    stackedWidget->setCurrentWidget(mainMenuScreen);
}

void MainWindow::setDifficultyEasy()
{
    currentDifficulty = "Easy";
    qDebug() << "Difficulty set to: Easy";
    goToCharacterSelect();
}

void MainWindow::setDifficultyMedium()
{
    currentDifficulty = "Medium";
    qDebug() << "Difficulty set to: Medium";
    goToCharacterSelect();
}

void MainWindow::setDifficultyHard()
{
    currentDifficulty = "Hard";
    qDebug() << "Difficulty set to: Hard";
    goToCharacterSelect();
}

void MainWindow::endBattle(bool victory)
{
    qDebug() << "Battle ended. Victory:" << victory;

    if (battleSystem) {
        battleSystem->stopBattle();
        battleSystem->disconnect(this);
    }

    if (battleScreen) {
        battleScreen->resetBattleState();
    }

    QTimer::singleShot(500, this, [this, victory]() {
        if (victory) {
            stackedWidget->setCurrentWidget(victoryScreen);
        } else {
            stackedWidget->setCurrentWidget(defeatScreen);
        }
    });
}

void MainWindow::exitGame()
{
    QApplication::quit();
}

void MainWindow::resetGame()
{
    qDebug() << "Resetting game...";

    selectedCharacters.clear();

    if (playerTeam) {
        playerTeam->clearTeam();
    }

    for (Enemy* enemy : enemies) {
        if (enemy) {
            delete enemy;
        }
    }
    enemies.clear();

    if (battleSystem) {
        battleSystem->disconnect();
        delete battleSystem;
        battleSystem = nullptr;
    }

    if (battleScreen) {
        battleScreen->resetBattleState();
    }

    updateSelectedCount();

    currentDifficulty = "";

    for (Character* character : availableCharacters) {
        if (character) {
            character->reset();
        }
    }

    createEnemies();

    if (!teamSlotLabels.isEmpty()) {
        for (int i = 0; i < 3; ++i) {
            if (i < teamSlotLabels.size()) {
                if (i == 1) {
                    teamSlotLabels[i]->setText("Dantus\n[Required]");
                    teamSlotLabels[i]->setStyleSheet(
                        "QLabel {"
                        "    border: 3px solid gold;"
                        "    border-radius: 5px;"
                        "    padding: 10px;"
                        "    background-color: #FFF8DC;"
                        "    font-weight: bold;"
                        "    color: #8B7355;"
                        "}"
                        );
                } else {
                    teamSlotLabels[i]->setText("[Empty Slot]");
                    teamSlotLabels[i]->setStyleSheet(
                        "QLabel {"
                        "    border: 2px dashed gray;"
                        "    border-radius: 5px;"
                        "    padding: 10px;"
                        "    background-color: #f8f8f8;"
                        "    color: #888888;"
                        "}"
                        );
                }
            }
        }
    }

    qDebug() << "Game reset complete. All enemies recreated:" << enemies.size() << "total.";
    qDebug() << "Available characters:" << availableCharacters.size();
    qDebug() << "Selected characters:" << selectedCharacters.size();
    qDebug() << "Team size:" << (playerTeam ? playerTeam->getSize() : 0);
    qDebug() << "Battle system:" << (battleSystem ? "exists" : "null");
    qDebug() << "Current difficulty:" << currentDifficulty;
}





BattleScreen::~BattleScreen()
{
    cleanupConnections();
}

void BattleScreen::cleanupConnections()
{
    if (battleSystem) {
        battleSystem->disconnect(this);
    }

    if (team) {
        for (int i = 0; i < team->getSize(); ++i) {
            Character *character = team->getCharacterAt(i);
            if (character) {
                character->disconnect(this);
            }
        }
    }

    for (Enemy* enemy : enemies) {
        if (enemy) {
            enemy->disconnect(this);
        }
    }

    enemies.clear();
}

void BattleScreen::resetBattleState()
{
    if (updateTimer && updateTimer->isActive()) {
        updateTimer->stop();
    }

    cleanupConnections();

    team = nullptr;
    enemies.clear();
    battleSystem = nullptr;

    normalAttackButton->setEnabled(false);
    abilityButton->setEnabled(false);
    ultimateButton->setEnabled(false);

    abilityPointsLabel->setText("Ability Points: 0");
    turnLabel->setText("Battle Ended");
    enemyNameLabel->setText("Enemy");
    enemyProgressLabel->setText("Sin 0 of 7");

    enemyHealthBar->setRange(0, 100);
    enemyHealthBar->setValue(0);
    enemyHealthBar->setFormat("0/0 HP");

    if (logTextEdit) {
        logTextEdit->clear();
        logTextEdit->append("=== BATTLE ENDED ===");
    }

    qDebug() << "BattleScreen state reset";
}

void MainWindow::showError(const QString &title, const QString &message, bool isCritical)
{
    if (isCritical) {
        QMessageBox::critical(this, title, message);
    } else {
        QMessageBox::warning(this, title, message);
    }

    logError(message, isCritical ? QtCriticalMsg : QtWarningMsg);
}

void MainWindow::logError(const QString &message, QtMsgType type)
{
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");

    switch (type) {
    case QtDebugMsg:
        qDebug() << timestamp << message;
        break;
    case QtWarningMsg:
        qWarning() << timestamp << message;
        break;
    case QtCriticalMsg:
        qCritical() << timestamp << message;
        break;
    default:
        qDebug() << timestamp << message;
    }
}

void MainWindow::createDefaultCharacters()
{
    qWarning() << "Creating default characters as fallback";

    for (Character *character : availableCharacters) {
        delete character;
    }
    availableCharacters.clear();

    try {
        availableCharacters.append(new HealerCharacter("The Holy One", 200, 25, 50, 100));
        availableCharacters.append(new DPSCharacter("DPSHero", 150, 25, 40, 100));
        availableCharacters.append(new ShielderCharacter("ShielderHero", 150, 20, 50, 100));
    } catch (const std::exception& e) {
        qCritical() << "Failed to create even default characters:" << e.what();
    }
}

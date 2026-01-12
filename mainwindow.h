#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QVector>
#include <QTextEdit>
#include "character.h"
#include "dpscharacter.h"
#include "healercharacter.h"
#include "shieldercharacter.h"
#include "amplifiercharacter.h"
#include "team.h"
#include "battlesystem.h"
#include "gluttonyenemy.h"
#include "lustenemy.h"
#include "greedenemy.h"
#include "slothenemy.h"
#include "wrathenemy.h"
#include "envyenemy.h"
#include "prideenemy.h"

class BattleScreen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onEnemyChanged(int currentIndex, int totalEnemies);
    void onEnemyDefeated(const QString &enemyName);

private slots:
    void goToDifficultySelect();
    void goToCharacterSelect();
    void goToBattle();
    void goToMainMenu();
    void setDifficultyEasy();
    void setDifficultyMedium();
    void setDifficultyHard();
    void selectHolyOne();
    void selectGabriel();
    void selectCain();
    void selectMagdalene();
    void selectEsther();
    void startBattle();
    void endBattle(bool victory);
    void exitGame();
    void clearTeam();
    void updateSelectedCount();

private:
    QStackedWidget *stackedWidget;

    QWidget *mainMenuScreen;
    QWidget *difficultyScreen;
    QWidget *characterSelectScreen;
    BattleScreen *battleScreen;
    QWidget *victoryScreen;
    QWidget *defeatScreen;

    QString currentDifficulty;
    Team *playerTeam;
    QVector<Character*> availableCharacters;
    QVector<Character*> selectedCharacters;
    QVector<Enemy*> enemies;
    BattleSystem *battleSystem;

    QLabel *selectedCountLabel;
    QVector<QLabel*> teamSlotLabels;
    QLabel *enemyProgressLabel;

    void setupMainMenu();
    void setupDifficultySelect();
    void setupCharacterSelect();
    void setupVictoryScreen();
    void setupDefeatScreen();
    void createCharacters();
    void createEnemies();
    void resetGame();
    void selectOtherCharacter(int index);

    void showError(const QString &title, const QString &message, bool isCritical = false);
    void logError(const QString &message, QtMsgType type = QtCriticalMsg);
    void createDefaultCharacters();
};

class BattleScreen : public QWidget
{
    Q_OBJECT

public:
    BattleScreen(QWidget *parent = nullptr);
    ~BattleScreen();

    void initialize(Team *team, QVector<Enemy*> enemies, BattleSystem *battleSystem);
    void resetBattleState();
    void cleanupConnections();

public  slots:
    void onEnemyChanged(int currentIndex, int totalEnemies);
    void onEnemyDefeated(const QString &enemyName);

private slots:
    void useNormalAttack();
    void useAbility();
    void useUltimate();
    void updateUI();
    void onBattleStarted();
    void onTurnChanged(int newTurn);
    void onAbilityPointsChanged(int newPoints);
    void onAttackPerformed(const QString &message);
    void onEnemyAttackPerformed(const QString &message);

private:
    Team *team;
    QVector<Enemy*> enemies;
    BattleSystem *battleSystem;

    QVector<QLabel*> characterNames;
    QVector<QProgressBar*> healthBars;
    QVector<QProgressBar*> ultimateBars;
    QVector<QLabel*> characterStatus;
    QVector<QLabel*> turnIndicators;
    QVector<QLabel*> characterImages;

    QLabel *enemyNameLabel;
    QProgressBar *enemyHealthBar;
    QLabel *enemyTurnIndicator;
    QLabel *enemyProgressLabel;
    QLabel *enemyImageLabel;

    QLabel *abilityPointsLabel;
    QLabel *turnLabel;
    QLabel *logLabel;
    QTextEdit *logTextEdit;

    QPushButton *normalAttackButton;
    QPushButton *abilityButton;
    QPushButton *ultimateButton;

    QTimer *updateTimer;

    void setupUI();
    void updateCharacterUI(int index);
    void updateEnemyImage(const QString &enemyName);
    void updateTurnIndicator();
    void addToLog(const QString &message);
};

#endif

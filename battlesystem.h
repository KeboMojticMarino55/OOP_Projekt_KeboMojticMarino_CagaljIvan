#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include <QObject>
#include <QRandomGenerator>
#include <QVector>
#include <QTimer>
#include "team.h"
#include "enemy.h"

class BattleSystem : public QObject
{
    Q_OBJECT

public:
    BattleSystem(Team *team, QVector<Enemy*> enemies);
    ~BattleSystem();

    void startBattle();
    void performAttack(const QString &attackType, int characterIndex);
    void enemyTurn();

    int getAbilityPoints() const { return abilityPoints; }
    int getCurrentTurn() const { return currentTurn; }
    bool isBattleActive() const { return battleActive; }
    void stopBattle() { battleActive = false; }
    bool isHolyOneAlive() const;

    Team* getTeam() const { return team; }
    Enemy* getCurrentEnemy() const { return currentEnemy; }
    int getCurrentEnemyIndex() const { return currentEnemyIndex; }
    int getTotalEnemies() const { return enemies.size(); }
    int getRemainingEnemies() const;

signals:
    void battleStarted();
    void turnChanged(int newTurn);
    void abilityPointsChanged(int newPoints);
    void battleEnded(bool victory);
    void attackPerformed(const QString &message);
    void enemyAttackPerformed(const QString &message);
    void enemyChanged(int currentIndex, int totalEnemies);
    void enemyDefeated(const QString &enemyName);

private:
    Team *team;
    QVector<Enemy*> enemies;
    Enemy *currentEnemy;
    int currentEnemyIndex;

    int abilityPoints;
    int currentTurn;
    bool battleActive;
    int charactersAttackedThisRound;

    void nextTurn();
    void checkBattleEnd();
    void nextEnemy();
    int calculateDamage(const QString &attackType, int characterIndex);
    void applyCharacterEffects(Character* character, const QString &attackType, int characterIndex);
    QString getAttackDescriptionWithDamage(Character* character, const QString &attackType, int finalDamage);

    QVector<Enemy*> immuneEnemies;
    void clearEnemyImmunities();
};

#endif

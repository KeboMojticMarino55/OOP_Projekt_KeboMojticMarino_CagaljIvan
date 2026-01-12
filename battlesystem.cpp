#include "battlesystem.h"
#include "dpscharacter.h"
#include "healercharacter.h"
#include "shieldercharacter.h"
#include "amplifiercharacter.h"
#include <QDebug>

BattleSystem::BattleSystem(Team *team, QVector<Enemy*> enemies)
    : team(team), enemies(enemies), currentEnemy(nullptr), currentEnemyIndex(0),
    abilityPoints(3), currentTurn(0), battleActive(false), charactersAttackedThisRound(0)
{
    if (!enemies.isEmpty()) {
        currentEnemy = enemies[0];
        currentEnemyIndex = 0;
    }
}

BattleSystem::~BattleSystem()
{
}

int BattleSystem::getRemainingEnemies() const
{
    int count = 0;
    for (Enemy* enemy : enemies) {
        if (enemy->isAlive()) {
            count++;
        }
    }
    return count;
}

bool BattleSystem::isHolyOneAlive() const
{
    Character* holyOne = team->getCharacterAt(1);
    return holyOne && holyOne->isAlive();
}

void BattleSystem::startBattle()
{
    battleActive = true;
    currentTurn = 0;
    abilityPoints = 3;
    charactersAttackedThisRound = 0;

    if (!enemies.isEmpty()) {
        currentEnemy = enemies[0];
        currentEnemyIndex = 0;
        currentEnemy->reset();
    }

    emit battleStarted();
    emit enemyChanged(currentEnemyIndex, enemies.size());
    emit turnChanged(currentTurn);
    emit abilityPointsChanged(abilityPoints);
}

void BattleSystem::performAttack(const QString &attackType, int characterIndex)
{
    try {
        if (!battleActive || !currentEnemy) {
            qDebug() << "Battle not active or no current enemy";
            return;
        }

        Character *character = team->getCharacterAt(characterIndex);
        if (!character || !character->isAlive()) {
            qDebug() << "Character not valid or not alive";
            return;
        }

        int damage = 0;
        try {
            damage = calculateDamage(attackType, characterIndex);
        } catch (const std::exception& e) {
            qCritical() << "Error calculating damage:" << e.what();
            emit attackPerformed("Error calculating damage!");
            return;
        }

        try {
            applyCharacterEffects(character, attackType, characterIndex);
        } catch (const std::exception& e) {
            qCritical() << "Error applying character effects:" << e.what();
            emit attackPerformed("Error applying effects!");
        }

        if (currentEnemy && currentEnemy->isImmune()) {
            emit attackPerformed(QString("%1 is immune to damage this turn!")
                                     .arg(currentEnemy->getName()));
            damage = 0;
        }

        if (damage > 0) {
            try {
                currentEnemy->takeDamage(damage);
            } catch (const std::exception& e) {
                qCritical() << "Error applying damage:" << e.what();
            }
        }

        if (attackType == "Normal") {
            try {
                character->addUltimateCharge(20.0f);
                abilityPoints++;
                if (abilityPoints > 5) abilityPoints = 5;
            } catch (const std::exception& e) {
                qCritical() << "Error updating ultimate charge:" << e.what();
            }
        } else if (attackType == "Ability") {
            try {
                character->addUltimateCharge(80.0f);
                abilityPoints--;
            } catch (const std::exception& e) {
                qCritical() << "Error updating ability points:" << e.what();
            }
        } else if (attackType == "Ultimate") {
            try {
                character->resetUltimate();
            } catch (const std::exception& e) {
                qCritical() << "Error resetting ultimate:" << e.what();
            }
        }

        QString attackDesc;
        try {
            if ((attackType == "Normal" && character->getNormalDamage() > 0) ||
                (attackType == "Ability" && character->getAbilityDamage() > 0) ||
                (attackType == "Ultimate" && character->getUltimateDamage() > 0)) {

                attackDesc = character->getAttackDescription(attackType);

                if (character->hasDamageBuff() && damage > 0) {
                    attackDesc += QString(" (+%1% buff)").arg(character->getDamageMultiplier() - 100);
                }
            } else {
                attackDesc = character->getAttackDescription(attackType);
            }
        } catch (const std::exception& e) {
            qCritical() << "Error generating attack description:" << e.what();
            attackDesc = QString("%1 attacks!").arg(character->getName());
        }

        emit attackPerformed(attackDesc);
        emit abilityPointsChanged(abilityPoints);

        try {
            checkBattleEnd();
        } catch (const std::exception& e) {
            qCritical() << "Error in checkBattleEnd:" << e.what();
        }

        if (!battleActive) return;

        charactersAttackedThisRound++;

        if (charactersAttackedThisRound >= 3) {
            try {
                clearEnemyImmunities();
                enemyTurn();
            } catch (const std::exception& e) {
                qCritical() << "Error in enemy turn:" << e.what();
            }
            charactersAttackedThisRound = 0;
        }

        if (battleActive) {
            try {
                nextTurn();
            } catch (const std::exception& e) {
                qCritical() << "Error in nextTurn:" << e.what();
            }
        }

    } catch (const std::exception& e) {
        qCritical() << "Unhandled exception in performAttack:" << e.what();
        emit attackPerformed("Critical error during attack!");
    } catch (...) {
        qCritical() << "Unknown exception in performAttack";
        emit attackPerformed("Unknown error during attack!");
    }
}

int BattleSystem::calculateDamage(const QString &attackType, int characterIndex)
{
    try {
        Character *character = team->getCharacterAt(characterIndex);
        if (!character) {
            qWarning() << "Character not found at index:" << characterIndex;
            return 0;
        }

        int baseDamage = 0;

        if (attackType == "Normal") {
            baseDamage = character->getNormalDamage();
        } else if (attackType == "Ability") {
            if (abilityPoints <= 0) {
                qDebug() << "Not enough ability points";
                return 0;
            }
            baseDamage = character->getAbilityDamage();
        } else if (attackType == "Ultimate") {
            if (!character->isUltimateReady()) {
                qDebug() << "Ultimate not ready";
                return 0;
            }
            baseDamage = character->getUltimateDamage();
        } else {
            qWarning() << "Unknown attack type:" << attackType;
            return 0;
        }

        bool hadBuff = character->hasDamageBuff();
        int multiplier = character->getDamageMultiplier();

        int finalDamage = baseDamage * multiplier / 100;

        if (hadBuff) {
            try {
                character->clearDamageMultiplier();

                QString buffUsedMsg = QString("%1's damage buff has been consumed!")
                                          .arg(character->getName());

                QTimer::singleShot(100, this, [this, buffUsedMsg]() {
                    emit attackPerformed(buffUsedMsg);
                });
            } catch (const std::exception& e) {
                qCritical() << "Error clearing damage multiplier:" << e.what();
            }
        }

        qDebug() << character->getName() << "damage:" << baseDamage
                 << "*" << multiplier << "% =" << finalDamage
                 << "(had buff:" << hadBuff << ")";

        return finalDamage;

    } catch (const std::exception& e) {
        qCritical() << "Exception in calculateDamage:" << e.what();
        return 0;
    } catch (...) {
        qCritical() << "Unknown exception in calculateDamage";
        return 0;
    }
}

void BattleSystem::applyCharacterEffects(Character* character, const QString &attackType, int characterIndex)
{
    if (attackType == "Ability") {
        QVector<Character*> aliveCharacters;
        QVector<Character*> allAliveCharacters;

        for (int i = 0; i < 3; i++) {
            Character *target = team->getCharacterAt(i);
            if (target && target->isAlive()) {
                allAliveCharacters.append(target);

                if (character->getType() != "Amplifier" || target != character) {
                    aliveCharacters.append(target);
                }
            }
        }

        if (!aliveCharacters.isEmpty()) {
            int randomIndex = QRandomGenerator::global()->bounded(aliveCharacters.size());
            Character* target = aliveCharacters[randomIndex];
            character->applyAbilityEffect(target);

            if (character->getType() == "Amplifier") {
                emit attackPerformed(QString("%1 buffs %2! Next attack +%3% damage!")
                                         .arg(character->getName())
                                         .arg(target->getName())
                                         .arg(dynamic_cast<AmplifierCharacter*>(character)->getAbilityBuff()));
            }
        } else {
            if (character->getType() == "Amplifier") {
                if (allAliveCharacters.size() == 1) {
                    emit attackPerformed(QString("%1 has no allies to buff!").arg(character->getName()));
                } else if (allAliveCharacters.isEmpty()) {
                    emit attackPerformed(QString("%1 cannot buff - no allies alive!").arg(character->getName()));
                }
            }
        }
    } else if (attackType == "Ultimate") {
        QVector<Character*> teamMembers;
        for (int i = 0; i < 3; i++) {
            teamMembers.append(team->getCharacterAt(i));
        }
        character->applyUltimateEffect(teamMembers);

        if (character->getType() == "Amplifier") {
            int buffCount = 0;
            for (Character* member : teamMembers) {
                if (member && member->isAlive() && member != character) {
                    buffCount++;
                }
            }

            if (buffCount > 0) {
                emit attackPerformed(QString("%1 buffs %2 allies! Next attacks +%3% damage!")
                                         .arg(character->getName())
                                         .arg(buffCount)
                                         .arg(dynamic_cast<AmplifierCharacter*>(character)->getUltimateBuff()));
            } else {
                emit attackPerformed(QString("%1 has no allies to buff with ultimate!").arg(character->getName()));
            }
        }
    }
}

QString BattleSystem::getAttackDescriptionWithDamage(Character* character, const QString &attackType, int finalDamage)
{
    QString baseDescription = character->getAttackDescription(attackType);

    if (character->getType() == "Amplifier" && (attackType == "Ability" || attackType == "Ultimate")) {
        return baseDescription;
    }

    QString description;

    if (attackType == "Normal") {
        int baseDamage = character->getNormalDamage();
        if (finalDamage > baseDamage) {
            description = QString("%1 attacks for %2 damage! (+%3 damage)")
                              .arg(character->getName())
                              .arg(finalDamage)
                              .arg(finalDamage-baseDamage);
        } else {
            description = QString("%1 attacks for %2 damage!")
                              .arg(character->getName())
                              .arg(finalDamage);
        }
    }
    else if (attackType == "Ability" && character->getAbilityDamage() > 0) {
        int baseDamage = character->getAbilityDamage();
        if (finalDamage > baseDamage) {
            description = QString("%1 uses special ability for %2 damage! (+%3 damage)")
                              .arg(character->getName())
                              .arg(finalDamage)
                              .arg(finalDamage-baseDamage);
        } else {
            description = QString("%1 uses special ability for %2 damage!")
                              .arg(character->getName())
                              .arg(finalDamage);
        }
    }
    else if (attackType == "Ultimate" && character->getUltimateDamage() > 0) {
        int baseDamage = character->getUltimateDamage();
        if (finalDamage > baseDamage) {
            description = QString("%1 unleashes ULTIMATE for %2 damage! (+%3 damage)")
                              .arg(character->getName())
                              .arg(finalDamage)
                              .arg(finalDamage-baseDamage);
        } else {
            description = QString("%1 unleashes ULTIMATE for %2 damage!")
                              .arg(character->getName())
                              .arg(finalDamage);
        }
    }
    else {
        description = baseDescription;
    }

    return description;
}

void BattleSystem::nextTurn()
{
    int startTurn = currentTurn;
    int attempts = 0;

    do {
        currentTurn = (currentTurn + 1) % 3;
        attempts++;

        if (attempts >= 3) {
            break;
        }

        Character *nextChar = team->getCharacterAt(currentTurn);
        if (nextChar && nextChar->isAlive()) {
            emit turnChanged(currentTurn);
            return;
        }
    } while (currentTurn != startTurn);

    checkBattleEnd();
}

void BattleSystem::enemyTurn()
{
    if (!battleActive || !currentEnemy || !currentEnemy->isAlive() || team->allDead()) {
        return;
    }

    QVector<Character*> aliveCharacters;
    for (int i = 0; i < 3; i++) {
        Character *character = team->getCharacterAt(i);
        if (character && character->isAlive()) {
            aliveCharacters.append(character);
        }
    }

    if (aliveCharacters.isEmpty()) {
        battleActive = false;
        emit battleEnded(false);
        return;
    }

    bool useAbility = QRandomGenerator::global()->bounded(100) < 50;

    if (useAbility) {
        if (currentEnemy->getName() == "Pride") {
            emit enemyAttackPerformed(QString("%1 declares superiority and becomes immune to damage this turn!")
                                          .arg(currentEnemy->getName()));
        }

        currentEnemy->performAbility(aliveCharacters);

        if (currentEnemy->getName() != "Pride") {
            emit enemyAttackPerformed(currentEnemy->getAbilityDescription());
        }
    } else {
        int randomIndex = QRandomGenerator::global()->bounded(aliveCharacters.size());
        Character *target = aliveCharacters[randomIndex];
        currentEnemy->performNormalAttack(target);
        emit enemyAttackPerformed(currentEnemy->getNormalAttackDescription());
    }

    checkBattleEnd();
}

void BattleSystem::clearEnemyImmunities()
{
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->isImmune()) {
            enemy->clearImmunity();
        }
    }
}

void BattleSystem::nextEnemy()
{
    int nextIndex = -1;
    for (int i = currentEnemyIndex + 1; i < enemies.size(); i++) {
        if (enemies[i]->isAlive()) {
            nextIndex = i;
            break;
        }
    }

    if (nextIndex != -1) {
        currentEnemyIndex = nextIndex;
        currentEnemy = enemies[currentEnemyIndex];
        currentEnemy->reset();
        emit enemyChanged(currentEnemyIndex, enemies.size());
    } else {
        battleActive = false;
        emit battleEnded(true);
    }
}

void BattleSystem::checkBattleEnd()
{
    if (!isHolyOneAlive()) {
        battleActive = false;
        emit battleEnded(false);
        return;
    }

    if (currentEnemy && !currentEnemy->isAlive()) {
        QString enemyName = currentEnemy->getName();

        bool isLastEnemy = true;
        for (int i = currentEnemyIndex + 1; i < enemies.size(); i++) {
            if (enemies[i]->isAlive()) {
                isLastEnemy = false;
                break;
            }
        }

        if (isLastEnemy) {
            emit attackPerformed(QString("%1 defeated! ALL ENEMIES VANQUISHED!").arg(enemyName));
            battleActive = false;
            emit battleEnded(true);
        } else {
            emit attackPerformed(QString("%1 defeated! Next enemy incoming...").arg(enemyName));

            QTimer::singleShot(2000, this, [this, enemyName]() {
                if (battleActive) {
                    nextEnemy();
                    if (currentEnemy) {
                        emit attackPerformed(QString("%1 appears!").arg(currentEnemy->getName()));
                    }
                }
            });
        }

        emit enemyDefeated(enemyName);
        return;
    }

    if (team->allDead()) {
        battleActive = false;
        emit battleEnded(false);
    }
}
